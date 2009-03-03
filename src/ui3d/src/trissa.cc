#include "input.h"
#include "state_manager.h"
#include "MainMenu.h"

#include <Ogre.h>
#include <OgreWindowEventUtilities.h>

#include <OgreCEGUIRenderer.h>
#include <CEGUISystem.h>
#include <CEGUILogger.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <CEGUISchemeManager.h>

using namespace Ogre;


class Application
{
public:
    void setup(){
        mRoot = new Root();
        mStateManager = new StateManager(STARTUP);

        //Load all resources defined in resources.cfg
        setupResources();

        //Load from ogre.cfg or Call dialog
        setupRenderSystem();

        //Create 1 window to render into.
        mWindow = mRoot->initialise(true, "Trissa 3D UI");

        //Initialize all groups
        ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        setupScene();

        setupCEGUI();

        //mInputHandler = new InputHandler( mWindow, mStateManager, mSceneMgr, mCamera );
        mInputHandler = new InputHandler( mWindow, mStateManager, mCEGUISystem );

        //createScene();
    }
    void go() {
        if( mStateManager->requestStateChange(GUI) == false ) return;

        while(mStateManager->getCurrentState() != SHUTDOWN){
            mInputHandler->capture();
            WindowEventUtilities::messagePump();
            mRoot->renderOneFrame();
        }
    }

    Application() :
        mRoot( 0 ),
        mStateManager( 0 ),
        mWindow( 0 ),
        mSceneMgr( 0 ),
        mCamera( 0 ),
        mViewport( 0 ),
        mInputHandler( 0 ),
        mCEGUIRenderer( 0 ),
        mCEGUISystem( 0 ),
        mMainMenu( 0 ),
        mForceConfigDialogDisplay(false){
    }

    Application( bool forceConfigDialogDisplay )
            : mForceConfigDialogDisplay(forceConfigDialogDisplay) {
    }

    ~Application() {
        delete mMainMenu;
        delete mStateManager;
        delete mRoot;
    }

private:
    Root* mRoot;
    StateManager* mStateManager;
    RenderWindow* mWindow;
    SceneManager* mSceneMgr;
    Camera* mCamera;
    Viewport* mViewport;
    InputHandler *mInputHandler;

    //CEGUI
    CEGUI::OgreCEGUIRenderer* mCEGUIRenderer;
    CEGUI::System* mCEGUISystem;

    MainMenu* mMainMenu;

    bool mForceConfigDialogDisplay;

    inline void setupResources() {
        String secName, typeName, archName;
        ConfigFile cf;
        cf.load("resources.cfg");
        ConfigFile::SectionIterator seci = cf.getSectionIterator();
        while (seci.hasMoreElements()) {
            secName = seci.peekNextKey();
            ConfigFile::SettingsMultiMap *settings = seci.getNext();
            ConfigFile::SettingsMultiMap::iterator i;
            for (i = settings->begin(); i != settings->end(); ++i) {
                typeName = i->first;
                archName = i->second;
                ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
            }
        }
    }

    inline void setupRenderSystem() {
        if (mForceConfigDialogDisplay || !mRoot->restoreConfig()) {
            if (!mRoot->showConfigDialog())
                throw Exception(52, "User canceled the config dialog!", "Application::setupRenderSystem()");
        }
    }

    inline void setupScene() {
        //create SceneManager
        mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "Default SceneManager");

        //create Camera
        mCamera = mSceneMgr->createCamera( "Camera" );
        //mCamera->setPosition( Vector3( 0, 0, 500 ) );
        //mCamera->lookAt( Vector3( 0, 0, 0 ) );
        mCamera->setNearClipDistance(5);

        //create Viewport and attach to mCamera
        Viewport* vp = mWindow->addViewport( mCamera );
        vp->setBackgroundColour( ColourValue( 0, 0, 0 ) );
        mCamera->setAspectRatio( Real( vp->getActualWidth() ) / Real( vp->getActualHeight() ) );

        TextureManager::getSingleton().setDefaultNumMipmaps(5);
    }


    void createScene(){
        mSceneMgr->setAmbientLight(ColourValue(0.25, 0.25, 0.25));

        Entity *ent = 0;

        Plane plane( Vector3::UNIT_Y, 0 );
        MeshManager::getSingleton().createPlane("ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 1500,1500,20,20,true,1,5,5,Vector3::UNIT_Z);
        ent = mSceneMgr->createEntity( "GroundEntity", "ground" );
        mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
        ent->setMaterialName( "BumpyMetal" );
        ent->setCastShadows( false );


        // add the ninja
        ent = mSceneMgr->createEntity("Ninja", "ninja.mesh");
        SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("NinjaNode", Vector3( 0, 50, 0 ) );
        node->attachObject(ent);
        //node->roll( Degree( -90 ) );

        // create the light
        Light *light = mSceneMgr->createLight("Light1");
        light->setType(Light::LT_POINT);
        light->setPosition(Vector3(250, 150, 250));
        light->setDiffuseColour(ColourValue::White);
        light->setSpecularColour(ColourValue::White);

        mCamera->setPosition( Vector3( 0, 50, 500 ) );
        mCamera->lookAt( Vector3( 0, 0, 0 ) );

        // Create the scene node
        node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode1", Vector3(-400, 200, 400));

        // Make it look towards the ninja
        node->yaw(Degree(-45));

        node->attachObject(mCamera);

        // create the second camera node/pitch node
        mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode2", Vector3(0, 200, 400));
    }


    void setupCEGUI() {
        mCEGUIRenderer = new CEGUI::OgreCEGUIRenderer(mWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mSceneMgr);

        // create the root CEGUI class
        mCEGUISystem = new CEGUI::System(mCEGUIRenderer);
        // tell us a lot about what is going on (see CEGUI.log in the working directory)
        CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);

        // use this CEGUI scheme definition (see CEGUI docs for more)
        CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");

        mCEGUISystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
        mCEGUISystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");

        //mStateManager->requestStateChange(GUI);
        CEGUI::Window* pLayout = CEGUI::WindowManager::getSingleton().loadWindowLayout("welcome.layout");
        //CEGUI::Window* pLayout = CEGUI::WindowManager::getSingleton().loadWindowLayout("settings.layout");
        //CEGUI::MouseCursor::getSingleton().setImage(CEGUI::System::getSingleton().getDefaultMouseCursor());
        mCEGUISystem->setGUISheet(pLayout);
        mMainMenu = new MainMenu( mCEGUISystem, pLayout, mStateManager, 0 );

    }

};

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char **argv)
#endif
{
    try {
        Application app;
        app.setup();
        app.go();
    } catch (Exception& e) {
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBoxA(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occurred: %s\n",
                e.getFullDescription().c_str());
#endif
    }

    return 0;
}
