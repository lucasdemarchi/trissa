/*
 * StateManager.cc
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
#include "StateManager.h"
#include <exception>

namespace trissa {

    StateManager::StateManager(GameState state) :
            mState( state ) {
    }

    StateManager::~StateManager() {
    }

    GameState StateManager::getCurrentState() {
        return mState;
    }
//TODO Implement StateManager::lockState
    bool StateManager::lockState() {
//    if( mLocked == false ) {
//        mLocked = true;
//        return true;
//    } else
//        return false;
        return true;
    }
//TODO Implement StateManager::unlockState
    bool StateManager::unlockState() {
//    if( mLocked == true ) {
//        mLocked = false;
//        return true;
//    } else
//        return false;
        return true;
    }

    bool StateManager::requestStateChange( GameState state ) {
        if ( mState == state )
            throw std::exception();
        if ( mState == STARTUP ) {
            mLocked = false;
            mState = state;
            return true;
        } else if ( ( mState == GUI || mState == GAME ) && ( state != STARTUP && state != mState ) ) {
            mState = state;
            return true;
        } else if ( mState == SHUTDOWN )
            return false;
        else
            return false;
    }

}
