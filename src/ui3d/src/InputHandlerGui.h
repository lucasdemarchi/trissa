/*
 * InputHandlerGui.h
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

#ifndef _TRISSA_INPUTHANDLERGUI_H_
#define _TRISSA_INPUTHANDLERGUI_H_ 1

#include "InputHandler.h"

#include <OISEvents.h>

#include <OgreRenderWindow.h>
#include <CEGUISystem.h>

class StateManager;

class InputHandlerGui : public InputHandler {
public:
    InputHandlerGui(Ogre::RenderWindow* win, StateManager* stateManager, CEGUI::System* CEGUISystem);
    ~InputHandlerGui();
    bool mouseMoved(const OIS::MouseEvent &e);
    bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    bool keyPressed(const OIS::KeyEvent &e);
    bool keyReleased(const OIS::KeyEvent &e);
};

#endif //_TRISSA_INPUTHANDLERGUI_H_
