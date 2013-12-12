#include "Game.h"
#include <cctype>
#include <string>
#include <regex>
using namespace std;

Game::Game()
{

}

MoveResult Game::Move(string move)
{
	if (move == "resign")
		return whiteToMove ? BlackWins : WhiteWins;

	basic_regex kingsideCastling("0[^a-zA-Z0-9]0[^a-zA-Z0-9]");

//	regex_match(move, "0[^a-zA-Z0-9]0[^a-zA-Z0-9](0[^a-zA-Z0-9])?");

}
