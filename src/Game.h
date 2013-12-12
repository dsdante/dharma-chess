#ifndef GAME_H_
#define GAME_H_

#include <string>
using namespace std;

enum MoveResult
{
	Default = 0x00,

	IncorrectInput = 0x01,
	InvalidMove = 0x02,

	WhiteWins = 0x10,
	BlackWins = 0x20,
	Draw = 0x40,
};

class Game
{
private:
	bool whiteToMove = true;

public:
	Game();
	MoveResult Move(string move);
};

#endif /* GAME_H_ */
