/*
 * Menu.h
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

#ifndef _TRISSA_MENU_H
#define _TRISSA_MENU_H 1


#include "CEGUIWindow.h"

namespace CEGUI
{
	class System;
	class Window;
}

namespace trissa
{

class StateManager;
class ConfigManager;

class Menu {
public:
    Menu(CEGUI::System* CEGUISystem, StateManager* stateManager,
			ConfigManager* configManager, CEGUI::Window* window, Menu* father);
	Menu(CEGUI::Window* window, Menu* father);
    virtual ~Menu();
    CEGUI::Window* getWindow();

protected:

	//shared by all Menus
	static CEGUI::System* mCEGUISystem;
	static StateManager* mStateManager;
	static ConfigManager* mConfigManager;

	CEGUI::Window* mWindow;
    Menu* mFather;
};

}

#endif
