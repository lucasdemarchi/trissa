/*
 * InputHandlerGui.cc
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

#include "InputHandlerGui.h"
#include "StateManager.h"

#include <OISInputManager.h>
#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OgreStringConverter.h>
#include <OISException.h>


using namespace Ogre;

namespace trissa
{

InputHandlerGui::InputHandlerGui(Ogre::RenderWindow* win, StateManager* stateManager, CEGUI::System* CEGUISystem) :
    InputHandler( win, stateManager, CEGUISystem ){

}

InputHandlerGui::~InputHandlerGui(){
}
    // MouseListener
bool InputHandlerGui::mouseMoved(const OIS::MouseEvent &evt) {
    if( evt.state.Z.rel != 0 ) mCEGUISystem->injectMouseWheelChange(evt.state.Z.rel);
    return  mCEGUISystem->injectMouseMove(evt.state.X.rel, evt.state.Y.rel);
}
bool InputHandlerGui::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
    return mCEGUISystem->injectMouseButtonDown(convertButton(id));
}
bool InputHandlerGui::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
    return mCEGUISystem->injectMouseButtonUp(convertButton(id));
}

// KeyListener
bool InputHandlerGui::keyPressed(const OIS::KeyEvent &e) {
    if(e.key == OIS::KC_ESCAPE)
        mStateManager->requestStateChange( SHUTDOWN );
    mCEGUISystem->injectKeyDown(e.key);
    mCEGUISystem->injectChar(e.text);
    return true;
}
bool InputHandlerGui::keyReleased(const OIS::KeyEvent &e) {
    mCEGUISystem->injectKeyUp(e.key);
    return true;
}

}
