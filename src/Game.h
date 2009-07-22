/*
 * Game.h
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
#ifndef _GAME_H_
#define _GAME_H_ 1
#include "common.h"
#include "ConfigManager.h"
#include "PlayerFactory.h"
#include "StateManager.h"
#include <string>



namespace trissa
{

//forward declarations

int main (int argc, char * argv[]);

class Player;
class UI;
class ConfigManager;

class Game
{
	//friend int main (int argc, char * argv[]);
public:
	//Game();
	Game(int argc, char *argv[]);
	/** Abc
	  *
	  */
	int startGame();
	~Game();
private:
	int load();
	void configure();
	void run();

	//helper functions
	bool isInsideBoard(Move &m);
	bool isFreePosition(Move &m);

	//PlayerType goalTest() const;
	PlayerType goalTest(Move const& lastMove, PlayerType player_type);
	Player* mPlayerA;
	Player* mPlayerB;
	Cube* mBoard;
	UI* mUi;
	PlayerFactory mPlayerFactory;
	ConfigManager mConfigManager;
	StateManager mStateManager;

	Move mWinnerDirection;
	Move mWinnerStartPos;
	int mDimension;

	static const int n_retry; /**< Number of chances to give a player when it previously returned an invalid position */

};

}

#endif /* _GAME_H_ */
