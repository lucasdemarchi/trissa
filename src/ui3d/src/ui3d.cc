/*
 * ui3d.cc
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
#include "InputHandler.h"
#include "InputHandlerGame.h"
#include "InputHandlerGui.h"
#include "StateManager.h"
#include "MainMenu.h"

#include <Ogre.h>
#include <OgreWindowEventUtilities.h>

#include <OgreCEGUIRenderer.h>
#include <CEGUISystem.h>
#include <CEGUILogger.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <CEGUISchemeManager.h>

using namespace Ogre;


class Application
{
public:
    void setup(){
        mRoot = new Root();
        mStateManager = new StateManager(STARTUP);
        //Load all resources defined in resources.cfg
        setupResources();
        //Load from ogre.cfg or Call dialog
        setupRenderSystem();
        //Create 1 window to render into.
        mWindow = mRoot->initialise(true, "Trissa 3D UI");
        //Initialize all groups
        ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
        setupScene();
        setupCEGUI();

    }
    void go() {
        if( mStateManager->requestStateChange(GUI) == false ) return;

        while(true) {
            createGUI();
            while(mStateManager->getCurrentState() == GUI) {
                mInputHandler->capture();
                WindowEventUtilities::messagePump();
                mRoot->renderOneFrame();
            }
            destroyGUI();
            if(mStateManager->getCurrentState() == SHUTDOWN)
                break;

            createScene();
            while(mStateManager->getCurrentState() == GAME) {
                mInputHandler->capture();
                WindowEventUtilities::messagePump();
                mRoot->renderOneFrame();
            }
            destroyScene();
            if(mStateManager->getCurrentState() == SHUTDOWN)
                break;
        }


    }

    Application() :
        mRoot( 0 ),
        mStateManager( 0 ),
        mWindow( 0 ),
        mSceneMgr( 0 ),
        mCamera( 0 ),
        mViewport( 0 ),
        mInputHandler( 0 ),
        mCEGUIRenderer( 0 ),
        mCEGUISystem( 0 ),
        mMainMenu( 0 ),
        mForceConfigDialogDisplay(false){
    }

    Application( bool forceConfigDialogDisplay )
            : mForceConfigDialogDisplay(forceConfigDialogDisplay) {
    }

    ~Application() {
        delete mStateManager;
        delete mRoot;
    }

private:
    Root* mRoot;
    StateManager* mStateManager;
    RenderWindow* mWindow;
    SceneManager* mSceneMgr;
    Camera* mCamera;
    Viewport* mViewport;
    InputHandler *mInputHandler;

    //CEGUI
    CEGUI::OgreCEGUIRenderer* mCEGUIRenderer;
    CEGUI::System* mCEGUISystem;

    MainMenu* mMainMenu;

    bool mForceConfigDialogDisplay;

    inline void setupResources() {
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

    inline void setupRenderSystem() {
        if (mForceConfigDialogDisplay || !mRoot->restoreConfig()) {
            if (!mRoot->showConfigDialog())
                throw Exception(52, "User canceled the config dialog!", "Application::setupRenderSystem()");
        }
    }

    inline void setupScene() {
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

    void setupCEGUI() {

        mCEGUIRenderer = new CEGUI::OgreCEGUIRenderer(mWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mSceneMgr);
        // create the root CEGUI class
        mCEGUISystem = new CEGUI::System(mCEGUIRenderer);
        // tell us a lot about what is going on (see CEGUI.log in the working directory)
        CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);
        // use this CEGUI scheme definition (see CEGUI docs for more)
        CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");

        mCEGUISystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
        mCEGUISystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");

    }

    void createGUI(){
        if(!mMainMenu){
            mInputHandler = new InputHandlerGui ( mWindow, mStateManager, mCEGUISystem );
            CEGUI::Window* pLayout = CEGUI::WindowManager::getSingleton().loadWindowLayout("welcome.layout");
            mCEGUISystem->setGUISheet(pLayout);
            mMainMenu = new MainMenu( mCEGUISystem, pLayout, mStateManager, 0 );
        }
        else
            throw std::exception();
    }

    void destroyGUI(){
        delete mInputHandler;
        delete mMainMenu;
        mInputHandler = 0;
        mMainMenu = 0;
    }

    void createScene(){
        mSceneMgr->setAmbientLight(ColourValue(0.25, 0.25, 0.25));

        Entity *ent = 0;

        Plane plane( Vector3::UNIT_Y, 0 );
        MeshManager::getSingleton().createPlane("ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 1500,1500,20,20,true,1,5,5,Vector3::UNIT_Z);
        ent = mSceneMgr->createEntity( "GroundEntity", "ground" );
        mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
        ent->setMaterialName( "BumpyMetal" );
        ent->setCastShadows( false );


        // add the Board
        ent = mSceneMgr->createEntity("Board", "ninja.mesh");
        SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("BoardNode", Vector3( 0, 20, 0 ) );
        node->attachObject(ent);
        //node->roll( Degree( -90 ) );

        // create the light
        Light *light = mSceneMgr->createLight("Light1");
        light->setType(Light::LT_POINT);
        light->setPosition(Vector3(250, 150, 250));
        light->setDiffuseColour(ColourValue::White);
        light->setSpecularColour(ColourValue::White);

        mCamera->setPosition( Vector3( 0, 50, 500 ) );
        mCamera->lookAt( Vector3( 0, 0, 0 ) );

        // Create the scene nodes
        node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode1", Vector3(-300, 200, 300));
        node->yaw( Degree( -45 ) );
        node->lookAt( Vector3( 0, 0, 0 ), Node::TS_PARENT );
        node->attachObject(mCamera);

        node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode2", Vector3(300, 200, 300));
        node->yaw( Degree( 45 ) );
        node->lookAt( Vector3( 0, 0, 0 ), Node::TS_PARENT );

        node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode3", Vector3(-300, 200, -300));
        node->yaw( Degree( -135 ) );
        node->lookAt( Vector3( 0, 0, 0 ), Node::TS_PARENT );

        node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode4", Vector3(300, 200, -300));
        node->yaw( Degree( 135 ) );
        node->lookAt( Vector3( 0, 0, 0 ), Node::TS_PARENT );

        mInputHandler = new InputHandlerGame( mWindow, mStateManager, mCEGUISystem, mSceneMgr );

    }

    void destroyScene(){
        mSceneMgr->destroyAllLights();
        mSceneMgr->destroyAllEntities();
        mSceneMgr->getRootSceneNode()->removeAndDestroyAllChildren();
        MeshManager::getSingleton().removeAll();

        delete mInputHandler;
        mInputHandler = 0;
    }

};

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char **argv)
#endif
{
    try {
        Application app;
        app.setup();
        app.go();
    } catch (Exception& e) {
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBoxA(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occurred: %s\n",
                e.getFullDescription().c_str());
#endif
    }

    return 0;
}
