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
#ifndef _UI_H_
#define _UI_H_ 1
#include <vector>
#include <string>
#include <iostream>

#include "common.h"
#include "UIInputOutput.h"

namespace trissa
{
class ConfigManager;
class PlayerFactory;
class StateManager;

class UI : public UIInputOutput
{

public:
	UI(ConfigManager* cm, StateManager* sm, PlayerFactory const* pf);
	virtual ~UI();

	virtual void configure();
	virtual void start(Cube const& board) = 0;
	virtual void start_thread() = 0;
	virtual void setPos(Move const& m, PlayerType player) = 0;

	virtual void gameOver(Move const& startPosition, Move const& direction, PlayerType winner) = 0;

protected:
	ConfigManager* mCm;
	StateManager* mSm;
	PlayerFactory const* mPf;
};
}


#endif /* _UI_H_ */
