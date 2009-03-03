#include "MainMenu.h"
#include "SettingsMenu.h"
#include "state_manager.h"


#include <CEGUISystem.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <elements/CEGUIPushButton.h>

MainMenu::MainMenu(CEGUI::System* CEGUISystem, CEGUI::Window* pSheet, StateManager* stateManager, Menu* father) :
    Menu ( CEGUISystem, pSheet, stateManager, 0 ),
    mSettingsMenu( 0 ) {

	CEGUI::PushButton* pPlayButton = (CEGUI::PushButton *)CEGUI::WindowManager::getSingleton().getWindow("cmdPlay");
	pPlayButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::play_OnClick, this));

	CEGUI::PushButton* pOptionsButton = (CEGUI::PushButton *)CEGUI::WindowManager::getSingleton().getWindow("cmdOptions");
	pOptionsButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::options_OnClick, this));

	CEGUI::PushButton* pQuitButton = (CEGUI::PushButton *)CEGUI::WindowManager::getSingleton().getWindow("cmdQuit");
	pQuitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::quit_OnClick, this));


    CEGUI::Window* pLayout = 0;
    pLayout = CEGUI::WindowManager::getSingleton().loadWindowLayout("settings.layout");

	mSettingsMenu = new SettingsMenu(mCEGUISystem, pLayout, mStateManager, this );
}
MainMenu::~MainMenu(){
    delete mSettingsMenu;
}

bool MainMenu::quit_OnClick( const CEGUI::EventArgs &args ){
    mStateManager->requestStateChange(SHUTDOWN);
    return true;
}

bool MainMenu::options_OnClick( const CEGUI::EventArgs &args ){
    mCEGUISystem->setGUISheet( mSettingsMenu->getWindow() );
    return true;
}
bool MainMenu::play_OnClick(const CEGUI::EventArgs &args){
    return true;
}
