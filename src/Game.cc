/*
 * Game.cc
 * This file is part of Trissa
 *
 * Copyright (C) 2008 - Lucas De Marchi
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
#include "Game.h"
#include "Player.h"
#include "UI.h"
#include "UIText.h"
#ifdef _TRISSA_UI3D_
#include "ui3d/UI3d.h"
#endif
// #include "UIInputOutput.h"
#include "common.h"

#include <iostream>


using namespace std;



namespace trissa
{

Game::Game(int argc, char *argv[]) :
		mBoard( 0 ),
		mUi( 0 ),
		mPlayerFactory(),
		mConfigManager ( argc, argv ),
		mStateManager(STARTUP)
{

	if ( mConfigManager.isGetUsage() ) {
		mConfigManager.printVersion();
		mConfigManager.printUsage();
		mStateManager.requestStateChange( SHUTDOWN );
		return;
	}
	if ( mConfigManager.isGetVersion() ) {
		mConfigManager.printVersion();
		mStateManager.requestStateChange( SHUTDOWN );
		return;
	}
	mStateManager.requestStateChange(LOADING);

}

int Game::startGame()
{

	if ( this->mStateManager.getCurrentState() == SHUTDOWN ) {
		return 0;
	}

	//LOADING -> GUI
	if (this->load() < 0)
		return -1;  // error: it was not possible to load
	// required gui

	//GUI -> GAME STATE
	while ( this->mStateManager.getCurrentState() != SHUTDOWN ) {

		// run configurations (a.k.a Menus in 3D or some questions in text)
		this->configure();
		if ( this->mStateManager.getCurrentState() == GAME ) {
			this->run();
		}

	}

	return 0;
}
int Game::load()
{

	//attach and load Player libraries
	mConfigManager.attachPlayerFactory(&mPlayerFactory);

	//Create the UI based on the parameter got through ConfigManager
#ifdef _TRISSA_UI3D_
	if (mConfigManager.getUIType() == ConfigManager::UI_3D)
		//mUi = new UI3d(&ConfigManager, &mStateManager, &mPlayerFactory)
		return -1;
	else
#endif
	if (mConfigManager.getUIType() == ConfigManager::UI_TEXT)
		mUi = new UIText(&mConfigManager, &mStateManager, &mPlayerFactory);
	else
		return -1;

	if ( mStateManager.getCurrentState() != GUI ) {
		//Probably an error loading resources.
		//TODO: write it to log
		if ( mStateManager.requestStateChange( SHUTDOWN ) )
			return -3;
		else {
			//TODO: write this bug to log
			return -2;
		}
	}
	return 0;
}

void Game::configure()
{
	mUi->configure();
}

void Game::run()
{
	//Finish loading configurations
	int dimension  = (int) mConfigManager.getDimension();

	if ( mBoard )
		delete mBoard;
	mBoard = new vector<vector<vector<PlayerType> > >(dimension, vector<vector<PlayerType> >(dimension, vector<PlayerType>(dimension, PLAYER_BLANK)));

	if ( mPlayerA || mPlayerB )
		mPlayerFactory.destroyPlayers();

	mPlayerA = mPlayerFactory.create_player ( mConfigManager.getPlayerA(), dimension, PLAYER_CROSS, mUi );
	mPlayerB = mPlayerFactory.create_player ( mConfigManager.getPlayerB(), dimension, PLAYER_CIRCLE, mUi );


//Start game
	mUi->refresh(*mBoard, Move(-1,-1,-1), true);

	int turn;
	Move* move = mPlayerA->firstPlay();
	Player* player = mPlayerA;

	if (move->z < dimension && move->y < dimension &&
	        move->x < dimension && (*mBoard)[move->z][move->y][move->x] == PLAYER_BLANK) {
		(*mBoard)[move->z][move->y][move->x] = mPlayerA->getPlayerType();
	} else {
		cerr << "Player returned invalid position (z,y,x): ["
		     << move->z << "," << move->y << "," << move->x << "]\n";
	}

	for (turn = 1;
	        goalTest(*move,player->getPlayerType()) == PLAYER_BLANK
	        && turn < (dimension*dimension*dimension)
	        ; turn++) {
		PlayerType player_type ;
		if (turn%2) {
			player = mPlayerB;
			player_type = PLAYER_CIRCLE;
		} else {
			player = mPlayerA;
			player_type = PLAYER_CROSS;
		}
		mUi->refresh(*mBoard,*move, true);

		int retry = n_retry;
		for (move = player->play(*mBoard,*move);
		        !(move->z < dimension && move->y < dimension && move->x < dimension  //Move is not inside the board or
		          && (*mBoard)[move->z][move->y][move->x] == PLAYER_BLANK) && retry; //It's not a free position && retry>0
		        move = player->play(*mBoard,*move)) {                                //try again
			cerr << "Player returned invalid position (z,y,x): ["
			     << move->z << "," << move->y << "," << move->x << "]\n";
			cerr << "This is probably a bug in Player's algorithm\n";
			retry--;
		}
		if (retry)
			(*mBoard)[move->z][move->y][move->x] = player_type;
		else {
			cerr << "Player \'" << player->getName() << "\' returned an invalid position for more than " << n_retry
			     << "times.\nPlease fix your algorithm before trying to play. The other player is proclaimed winner";
			break;
		}
	}
	mUi->refresh(*mBoard,*move,false);
	if (mUi->gameOver())
		mStateManager.requestStateChange(GUI);
	else
		mStateManager.requestStateChange(SHUTDOWN);
}

Game::~Game()
{
	if ( mUi ) delete mUi;
	//if ( mPlayerFactory ) delete mPlayerFactory; //and all players...
	if ( mBoard ) delete mBoard;
}


PlayerType Game::goalTest(Move const& lastMove, PlayerType player_type)
{
	Move directions[] = {
		Move(1,0,0) , Move(0,1,0) , Move(0,0,1) ,
		Move(1,1,0) , Move(1,0,1) , Move(0,1,1) ,
		Move(1,-1,0), Move(1,0,-1), Move(0,1,-1),
		Move(1,1,1) , Move(1,-1,1), Move(-1,1,1), Move(1,1,-1)
	};
	int n_directions = 13;

	int dimension = mConfigManager.getDimension();
	for (int i=0; i < n_directions; i++) {
		bool invalid_direction = false;
		Move new_pos(lastMove.x,lastMove.y,lastMove.z);
		unsigned int n_pieces = 1;
		//first subtract direction
		while (true) {
			new_pos -= directions[i];
			if (new_pos.x >=0 && new_pos.y >=0 && new_pos.z >=0 &&
			        new_pos.x < dimension && new_pos.y < dimension && new_pos.z < dimension) {
				if ( (*mBoard)[new_pos.z][new_pos.y][new_pos.x] != player_type) {
					invalid_direction = true;
					break;
				} else
					n_pieces++;
			} else { //out of cube
				break;
			}
		}
		if (invalid_direction) continue; //next direction, this is not a winner direction

		//add direction to position until go out of cube
		new_pos.x = lastMove.x;
		new_pos.y = lastMove.y;
		new_pos.z = lastMove.z;
		while (true) {
			new_pos += directions[i];
			if (new_pos.x >=0 && new_pos.y >=0 && new_pos.z >=0 &&
			        new_pos.x < dimension && new_pos.y < dimension && new_pos.z < dimension) {
				if ( (*mBoard)[new_pos.z][new_pos.y][new_pos.x] != player_type) {
					invalid_direction = true;
					break;
				} else
					n_pieces++;
			} else { //out of cube
				break;
			}
		}
		if (!invalid_direction && (int)n_pieces == dimension) {
			//TODO: set member variable for winner direction(s) and point
#ifdef _TRISSA_DEBUG_
			cout << "Winner direction: ["
			     << directions[i].x << "," << directions[i].y << "," << directions[i].z << "]\n";
			cout << "Last position played: ["
			     << lastMove.x << "," << lastMove.y << "," << lastMove.z << "]\n";
#endif // _TRISSA_DEBUG_
			return (player_type);
		}
	}
	return PLAYER_BLANK;

}
const int Game::n_retry = 3;


}

int main(int argc, char * argv[])
{
	trissa::Game game(argc, argv);
	return game.startGame();
}
