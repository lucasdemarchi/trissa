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
#ifndef PLAYER_FACTORY_H__
#define PLAYER_FACTORY_H__
#include <vector>
#include <map>
#include <string>
#include "common.h"


namespace trissa {
	//Forward declaration of player
	class Player;

	//TODO: Put a list with boards player knows how to play into.
	typedef struct Player_details {
		function_creator_ptr player_creator_ptr;
		void * dlib;
	} Player_details;

	class PlayerFactory {
		public:
			PlayerFactory(std::string path);
			~PlayerFactory();
			
			Player * create_player (std::string player_name, int dimension);
			void getPlayersList(std::vector<std::string>& strplayers);
		private:
			std::map<std::string, Player_details> factory;
			std::vector<Player*> created_players;
	};
}
#endif /* PLAYER_FACTORY_H__ */
