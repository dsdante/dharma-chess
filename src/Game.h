#ifndef GAME_H_
#define GAME_H_

#include <string>
#include "Board.h"
using namespace std;

class Game
{
private:
	Board board;
public:
	Game();
	MoveResult Move(string move);
};

#endif /* GAME_H_ */
