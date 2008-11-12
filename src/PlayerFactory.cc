/*
 * PlayerFactory.cc
 * This file is part of Trissa
 *
 * Copyright (C) 2008 - Lucas De Marchi
 *
 * Trissa is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
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
					if ( fs::is_regular_file(dir_itr->status()) && dir_itr->path().extension() == ".so"){
						// Try to load dynamic library
						const string& filename = dir_itr->path().string();
						
						player_details.dlib = dlopen(filename.c_str(), RTLD_NOW);
						if(player_details.dlib == NULL){
							cerr<< "PlayerFactory: " << "Unable do load library " << filename;
							continue;
						}

						//Get a pointer to function that creates this player and insert it in factory map;
						player_details.player_creator_ptr = (function_creator_ptr) dlsym(player_details.dlib, "getPlayerCreator_ptr");
						if(dlerror()){
							cerr<< "PlayerFactory: " << "Unable get Player's  creator function from library " << filename;
							dlclose(player_details.dlib);
						}
						
						//Execute function "getPlayerName" inside library
						string playerName = ((char *(*)())dlsym(player_details.dlib, "getPlayerName"))();
						if(dlerror()){
							cerr<< "PlayerFactory: " << "Unable get Player name from library " << filename;
							dlclose(player_details.dlib);
							continue;
						}
						
						this->factory[playerName] = player_details;
					}
				}
				catch ( const std::exception & ex ){
					cerr<< "PlayerFactory: " << "Unable to access file " << dir_itr->path().filename() << endl;
				}
			}
		}
	}
	PlayerFactory::~PlayerFactory(){
		//iterate through factory and close all dlibs
		for (map<string, Player_details>::iterator it = factory.begin();
				it != factory.end();
				it++){
			dlclose(it->second.dlib);
		}
	}
}

int main(){
	return 0;
}
