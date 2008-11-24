/*
 * Player.cc
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
#include "Player.h"
#include <iostream>
namespace trissa {
			
	PlayerException::PlayerException(int idException, Player*  player): 
			idException(idException),
			player(player){
	}
	const char* PlayerException::what(){
		std::string str_error("Player error: ");
		if (idException == BOARD_NOT_KNOWN) {
			str_error += (player->getName());
			str_error += " doesn't know how to play in board with size ";
			str_error += (player->dimension);
			return str_error.c_str();
		}
		return "Unknown error";
	}
	
	Player::Player(unsigned int dimension) throw (PlayerException) : dimension(dimension) {	
	}
		
	Player::~Player(){
	}
	
}




