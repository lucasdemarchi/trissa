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

SettingsMenu::SettingsMenu(CEGUI::System* CEGUISystem, CEGUI::Window* pSheet, StateManager* stateManager, Menu* father) :
    Menu( CEGUISystem, pSheet, stateManager, father ) {

	CEGUI::PushButton* pPlayButton = (CEGUI::PushButton *)CEGUI::WindowManager::getSingleton().getWindow("cmdOk");
	pPlayButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SettingsMenu::ok_OnClick, this));

}

bool SettingsMenu::ok_OnClick(const CEGUI::EventArgs &args){
    mCEGUISystem->setGUISheet( mFather->getWindow() );
    return true;
}

}
