/*
 * UI3d.h
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
#ifndef _UI3D_H_
#define _UI3D_H_ 1

#include "../UI.h"

#include "InputHandler.h"
#include "MainMenu.h"

#include <Ogre.h>
#include <OgreWindowEventUtilities.h>

#include <OgreCEGUIRenderer.h>
#include <CEGUISystem.h>
#include <CEGUILogger.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <CEGUISchemeManager.h>

#include <sstream>
#include <boost/thread/thread.hpp>

namespace trissa
{

class UI3d : public UI
{
public:
	UI3d (ConfigManager* cm, StateManager* sm, PlayerFactory const* pf);
	void configure();
	void start();
	void refresh(Cube const& board, Move const& lastMove, bool wait);
	bool gameOver();
	void setPos(Move m, PlayerType player);

	Move getUserInput();
	void printWinnerMessage(std::string msg);
	void printLooserMessage(std::string msg);

private:
	void refresh(Cube const& board, Move const& lastMove);

	Ogre::Root* mRoot;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
	Ogre::Viewport* mViewport;
	InputHandler* mInputHandler;

	CEGUI::OgreCEGUIRenderer* mCEGUIRenderer;
	CEGUI::System* mCEGUISystem;

	MainMenu* mMainMenu;
	Ogre::SceneNode* mBoardNode;

	static const Ogre::Real VAR_DIST_PLANES;
	static const Ogre::String posNames;
	static const Ogre::Real QUAD_SIZE;

	bool mForceConfigDialogDisplay;

	void setupResources();
	void setupRenderSystem();
	void setupScene();
	
	void setupCEGUI();
	void createGUI();
	void destroyGUI();

	void createScene();
	void destroyScene();

};



}


#endif

