/*
 * PlayerFactory.h
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
#ifndef _PLAYER_FACTORY_H_
#define _PLAYER_FACTORY_H_ 1
#include <vector>
#include <map>
#include <string>
#include "common.h"
#include "UIInputOutput.h"

namespace trissa
{
//Forward declaration of player
class Player;

/** Struct that contains details for each player, not instantiated yet
  *
  */
typedef struct Player_details {
	function_creator_ptr player_creator_ptr; /**< Pointer to a function that creates a new player*/
	void * dlib; /** Pointer to dynamic lybrary loaded*/
} Player_details;

class PlayerFactory
{
public:

	/** Constructor
	  * @remarks A call to loadPlayerLibraries need to be made before trying to create players
	  */
	PlayerFactory();
	/** Constructor
	  * @remarks Automatically load Player's libraries at path
	  * @param path The path where to look for players
	  */
	PlayerFactory(std::string path);

	/** Destructor
	  * @remarks Call unloadPlayerLibraries and close all dynamic libraries
	  */
	~PlayerFactory();

	void loadPlayerLibraries(std::string path);
	void unloadPlayerLibraries();

	/** Factory function
	  * @remarks It creates a player using function pointer player_creator_ptr of the player specified by player_name
	  * @param player_name The Player to be created
	  * @param dimension The dimension of the game to be passed to player (just to check if it can play in this board)
	  * @param player_type Cross or Circle
	  * @param ui Interface of player with UI
	  */
	Player * create_player (std::string player_name, int dimension, PlayerType player_type, UIInputOutput* ui);

	/** Go through all created Players and call their destructors
	  * @remarks It doesn't actually unload dynamic libraries. It just "uninstantiate" all Players
	  */
	void destroyPlayers();

	/** Get list of known players
	  * @param strplayers A vector of strings in which will be put all Player's names
	  */
	void getPlayersList(std::vector<std::string>& strplayers) const;

	/** Get the first player
	  * @return The first player loaded or ""
	  */
	std::string getFirstPlayer() const;
private:
	std::map<std::string, Player_details> factory; /**< A map between each Player's name and its Player_details*/
	/** A vector carrying pointers to all created Players (since it's a 2 players game, it should have 2 or less elements*/
	std::vector<Player*> created_players;
};
}
#endif /* _PLAYER_FACTORY_H_ */
