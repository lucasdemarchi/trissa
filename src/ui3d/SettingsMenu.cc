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
	CEGUI::WindowManager &wmng = CEGUI::WindowManager::getSingleton();

	mConfigManager->getPlayersList(strplayers);
	CEGUI::Combobox* comboPlayerA = (CEGUI::Combobox*) wmng.getWindow("comboPlayerA");
	CEGUI::Combobox* comboPlayerB = (CEGUI::Combobox*) wmng.getWindow("comboPlayerB");

//	comboPlayerA->resetList();
	comboPlayerB->resetList();
	//CEGUI::ListboxTextItem *lb = (CEGUI::ListboxTextItem*) wmng.createWindow("QuadraticLook/ListboxItem", "test");
	//comboPlayerA->addItem(lb);

        CEGUI::Combobox* objectComboBox = (CEGUI::Combobox*)CEGUI::WindowManager::getSingleton().getWindow("comboPlayerA");

        CEGUI::ListboxTextItem* item = new CEGUI::ListboxTextItem((CEGUI::utf8*)"FrameWindow", 0);
        objectComboBox->addItem(item);
        item = new CEGUI::ListboxTextItem((CEGUI::utf8*)"Horizontal Scrollbar", 1);
        objectComboBox->addItem(item);
        item = new CEGUI::ListboxTextItem((CEGUI::utf8*)"Vertical Scrollbar", 2);
        objectComboBox->addItem(item);
        item = new CEGUI::ListboxTextItem((CEGUI::utf8*)"StaticText", 3);
        objectComboBox->addItem(item);
        item = new CEGUI::ListboxTextItem((CEGUI::utf8*)"StaticImage", 4);
        objectComboBox->addItem(item);
        item = new CEGUI::ListboxTextItem((CEGUI::utf8*)"Render to Texture", 5);
        objectComboBox->addItem(item);


	for(iter = strplayers.begin(); iter != strplayers.end(); iter++){
		CEGUI::ListboxTextItem* player;
		player = new CEGUI::ListboxTextItem(*iter);
		comboPlayerA->addItem(player);

//		comboPlayerA->addItem(&lb);
//		comboPlayerB->addItem(&lb);
		std::cout << *iter << std::endl;
	}

	comboPlayerA->handleUpdatedListItemData();
}

}
