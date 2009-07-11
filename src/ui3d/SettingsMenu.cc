/*
 * SettingsMenu.cc
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

#include "SettingsMenu.h"
#include "StateManager.h"
#include "MainMenu.h"

#include <CEGUISystem.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <elements/CEGUIPushButton.h>


namespace trissa
{

SettingsMenu::SettingsMenu(Menu* father) :
    Menu(CEGUI::WindowManager::getSingleton().loadWindowLayout("settings.layout"),
			father )
{

	CEGUI::PushButton* pOkButton =
		(CEGUI::PushButton *)CEGUI::WindowManager::getSingleton().getWindow("cmdOk");
	pOkButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&SettingsMenu::ok_OnClick, this));
	
	CEGUI::PushButton* pCancelButton =
		(CEGUI::PushButton*) CEGUI::WindowManager::getSingleton().getWindow("cmdCancel");
	pCancelButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&SettingsMenu::cancel_OnClick, this));

}

bool SettingsMenu::cancel_OnClick(const CEGUI::EventArgs &args){
    mCEGUISystem->setGUISheet( mFather->getWindow() );
    return true;
}

bool SettingsMenu::ok_OnClick(const CEGUI::EventArgs &args){
	//Use ConfigManager to save changes
	

    mCEGUISystem->setGUISheet( mFather->getWindow() );
    return true;
}

}
