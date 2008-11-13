/*
 * Player.h
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
#ifndef PLAYER_H__
#define PLAYER_H__

#include <string>
#include <exception>
#include "common.h"
		
namespace trissa {
	class Player;

	class PlayerException : public std::exception{
	public:
		static const int BOARD_NOT_KNOWN = 0;
		PlayerException(int idException, Player*  player);
		virtual const char* what();
	private:
		int idException;
		Player* player;
	};
	
	class Player{
	public:
		Player(unsigned int dimension) throw (PlayerException) : dimension(dimension) {
			if (!isKnownBoard(dimension))
				throw PlayerException(0, this);
				
		}
		
		virtual ~Player(){
		}
		virtual Move& play(Cube const& board, Move const& opponentMove) = 0;
		virtual bool isKnownBoard(unsigned int dimension) const {
			return false;
		}
		virtual Move& firstPlay() = 0;
		virtual const char * getName() const = 0;
	protected:
		unsigned int dimension;
		friend class PlayerException;
	};
	
	PlayerException::PlayerException(int idException, Player*  player): 
			idException(idException),
			player(player){
	}
	const char * PlayerException::what(){
		std::string str_error("Player error: ");
		if (idException == BOARD_NOT_KNOWN) {
				str_error += (player->getName());
				str_error += " doesn't know how to play in board with size ";
				str_error += (player->dimension);
				return str_error.c_str();
		}
		return "Unknown error";
	}	


}

#endif /* PLAYER_H__ */
