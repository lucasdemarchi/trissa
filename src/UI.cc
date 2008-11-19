/*
 * UI.cc
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
#include "UI.h"

namespace trissa {
	using namespace std;

	//TODO: all
	UI::UI(){
	}
	
	//TODO: all
	UI::~UI(){
	
	}
	
	//TODO: all
	unsigned int UI::getDimension(){
		int d;
		cout << "Dimension of desired game: ";
		cin >> d;
		return d;
	}
	
	//TODO: all
	void UI::getPlayers(vector<string> strplayers, string& strplayerA, string& strplayerB){
		cout << "Available players: " << endl;
		for (int i=0; i < strplayers.size(); i++){
			cout << i << ": " << strplayers[i] << endl;
		}
		int playerA, playerB;
		cout << "Enter number for player A: ";
		cin >> playerA;
		cout << "Enter number for player B: ";		
		cin >> playerB;
		strplayerA = strplayers[playerA];
		strplayerB = strplayers[playerB];		
	}
}
