#include "input.h"
#include "state_manager.h"
#include <OgreStringConverter.h>
#include <OISException.h>


using namespace Ogre;

InputHandler::InputHandler(Ogre::RenderWindow* win, StateManager* stateManager, CEGUI::System* CEGUISystem) :  //, Ogre::SceneManager* mgr, Ogre::Camera* cam) :
    mWindow( win ),
//    mSceneMgr( mgr ),
    mStateManager( stateManager ),
//    mCamera( cam ),
//    mRotate( 0.4 ),
//    mMove( 250 ),
//    mDirection( Vector3::ZERO )
    mCEGUISystem ( CEGUISystem ){

    setupInputSystem();
    windowResized( mWindow );

    //WindowEventUtilities::addWindowEventListener( mWindow, this );
}

InputHandler::~InputHandler(){
    //WindowEventUtilities::removeWindowEventListener( mWindow, this );
    //windowClosed( mWindow );
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
    // MouseListener
bool InputHandler::mouseMoved(const OIS::MouseEvent &evt) {
    //mCEGUISystem->injectMouseWheelChange(evt.state.Z.rel);
	return mCEGUISystem->injectMouseMove(evt.state.X.rel, evt.state.Y.rel);


//    mCamNode->translate( Vector3( 0, 0, -e.state.Z.rel * 0.6 ), Node::TS_LOCAL );
//    SceneNode* camNode = mCamera->getParentSceneNode();
//    camNode->translate( Vector3( 0, 0, -e.state.Z.rel * 0.6 ), Node::TS_LOCAL );
//
//    if (e.state.buttonDown(OIS::MB_Right)){
//        SceneNode* ninjaNode = mSceneMgr->getSceneNode("NinjaNode");
//        ninjaNode->yaw(Degree(-mRotate * e.state.X.rel), Node::TS_WORLD);
//    }
//  return true;
}
bool InputHandler::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
	return mCEGUISystem->injectMouseButtonDown(convertButton(id));


    //CEGUIsys->injectMouseButtonDown(convertButton(id));
    return true;
}
bool InputHandler::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
    mCEGUISystem->injectMouseButtonUp(convertButton(id));
    return true;
}

// KeyListener
bool InputHandler::keyPressed(const OIS::KeyEvent &e) {
    //SceneNode* camNode = 0;
    switch (e.key)
    {
        case OIS::KC_ESCAPE:
            mStateManager->requestStateChange( SHUTDOWN );
            break;
//        case OIS::KC_1:
//            mCamera->getParentSceneNode()->detachObject(mCamera);
//            camNode = mSceneMgr->getSceneNode("CamNode1");
//            camNode->attachObject(mCamera);
//            break;
//        case OIS::KC_2:
//            mCamera->getParentSceneNode()->detachObject(mCamera);
//            camNode = mSceneMgr->getSceneNode("CamNode2");
//            camNode->attachObject(mCamera);
//            break;
        default:
            break;
    }
    mCEGUISystem->injectKeyDown(e.key);
    mCEGUISystem->injectChar(e.text);
    return true;
}
bool InputHandler::keyReleased(const OIS::KeyEvent &e) {
    mCEGUISystem->injectKeyUp(e.key);
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


//
//bool InputHandler::windowClosing(RenderWindow* rw){
//    if( rw == mWindow){
//        mContinue = false;
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
