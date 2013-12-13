#include "Game.h"
#include <cctype>
#include <string>
using namespace std;

Game::Game()
{
}

MoveResult Game::Move(string move)
{
	if (move == "resign")
		return board.IsWhiteToMove() ? BlackWins : WhiteWins;
	return MoveResult::Default;
}
