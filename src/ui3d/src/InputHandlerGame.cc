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
#include <OISException.h>

#define CAMERA_NAME "Camera1"
#define CAMERA_POS_1 "CamNode1"
#define CAMERA_POS_2 "CamNode2"
#define CAMERA_POS_3 "CamNode3"
#define CAMERA_POS_4 "CamNode4"

using namespace Ogre;

InputHandlerGame::InputHandlerGame(Ogre::RenderWindow* win, StateManager* stateManager, CEGUI::System* CEGUISystem,
                                    SceneManager* sceneMgr) :
    InputHandler( win, stateManager, CEGUISystem ),
    mSceneMgr( sceneMgr ),
    mCamera( 0 ),
    mRotateSpeed( 0.5 ),
    mMoveSpeed( 0.3 ),
    mZoomSpeed( 0.6 ) {
    //TODO: change mouse speed according to configuration
    mCamera = mSceneMgr->getCamera(CAMERA_NAME);
    //if( mCamera != 0 && mCamera->isAttached() )
        mCameraNode = mCamera->getParentSceneNode();
    //else
      //  throw std::exception();

    mBoardNode = mSceneMgr->getSceneNode("BoardNode");

}

InputHandlerGame::~InputHandlerGame(){
}

// MouseListener
bool InputHandlerGame::mouseMoved(const OIS::MouseEvent &evt) {
    bool ret = InputHandler::mouseMoved(evt);
    if(evt.state.Z.rel != 0)
        mCameraNode->translate( Vector3( 0, 0, -evt.state.Z.rel * mZoomSpeed), Node::TS_LOCAL );

    if (evt.state.buttonDown(OIS::MB_Right) && evt.state.X.rel != 0 ){
        mBoardNode->yaw(Degree(-mRotateSpeed * evt.state.X.rel), Node::TS_WORLD);
    }

    //TODO: catch board position being pointed by mouse

    return ret;
}
bool InputHandlerGame::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
    return true;
}
bool InputHandlerGame::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
    return true;
}

// KeyListener
bool InputHandlerGame::keyPressed(const OIS::KeyEvent &e) {
    switch (e.key){
        case OIS::KC_ESCAPE:
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
        default:
            break;
    }
    return true;
}
bool InputHandlerGame::keyReleased(const OIS::KeyEvent &e) {
    return true;
}
