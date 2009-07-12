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
#include "ConfigManager.h"
#include "MainMenu.h"

#include <CEGUISystem.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <elements/CEGUIPushButton.h>
#include <elements/CEGUICombobox.h>
#include <elements/CEGUIListboxTextItem.h>

#include <vector>
#include <string>



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

	this->populatePlayers();

}

bool SettingsMenu::cancel_OnClick(const CEGUI::EventArgs &args){
    mCEGUISystem->setGUISheet( mFather->getWindow() );
    return true;
}

bool SettingsMenu::ok_OnClick(const CEGUI::EventArgs &args){
	//Use ConfigManager to save changes
	std::cout << CEGUI::WindowManager::getSingleton().getWindow("comboPlayerA")->getText() << std::endl;
	std::cout << CEGUI::WindowManager::getSingleton().getWindow("comboPlayerB")->getText() << std::endl;
	std::cout << CEGUI::WindowManager::getSingleton().getWindow("txtDimension")->getText() << std::endl;


    mCEGUISystem->setGUISheet( mFather->getWindow() );
    return true;
}

void SettingsMenu::populatePlayers()
{
	std::vector<std::string> strplayers;
	std::vector<std::string>::iterator iter;

	mConfigManager->getPlayersList(strplayers);
	CEGUI::Combobox* comboPlayerA = (CEGUI::Combobox*) CEGUI::WindowManager::getSingleton().getWindow("comboPlayerA");
	CEGUI::Combobox* comboPlayerB = (CEGUI::Combobox*) CEGUI::WindowManager::getSingleton().getWindow("comboPlayerB");

	comboPlayerA->resetList();
	comboPlayerB->resetList();

	for(iter = strplayers.begin(); iter != strplayers.end(); iter++){
		CEGUI::ListboxTextItem lb(*iter);
		comboPlayerA->addItem(&lb);
		comboPlayerB->addItem(&lb);
		std::cout << *iter << std::endl;
	}
}

}
