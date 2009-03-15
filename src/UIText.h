/*
 * UIText.h
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
#ifndef _UITEXT_H_
#define _UITEXT_H_ 1

#include "UI.h"

namespace trissa {
    class UIText : public UI {
    public:
        UIText(ConfigManager* cm, PlayerFactory* pf, StateManager* sm) : UI(cm, pf, sm){ }
        void configure();
        void start();
        void refresh(Cube const& board, Move const& lastMove, bool wait);
        void setPos(Move m, PlayerType player);
    private:
        bool confirmConfig();
        unsigned int getDimension();
        std::string getPlayer(std::string player);

        void refresh(Cube const& board, Move const& lastMove);
    };
}

#endif /*_UITEXT_H_*/
