/*
 * Game.cc
 * This file is part of Trissa
 *
 * Copyright (C) 2008-2009 - Lucas De Marchi
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

	//GUI -> GAME STATE
	while ( this->mStateManager.getCurrentState() != SHUTDOWN ) {

		// run configurations (a.k.a Menus in 3D or some questions in text)
		this->configure();
		if(mConfigManager.getPlayerA() != ""
			   	&& mConfigManager.getPlayerB() != ""
				&& mConfigManager.getDimension() 
				&& this->mStateManager.getCurrentState() == GAME ) {
			boost::thread thr(boost::bind(&Game::run,this));
			static_cast<UI3d*>(mUi)->start_thread();
			thr.join();
		}
		if(mStateManager.getCurrentState() == GAME)
			mStateManager.requestStateChange(GUI);

	}

	return 0;
}
int Game::load()
{

	//attach and load Player libraries
	mConfigManager.attachPlayerFactory(&mPlayerFactory);

	//Create the UI based on the parameter got through ConfigManager
#ifdef _TRISSA_UI3D_
	if (mConfigManager.getUIType() == ConfigManager::UI_3D){
		cout << "Loading UI: 3d...\n";
		mUi = new UI3d(&mConfigManager, &mStateManager, &mPlayerFactory);
	}
	else
#endif
	if (mConfigManager.getUIType() == ConfigManager::UI_TEXT){
		cout << "Loading UI: text... \n";
		mUi = new UIText(&mConfigManager, &mStateManager, &mPlayerFactory);
	}
	else{
		cerr << "UI not available...\n";
		return -1;
	}

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
	cout << "Loading game... " << endl;
	cout << "Dimension: " << mConfigManager.getDimension();

	mDimension  = (int) mConfigManager.getDimension();

	if ( mBoard )
		delete mBoard;

	mBoard = new vector<vector<vector<PlayerType> > >
		(mDimension, vector<vector<PlayerType> >
		 (mDimension, vector<PlayerType>(mDimension, PLAYER_BLANK)));

	if ( mPlayerA || mPlayerB )
		mPlayerFactory.destroyPlayers();

	cout << "Creating players..." << endl;
	try{
		mPlayerA = mPlayerFactory.create_player (mConfigManager.getPlayerA(),
	                                         mDimension, PLAYER_CROSS, mUi);
		mPlayerB = mPlayerFactory.create_player (mConfigManager.getPlayerB(),
	                                         mDimension, PLAYER_CIRCLE, mUi );
	} catch(exception& ex) {
		cerr<< "Error creating Player. " << ex.what() << ex.what();
		exit(1);
	}


	cout << "Calling UI specific start method" << endl;
	//Start game
	mUi->start(*mBoard);
	//wait GUI loading
	boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

	PlayerType winner = PLAYER_BLANK;
	try {
		int turn = 0;
		int retry = n_retry;
		Move* move = mPlayerA->firstPlay();
		Player* player_p = mPlayerA;
		PlayerType player = PLAYER_CROSS;


		if (isInsideBoard(*move) && isFreePosition(*move)) {
			(*mBoard)[move->z][move->y][move->x] = player;
			mUi->setPos(*move,player);
		} else {
			cerr << "PlayerA returned invalid position in first play (z,y,x): ["
				 << move->z << "," << move->y << "," << move->x << "]\n";
			winner = PLAYER_CIRCLE;
			goto out;
		}

		//while there's no winner 
		//and not all positions are occupied
		for (turn = 1 ; winner == PLAYER_BLANK                                     
						&& turn < (mDimension*mDimension*mDimension); turn++) {    

			//decide which player plays next
			//they take turns
			if (turn%2) {
				player_p = mPlayerB;
				player = PLAYER_CIRCLE;
			} else {
				player_p = mPlayerA;
				player = PLAYER_CROSS;
			}
			

			// ask Player the position to play and keep trying n_retry times
			// if it returns an invalid position
			for (move = player_p->play(*mBoard,*move)
				;!(isInsideBoard(*move) && isFreePosition(*move)) && retry
				;move = player_p->play(*mBoard,*move)) {   //try again

				cerr << "Player returned invalid position (z,y,x): ["
					 << move->z << "," << move->y << "," << move->x << "]\n";
				cerr << "This is probably a bug in Player's algorithm\n";
				retry--;
			}
			if (retry){
				(*mBoard)[move->z][move->y][move->x] = player;
				mUi->setPos(*move, player);
				winner = goalTest(*move,player_p->getPlayerType());
			}
			else {
				cerr << "Player \'" << player_p->getName() << "\'"
					"returned an invalid position for more than " << n_retry 
					<< "times.\nPlease fix your algorithm before trying to play. "
					"The other player is proclaimed winner";
				
				//the other player
				winner = (player == PLAYER_CROSS) ? PLAYER_CIRCLE:PLAYER_CROSS; 
			}
		}
	}
	catch(exception& e){
		//user canceled
		cout << "User canceled game" << endl;
		return;
	}
out:
	if(winner != PLAYER_BLANK)
		mUi->gameOver(mWinnerStartPos, mWinnerDirection);
	else
		mUi->gameOver();

}

inline bool Game::isInsideBoard(Move &move)
{
	return (move.z < mDimension && move.y < mDimension && move.z < mDimension);
}

inline bool Game::isFreePosition(Move &move)
{
	return ( (*mBoard)[move.z][move.y][move.x] == PLAYER_BLANK );
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
			        new_pos.x < dimension && new_pos.y < dimension &&
					new_pos.z < dimension) {

				if ( (*mBoard)[new_pos.z][new_pos.y][new_pos.x] != player_type){
					invalid_direction = true;
					break;
				} else
					n_pieces++;
			} else { //out of cube
				break;
			}
		}

		//next direction, this is not a winner direction
		if (invalid_direction)
			continue;

		//add direction to position until go out of cube
		new_pos.x = lastMove.x;
		new_pos.y = lastMove.y;
		new_pos.z = lastMove.z;
		while (true) {
			new_pos += directions[i];
			if (new_pos.x >=0 && new_pos.y >=0 && new_pos.z >=0 &&
			        new_pos.x < dimension && new_pos.y < dimension &&
					new_pos.z < dimension) {
				if ( (*mBoard)[new_pos.z][new_pos.y][new_pos.x] != player_type){
					invalid_direction = true;
					break;
				} else
					n_pieces++;
			} else { //out of cube
				break;
			}
		}
		if (!invalid_direction && (int)n_pieces == dimension) {
			mWinnerDirection = directions[i];
			mWinnerStartPos = lastMove;
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
