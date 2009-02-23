#include "Player.h"
#include <iostream>
#include <stack>
#include <vector>
#include <cstring>

#define INF 65000
#define MAX(a,b) a>b?a:b
#define THE_OTHER player==player_type?other_player:player_type
using namespace std;

struct Pieces {
	int* mine;
	int* opponent;
};

class MinmaxPlayer : public trissa::Player {
public:
	static char* name;
	MinmaxPlayer(int dimension, trissa::PlayerType player_type) : 
		trissa::Player(dimension, player_type),
		depth(3)
	{
		other_player = player_type==trissa::PLAYER_CROSS ? trissa::PLAYER_CIRCLE : trissa::PLAYER_CROSS;

		init_pieces();

		my_board = new vector<vector<vector<trissa::PlayerType> > >(dimension, vector<vector<trissa::PlayerType> >(dimension, vector<trissa::PlayerType>(dimension, trissa::PLAYER_BLANK)));

		//For firstPlay usage
		next_move.x = dimension /2;
		next_move.y = dimension /2;
		next_move.z = dimension /2;
	}
	~MinmaxPlayer(){
		delete my_board;
		
	}
	virtual trissa::Move* play(trissa::Cube const& board, trissa::Move const& opponentMove){
		
		//update our copy of board
		(*my_board)[opponentMove.z][opponentMove.y][opponentMove.x] = other_player;
				
		this->alphabeta(this->depth, -INF, INF, player_type);
		(*my_board)[next_move.z][next_move.y][next_move.x] = player_type;
		
		return &next_move;

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
	int alphabeta(unsigned int depth, int alpha, int beta, trissa::PlayerType player){
		int alpha2;
		bool beta_cutoff = false;
		if(depth == 0 ) // || it's a final node
			return eval();
			
		for (unsigned int k=0; k < dimension  && !beta_cutoff; k++)				// | For each
			for (unsigned int j=0; j < dimension && !beta_cutoff; k++)			// | child of 
				for (unsigned int i=0; i < dimension && !beta_cutoff; i++){		// | current node	
					if((*my_board)[k][j][i] == trissa::PLAYER_BLANK){
						(*my_board)[k][j][i] = player;
						
						alpha2 = -alphabeta(depth-1, -beta, -alpha, THE_OTHER);
						alpha = MAX(alpha, alpha2);
						if(depth == this->depth && alpha2 > alpha){
							next_move.x=i; next_move.y=j; next_move.z=k;
						}
						(*my_board)[k][j][i] = trissa::PLAYER_BLANK;
						
						if(beta <= alpha)
							beta_cutoff = true;
					}
				}
		return alpha;
	}
	int eval(){
#define UPDATE_BOARDEVAL						\
	if(n_mine ^ n_opponent){ 					\
		if(n_mine) boardEval.mine[n_mine]++; 	\
		else boardEval.opponent[n_opponent]++; 	\
	}											\
	
		int n_mine = 0;
		int n_opponent = 0;
		for (unsigned int k=0; k<dimension; k++){
			//planes xy - horizontals
			for (unsigned int j=0; j<dimension; j++){
				for (unsigned int i=0; i<dimension; i++){
					if((*my_board)[k][j][i] == player_type)
						n_mine++;
					else if((*my_board)[k][j][i] == other_player)
						n_opponent++;
				}
				UPDATE_BOARDEVAL		
			}
			
			//planes xy - verticals
			n_mine = 0;
			n_opponent = 0;
			for (unsigned int i=0; i<dimension; i++){
				for (unsigned int j=0; j<dimension; j++){
					if((*my_board)[k][j][i] == player_type)
						n_mine++;
					else if((*my_board)[k][j][i] == other_player)
						n_opponent++;
				}
				UPDATE_BOARDEVAL
			}
			//planes xy - diagonal 1
			n_mine = 0;
			n_opponent = 0;			
			for (unsigned int i=0; i<dimension; i++){
				if((*my_board)[k][i][i] == player_type)
					n_mine++;
				else if((*my_board)[k][i][i] == other_player)
					n_opponent++;
			}
			UPDATE_BOARDEVAL
			//planes xy - diagonal 2
			n_mine = 0;
			n_opponent = 0;
			for (unsigned int i=0, j=dimension; i<dimension; i++, j--){
				if((*my_board)[k][j][i] == player_type)
					n_mine++;
				else if((*my_board)[k][j][i] == other_player)
					n_opponent++;
			}			
			UPDATE_BOARDEVAL
		}

		n_mine = 0;
		n_opponent = 0;
		for (unsigned int i=0; i<dimension; i++){
			//plane yz - verticals
			for (unsigned int j=0; j<dimension; j++){
				for (unsigned int k=0; k<dimension; k++){
					if((*my_board)[k][j][i] == player_type)
						n_mine++;
					else if((*my_board)[k][j][i] == other_player)
						n_opponent++;
				}			
				UPDATE_BOARDEVAL
			}
			//plane yz - diagonal 1
			n_mine = 0;
			n_opponent = 0;			
			for (unsigned int k=0; k<dimension; k++){
				if((*my_board)[k][k][i] == player_type)
					n_mine++;
				else if((*my_board)[k][k][i] == other_player)
					n_opponent++;
			}
			UPDATE_BOARDEVAL
			
			//plane yz - diagonal 2
			n_mine = 0;
			n_opponent = 0;
			for (unsigned int k=0, j=dimension; k<dimension; k++, j--){
				if((*my_board)[k][j][i] == player_type)
					n_mine++;
				else if((*my_board)[k][j][i] == other_player)
					n_opponent++;
			}			
			UPDATE_BOARDEVAL
			
		}
		
		//planes xz
		for (unsigned int j=0; j<dimension; j++){
			//Diagonal 1
			n_mine = 0;
			n_opponent = 0;			
			for (unsigned int k=0; k<dimension; k++){
				if((*my_board)[k][j][k] == player_type)
					n_mine++;
				else if((*my_board)[k][j][k] == other_player)
					n_opponent++;
			}
			UPDATE_BOARDEVAL
			
			//Diagonal 2
			n_mine = 0;
			n_opponent = 0;
			for (unsigned int k=0, i=dimension; k<dimension; k++, i--){
				if((*my_board)[k][j][i] == player_type)
					n_mine++;
				else if((*my_board)[k][j][i] == other_player)
					n_opponent++;
			}			
			UPDATE_BOARDEVAL
		}
		
		//Cube's Diagonal 1
		n_mine = 0;
		n_opponent = 0;
		for(unsigned int i=0; i<dimension; i++){
			if((*my_board)[i][i][i] == player_type)
				n_mine++;
			else if((*my_board)[i][i][i] == other_player)
				n_opponent++;		
		}
		UPDATE_BOARDEVAL

		//Cube's Diagonal 2
		n_mine = 0;
		n_opponent = 0;
		for(unsigned int i=0, k=dimension; i<dimension; i++, k--){
			if((*my_board)[k][i][i] == player_type)
				n_mine++;
			else if((*my_board)[k][i][i] == other_player)
				n_opponent++;		
		}
		UPDATE_BOARDEVAL

		//Cube's Diagonal 3
		n_mine = 0;
		n_opponent = 0;
		for(unsigned int i=0, j=dimension; i<dimension; i++, j--){
			if((*my_board)[i][j][i] == player_type)
				n_mine++;
			else if((*my_board)[i][j][i] == other_player)
				n_opponent++;		
		}
		UPDATE_BOARDEVAL

		//Cube's Diagonal 4
		n_mine = 0;
		n_opponent = 0;
		for(unsigned int i=dimension, j=0; j<dimension; i--, j++){
			if((*my_board)[j][j][i] == player_type)
				n_mine++;
			else if((*my_board)[j][j][i] == other_player)
				n_opponent++;		
		}
		UPDATE_BOARDEVAL
		return heuristicEval();
	}
	
	inline void init_pieces (){
		boardEval.mine = new int[dimension];
		boardEval.opponent = new int[dimension];
		memset(boardEval.mine, 0, dimension * sizeof(int));
		memset(boardEval.opponent, 0, dimension * sizeof(int));
	}
	
	inline int heuristicEval(){
		int ret = 0;
		for (unsigned int i=1; i<dimension-2; i++)
			ret += (boardEval.mine[i] - boardEval.opponent[i]) * i * i;
		
		if(boardEval.mine[dimension-1]) return INF/2;
		else if(boardEval.opponent[dimension-1]) return -INF/2;
		else return ret;
	}
	
	trissa::Move next_move;
	unsigned int depth;
	
	Pieces boardEval;

	/* A copy of the board is necessary in order to be able to play and "unplay" while finding the
	 * best position to play into.
	 */
	trissa::Cube* my_board;
	trissa::PlayerType other_player;
	
	
};
char* MinmaxPlayer::name = (char*)"Minmax Player";

REGISTER_PLAYER(MinmaxPlayer)

