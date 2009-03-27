/*
 * StateManager.h
 * This file is part of Trissa
 *
 * Copyright (C) 2009 - Lucas De Marchi
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

#ifndef _STATEMANAGER_H_
#define _STATEMANAGER_H_ 1
#include <vector>
#include <map>
namespace trissa
{
/** Enum with possible states of the game*/
typedef enum {
    STARTUP,    /**< When starting game*/
    LOADING,    /**< When loading resources, before loading any graphics*/
    GUI,        /**< While in configuration screens*/
    GAME,       /**< After has configured game, the final loading and resources and game loop*/
    SHUTDOWN    /**< Any time a SHUTDOWN request is received, game starts the shutdown process*/
} GameState;

class StateManager
{

public:
    /** Constructor
      * @param state Initial state. No special purpose to this parameter; it must be STARTUP
      */
    StateManager(GameState state);
    virtual ~StateManager();

    /** Get current state of game
      * @return One of GameState enum values
      */
    GameState getCurrentState();

    /** Lock the state so anyone asking to change it get blocked
      * @remarks Not used yet
      */
    bool lockState();

    /** Unlock the state so anyone blocked because has asked to change is waken up
      * @remarks Not used yet
      */
    bool unlockState();

    /** Request for changing the current state
      * @remarks The name "request" was chosen instead of "set" because who is calling it could have
      * the change rejected. In fact, in future the caller will get blocked in this function
      * @param state The state to change to
      */
    bool requestStateChange(GameState state);
protected:

    GameState mState; /**< Current state of the game*/
    bool mLocked; /** For future use... locking mechanism */

};
}
#endif //_STATEMANAGER_H_
