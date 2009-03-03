//#include <OIS/OIS.h>
#ifndef _TRISSA_INPUT_H_
#define _TRISSA_INPUT_H_ 1
#include "OISEvents.h"
#include "OISInputManager.h"
#include "OISMouse.h"
#include "OISKeyboard.h"

#include <OgreRenderWindow.h>
#include <OgreCamera.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>

#include <CEGUISystem.h>

class StateManager;

class InputHandler :
            public OIS::MouseListener,
            public OIS::KeyListener {
            //public WindowEventListener
public:
    InputHandler(Ogre::RenderWindow* win, StateManager* stateManager, CEGUI::System* CEGUISystem);
    ~InputHandler();

    void capture();
    bool mouseMoved(const OIS::MouseEvent &e);
    bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    bool keyPressed(const OIS::KeyEvent &e);
    bool keyReleased(const OIS::KeyEvent &e);
    void windowResized(Ogre::RenderWindow* rw);

protected:
    void setupInputSystem();

    CEGUI::MouseButton convertButton(OIS::MouseButtonID btn);

    Ogre::RenderWindow* mWindow;
    Ogre::SceneManager* mSceneMgr;
    StateManager* mStateManager;

    OIS::InputManager* mInputManager;
    OIS::Keyboard* mKeyboard;
    OIS::Mouse* mMouse;

    CEGUI::System* mCEGUISystem;
//    Ogre::Camera* mCamera;
//
//    Ogre::Real mRotate;
//    Ogre::Real mMove;
//    Ogre::Vector3 mDirection;

};

#endif //_TRISSA_INPUT_H_
