#ifndef _TRISSA_SETTINGSMENU_H
#define _TRISSA_SETTINGSMENU_H 1

#include "Menu.h"
#include <CEGUIWindow.h>

namespace CEGUI
{
	class System;
	class Window;
}

class StateManager;
class MainMenu;

class SettingsMenu : public Menu{
public:
    SettingsMenu(CEGUI::System* CEGUISystem, CEGUI::Window* pSheet, StateManager* stateManager, Menu* father);
    bool ok_OnClick(const CEGUI::EventArgs &args);
};


#endif
