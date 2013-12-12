#include "Board.h"
/*
Board::Board()
{
	squares[0][0] = Piece::WRook;
	squares[1][0] = Piece::WKnight;
	squares[2][0] = Piece::WBishop;
	squares[3][0] = Piece::WQueen;
	squares[4][0] = Piece::WKing;
	squares[5][0] = Piece::WBishop;
	squares[6][0] = Piece::WKnight;
	squares[7][0] = Piece::WRook;
	squares[0][1] = Piece::WPawn;
	squares[1][1] = Piece::WPawn;
	squares[2][1] = Piece::WPawn;
	squares[3][1] = Piece::WPawn;
	squares[4][1] = Piece::WPawn;
	squares[5][1] = Piece::WPawn;
	squares[6][1] = Piece::WPawn;
	squares[7][1] = Piece::WPawn;
	squares[0][6] = Piece::BPawn;
	squares[1][6] = Piece::BPawn;
	squares[2][6] = Piece::BPawn;
	squares[3][6] = Piece::BPawn;
	squares[4][6] = Piece::BPawn;
	squares[5][6] = Piece::BPawn;
	squares[6][6] = Piece::BPawn;
	squares[7][6] = Piece::BPawn;
	squares[0][7] = Piece::BRook;
	squares[1][7] = Piece::BKnight;
	squares[2][7] = Piece::BBishop;
	squares[3][7] = Piece::BQueen;
	squares[4][7] = Piece::BKing;
	squares[5][7] = Piece::BBishop;
	squares[6][7] = Piece::BKnight;
	squares[7][7] = Piece::BRook;
}

bool Board::IsCorrectMove(Square from, Square to)
{
	if (squares[from.column][from.row] == Piece::None)
		return false;

	// TODO: implement IsCorrectMove
	return true;
}

// return true if the move is valid
bool Board::Move(Square from, Square to)
{
	if(!IsCorrectMove(from, to))
		return false;

	squares[to.column][to.row] = squares[from.column][from.row];
	squares[from.column][from.row] = Piece::None;
	return true;
}
*/
