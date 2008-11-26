/*
 * PlayerFactory.cc
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
#include "PlayerFactory.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <dlfcn.h>
#include <iostream>

#include "Player.h"

namespace trissa {
	using namespace std;
	namespace fs = boost::filesystem;
	
	PlayerFactory::PlayerFactory(string path){
		fs::path full_path(fs::system_complete(path));
		
		if( fs::exists( full_path ) && fs::is_directory( full_path )){
			fs::directory_iterator end_iter;
			for ( fs::directory_iterator dir_itr( full_path ); 
					dir_itr != end_iter;
					++dir_itr ){
				Player_details player_details;
				try{
					if ( fs::is_regular_file(dir_itr->status()) && dir_itr->path().extension() == ".so" ){
						// Try to load dynamic library
						const string& filename = dir_itr->path().string();
						
						player_details.dlib = dlopen(filename.c_str(), RTLD_NOW);
						if(player_details.dlib == NULL){
							cerr<< "PlayerFactory: " << "Unable to load library " << filename << endl << dlerror() << endl;
							continue;
						}

						//Get a pointer to function that creates this player and insert it in factory map;
						player_details.player_creator_ptr = (function_creator_ptr) dlsym(player_details.dlib, "create_player");
						if(dlerror()){
							cerr<< "PlayerFactory: " << "Unable to get Player's  creator function from library " << filename;
							dlclose(player_details.dlib);
						}
						
						//Execute function "getPlayerName" inside library
						string playerName = ((char *(*)())dlsym(player_details.dlib, "getPlayerName"))();
						if(dlerror()){
							cerr<< "PlayerFactory: " << "Unable to get Player name from library " << filename;
							dlclose(player_details.dlib);
							continue;
						}
						
						this->factory[playerName] = player_details;
					}
				}
				catch ( const std::exception & ex ){
					cerr<< "PlayerFactory: " << "Unable to access file " << dir_itr->path().filename() << endl;
					cerr << ex.what() << endl;
				}
			}
		}
	}
	PlayerFactory::~PlayerFactory(){

		for(int i = 0; i < created_players.size(); i++)
			delete (created_players[i]);

		//iterate through factory and close all dlibs
		for (map<string, Player_details>::iterator it = factory.begin();
				it != factory.end();
				it++){
			dlclose(it->second.dlib);
		}
	}
	
	Player* PlayerFactory::create_player (string player_name, int dimension, PlayerType player_type){
		Player* p;
		try{
			p = (factory[player_name].player_creator_ptr)(dimension, player_type);
		}
		catch(exception& ex){
			cerr << ex.what();
			return NULL;
		}
		
		created_players.push_back(p);
			
		return p;
	}
	
	void PlayerFactory::getPlayersList(vector<string>& strplayers){		
		for (map<string, Player_details>::iterator it = factory.begin();
			it != factory.end();
			it++){
			
			strplayers.push_back(it->first);
		}
	}
	
}
