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

#ifndef _TRISSA_STATEMANAGER_H_
#define _TRISSA_STATEMANAGER_H_ 1
#include <vector>
#include <map>
namespace trissa {
    typedef enum {
        STARTUP,
        LOADING,
        GUI,
        GAME,
        SHUTDOWN
    } GameState;

    class StateManager {

    public:
        StateManager(GameState state);
        virtual ~StateManager();

        GameState getCurrentState();

        bool lockState();
        bool unlockState();

        bool requestStateChange(GameState state);
    protected:
        GameState mState;
        bool mLocked;
//	float mFrameTime;
    };
}
#endif //_TRISSA_STATEMANAGER_H_
