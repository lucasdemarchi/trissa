#include <string>
#include "common.h"
		
namespace trissa {
	class Player;

	class PlayerException : public exception{
	public:
		static const int BOARD_NOT_KNOWN = 0;
		PlayerException(int idException, Player*  player);
		virtual const char* what();
	private:
		int idException;
		Player* player;
	};
	
	class Player{
	public:
		Player(unsigned int dimension) throw () : dimension(dimension) {
			if (!isKnownBoard(dimension))
				throw PlayerException(0, this);
				
		}
		
		virtual ~Player(){
		}
		virtual Move& play(Cube const& board, const Move* opponentMove) = 0;
		//virtual Move& firstPlay() = 0;
		virtual const char * getName() = 0;
		virtual bool isKnownBoard(unsigned int dimension) {
			return false;
		}
	protected:
		unsigned int dimension;
		friend class PlayerException;
	};
	
	PlayerException::PlayerException(int idException, Player*  player): 
			idException(idException),
			player(player){
	}
	const char * PlayerException::what(){
		string str_error("Player error: ");
		if (idException == BOARD_NOT_KNOWN) {
				str_error += (player->getName());
				str_error += " doesn't know how to play in board with size ";
				str_error += (player->dimension);
				return str_error.c_str();
		}
		return "Unknown error";
	}	


}
