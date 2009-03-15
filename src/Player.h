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
#ifndef PLAYER_H_
#define PLAYER_H_ 1

#include <string>

#include <exception>
#include "common.h"

#define REGISTER_PLAYER(classname) 																\
	extern "C" trissa::Player * create_player(int dimension, trissa::PlayerType player_type){	\
		return new classname (dimension, player_type);											\
	}																							\
	extern "C" char * getPlayerName(){															\
		return classname::name;																	\
	}

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
		Player(unsigned int dimension, PlayerType player_type) throw (PlayerException);
		virtual ~Player();
		virtual Move* play(Cube const& board, Move const& opponentMove) = 0;
		virtual Move* firstPlay() = 0;
		virtual const char * getName() const = 0;
		PlayerType getPlayerType() const;
	protected:
		unsigned int dimension;
		PlayerType player_type;
		friend class PlayerException;
	};

}

#endif /* PLAYER_H__ */
