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
#include "ConfigManager.h"
#include "PlayerFactory.h"
#include "StateManager.h"
#include <cstring>
#include <string>

namespace trissa {
	using namespace std;

	UI::UI(ConfigManager* cm, PlayerFactory* pf, StateManager* sm) :
        mCm( cm ),
        mPf( pf ),
        mSm( sm ){

        // CONFIGURE STUFF: in a Text UI, could be only "linking" to an output text file
        //                  in a 3D UI, must configure the window, renderer system, etc
        mSm->requestStateChange(GUI);

	}

    void UI::configure(){
        // If it's monothreaded, it must remain here till changing state
        //.
        //.
        //.
        //TODO: interact with ConfigManager and PlayerFactory to change options
        //.
        //.
        //.
        mSm->requestStateChange(GAME);
    }

    void UI::start() {
        // Finish loading resources and, render stuff
    }

	UI::~UI(){
	}

    //TODO: remove this and put it to set the player into ConfigManager
//	void UI::getPlayers(vector<string> strplayers, string& strplayerA, string& strplayerB){
//		cout << "Available players: " << endl;
//		for (unsigned int i=0; i < strplayers.size(); i++){
//			cout << i << ": " << strplayers[i] << endl;
//		}
//		int playerA, playerB;
//		cout << "Enter number for player A (it will have the first move): ";
//		cin >> playerA;
//		cout << "Enter number for player B: ";
//		cin >> playerB;
//		strplayerA = strplayers[playerA];
//		strplayerB = strplayers[playerB];
//	}


	void UI::refresh(Cube const& board, Move const& lastMove){
		char * dash_space = (char*) "---";
		char * blank_space = (char*) "   ";

		for (unsigned int j = 0; j < board.size(); j++){
			for (unsigned int k = 0; k < board.size(); k++)
			{
				cout << "|";
				for(unsigned int i=0;i<board.size()-1;i++)
					cout << dash_space << "+";
				cout << dash_space << "| ";
			}

			cout << "\n";
			for (unsigned int k = 0; k < board.size(); k++)
			{
				cout << "|";
				for(unsigned int i=0;i<board.size();i++){
					if(board[k][j][i] == PLAYER_BLANK)
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
		for (unsigned int k = 0; k < board.size(); k++)
		{
			cout << "|";
			for(unsigned int i=0;i<board.size()-1;i++)
				cout << dash_space << "+";
			cout << dash_space << "| ";
		}

		cout << "\n\n";
	}

	void UI::refresh(Cube const& board, Move const& lastMove, bool wait){
	    string s;
	    this->refresh(board, lastMove);
	    if(wait){
	        cin.sync();
	        cout << "Press [ENTER] to get next move" << endl;
	        getline(cin,s);
	    }
	}

}
