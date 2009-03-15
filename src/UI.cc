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

namespace trissa {
	using namespace std;

	UI::UI(ConfigManager* cm, PlayerFactory* pf, StateManager* sm) :
        mCm( cm ),
        mPf( pf ),
        mSm( sm ){

        // CONFIGURE STUFF: in a Text UI, could be only "linking" to an output text file
        //                  in a 3D UI, must configure the window, renderer system, etc
        mSm->requestStateChange(GUI);
	}

    void UI::configure(){
        // If it's monothreaded, it must remain here till changing state
        //TODO: interact with ConfigManager and PlayerFactory to change options
        mSm->requestStateChange(GAME);
    }

    void UI::start() {
        // Finish loading resources and, render stuff
    }

	UI::~UI(){
	}

	void UI::refresh(Cube const& board, Move const& lastMove){

	}

	void UI::refresh(Cube const& board, Move const& lastMove, bool wait){

	}

}
