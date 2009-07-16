/*
 * AboutMenu.h
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

#ifndef _TRISSA_ABOUTMENU_H
#define _TRISSA_ABOUTMENU_H

#include "Menu.h"
#include <CEGUIWindow.h>

namespace CEGUI
{
	class System;
	class Window;
}

namespace trissa
{

class AboutMenu : public Menu{
public:
    AboutMenu(Menu* father);
	bool ok_OnClick(const CEGUI::EventArgs &args);
};

}
#endif
