#ifndef _TRISSA_MENU_H
#define _TRISSA_MENU_H 1


#include "CEGUIWindow.h"

namespace CEGUI
{
	class System;
	class Window;
}

class StateManager;

class Menu {
public:
    Menu(CEGUI::System* CEGUISystem, CEGUI::Window* pSheet, StateManager* stateManager, Menu* father);
    virtual ~Menu();
    CEGUI::Window* getWindow();

protected:

    CEGUI::System* mCEGUISystem;
    CEGUI::Window* mWindow;
    StateManager* mStateManager;
    Menu* mFather;
};


#endif
