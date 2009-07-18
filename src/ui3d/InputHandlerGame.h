/*
 * InputHandlerGame.h
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

#ifndef _TRISSA_INPUTHANDLERGAME_H_
#define _TRISSA_INPUTHANDLERGAME_H_ 1

#include "InputHandler.h"
#include "common.h"
//OIS includes
#include <OISEvents.h>
//Ogre Includes
#include <OgreRenderWindow.h>
#include <OgreCamera.h>
#include <OgreSceneManager.h>
#include <OgrePrerequisites.h>
#include <OgreVector3.h>
//CEGUI includes
#include <CEGUISystem.h>



#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

namespace trissa
{

class StateManager;

class InputHandlerGame : public InputHandler {
public:

    enum QueryFlags {
        POSITION_AVAILABLE_MASK = 1 << 0,
        POSITION_OCCUPIED_MASK = 1 << 1,
        PLANE_MASK = 1 << 2,
        CAMERA_MASK = 1 << 3,
        LIGHT_MASK = 1 << 4,
        STICK_MASK = 1 << 5,
        BALL_MASK = 1 << 6
    };

    InputHandlerGame(Ogre::RenderWindow* win, StateManager* stateManager, CEGUI::System* CEGUISystem, Ogre::SceneManager* sceneMgr);
    ~InputHandlerGame();
    void treatPressingEvents();
    bool mouseMoved(const OIS::MouseEvent &e);
    bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    bool keyPressed(const OIS::KeyEvent &e);
    bool keyReleased(const OIS::KeyEvent &e);

	Ogre::Entity* getUserInput();

protected:
    Ogre::SceneManager* mSceneMgr;
    Ogre::Camera* mCamera;
    Ogre::SceneNode* mBoardNode;
    Ogre::SceneNode* mCameraNode;

    //Selection Variables
    Ogre::RaySceneQuery *mRaySceneQuery;
    Ogre::Entity *mSelPos;
	bool mUserInputEnabled;
	boost::mutex mMutexUserInput;
	boost::condition_variable mCondUserInput;
	Move mUserInput;

    bool mWaitingSelConfirmation;



    Ogre::Real mRotateSpeed;
    Ogre::Real mCameraRotateSpeed;
    Ogre::Real mMoveSpeed;
    Ogre::Real mZoomSpeed;
    Ogre::Real mZoomSens;
    Ogre::Real mZoomPrev;
    Ogre::Real mMovingY;

    //Eye-candy
    int mNumScreenshots;

    //Ogre::Vector3 mDirection;

};

}
#endif //_TRISSA_INPUTHANDLERGAME_H_
