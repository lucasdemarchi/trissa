/*
 * UIText.cc
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
#include "UIText.h"
#include "ConfigManager.h"
#include "PlayerFactory.h"
#include <boost/lexical_cast.hpp>
#include <vector>

namespace trissa {
    using namespace std;
    void UIText::configure(){
        cout << endl << "=== Configuration Options === " << endl;
        if(!mCm->getDimension())
            mCm->setDimension(getDimension());
        else
            cout << "Dimension: " << mCm->getDimension() << endl;

        if(mCm->getPlayerA() == "" )
            mCm->setPlayerA(getPlayer("Player A"));
        else
            cout << "Player A (it will start): " << mCm->getPlayerA() << endl;


        if(mCm->getPlayerB() == "" )
            mCm->setPlayerB(getPlayer("Player B"));
        else
            cout << "Player B: " << mCm->getPlayerB() << endl;


        while ( !confirmConfig() ){
            cout << endl << endl << "=== Configuration Options === " << endl;
            mCm->setDimension(getDimension());
            mCm->setPlayerA(getPlayer("Player A"));
            mCm->setPlayerB(getPlayer("Player B"));
        }
        UI::configure();
    }
    bool UIText::confirmConfig(){
        string resp;
        cout << endl << "Current configuration: " << endl;
        cout << "Dimension: " << mCm->getDimension() << endl;
        cout << "Player A (it will start): " << mCm->getPlayerA() << endl;
        cout << "Player B: " << mCm->getPlayerB() << endl;
        cout << "Confirm configuration for game? (yes/no): ";
        cin >> resp;
        return (resp == "yes");
    }
    unsigned int UIText::getDimension(){
        int d;
        string s;
        cin.sync();
        do{
            cout << "Enter game's dimension ( >= 3): ";
            cin >> s;
            try {
                d = boost::lexical_cast<int>( s );
            } catch ( boost::bad_lexical_cast const& ) {
                cerr << "Not a good value. It must be a number and >= 3" << endl;
                d=0;
            }
        } while ( d < 3 );
        return d;
    }
    string UIText::getPlayer(std::string player){
        vector<string> strplayers;
        mPf->getPlayersList(strplayers);

        cout << "Available players: " << endl;
        for (unsigned int i=0; i < strplayers.size(); i++){
            cout << i << ": " << strplayers[i] << endl;
        }
		while(true) {
		    int player_n;
		    string s;

		    cin.sync();
            cout << "Enter number for " << player << ": ";
            cin >> s;

            try {
                player_n = boost::lexical_cast<int>( s );
            } catch ( boost::bad_lexical_cast const& ) {
                player_n = -1;
            }

            if ( player_n < 0 || player_n >= static_cast<int>(strplayers.size()))
                cerr << endl << "Invalid player option. Choose one from the list above." << endl;
            else
                return string( strplayers[player_n] );

        }

    }
    void UIText::start(){
        UI::start();
    }
    void UIText::refresh(Cube const& board, Move const& lastMove){
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
    void UIText::refresh(Cube const& board, Move const& lastMove, bool wait){
        string s;
	    this->refresh(board, lastMove);
	    if(wait){
	        cin.sync();
	        cout << "Press [ENTER] to get next move" << endl;
	        getline(cin,s);
	    }
    }
    void UIText::setPos(Move m, PlayerType player){

    }
}
