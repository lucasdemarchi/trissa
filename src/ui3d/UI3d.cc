/*
 * UI3d.cc
 * This file is part of Trissa
 *
 * Copyright (C) 2009 - Lucas De Marchi
 *
 * Trissa is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * Trissa is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Trissa; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */
#include "UI3d.h"
#include "ConfigManager.h"
#include "PlayerFactory.h"
#include "StateManager.h"

#include "InputHandler.h"
#include "InputHandlerGame.h"
#include "InputHandlerGui.h"
#include "MainMenu.h"

#include <Ogre.h>
#include <OgreWindowEventUtilities.h>

#include <OgreCEGUIRenderer.h>
#include <CEGUISystem.h>
#include <CEGUILogger.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <CEGUISchemeManager.h>

#include <sstream>

using namespace Ogre;

namespace trissa
{

UI3d::UI3d (ConfigManager* cm, StateManager* sm, PlayerFactory const* pf) :
	UI(cm, sm, pf),
	//related to Ogre:
	mRoot( 0 ), mWindow( 0 ), mSceneMgr( 0 ),
	mCamera( 0 ), mViewport( 0 ), mInputHandler( 0 ),
	//Config screens:
	mCEGUIRenderer( 0 ), mCEGUISystem( 0 ), mMainMenu( 0 ),
	//Display configuration?
	mForceConfigDialogDisplay(false), mCanLoadUi(false), mHasNewPos(false)
{
	mRoot = new Root();
	//Load all resources defined in resources.cfg
	setupResources();
	//Load from ogre.cfg or Call dialog
	setupRenderSystem();
	//Create 1 window to render into.
	mWindow = mRoot->initialise(true, "Trissa 3D");
	//Initialize all groups
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	setupScene();
	setupCEGUI();

}
UI3d::~UI3d()
{
	delete mRoot;
}
void UI3d::configure()
{
	createGUI();
	while(mSm->getCurrentState() == GUI) {
		mInputHandler->capture();
		WindowEventUtilities::messagePump();
		mRoot->renderOneFrame();
	}
	destroyGUI();
}

void UI3d::start(Cube const& board)
{
//	mThread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&UI3d::start_thread, this)));
	mCanLoadUi = true;
}

void UI3d::wait_end()
{
	//assert(mThread);
	//mThread->join();
}
void UI3d::start_thread()
{
	//wait game load - busy wait
	while(!mCanLoadUi){
		boost::this_thread::sleep(boost::posix_time::milliseconds(100));
	}


	std::cout << "UI3d::start_thread:  render thread.\n";
	
	createScene();
	while(mSm->getCurrentState() == GAME) {
		if(mHasNewPos)
			updateBoard();
		mInputHandler->capture();
		mInputHandler->treatPressingEvents();
		WindowEventUtilities::messagePump();
		mRoot->renderOneFrame();
	}
	destroyScene();
	mCanLoadUi = false;
}

void UI3d::updateBoard()
{
	boost::lock_guard<boost::mutex> lock(mMutexNewPos);
	
	std::stringstream father_name("");
	father_name << posNames.c_str() << "[" << player_move.m.x << "][" << player_move.m.y << "]["
		<< player_move.m.z << "]";
	Entity* father_ent = mSceneMgr->getEntity(father_name.str());
	
	assert(father_ent);
	std::stringstream ball_name("");
	ball_name << father_name.str() << "Ball";
	Entity*	new_ball = mSceneMgr->createEntity
		(ball_name.str(), "ball.mesh" );

	new_ball->setQueryFlags(InputHandlerGame::BALL_MASK);
	if(player_move.player == PLAYER_CROSS)
		new_ball->setMaterialName("Ball/Cross");
	else
		new_ball->setMaterialName("Ball/Circle");
	
	father_ent->setQueryFlags(InputHandlerGame::POSITION_OCCUPIED_MASK);
	father_ent->getParentSceneNode()->attachObject(new_ball);

	mHasNewPos = false;
	mCondNewPosHandled.notify_one();

}

void UI3d::gameOver(Move const& startPosition, Move const& direction,
		PlayerType winner)
{
	mGameOver = true;
	if(winner == PLAYER_BLANK)
		return;

	int dimension = mCm->getDimension();

	Move m = startPosition;
	while (m.x != dimension && m.y != dimension && m.z != dimension
			&& m.x >= 0 && m.y >= 0 && m.z >= 0)
		m -= direction;
	
	for(int i = 0; i < dimension; i++){
		m += direction;
		std::stringstream ball_name("");
		ball_name << posNames.c_str() << "[" << m.x << "][" << m.y << "]["
		<< m.z << "]" << "Ball";
		Entity* ent = mSceneMgr->getEntity(ball_name.str());
		ent->setMaterialName("Ball/Winner");
	}

	//set winner name
	Overlay* pOver = OverlayManager::getSingleton().getByName("Game/InfoOverlay");
	pOver->hide();
	if(winner == PLAYER_CROSS)
		pOver = OverlayManager::getSingleton().getByName("Game/WinnerCrossOverlay");
	else
		pOver = OverlayManager::getSingleton().getByName("Game/WinnerCircleOverlay");

	if(pOver)
		pOver->show();

	pOver = OverlayManager::getSingleton().getByName("Game/StatusOverlay");
	pOver->show();

}

void UI3d::setPos(Move const &m, PlayerType player)
{
	player_move.m = m;
	player_move.player = player;

	boost::unique_lock<boost::mutex> lock(mMutexNewPos);
	mHasNewPos = true;
	while (mHasNewPos)
		mCondNewPosHandled.wait(lock);

}

Move UI3d::getUserInput()
{
	InputHandlerGame* ih = static_cast<InputHandlerGame*>(mInputHandler);
	//blocks until user play
	Entity* ent = ih->getUserInput();
	if(ent != NULL)
		return boardMap[ent];
	else
		throw std::exception();
}

void UI3d::printWinnerMessage(std::string msg)
{
}
void UI3d::printLooserMessage(std::string msg)
{
}

void UI3d::setupResources() {
	String secName, typeName, archName;
	ConfigFile cf;
	cf.load("resources.cfg");
	ConfigFile::SectionIterator seci = cf.getSectionIterator();
	while (seci.hasMoreElements()) {
		secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = seci.getNext();
		ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i) {
			typeName = i->first;
			archName = i->second;
			ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}
}

void UI3d::setupRenderSystem() {
	if (mForceConfigDialogDisplay || !mRoot->restoreConfig()) {
		if (!mRoot->showConfigDialog())
			throw Exception(52, "User canceled the config dialog!", "Application::setupRenderSystem()");
	}
}

void UI3d::setupScene() {
	//create SceneManager
	mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "Default SceneManager");

	//create Camera
	mCamera = mSceneMgr->createCamera( "Camera1" );
	mCamera->setNearClipDistance(5);

	//create Viewport and attach to mCamera
	Viewport* vp = mWindow->addViewport( mCamera );
	vp->setBackgroundColour( ColourValue( 0, 0, 0 ) );
	mCamera->setAspectRatio( Real( vp->getActualWidth() ) / Real( vp->getActualHeight() ) );

	TextureManager::getSingleton().setDefaultNumMipmaps(5);
}

void UI3d::setupCEGUI() {

	mCEGUIRenderer = new CEGUI::OgreCEGUIRenderer(mWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mSceneMgr);
	// create the root CEGUI class
	mCEGUISystem = new CEGUI::System(mCEGUIRenderer);
	// tell us a lot about what is going on (see CEGUI.log in the working directory)
	CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);
	// use this CEGUI scheme definition (see CEGUI docs for more)
	CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"QuadraticLook.scheme");

	mCEGUISystem->setDefaultMouseCursor((CEGUI::utf8*)"QuadraticLook", (CEGUI::utf8*)"MouseArrow");
	mCEGUISystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");

}


void UI3d::createGUI(){
	if(!mMainMenu){
		mInputHandler = new InputHandlerGui ( mWindow, mSm, mCEGUISystem );
		mMainMenu = new MainMenu( mCEGUISystem, mSm, mCm );
	}
	else
		throw std::exception();
}
void UI3d::destroyGUI(){
	delete mInputHandler;
	delete mMainMenu;
	mInputHandler = 0;
	mMainMenu = 0;
}

void UI3d::createScene(){
	mSceneMgr->setAmbientLight(ColourValue(0.25, 0.25, 0.25));
	Entity *ent = 0;
	SceneNode *node = 0;
	Light* light = 0;

	mCamera->setQueryFlags( InputHandlerGame::CAMERA_MASK );
	//Ground
	Plane plane( Vector3::UNIT_Y, 0 );
	MeshManager::getSingleton().createPlane
		("ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
		 1500,1500,20,20,true,1,5,5,Vector3::UNIT_Z);

	ent = mSceneMgr->createEntity("GroundEntity", "ground" );
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
	ent->setMaterialName( "World/Ground" );
	ent->setCastShadows( false );
	ent->setQueryFlags( InputHandlerGame::PLANE_MASK );

	light = mSceneMgr->createLight( "Light1" );
	light->setType( Light::LT_POINT );
	light->setPosition( Vector3( 0, 150, 250 ) );
	light->setDiffuseColour(0.2, 0.2, 0.2);
	light->setSpecularColour(0.2, 0.2, 0.2);
	light->setQueryFlags( InputHandlerGame::LIGHT_MASK );

	light = mSceneMgr->createLight("Light3");
	light->setType(Light::LT_DIRECTIONAL);
	light->setDiffuseColour(ColourValue(.25, .25, .25));
	light->setSpecularColour(ColourValue(.25, .25, .25));
	light->setDirection(Vector3( 0, -1, 1 ));
	light->setQueryFlags( InputHandlerGame::LIGHT_MASK );

	//Board
	int boardDimension = mCm->getDimension();
	Real init_position = (boardDimension*QUAD_SIZE)/2 -QUAD_SIZE/2;
	mBoardNode = mSceneMgr->getRootSceneNode()->createChildSceneNode
		(String("BoardNode"));

	for(int j = 0 ; j < boardDimension ; j++){
		for(int i = 0 ; i < boardDimension; i++){
			for (int k = 0 ; k < boardDimension ; k++){
				std::stringstream sName("");
				sName << posNames.c_str() << "[" << i << "][" << j << "]["
					<< k << "]";
				ent = mSceneMgr->createEntity(sName.str(), "glass.mesh" );
				ent->setMaterialName("Board/Glass");
				ent->setQueryFlags( InputHandlerGame::POSITION_AVAILABLE_MASK );
				SceneNode* sc = mBoardNode->createChildSceneNode();
				sc->setPosition(init_position - (QUAD_SIZE * i),			//X
						QUAD_SIZE + (j * VAR_DIST_PLANES * boardDimension),	//Y
						init_position - (QUAD_SIZE * k));					//Z
				
				sc->attachObject(ent);
				boardMap[ent] = Move(i,j,k);
			}
		}
	}

	//sticks
	init_position +=  QUAD_SIZE/2;

	ent = mSceneMgr->createEntity("stick1", "stick.mesh");
	ent->setQueryFlags( InputHandlerGame::STICK_MASK );
	Vector3 vScale(2, 2 + QUAD_SIZE + (VAR_DIST_PLANES * boardDimension *
				(boardDimension-1)), 2);
	node = mBoardNode->createChildSceneNode(Vector3(init_position,
				0, init_position));
	node->setScale(vScale);
	node->attachObject(ent);

	ent = mSceneMgr->createEntity("stick2", "stick.mesh");
	ent->setQueryFlags( InputHandlerGame::STICK_MASK );
	node = mBoardNode->createChildSceneNode(Vector3(-init_position , 0,
				init_position));
	node->setScale(vScale);
	node->attachObject(ent);

	ent = mSceneMgr->createEntity("stick3", "stick.mesh");
	ent->setQueryFlags( InputHandlerGame::STICK_MASK );
	node = mBoardNode->createChildSceneNode(Vector3(init_position , 0,
				-init_position));
	node->setScale(vScale);
	node->attachObject(ent);

	ent = mSceneMgr->createEntity("stick4", "stick.mesh");
	ent->setQueryFlags( InputHandlerGame::STICK_MASK );
	node = mBoardNode->createChildSceneNode(Vector3(-init_position , 0,
				-init_position));
	node->setScale(vScale);
	node->attachObject(ent);


	//Create the scene nodes for cameras
	//Calculate the Y position according to board size:


	//Vector3 cornerPos(init_position, yPos = QUAD_SIZE + (VAR_DIST_PLANES * boardDimension * (boardDimension-1)), init_position);

	Real yPos = QUAD_SIZE +
		(VAR_DIST_PLANES * boardDimension * (boardDimension-1));

	node = mSceneMgr->getRootSceneNode()->createChildSceneNode
		("CamNode1", Vector3(-300, yPos, 300));
	node->yaw( Degree( -45 ) );
	node->lookAt( Vector3( 0, Real(QUAD_SIZE) + Real(VAR_DIST_PLANES) * 
				Real(boardDimension* (boardDimension-1)) , 0 ), Node::TS_PARENT );
	node->attachObject(mCamera);

	node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode2", Vector3(300, yPos, 300));
	node->yaw( Degree( 45 ) );
	node->lookAt( Vector3( 0, 0, 0 ), Node::TS_PARENT );

	node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode3", Vector3(-300, yPos, -300));
	node->yaw( Degree( -135 ) );
	node->lookAt( Vector3( 0, 0, 0 ), Node::TS_PARENT );

	node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode4", Vector3(300, yPos, -300));
	node->yaw( Degree( 135 ) );
	node->lookAt( Vector3( 0, 0, 0 ), Node::TS_PARENT );


	mSceneMgr->setSkyBox(true, "World/MorningSkyBox");

	Overlay* pOver = OverlayManager::getSingleton().getByName("Game/InfoOverlay");

	if(pOver)
		pOver->show();

	//load the others
	pOver = OverlayManager::getSingleton().getByName("Game/WinnerCrossOverlay");
	pOver->show();
	pOver->hide();
	
	pOver = OverlayManager::getSingleton().getByName("Game/WinnerCircleOverlay");
	pOver->show();
	pOver->hide();


	pOver = OverlayManager::getSingleton().getByName("Game/StatusOverlay");
	pOver->show();
	pOver->hide();

	//set captions
	OverlayElement* ov = OverlayManager::getSingleton().getOverlayElement("Game/InfoOverlay/PlayerA/text");
	ov->setCaption(mCm->getPlayerA());
	ov = OverlayManager::getSingleton().getOverlayElement("Game/WinnerCrossOverlay/winnerName");
	ov->setCaption(mCm->getPlayerA());

	ov = OverlayManager::getSingleton().getOverlayElement("Game/InfoOverlay/PlayerB/text");
	ov->setCaption(mCm->getPlayerB());
	ov = OverlayManager::getSingleton().getOverlayElement("Game/WinnerCircleOverlay/winnerName");
	ov->setCaption(mCm->getPlayerB());
				
	mInputHandler = new InputHandlerGame( mWindow, mSm, mCEGUISystem, mSceneMgr );

}

void UI3d::destroyScene(){
	//TODO: wait for inputmanager be ok with that, cause the other can be
	//accessing it while we destroy
	mSceneMgr->destroyAllLights();
	mSceneMgr->destroyAllEntities();
	mSceneMgr->getRootSceneNode()->removeAndDestroyAllChildren();
	MeshManager::getSingleton().removeAll();

	Overlay* pOver = OverlayManager::getSingleton().getByName("Game/InfoOverlay");
	pOver->hide();
	pOver = OverlayManager::getSingleton().getByName("Game/WinnerCrossOverlay");
	pOver->hide();
	pOver = OverlayManager::getSingleton().getByName("Game/WinnerCircleOverlay");
	pOver->hide();
	pOver = OverlayManager::getSingleton().getByName("Game/StatusOverlay");
	pOver->hide();

	delete mInputHandler;
	mInputHandler = 0;
}

const Real UI3d::VAR_DIST_PLANES = 6;
const String UI3d::posNames("BoardPlace");
const Real UI3d::QUAD_SIZE = 20;

}

