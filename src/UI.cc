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
#include <cstring>

namespace trissa {
	using namespace std;

	//TODO: all
	UI::UI(){
	}
	
	//TODO: all
	UI::~UI(){
	
	}
	
	unsigned int UI::getDimension(){
		int d;
		cout << "Dimension of desired game: ";
		cin >> d;
		return d;
	}
	
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
	
	
	void UI::refresh(Cube const& board, Move const& lastMove){
		char * dash_space = (char*) "---";
		char * blank_space = (char*) "   ";

		for (int j = 0; j < board.size(); j++){
			for (int k = 0; k < board.size(); k++)
			{
				cout << "|";
				for(int i=0;i<board.size()-1;i++)
					cout << dash_space << "+";
				cout << dash_space << "| ";
			}

			cout << "\n";
			for (int k = 0; k < board.size(); k++)
			{
				cout << "|";
				for(int i=0;i<board.size();i++){
					if( board[k][j][i] == PLAYER_BLANK)
						cout << blank_space;
					else if (board[k][j][i] == PLAYER_CROSS)
						cout << " X ";
					else
						cout << " O ";

					cout << "|";
				}
				cout << " ";
			}
			cout << "\n";	
		}
		for (int k = 0; k < board.size(); k++)
		{
			cout << "|";
			for(int i=0;i<board.size()-1;i++)
				cout << dash_space << "+";
			cout << dash_space << "| ";
		}
		
		cout << "\n\n";
	}
}
