#include "state_manager.h"

StateManager::StateManager(GameState state) :
    mState( state ){
}

StateManager::~StateManager() {
}

GameState StateManager::getCurrentState(){
    return mState;
}

bool StateManager::lockState() {
    if( mLocked == false ) {
        mLocked = true;
        return true;
    } else
        return false;
}

bool StateManager::unlockState(){
    if( mLocked == true ) {
        mLocked = false;
        return true;
    } else
        return false;
}

bool StateManager::requestStateChange( GameState state ){
    if( mState == STARTUP ){
        mLocked = false;
        mState = state;
        return true;
    }
    if( mState == SHUTDOWN )
        return false;

    if( ( mState == GUI || mState == GAME ) && ( state != STARTUP && state != mState ) ){
        mState = state;
        return true;
    }
    else
        return false;

}
