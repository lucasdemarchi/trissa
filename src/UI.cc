/*
 * UI.cc
 * This file is part of Trissa
 *
 * Copyright (C) 2008 - Lucas De Marchi
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
#include "UI.h"
#include "ConfigManager.h"
#include "PlayerFactory.h"
#include "StateManager.h"
#include <cstring>
#include <string>

namespace trissa
{
using namespace std;

UI::UI(ConfigManager* cm, StateManager* sm, PlayerFactory const* pf) :
		mCm( cm ),
		mSm( sm ),
		mPf( pf )
{

	// CONFIGURE STUFF: in a Text UI, could be only "linking" to an output text file
	//                  in a 3D UI, must configure the window, renderer system, etc
	mSm->requestStateChange(GUI);
}

void UI::configure()
{
	// sub-classes must override this method and after all work is done call
	// UI::configure() to make this requestStateChange
	mSm->requestStateChange(GAME);
}

bool UI::gameOver()
{
	return true;
}

UI::~UI()
{
}

}
