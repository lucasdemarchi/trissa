#include "Player.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

class RandomPlayer : public trissa::Player {
public:
	static char* name;
	RandomPlayer(int dimension) : trissa::Player(dimension){
		srand (time(NULL));
		//For firstPlay usage
		next_move.x = dimension /2;
		next_move.y = dimension /2;
		next_move.z = dimension /2;
	}
	~RandomPlayer(){
	}
	virtual trissa::Move* play(trissa::Cube const& board, trissa::Move const& opponentMove){
		int rx, ry, rz;
		rx = rand() % (dimension);
		ry = rand() % (dimension);
		rz = rand() % (dimension);
		//pos occupied? Get next available
		for(int z = rz; z-rz < dimension; z++)
			for(int y = ry; y-ry < dimension; y++)
				for(int x = rx; x-rx < dimension; x++)
					if( board[z%dimension][y%dimension][x%dimension] == PLAYER_NONE){
						next_move.x = x % dimension;
						next_move.y = y % dimension;
						next_move.z = z % dimension;
						return &next_move;
					}

		//if i'm here, no available positions to play
		return NULL;
	}
	virtual trissa::Move* firstPlay(){
		return &next_move;
	}
	virtual const char * getName() const {
		return name;
	}
	virtual bool isKnownBoard(unsigned int dimension) const {
		return true;
	}
private:
	trissa::Move next_move;
};
char* RandomPlayer::name = (char*)"Random Player";

REGISTER_PLAYER(RandomPlayer)

