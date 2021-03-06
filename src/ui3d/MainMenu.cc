/*
 * MainMenu.cc
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

#include "MainMenu.h"
#include "AboutMenu.h"
#include "SettingsMenu.h"
#include "StateManager.h"


#include <CEGUISystem.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <elements/CEGUIPushButton.h>

#include <OgreTexture.h>

namespace trissa
{

MainMenu::MainMenu(CEGUI::System* CEGUISystem, StateManager* stateManager,
		ConfigManager* configManager) :
    Menu(CEGUISystem, stateManager, configManager,
	CEGUI::WindowManager::getSingleton().loadWindowLayout("welcome.layout"), 0),
	mSettingsMenu(0)
{
	//Connect button events
	CEGUI::PushButton* pPlayButton =
		(CEGUI::PushButton *)CEGUI::WindowManager::getSingleton().getWindow("cmdPlay");
	pPlayButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&MainMenu::play_OnClick, this));


	CEGUI::PushButton* pOptionsButton =
		(CEGUI::PushButton *)CEGUI::WindowManager::getSingleton().getWindow("cmdOptions");
	pOptionsButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&MainMenu::options_OnClick, this));

	CEGUI::PushButton* pQuitButton =
		(CEGUI::PushButton *)CEGUI::WindowManager::getSingleton().getWindow("cmdQuit");
	pQuitButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&MainMenu::quit_OnClick, this));

	CEGUI::PushButton* pAboutButton =
		(CEGUI::PushButton *)CEGUI::WindowManager::getSingleton().getWindow("cmdAbout");
	pAboutButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&MainMenu::about_OnClick, this));

	//create sons
	mSettingsMenu = new SettingsMenu(this);
	mAboutMenu = new AboutMenu(this);

	//make me visible
	mCEGUISystem->setGUISheet(mWindow);
}
MainMenu::~MainMenu()
{
    delete mSettingsMenu;
	delete mAboutMenu;
}

bool MainMenu::quit_OnClick( const CEGUI::EventArgs &args )
{
    mStateManager->requestStateChange(SHUTDOWN);
    return true;
}

bool MainMenu::options_OnClick( const CEGUI::EventArgs &args )
{
    mCEGUISystem->setGUISheet( mSettingsMenu->getWindow() );
    return true;
}
bool MainMenu::play_OnClick(const CEGUI::EventArgs &args)
{
    mStateManager->requestStateChange(GAME);
    return true;
}

bool MainMenu::about_OnClick(const CEGUI::EventArgs &args)
{
	mCEGUISystem->setGUISheet( mAboutMenu->getWindow() );
	return true;
}

}
