#include "Player.h"
#include <iostream>
#include <stack>
#include <vector>
#include <cstring>

#define INF 65000
#define MAX(a,b) a>b?a:b
#define MIN(a,b) a<b?a:b
#define THE_OTHER player==player_type?other_player:player_type

using namespace std;

struct Pieces {
    int* mine;
    int* opponent;
};

class MinmaxPlayer : public trissa::Player
{
public:
    static char* name;
    MinmaxPlayer(int dimension, trissa::PlayerType player_type, trissa::UIInputOutput* ui) :
            trissa::Player(dimension, player_type, NULL),
            next_move(0,0,0),
            depth(2) //,
            //my_player(player_type)
    {
        other_player = player_type==trissa::PLAYER_CROSS ? trissa::PLAYER_CIRCLE : trissa::PLAYER_CROSS;

        init_pieces();

        my_board = new vector<vector<vector<trissa::PlayerType> > >(dimension, vector<vector<trissa::PlayerType> >(dimension, vector<trissa::PlayerType>(dimension, trissa::PLAYER_BLANK)));

        //For firstPlay usage
        next_move.x = dimension /2;
        next_move.y = dimension /2;
        next_move.z = dimension /2;
    }
    ~MinmaxPlayer() {
        delete[] boardEval.mine;
        delete[] boardEval.opponent;
        delete my_board;
    }
    virtual trissa::Move* play(trissa::Cube const& board, trissa::Move const& opponentMove) {

        //update our copy of board
        (*my_board)[opponentMove.z][opponentMove.y][opponentMove.x] = other_player;

#ifdef _TRISSA_DEBUG_
        int r = maximize(depth,-INF,INF, player_type, NULL);
        cout << "Ret: " << r << endl;
#else
        maximize(depth,-INF,INF, player_type, NULL);
#endif
        (*my_board)[next_move.z][next_move.y][next_move.x] = player_type;
        return &next_move;

    }
    virtual trissa::Move* firstPlay() {
        (*my_board)[next_move.z][next_move.y][next_move.x] = player_type;
        return &next_move;
    }
    virtual const char * getName() const {
        return name;
    }
    virtual bool isKnownBoard(unsigned int dimension) const {
        return true;
    }
private:
    bool goalTest(trissa::Move const& lastMove) {
        trissa::Move directions[] = {
            trissa::Move(1,0,0) , trissa::Move(0,1,0) , trissa::Move(0,0,1) ,
            trissa::Move(1,1,0) , trissa::Move(1,0,1) , trissa::Move(0,1,1) ,
            trissa::Move(1,-1,0), trissa::Move(1,0,-1), trissa::Move(0,1,-1),
            trissa::Move(1,1,1) , trissa::Move(1,-1,1), trissa::Move(-1,1,1), trissa::Move(1,1,-1)
        };
        int n_directions = 13;
        trissa::PlayerType player = (*my_board)[lastMove.z][lastMove.y][lastMove.x];

        for (int i=0; i < n_directions; i++) {
            bool invalid_direction = false;
            trissa::Move new_pos(lastMove.x,lastMove.y,lastMove.z);
            unsigned int n_pieces = 1;
            //first subtract direction
            while (true) {
                new_pos -= directions[i];
                if (new_pos.x >=0 && new_pos.y >=0 && new_pos.z >=0 &&
                        new_pos.x < (int)dimension && new_pos.y < (int)dimension && new_pos.z < (int)dimension) {
                    if ( (*my_board)[new_pos.z][new_pos.y][new_pos.x] != player) {
                        invalid_direction = true;
                        break;
                    } else
                        n_pieces++;
                } else { //out of cube
                    break;
                }
            }
            if (invalid_direction) continue; //next direction, this is not a winner direction

            //add direction to position until go out of cube
            new_pos.x = lastMove.x;
            new_pos.y = lastMove.y;
            new_pos.z = lastMove.z;
            while (true) {
                new_pos += directions[i];
                if (new_pos.x >=0 && new_pos.y >=0 && new_pos.z >=0 &&
                        new_pos.x < (int)dimension && new_pos.y < (int)dimension && new_pos.z < (int)dimension) {
                    if ( (*my_board)[new_pos.z][new_pos.y][new_pos.x] != player) {
                        invalid_direction = true;
                        break;
                    } else
                        n_pieces++;
                } else { //out of cube
                    break;
                }
            }
            if (!invalid_direction && n_pieces == dimension) {
                return (true);
            }
        }
        return false;

    }

    int maximize(unsigned int depth, int alpha, int beta, trissa::PlayerType player, trissa::Move const* lastMove) {
        if (depth == 0 || (lastMove != NULL && goalTest(*lastMove))) {
            return eval();
        }
        int v = -INF;
        for (unsigned int k=0; k < dimension; k++) {                // | For each
            for (unsigned int j=0; j < dimension; j++) {            // | child of
                for (unsigned int i=0; i < dimension; i++) {        // | current node
                    if ((*my_board)[k][j][i] == trissa::PLAYER_BLANK) {
                        (*my_board)[k][j][i] = player;                  //play in a blank position
                        trissa::Move move(i,j,k);
                        int v2 = minimize(depth-1, alpha, beta, THE_OTHER, &move);
                        (*my_board)[k][j][i] = trissa::PLAYER_BLANK;    //undo played position
                        if (v2 > v) {
                            v = v2;
                            if (depth == this->depth) {
                                next_move.x = i;
                                next_move.y = j;
                                next_move.z = k;
                            }
                        }
#ifdef _TRISSA_DEBUG_
                        if (depth == this->depth) {
                            cout << "[MINMAX] Son: [" << k << "," << j << "," << i <<"]" << endl << endl;
                        }
                        cout.flush();
#endif
                        if (v >= beta)
                            return v;
                        alpha = MAX (alpha, v);
                    }
                }
            }
        }
        return v;
    }

    int minimize(unsigned int depth, int alpha, int beta, trissa::PlayerType player, trissa::Move const* lastMove) {
        if (depth == 0 || (lastMove != NULL && goalTest(*lastMove))) {
            int r = eval();
#ifdef _TRISSA_DEBUG_
            cout << "[MINIMAX] mine: ";
            for (unsigned int i=0; i <= dimension; i++)
                cout << boardEval.mine[i] << " | ";
            cout << endl << "[MINIMAX] oppo: ";
            for (unsigned int i=0; i <= dimension; i++)
                cout << boardEval.opponent[i] << " | ";
            cout << endl;
            cout << "ret: " << r << endl;
            cout.flush();
#endif
            return r;
        }
        int v = INF;
        for (unsigned int k=0; k < dimension; k++) {				// | For each
            for (unsigned int j=0; j < dimension; j++) {			// | child of
                for (unsigned int i=0; i < dimension; i++) {		// | current node
                    if ((*my_board)[k][j][i] == trissa::PLAYER_BLANK) {
                        (*my_board)[k][j][i] = player;                  //play in a blank position

                        trissa::Move move(i,j,k);
                        v = MIN (v, maximize(depth-1, alpha, beta, THE_OTHER, &move));

                        (*my_board)[k][j][i] = trissa::PLAYER_BLANK;    //undo played position
                        if (v <= alpha)
                            return v;
                        beta = MIN (beta, v);
                    }
                }
            }
        }
        return v;
    }


    /*Sum up the number of i-aligned pieces of each player and update boardEval structure,
     *where i = 1,..,dimension
     */
    int eval() {
#define UPDATE_BOARDEVAL                     \
        if(n_mine>0 && n_opponent==0 )       \
            boardEval.mine[n_mine]++;        \
        else if (n_opponent>0 && n_mine==0 ) \
            boardEval.opponent[n_opponent]++;

        //trissa::PlayerType& my_player = this->player_type;

        memset(boardEval.mine, 0, (dimension+1) * sizeof(int));
        memset(boardEval.opponent, 0, (dimension+1) * sizeof(int));
        //planes XY
        for (unsigned int k=0; k<dimension; k++) {
            //planes xy - horizontals
            for (unsigned int j=0; j<dimension; j++) {
                unsigned int n_mine = 0, n_opponent = 0;
                for (unsigned int i=0; i<dimension; i++) {
                    if ((*my_board)[k][j][i] == player_type)
                        n_mine++;
                    else if ((*my_board)[k][j][i] == other_player)
                        n_opponent++;
                }
                UPDATE_BOARDEVAL
            }

            //planes xy - verticals
            for (unsigned int i=0; i<dimension; i++) {
                unsigned int n_mine = 0, n_opponent = 0;
                for (unsigned int j=0; j<dimension; j++) {
                    if ((*my_board)[k][j][i] == player_type)
                        n_mine++;
                    else if ((*my_board)[k][j][i] == other_player)
                        n_opponent++;
                }
                UPDATE_BOARDEVAL
            }

            //planes xy - diagonal 1
            {
                unsigned int n_mine = 0, n_opponent = 0;
                for (unsigned int i=0; i<dimension; i++) {
                    if ((*my_board)[k][i][i] == player_type)
                        n_mine++;
                    else if ((*my_board)[k][i][i] == other_player)
                        n_opponent++;
                }
                UPDATE_BOARDEVAL
            }
            //planes xy - diagonal 2
            {
                unsigned int n_mine = 0, n_opponent = 0;
                for (unsigned int i=0, j=dimension-1; i<dimension; i++, j--) {
                    if ((*my_board)[k][j][i] == player_type)
                        n_mine++;
                    else if ((*my_board)[k][j][i] == other_player)
                        n_opponent++;
                }
                UPDATE_BOARDEVAL
            }
        }

        //planes YZ
        for (unsigned int i=0; i<dimension; i++) {
            //plane yz - horizontals == plane xy - verticals, already made
            //plane yz - verticals
            for (unsigned int j=0; j<dimension; j++) {
                unsigned int n_mine = 0, n_opponent = 0;
                for (unsigned int k=0; k<dimension; k++) {
                    if ((*my_board)[k][j][i] == player_type)
                        n_mine++;
                    else if ((*my_board)[k][j][i] == other_player)
                        n_opponent++;
                }
                UPDATE_BOARDEVAL
            }
            //plane yz - diagonal 1
            {
                unsigned int n_mine = 0, n_opponent = 0;
                for (unsigned int k=0; k<dimension; k++) {
                    if ((*my_board)[k][k][i] == player_type)
                        n_mine++;
                    else if ((*my_board)[k][k][i] == other_player)
                        n_opponent++;
                }
                UPDATE_BOARDEVAL
            }
            //plane yz - diagonal 2
            {
                unsigned int n_mine = 0, n_opponent = 0;
                for (unsigned int k=0, j=dimension-1; k<dimension; k++, j--) {
                    if ((*my_board)[k][j][i] == player_type)
                        n_mine++;
                    else if ((*my_board)[k][j][i] == other_player)
                        n_opponent++;
                }
                UPDATE_BOARDEVAL
            }
        }

        //planes XZ
        for (unsigned int j=0; j<dimension; j++) {
            //plane XZ - horizontals == plane XY - horizontals, already made
            //plane XZ - verticals   == plane YZ - verticals, already mad
            //Diagonal 1
            {
                unsigned int n_mine = 0, n_opponent = 0;
                for (unsigned int k=0; k<dimension; k++) {
                    if ((*my_board)[k][j][k] == player_type)
                        n_mine++;
                    else if ((*my_board)[k][j][k] == other_player)
                        n_opponent++;
                }
                UPDATE_BOARDEVAL
            }

            //Diagonal 2
            {
                unsigned int n_mine = 0, n_opponent = 0;
                for (unsigned int k=0, i=dimension-1; k<dimension; k++, i--) {
                    if ((*my_board)[k][j][i] == player_type)
                        n_mine++;
                    else if ((*my_board)[k][j][i] == other_player)
                        n_opponent++;
                }
                UPDATE_BOARDEVAL
            }
        }

        //Cube's Diagonal 1
        {
            unsigned int n_mine = 0, n_opponent = 0;
            for (unsigned int i=0; i<dimension; i++) {
                if ((*my_board)[i][i][i] == player_type)
                    n_mine++;
                else if ((*my_board)[i][i][i] == other_player)
                    n_opponent++;
            }
            UPDATE_BOARDEVAL
        }

        //Cube's Diagonal 2
        {
            unsigned int n_mine = 0, n_opponent = 0;
            for (unsigned int i=0, k=dimension-1; i<dimension; i++, k--) {
                if ((*my_board)[k][i][i] == player_type)
                    n_mine++;
                else if ((*my_board)[k][i][i] == other_player)
                    n_opponent++;
            }
            UPDATE_BOARDEVAL
        }

        //Cube's Diagonal 3
        {
            unsigned int n_mine = 0, n_opponent = 0;
            for (unsigned int i=0, j=dimension-1; i<dimension; i++, j--) {
                if ((*my_board)[i][j][i] == player_type)
                    n_mine++;
                else if ((*my_board)[i][j][i] == other_player)
                    n_opponent++;
            }
            UPDATE_BOARDEVAL
        }
        //Cube's Diagonal 4
        {
            unsigned int n_mine = 0, n_opponent = 0;
            for (unsigned int i=dimension-1, j=0; j<dimension; i--, j++) {
                if ((*my_board)[j][j][i] == player_type)
                    n_mine++;
                else if ((*my_board)[j][j][i] == other_player)
                    n_opponent++;
            }
            UPDATE_BOARDEVAL
        }

        return heuristicEval();
    }

    /* Uses values stored in boardEval structure to return a "state pontuation"
    */
    inline int heuristicEval() {
        if (boardEval.mine[dimension] > 0) {
            return INF;
        } else if (boardEval.opponent[dimension] > 0) {
            return -INF;
        } else {
            int ret = 0;
            for (unsigned int i=1; i<dimension; i++)
                ret += (boardEval.mine[i] - boardEval.opponent[i]) * i * i;
            return ret;
        }
    }

    /* Initializes boardEval structure which keeps track of number of aligned pieces of each player
     * Basically there are 2 vectors each of which keeps track of i-aligned pieces,
     * where "i" is the index position in vector.
     */
    inline void init_pieces () {
        boardEval.mine = new int[dimension+1];
        boardEval.opponent = new int[dimension+1];
        memset(boardEval.mine, 0, (dimension+1) * sizeof(int));
        memset(boardEval.opponent, 0, (dimension+1) * sizeof(int));
    }


    trissa::Move next_move;
    unsigned int depth;
    Pieces boardEval;

    /* A copy of the board is necessary in order to be able to play and "unplay" while finding the
     * best position to play into.
     */
    trissa::Cube* my_board;

    trissa::PlayerType other_player;
    //TODO: Change variable name to become clearer: "my_player" instead of "player_type"
    //trissa::PlayerType& my_player;


};
char* MinmaxPlayer::name = (char*)"Minmax Player";

REGISTER_PLAYER(MinmaxPlayer)

