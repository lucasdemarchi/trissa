/*
 * Player.h
 * This file is part of Trissa
 *
 * Copyright (C) 2008-2009 - Lucas De Marchi
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
#ifndef _PLAYER_H_
#define _PLAYER_H_ 1

#include <string>

#include <exception>
#include "common.h"
#include "UIInputOutput.h"

#define REGISTER_PLAYER(classname) \
extern "C" trissa::Player * create_player(int dimension, trissa::PlayerType player_type, trissa::UIInputOutput* ui){\
        return new classname (dimension, player_type, ui);\
    }\
    extern "C" char * getPlayerName(){\
        return classname::name;\
    }

namespace trissa
{
class Player;

class PlayerException : public std::exception
{
public:
    static const int BOARD_NOT_KNOWN = 0;
    static const int NEED_UIINPUTOUTPUT = 1;
    PlayerException(int idException, Player*  player);
    virtual const char* what();
    int idException;
private:
    Player* player;
};

/** A Player. This is an abstract class. You have to derive a new one, implementing the virtual methods to
  * be able to use it.
  */
class Player
{
public:

    /** Constructor
      * @param dimension Desired dimension of game
      * @param player_type
      * @param ui Interface of UI
      * @throw PlayerException with <i>idException</i> = BOARD_NOT_KNOWN if implementation
      * of this Player does not know how to play in this dimension
      */
    Player(unsigned int dimension, PlayerType player_type, UIInputOutput* ui) throw (PlayerException);
    virtual ~Player();

    /** Its the main function of Player. It's called every time Player has a turn
      * @remarks Pure virtual method that has to be overriden by a Player's implementation
      * @param board A const reference to board. It's here only to allow simpler implementations of Players that
      * doesn't want the overhead of keep a copy of board
      * @param opponentMove A const reference to last opponent move
      * @note @param board is here solely as a helper to lazy Playes.
      * It's being deprecated and maybe it will be removed in future
      * @return A new <em>empty</em> position to play into.
      */
    virtual Move* play(Cube const& board, Move const& opponentMove) = 0;

    /** Method that is called when this is the first player and when board is completely empty
      * @remarks Pure virtual method that has to be overriden by a Player's implementation
      * @note It could be possible to delete this member and use only <i>play</i> method. This was the preferred
      * approach since most of algorithms do not have to "think" to play the first time
      * @return A new <em>empty</em> position to play into.
      */
    virtual Move* firstPlay() = 0;

    /** This method defines the name of the Player
      * @remarks It's a pure virtual function that must return the static name of the class. Since C++ doesn't support
      * static pure virtual members, it was separated into a pure virtual method that is implemented in derived classes
      * returning the static name
      * @return A unique name of Player's implementation
      */
    virtual const char * getName() const = 0;

    /** Get player type, defined at constructor.
      *
      */
    PlayerType getPlayerType() const;
protected:
    unsigned int dimension; /**< Dimension of game*/
    /** Player type, i.e. <em>Cross</em> or <em>Circle</em>
      * @remarks Since is no sense changing player's type after game's begin, it can be set only upon Player's construction
      */
    PlayerType player_type;
    UIInputOutput* ui;
    /** Allow PlayerException to access private/protected methods/attributes */
    friend class PlayerException;
};

}

#endif /* _PLAYER_H_ */
