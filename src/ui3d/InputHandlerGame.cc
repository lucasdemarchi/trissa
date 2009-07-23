/*
 * InputHandlerGame.cc
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

#include "InputHandlerGame.h"
#include "StateManager.h"
#include <exception>

#include <OISInputManager.h>
#include <OISMouse.h>
#include <OISKeyboard.h>

#include <OgreStringConverter.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OISException.h>

#define CAMERA_NAME "Camera1"
#define CAMERA_POS_1 "CamNode1"
#define CAMERA_POS_2 "CamNode2"
#define CAMERA_POS_3 "CamNode3"
#define CAMERA_POS_4 "CamNode4"

using namespace Ogre;

namespace trissa
{


InputHandlerGame::InputHandlerGame(Ogre::RenderWindow* win, StateManager* stateManager, CEGUI::System* CEGUISystem,
                                    SceneManager* sceneMgr) :
    InputHandler( win, stateManager, CEGUISystem ),
    mSceneMgr( sceneMgr ), mCamera( 0 ), mBoardNode( 0 ), mCameraNode( 0 ), mRaySceneQuery( 0 ), mSelPos( 0 ), mUserInputEnabled(false), mUserInput(0,0,0), mWaitingSelConfirmation( false ),
    mRotateSpeed( 0.5 ), mCameraRotateSpeed( 0.13 ), mMoveSpeed( 1 ), mZoomSpeed( 0.6 ), mZoomSens( 100 ), mZoomPrev ( 0 ),
    mMovingY( false ),
    mNumScreenshots( 0 ) {

    //TODO: create a menu to configure input parameters and set all of them according to this config

    mCamera = mSceneMgr->getCamera(CAMERA_NAME);
    mCameraNode = mCamera->getParentSceneNode();

    mBoardNode = mSceneMgr->getSceneNode("BoardNode");

    mRaySceneQuery = mSceneMgr->createRayQuery(Ray());
    mRaySceneQuery->setQueryMask( POSITION_AVAILABLE_MASK );
}

InputHandlerGame::~InputHandlerGame(){
}

void InputHandlerGame::treatPressingEvents(){
    if( mMovingY ){
        mCameraNode->translate( 0, mMovingY, 0 );
    }
}

// MouseListener
bool InputHandlerGame::mouseMoved(const OIS::MouseEvent &evt) {
    bool ret = InputHandler::mouseMoved(evt);
    //Zoom, using mouse sensibility
    if(evt.state.Z.rel){
        mZoomPrev += evt.state.Z.rel;
        if(abs(mZoomPrev) > mZoomSens){
            mCameraNode->translate( Vector3( 0, 0, -evt.state.Z.rel * mZoomSpeed), Node::TS_LOCAL );
            mZoomPrev = 0;
        }
    }
    else
        mZoomPrev = 0;

    //Mouse free-look
    if( evt.state.buttonDown(OIS::MB_Middle) ){
        mCameraNode->yaw(Degree(-mCameraRotateSpeed * evt.state.X.rel), Node::TS_WORLD);
        mCameraNode->pitch(Degree(-mCameraRotateSpeed * evt.state.Y.rel), Node::TS_LOCAL);
    }

    if (evt.state.buttonDown(OIS::MB_Right) && evt.state.X.rel != 0 ){
        mBoardNode->yaw(Degree(-mRotateSpeed * evt.state.X.rel), Node::TS_WORLD);
    }

    //TODO: catch board position being pointed by mouse

    return ret;
}
bool InputHandlerGame::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
    if(mUserInputEnabled && id == OIS::MB_Left ){
        if(!mWaitingSelConfirmation){
            CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getPosition();
            Ray mouseRay = mCamera->getCameraToViewportRay(mousePos.d_x/float(e.state.width), mousePos.d_y/float(e.state.height));
            mRaySceneQuery->setRay(mouseRay);
            mRaySceneQuery->setSortByDistance(true);
            RaySceneQueryResult &result = mRaySceneQuery->execute();
            for ( RaySceneQueryResult::iterator itr = result.begin( ); itr != result.end(); itr++){
                if (itr->movable && itr->movable != mCamera ){
                    mSelPos = mSceneMgr->getEntity(itr->movable->getName());
                    mSelPos->setMaterialName("Board/Selected");
                    mWaitingSelConfirmation = true;
                    break;
                }
            }
        }
        else {
			{
				boost::lock_guard<boost::mutex> lock(mMutexUserInput);
				mUserInputEnabled = false;
				
				mSelPos->setMaterialName("Board/Glass");
				mWaitingSelConfirmation = false;
			}
			mCondUserInput.notify_one();
		}
    }
    else if(mWaitingSelConfirmation && id == OIS::MB_Right ){
        mWaitingSelConfirmation = false;
        mSelPos->setMaterialName("Board/Glass");
        mSelPos = 0;
    }
    return true;
}
bool InputHandlerGame::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
    return true;
}

// KeyListener
bool InputHandlerGame::keyPressed(const OIS::KeyEvent &e) {
    switch (e.key){
        case OIS::KC_ESCAPE:
			mSelPos = NULL;
			{
				boost::lock_guard<boost::mutex> lock(mMutexUserInput);
				if(mUserInputEnabled || mWaitingSelConfirmation){
					mUserInputEnabled = false;
					mWaitingSelConfirmation = false;
				}
				mCondUserInput.notify_one();
			}
            mStateManager->requestStateChange( GUI );
            break;
        case OIS::KC_1:
            mCamera->getParentSceneNode()->detachObject(mCamera);
            mCameraNode = mSceneMgr->getSceneNode(CAMERA_POS_1);
            mCameraNode->attachObject(mCamera);
            break;
        case OIS::KC_2:
            mCamera->getParentSceneNode()->detachObject(mCamera);
            mCameraNode = mSceneMgr->getSceneNode(CAMERA_POS_2);
            mCameraNode->attachObject(mCamera);
            break;
        case OIS::KC_3:
            mCamera->getParentSceneNode()->detachObject(mCamera);
            mCameraNode = mSceneMgr->getSceneNode(CAMERA_POS_3);
            mCameraNode->attachObject(mCamera);
            break;
        case OIS::KC_4:
            mCamera->getParentSceneNode()->detachObject(mCamera);
            mCameraNode = mSceneMgr->getSceneNode(CAMERA_POS_4);
            mCameraNode->attachObject(mCamera);
            break;
		case OIS::KC_PGUP:
		case OIS::KC_W:
		case OIS::KC_UP:
            mMovingY = mMoveSpeed;
            break;
		case OIS::KC_PGDOWN:
        case OIS::KC_DOWN:
        case OIS::KC_S:
			mMovingY = -mMoveSpeed;
			break;
        case OIS::KC_SYSRQ:  // take a screenshot
        case OIS::KC_P:
			{
			    std::ostringstream ss;
                ss << "../screenshots/screenshot_" << mNumScreenshots++ << ".png";
                mWindow->writeContentsToFile(ss.str());
			}

        default:
            break;
    }
    return true;
}

Entity* InputHandlerGame::getUserInput()
{
	{
		boost::unique_lock<boost::mutex> lock(mMutexUserInput);
		mUserInputEnabled = true;
		while(mUserInputEnabled){
			mCondUserInput.wait(lock);
		}
	}
	return mSelPos; // == NULL if user canceled
}
Entity* InputHandlerGame::getSel()
{
	return mSelPos;
}

bool InputHandlerGame::keyReleased(const OIS::KeyEvent &e) {
    switch( e.key ){
		case OIS::KC_PGUP:
		case OIS::KC_W:
		case OIS::KC_UP:
		case OIS::KC_PGDOWN:
        case OIS::KC_DOWN:
        case OIS::KC_S:
            mMovingY = 0;
            break;
        default:
            break;
    }
    return true;
}

}
