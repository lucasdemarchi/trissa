#ifndef _TRISSA_MAINMENU_H
#define _TRISSA_MAINMENU_H 1

#include "Menu.h"
#include <CEGUIWindow.h>

namespace CEGUI
{
	class System;
	class Window;
}

class StateManager;
class SettingsMenu;

class MainMenu : public Menu {
public:
    MainMenu(CEGUI::System* CEGUISystem, CEGUI::Window* pSheet, StateManager* stateManager, Menu* father);
    ~MainMenu();

    bool quit_OnClick( const CEGUI::EventArgs &args );
    bool options_OnClick( const CEGUI::EventArgs &args );
    bool play_OnClick(const CEGUI::EventArgs &args);
protected:
    SettingsMenu* mSettingsMenu;
};


#endif
