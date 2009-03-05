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

class StateManager;

class InputHandlerGame : public InputHandler {
public:
    InputHandlerGame(Ogre::RenderWindow* win, StateManager* stateManager, CEGUI::System* CEGUISystem, Ogre::SceneManager* sceneMgr);
    ~InputHandlerGame();

    bool mouseMoved(const OIS::MouseEvent &e);
    bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    bool keyPressed(const OIS::KeyEvent &e);
    bool keyReleased(const OIS::KeyEvent &e);

protected:
    Ogre::SceneManager* mSceneMgr;
    Ogre::Camera* mCamera;
    Ogre::SceneNode* mBoardNode;
    Ogre::SceneNode* mCameraNode;
    Ogre::Real mRotateSpeed;
    Ogre::Real mMoveSpeed;
    Ogre::Real mZoomSpeed;
    Ogre::Vector3 mDirection;
};

#endif //_TRISSA_INPUTHANDLERGAME_H_
