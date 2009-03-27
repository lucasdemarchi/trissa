#include "Player.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

class RandomPlayer : public trissa::Player
{
public:
    static char* name;
    RandomPlayer(int dimension, trissa::PlayerType player_type) :
            trissa::Player(dimension, player_type, NULL),
            next_move(0,0,0) {
        srand (time(NULL));
        //For firstPlay usage
        next_move.x = dimension /2;
        next_move.y = dimension /2;
        next_move.z = dimension /2;
    }
    ~RandomPlayer() {
    }
    virtual trissa::Move* play(trissa::Cube const& board, trissa::Move const& opponentMove) {
        int rx, ry, rz;
        rx = rand() % (dimension);
        ry = rand() % (dimension);
        rz = rand() % (dimension);
        //pos occupied? Get next available
        for (unsigned int z = rz; z-rz < dimension; z++)
            for (unsigned int y = ry; y-ry < dimension; y++)
                for (unsigned int x = rx; x-rx < dimension; x++)
                    if ( board[z%dimension][y%dimension][x%dimension] == trissa::PLAYER_BLANK) {
                        next_move.x = x % dimension;
                        next_move.y = y % dimension;
                        next_move.z = z % dimension;
                        return &next_move;
                    }

        //if i'm here, no available positions to play
        cerr << "RandomPlayer: " << "No available positions to play. "
             "Maybe there's a bug in trissa's core. Contact developers.\n";
        return NULL;
    }
    virtual trissa::Move* firstPlay() {
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

