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

#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <CEGUISystem.h>
#include <CEGUILogger.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <CEGUISchemeManager.h>

#include <sstream>
#include <map>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/shared_ptr.hpp>

namespace trissa
{

class UI3d : public UI
{
	struct PlayerMove {
		Move m;
		PlayerType player;
	};

public:
	UI3d (ConfigManager* cm, StateManager* sm, PlayerFactory const* pf);
	~UI3d();
	void configure();
	void start(Cube const& board);
	void wait_end();
	void setPos(Move const& m, PlayerType player);

	void gameOver(Move const& startPosition, Move const& direction, PlayerType winner);

	void start_thread();

	//called by core
	Move getUserInput();

	void printWinnerMessage(std::string msg);
	void printLooserMessage(std::string msg);

private:
	Ogre::Root* mRoot;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
	Ogre::Viewport* mViewport;
	InputHandler* mInputHandler;

	CEGUI::OgreRenderer *mCEGUIRenderer;
	CEGUI::System* mCEGUISystem;

	MainMenu* mMainMenu;
	Ogre::SceneNode* mBoardNode;

	static const Ogre::Real VAR_DIST_PLANES;
	static const Ogre::String posNames;
	static const Ogre::Real QUAD_SIZE;

	bool mForceConfigDialogDisplay;
	volatile bool mCanLoadUi;
	volatile bool mGameOver;

	volatile bool mHasNewPos;
	boost::mutex mMutexNewPos;
	boost::condition_variable mCondNewPosHandled;
	PlayerMove player_move;

	void setupResources();
	void setupRenderSystem();
	void setupScene();

	void setupCEGUI();
	void createGUI();
	void destroyGUI();

	void createScene();
	void destroyScene();

	void updateBoard();

	std::map<Ogre::Entity*, Move> boardMap;
};



}


#endif

