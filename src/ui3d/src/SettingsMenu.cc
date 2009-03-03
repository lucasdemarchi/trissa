#include "SettingsMenu.h"
#include "state_manager.h"
#include "MainMenu.h"

#include <CEGUISystem.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <elements/CEGUIPushButton.h>


SettingsMenu::SettingsMenu(CEGUI::System* CEGUISystem, CEGUI::Window* pSheet, StateManager* stateManager, Menu* father) :
    Menu( CEGUISystem, pSheet, stateManager, father ) {

	CEGUI::PushButton* pPlayButton = (CEGUI::PushButton *)CEGUI::WindowManager::getSingleton().getWindow("cmdOk");
	pPlayButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SettingsMenu::ok_OnClick, this));

}

bool SettingsMenu::ok_OnClick(const CEGUI::EventArgs &args){
    mCEGUISystem->setGUISheet( mFather->getWindow() );
    return true;
}
