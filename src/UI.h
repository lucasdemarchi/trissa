/*
 * UI.h
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
#ifndef UI_H__
#define UI_H__
#include <vector>
#include <string>
#include <iostream>
#include "common.h"

namespace trissa {
    class ConfigManager;
    class PlayerFactory;
    class StateManager;
    //TODO: allow changing of playersPath in UI
    class UI {

    public:
        UI(ConfigManager* cm, PlayerFactory* pf, StateManager* sm);
        ~UI();

        void configure();
        void start();
        void getPlayers(std::vector<std::string> strplayers, std::string& strplayerA, std::string& strplayerB);
        void refresh(Cube const& board, Move const& lastMove, bool wait);
    private:
        void refresh(Cube const& board, Move const& lastMove);
        ConfigManager* mCm;
        PlayerFactory* mPf;
        StateManager* mSm;
    };
}


#endif /* UI_H__ */
