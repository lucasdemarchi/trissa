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
#include "common.h"

#include <iostream>


using namespace std;



namespace trissa {

    Game::Game(int argc, char *argv[]) :
            mBoard( 0 ),
            mUi( 0 ),
            mConfigManager ( argc, argv ),
            mStateManager(STARTUP) {

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
    void Game::load() {

        //attach and load Player libraries
        mConfigManager.attachPlayerFactory(&mPlayerFactory);

        //TODO: verify in ConfigManager if requested UI is 3D or not and load specified UI
        mUi = new UIText(&mConfigManager, &mStateManager, &mPlayerFactory);

        if ( mStateManager.getCurrentState() != GUI ) {
            //Probably an error loading resources.
            //TODO: write it to log
            if ( mStateManager.requestStateChange( SHUTDOWN ) )
                return;
            else {
                //TODO: write this bug to log
                return;
            }
        }
    }

    void Game::configure() {
        mUi->configure();
    }

    void Game::run() {
        //Finish loading configurations
        int dimension  = (int) mConfigManager.getDimension();

        if ( mBoard )
            delete mBoard;
        mBoard = new vector<vector<vector<PlayerType> > >(dimension, vector<vector<PlayerType> >(dimension, vector<PlayerType>(dimension, PLAYER_BLANK)));

        if ( mPlayerA || mPlayerB )
            mPlayerFactory.destroyPlayers();

        mPlayerA = mPlayerFactory.create_player ( mConfigManager.getPlayerA(), dimension, PLAYER_CROSS );
        mPlayerB = mPlayerFactory.create_player ( mConfigManager.getPlayerB(), dimension, PLAYER_CIRCLE );


//Start game
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
        cin.sync();
        mUi->refresh(*mBoard,*move, true);


        for (turn = 1;
                goalTest(*move,player->getPlayerType()) == PLAYER_BLANK
                && turn < (dimension*dimension*dimension)
                ; turn++) {

            if (turn%2)
                player = mPlayerB;
            else
                player = mPlayerA;

            move = player->play(*mBoard,*move);

            if (move->z < dimension && move->y < dimension &&
                    move->x < dimension && (*mBoard)[move->z][move->y][move->x] == PLAYER_BLANK) {
                (*mBoard)[move->z][move->y][move->x] = player->getPlayerType();
            } else {
                cerr << "Player returned invalid position (z,y,x): ["
                << move->z << "," << move->y << "," << move->x << "]\n";
            }
            mUi->refresh(*mBoard,*move, true);
        }
        mUi->refresh(*mBoard,*move,true);
        if(mUi->gameOver())
            mStateManager.requestStateChange(GUI);
        else
            mStateManager.requestStateChange(SHUTDOWN);
    }

    Game::~Game() {
        if ( mUi ) delete mUi;
        //if ( mPlayerFactory ) delete mPlayerFactory; //and all players...
        if ( mBoard ) delete mBoard;
    }


    PlayerType Game::goalTest(Move const& lastMove, PlayerType player_type) {
        Move directions[] = {
            {1,0,0} , {0,1,0} , {0,0,1} ,
            {1,1,0} , {1,0,1} , {0,1,1} ,
            {1,-1,0}, {1,0,-1}, {0,1,-1},
            {1,1,1} , {1,-1,1}, {-1,1,1}, {1,1,-1}
        };
        int n_directions = 13;

        int dimension = mConfigManager.getDimension();
        for (int i=0; i < n_directions; i++) {
            bool invalid_direction = false;
            Move new_pos;
            new_pos.x = lastMove.x;
            new_pos.y = lastMove.y;
            new_pos.z = lastMove.z;
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
#ifdef _trissa_debug_
                cout << "Winner direction: ["
                << directions[i].x << "," << directions[i].y << "," << directions[i].z << "]\n";
                cout << "Last position played: ["
                << lastMove.x << "," << lastMove.y << "," << lastMove.z << "]\n";
#endif // _trissa_debug_
                return (player_type);
            }
        }
        return PLAYER_BLANK;

    }
    int main (int argc, char * argv[]) {

        trissa::Game game(argc, argv);
        if ( game.mStateManager.getCurrentState() == SHUTDOWN ) {
            return 0;
        }

        game.load();
        //GUI -> GAME STATE
        while ( game.mStateManager.getCurrentState() != SHUTDOWN ) {

            // run configurations (a.k.a Menus in 3D or some questions in text)
            game.configure();
            if ( game.mStateManager.getCurrentState() == GAME ) {
                game.run();
            }

        }

        return 0;
    }


}

int main(int argc, char * argv[]) {
    return trissa::main(argc, argv);
}


