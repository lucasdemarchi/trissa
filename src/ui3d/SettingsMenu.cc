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
#include <elements/CEGUIListboxTextItem.h>
#include <elements/CEGUIListbox.h>
#include <elements/CEGUISpinner.h>

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

	loadConfig();

}

bool SettingsMenu::cancel_OnClick(const CEGUI::EventArgs &args){
    mCEGUISystem->setGUISheet( mFather->getWindow() );
    return true;
}

bool SettingsMenu::ok_OnClick(const CEGUI::EventArgs &args){
	//Use ConfigManager to save changes
	CEGUI::WindowManager &wmng = CEGUI::WindowManager::getSingleton();
	CEGUI::ListboxItem* playerA = static_cast<CEGUI::Listbox*>
		(wmng.getWindow("lstPlayerA"))->getFirstSelectedItem();
	CEGUI::ListboxItem* playerB = static_cast<CEGUI::Listbox*>
		(wmng.getWindow("lstPlayerB"))->getFirstSelectedItem();
	int dimension = static_cast<int>(static_cast<CEGUI::Spinner*>
		(wmng.getWindow("txtDimension"))->getCurrentValue());
	
	if (playerA == NULL){
		if(static_cast<CEGUI::Listbox*>
				(wmng.getWindow("lstPlayerA"))->getItemCount() > 0){

			std::cerr << "No player A selected, using the first one" << std::endl;
			playerA = static_cast<CEGUI::Listbox*>
				(wmng.getWindow("lstPlayerA"))->getListboxItemFromIndex(0);

		}
		else{
			std::cerr << "No player to select for player A" << std::endl;
			return true;
		}
	}
	if (playerB == NULL){
		if(static_cast<CEGUI::Listbox*>
				(wmng.getWindow("lstPlayerB"))->getItemCount() > 0){

			std::cerr << "No player B selected, using the first one" << std::endl;
			playerB = static_cast<CEGUI::Listbox*>
				(wmng.getWindow("lstPlayerB"))->getListboxItemFromIndex(0);

		}
		else{
			std::cerr << "No player to select for player A" << std::endl;
			return true;
		}
	}

	mConfigManager->setDimension(dimension);
	mConfigManager->setPlayerA(std::string(playerA->getText().c_str()));
	mConfigManager->setPlayerB(std::string(playerB->getText().c_str()));

    mCEGUISystem->setGUISheet( mFather->getWindow() );
    return true;
}

void SettingsMenu::populatePlayers()
{
	std::vector<std::string> strplayers;
	std::vector<std::string>::iterator iter;
	CEGUI::WindowManager &wmng = CEGUI::WindowManager::getSingleton();

	mConfigManager->getPlayersList(strplayers);
	CEGUI::Listbox* lstA = (CEGUI::Listbox*) wmng.getWindow("lstPlayerB");
	CEGUI::Listbox* lstB = (CEGUI::Listbox*) wmng.getWindow("lstPlayerA");


	lstA->resetList();
	lstB->resetList();

	for(iter = strplayers.begin(); iter != strplayers.end(); iter++){
		CEGUI::ListboxTextItem* playerA =
			new CEGUI::ListboxTextItem((CEGUI::utf8*)(*iter).c_str());
		CEGUI::ListboxTextItem* playerB =
			new CEGUI::ListboxTextItem((CEGUI::utf8*)(*iter).c_str());
		playerA->setSelectionBrushImage("QuadraticLook", "White");
		playerA->setSelectionColours(CEGUI::colour(0.0,0.5,0.8));

		playerB->setSelectionBrushImage("QuadraticLook", "White");
		playerB->setSelectionColours(CEGUI::colour(0.0,0.5,0.8));
		playerB->setSelected(false);
		lstA->addItem(playerA);
		lstB->addItem(playerB);

		std::cout << *iter << std::endl;
		bool sel = (mConfigManager->getPlayerA() == (*iter));
		playerA->setSelected(sel);
		sel = (mConfigManager->getPlayerB() == (*iter));
		playerB->setSelected(sel);
	}
	if(lstA->getItemCount() > 0 && lstA->getSelectedCount() == 0)
		lstA->setItemSelectState((size_t)0, true);

	if(lstB->getItemCount() > 0 && lstB->getSelectedCount() == 0)
		lstB->setItemSelectState((size_t)0, true);

}
void SettingsMenu::loadConfig()
{
	populatePlayers();
	
	//Dimension
	CEGUI::WindowManager &wmng = CEGUI::WindowManager::getSingleton();
	int dimension = mConfigManager->getDimension();
	static_cast<CEGUI::Spinner*>
		(wmng.getWindow("txtDimension"))->setCurrentValue(dimension);
}

}
