/*
 * InputHandler.h
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

#ifndef _TRISSA_INPUTHANDLER_H_
#define _TRISSA_INPUTHANDLER_H_ 1

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISMouse.h>
#include <OISKeyboard.h>

#include <OgreRenderWindow.h>
#include <OgreCamera.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>

#include <CEGUISystem.h>

namespace trissa
{

class StateManager;
class InputHandler :
            public OIS::MouseListener,
            public OIS::KeyListener {
public:
    InputHandler(Ogre::RenderWindow* win, StateManager* stateManager, CEGUI::System* CEGUISystem);
    virtual ~InputHandler();

    void capture();
    virtual void treatPressingEvents();
    virtual bool mouseMoved(const OIS::MouseEvent &e);
    virtual bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    virtual bool keyPressed(const OIS::KeyEvent &e);
    virtual bool keyReleased(const OIS::KeyEvent &e);
    void windowResized(Ogre::RenderWindow* rw);

protected:
    void setupInputSystem();
    CEGUI::MouseButton convertButton(OIS::MouseButtonID btn);

    Ogre::RenderWindow* mWindow;
    StateManager* mStateManager;

    OIS::InputManager* mInputManager;
    OIS::Keyboard* mKeyboard;
    OIS::Mouse* mMouse;

    CEGUI::System* mCEGUISystem;
};
}
#endif //_TRISSA_INPUT_H_
