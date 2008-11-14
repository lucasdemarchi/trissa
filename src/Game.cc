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
	
	//TODO: all
	Game::Game(string playersPath){
		playerFactory = new PlayerFactory(playersPath);
		ui = new UI();
		
		unsigned const int dimension = ui->getDimension();
		
		//Is there a better way of doing this?
		board = new vector<vector<vector<int> > >(dimension, vector<vector<int> >(dimension, vector<int>(dimension, PLAYER_NONE)));
		
		vector<string> strplayers;
		string strplayerA, strplayerB;
		playerFactory->getPlayersList(strplayers);
		
		ui->getPlayers(strplayers, strplayerA, strplayerB);
		playerA = playerFactory->create_player(strplayerA);
		playerB = playerFactory->create_player(strplayerB);
		
	}
	
	//TODO: all
	Game::~Game(){
		delete playerFactory; //and all players...
		delete board;
		delete ui;
	}
	
	//TODO: all
	void Game::start(){
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
	return 0;
}
