/*
 * InputHandler.cc
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
#include "StateManager.h"
#include <OgreStringConverter.h>
#include <OISException.h>


using namespace Ogre;

InputHandler::InputHandler(Ogre::RenderWindow* win, StateManager* stateManager, CEGUI::System* CEGUISystem) :
    mWindow( win ),
    mStateManager( stateManager ),
    mCEGUISystem ( CEGUISystem ){

    setupInputSystem();
    windowResized( mWindow );

    //WindowEventUtilities::addWindowEventListener( mWindow, this );
}

InputHandler::~InputHandler(){
    if( mInputManager ){
        mInputManager->destroyInputObject(mMouse);
        mInputManager->destroyInputObject(mKeyboard);

        OIS::InputManager::destroyInputSystem( mInputManager );
        mInputManager = 0; // Because if user clicks on X, it will be called twice
    }
}

void InputHandler::capture(){
    mKeyboard->capture();
    mMouse->capture();
}

void InputHandler::treatPressingEvents(){
}

// MouseListener
bool InputHandler::mouseMoved(const OIS::MouseEvent &evt) {
    return mCEGUISystem->injectMouseMove(evt.state.X.rel, evt.state.Y.rel);
}
bool InputHandler::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
    return true;
}
bool InputHandler::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
    return true;
}

// KeyListener
bool InputHandler::keyPressed(const OIS::KeyEvent &e) {
    return true;
}
bool InputHandler::keyReleased(const OIS::KeyEvent &e) {
    return true;
}

////WindowEventListener
void InputHandler::windowResized(RenderWindow* rw){
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

void InputHandler::setupInputSystem() {
    size_t windowHnd = 0;
    OIS::ParamList pl;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    pl.insert(std::make_pair(std::string("WINDOW"), StringConverter::toString(windowHnd)));

    mInputManager = OIS::InputManager::createInputSystem(pl);

    try {
        mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
        mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
    } catch (const OIS::Exception &e) {
        throw Exception(42, e.eText, "Application::setupInputSystem");
    }
    mKeyboard->setEventCallback( this );
    mMouse->setEventCallback( this );
}

CEGUI::MouseButton InputHandler::convertButton(OIS::MouseButtonID btn){
    switch (btn)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;

    case OIS::MB_Right:
        return CEGUI::RightButton;

    case OIS::MB_Middle:
        return CEGUI::MiddleButton;

    default:
        return CEGUI::LeftButton;
    }

}

//bool InputHandler::windowClosing(RenderWindow* rw){
//    if( rw == mWindow){
//        mStateManager->requestStateChange( SHUTDOWN );
//        return false;
//    }
//    return true;
//}
//
//void InputHandler::windowClosed(RenderWindow* rw){
//    if( rw == mWindow ){
//        if( mInputManager ){
//            mInputManager->destroyInputObject(mMouse);
//            mInputManager->destroyInputObject(mKeyboard);
//
//            OIS::InputManager::destroyInputSystem( mInputManager );
//            mInputManager = 0; // Because if user clicks on X, it will be called twice
//        }
//    }
//}
