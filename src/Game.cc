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
	
	Game::Game(string playersPath){
		playerFactory = new PlayerFactory(playersPath);
		ui = new UI();
		
		unsigned int dimension = ui->getDimension();
		this->dimension = dimension;
//		//Is there a better way of doing this?
		board = new vector<vector<vector<int> > >(dimension, vector<vector<int> >(dimension, vector<int>(dimension, PLAYER_NONE)));
		
		vector<string> strplayers;
		string strplayerA, strplayerB;
		playerFactory->getPlayersList(strplayers);
		
		ui->getPlayers(strplayers, strplayerA, strplayerB);
		playerA = playerFactory->create_player(strplayerA, dimension);
		playerB = playerFactory->create_player(strplayerB, dimension);
		
	}
	
	//TODO: all
	Game::~Game(){
		delete playerFactory; //and all players...
		delete board;
		delete ui;
	}

	void Game::run(){
		int turn;
		Move* moveA = playerA->firstPlay();
		Move* move = moveA;
		Move* moveB = NULL;
		
		if(move->z < dimension && move->y < dimension &&
		   move->x < dimension && (*board)[move->z][move->y][move->x] == PLAYER_NONE)
		{
			(*board)[move->z][move->y][move->x] = turn%2 ? PLAYER_A : PLAYER_B ;
		}
		ui->refresh(*board,*move);

		for(turn = 1; !isFinished() && turn < 20; turn++){
			if(turn%2){
				moveB = playerB->play(*board,*moveA);
				move = moveB;
			}
			else{
				moveA = playerA->play(*board,*moveB);
				move = moveA;
			}
			
			if(move->z < dimension && move->y < dimension &&
			   move->x < dimension && (*board)[move->z][move->y][move->x] == PLAYER_NONE)
			{
				(*board)[move->z][move->y][move->x] = turn%2 ? PLAYER_A : PLAYER_B ;
			}
			
		}
		ui->refresh(*board,*move);
	}
	
	int Game::isFinished(){
		return 0;
	}

}

void print_usage(){
	cout << "Usage: trissa path_to_players" << endl;	
}

int main (int argc, char * argv[]){

	if(argc != 2){
		print_usage();
		return -1;
	}
		
	trissa::Game game(argv[1]);
	game.run();
	
	
	return 0;
}
