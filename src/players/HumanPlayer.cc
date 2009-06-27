#include "Player.h"
//#include <iostream>


using namespace std;

class HumanPlayer : public trissa::Player
{
public:
	static char* name;

	HumanPlayer(int dimension, trissa::PlayerType my_player, trissa::UIInputOutput* ui) :
			trissa::Player(dimension, my_player, ui),
			next_move(0,0,0) {

	}
	~HumanPlayer() {
	}

	trissa::Move* play(trissa::Cube const& board, trissa::Move const& opponentMove) {
		next_move = ui->getUserInput();
		return &next_move;
	}
	trissa::Move* firstPlay() {
		next_move = ui->getUserInput();
		return &next_move;
	}
	const char* getName() const {
		return name;
	}
	bool isKnownBoard(unsigned int dimension) const {
		return true;
	}
protected:
	trissa::Move next_move;
};
char* HumanPlayer::name = (char*)"Human Player";

REGISTER_PLAYER(HumanPlayer);