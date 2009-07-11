/*
 * Menu.cc
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

#include "Menu.h"
#include "StateManager.h"

#include <CEGUISystem.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>

namespace trissa
{

Menu::Menu(CEGUI::System* CEGUISystem, StateManager* stateManager,
		ConfigManager* configManager, CEGUI::Window* window, Menu* father) :
	mWindow(window), mFather( father )
{
	mCEGUISystem = CEGUISystem;
	mStateManager = stateManager;
	mConfigManager = configManager;
}

Menu::Menu(CEGUI::Window* window, Menu* father) :
	mWindow(window), mFather( father )
{

}

Menu::~Menu()
{
	CEGUI::WindowManager::getSingleton().destroyWindow(mWindow);
}

CEGUI::Window* Menu::getWindow(){
    return mWindow;
}

CEGUI::System* Menu::mCEGUISystem = 0;
StateManager* Menu::mStateManager = 0;
ConfigManager* Menu::mConfigManager = 0;

}
