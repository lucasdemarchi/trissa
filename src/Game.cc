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
#include "PlayerFactory.h"
#include "Player.h"
#include "UI.h"
#include "common.h"

#include <iostream>


using namespace std;

namespace trissa {

	//TODO: all
//	Game::Game(){
//	}

	Game::Game(int argc, char *argv[]) :
        //TODO: FIXME
        configManager ( "./", argc, argv ){

        //TODO: FIXME
		playerFactory = new PlayerFactory("./players/");
		ui = new UI();

		unsigned int dimension = ui->getDimension();
		this->dimension = dimension;
//		//Is there a better way of doing this?
		board = new vector<vector<vector<PlayerType> > >(dimension, vector<vector<PlayerType> >(dimension, vector<PlayerType>(dimension, PLAYER_BLANK)));

		vector<string> strplayers;
		string strplayerA, strplayerB;
		playerFactory->getPlayersList(strplayers);

		ui->getPlayers(strplayers, strplayerA, strplayerB);
		playerA = playerFactory->create_player(strplayerA, dimension, PLAYER_CROSS);
		playerB = playerFactory->create_player(strplayerB, dimension, PLAYER_CIRCLE);

	}

	Game::~Game(){
		delete playerFactory; //and all players...
		delete board;
		delete ui;
	}

	void Game::run()
	{
	    configManager.printUsage();
	    return;
		unsigned int turn;
		Move* move = playerA->firstPlay();
		Player* player = playerA;

		if(move->z < dimension && move->y < dimension &&
		   move->x < dimension && (*board)[move->z][move->y][move->x] == PLAYER_BLANK)
		{
			(*board)[move->z][move->y][move->x] = playerA->getPlayerType();
		}
        else{
            cerr << "Player returned invalid position (z,y,x): ["
                 << move->z << "," << move->y << "," << move->x << "]\n";
        }

		ui->refresh(*board,*move, true);


		for(turn = 1;
		    goalTest(*move,player->getPlayerType()) == PLAYER_BLANK
		    && turn < (dimension*dimension*dimension)
		    ; turn++)
		{
			if(turn%2)
				player = playerB;
			else
				player = playerA;

			move = player->play(*board,*move);

			if(move->z < dimension && move->y < dimension &&
			   move->x < dimension && (*board)[move->z][move->y][move->x] == PLAYER_BLANK)
			{
				(*board)[move->z][move->y][move->x] = player->getPlayerType();
			}
			else{
				cerr << "Player returned invalid position (z,y,x): ["
					 << move->z << "," << move->y << "," << move->x << "]\n";
			}
			ui->refresh(*board,*move, true);
		}
		ui->refresh(*board,*move,true);
	}

	//PlayerType Game::goalTest() const
	//{
        //return NULL;
	//}
	PlayerType Game::goalTest(Move const& lastMove, PlayerType player_type)
	{
		Move directions[] = {
			{1,0,0} , {0,1,0} , {0,0,1} ,
			{1,1,0} , {1,0,1} , {0,1,1} ,
			{1,-1,0}, {1,0,-1}, {0,1,-1},
			{1,1,1} , {1,-1,1}, {-1,1,1}, {1,1,-1}
		};
		int n_directions = 13;


		for(int i=0; i < n_directions; i++){
			bool invalid_direction = false;
			Move new_pos;
			new_pos.x = lastMove.x;
			new_pos.y = lastMove.y;
			new_pos.z = lastMove.z;
			unsigned int n_pieces = 1;
			//first subtract direction
			while (true){
				new_pos -= directions[i];
				if(new_pos.x >=0 && new_pos.y >=0 && new_pos.z >=0 &&
				   new_pos.x < dimension && new_pos.y < dimension && new_pos.z < dimension)
				{
					if( (*board)[new_pos.z][new_pos.y][new_pos.x] != player_type){
						invalid_direction = true;
						break;
					}
					else
						n_pieces++;
				}
				else{ //out of cube
					break;
				}
			}
			if(invalid_direction) continue; //next direction, this is not a winner direction

			//add direction to position until go out of cube
            new_pos.x = lastMove.x;
			new_pos.y = lastMove.y;
			new_pos.z = lastMove.z;
			while (true){
				new_pos += directions[i];
				if(new_pos.x >=0 && new_pos.y >=0 && new_pos.z >=0 &&
				   new_pos.x < dimension && new_pos.y < dimension && new_pos.z < dimension)
				{
					if( (*board)[new_pos.z][new_pos.y][new_pos.x] != player_type){
						invalid_direction = true;
						break;
					}
					else
						n_pieces++;
				}
				else{ //out of cube
					break;
				}
			}
			if(!invalid_direction && n_pieces == dimension){
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

}


int main (int argc, char * argv[]){

	trissa::Game game(argc, argv);
	game.run();


	return 0;
}
