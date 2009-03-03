#include "Menu.h"
#include "state_manager.h"

#include <CEGUISystem.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>

Menu::Menu(CEGUI::System* CEGUISystem, CEGUI::Window* pSheet, StateManager* stateManager, Menu* father) :
    mCEGUISystem( CEGUISystem ),
    mWindow( pSheet ),
    mStateManager( stateManager ),
    mFather( father ) {
}
Menu::~Menu(){
}

CEGUI::Window* Menu::getWindow(){
    return mWindow;
}
