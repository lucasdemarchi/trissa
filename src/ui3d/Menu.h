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

class Menu {
public:
    Menu(CEGUI::System* CEGUISystem, CEGUI::Window* pSheet, StateManager* stateManager, Menu* father);
    virtual ~Menu();
    CEGUI::Window* getWindow();

protected:

    CEGUI::System* mCEGUISystem;
    CEGUI::Window* mWindow;
    StateManager* mStateManager;
    Menu* mFather;
};

}

#endif
