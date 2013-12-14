#include <cmath>
#include "Board.h"
using namespace std;

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

	whiteToMove = true;
	whiteKingsideCastlingAvaible = true;
	whiteQueensideCastlingAvaible = true;
	blackKingsideCastlingAvaible = true;
	blackQueensideCastlingAvaible = true;
	enPassantFile = -1;
}

bool Board::IsWhiteToMove()
{
	return whiteToMove;
}

// Check that the way is free (regardless final position)
bool Board::IsValidMove(int fromFile, int fromRank, int toFile, int toRank)
{
	if (squares[fromFile][fromRank] == Piece::None)
		return false;
	if (!IsWayFree(fromFile, fromRank, toFile, toRank))
		return false;

	const Piece ownColor = whiteToMove ? Piece::White : Piece::Black;

	const int lastRank = whiteToMove ? 7 : 0;
	if ((squares[fromFile][fromRank] & Piece::Pawn) && (toRank == lastRank))
		throw "Pawn promotion is not specified";

	if (IsKingChecked(ownColor))
		return false;

	// TODO: check castling
}

bool Board::IsWayFree(int fromFile, int fromRank, int toFile, int toRank)
{
	// Check from/to bounds
	if (fromFile < 0 || fromFile > 7 ||
		fromRank < 0 || fromRank > 7 ||
		toFile < 0 || toFile > 7 ||
		toRank < 0 || toRank > 7)
	{
		return false;
	}

	// Correct piece color
	const Piece ownColor = whiteToMove ? Piece::White : Piece::Black;
	if (!(squares[fromFile][fromRank] & ownColor))
		return false;

	const Piece pieceKind = squares[fromFile][fromRank] & ~Piece::White & ~Piece::Black; // strip color

	switch(pieceKind)
	{
	case Piece::Pawn:
		if (!IsPawnWayFree(fromFile, fromRank, toFile, toRank))
			return false;
		break;

	case Piece::Rook:
		if (!IsRookWayFree(fromFile, fromRank, toFile, toRank))
			return false;
		break;

	case Piece::Knight:
		if (!IsKnightWayFree(fromFile, fromRank, toFile, toRank))
			return false;
		break;

	case Piece::Bishop:
		if (!IsBishopWayFree(fromFile, fromRank, toFile, toRank))
			return false;
		break;

	case Piece::Queen:
		if (!IsQueenWayFree(fromFile, fromRank, toFile, toRank))
			return false;
		break;

	case Piece::King:
		if (!IsKnightWayFree(fromFile, fromRank, toFile, toRank))
			return false;
		break;
	}
	return true;
}

// Check that the way is free (regardless final position)
bool Board::IsPawnWayFree(int fromFile, int fromRank, int toFile, int toRank)
{
	const int pawnMoveDirection = whiteToMove ? 1 : -1;

	if (fromFile == toFile) // move without capture
	{
		if (toRank - fromRank == pawnMoveDirection) // simple move
			return true;
		if (toRank - fromRank == 2 * pawnMoveDirection) // 2-square move
			return squares[fromFile][(fromFile+toFile)/2] == Piece::None;
		return false;
	}
	if (abs(fromFile-toFile) == 1) // capture
	{
		if (toRank - fromRank != pawnMoveDirection)
			return false;
		const int enPassantRank = whiteToMove ? 5 : 2;
		return (squares[toFile][toRank] != Piece::None) || (toFile == enPassantFile && toRank == enPassantRank);
	}
	return false;
}

// Check that the way is free (regardless final position)
bool Board::IsRookWayFree(int fromFile, int fromRank, int toFile, int toRank)
{
	if (fromFile == toFile)
	{
		bool direction = toRank > fromRank ? 1 : -1;
		for (int rank = fromRank + direction; rank != toRank; rank += direction)
			if (squares[fromFile][rank] != Piece::None)
				return false;
		return true;
	}
	if (fromRank == toRank)
	{
		bool direction = toFile > fromFile ? 1 : -1;
		for (int file = fromFile + direction; file != toFile; file += direction)
			if (squares[file][fromRank] != Piece::None)
				return false;
		return true;
	}
	return false;
}

// Check that the way is free (regardless final position)
bool Board::IsKnightWayFree(int fromFile, int fromRank, int toFile, int toRank)
{
	return  (abs(toFile-fromFile) == 1 && abs(toRank-fromRank) == 2) ||
			(abs(toFile-fromFile) == 2 && abs(toRank-fromRank) == 1);
}

// Check that the way is free (regardless final position)
bool Board::IsBishopWayFree(int fromFile, int fromRank, int toFile, int toRank)
{
	if (abs(fromFile-toFile) != abs(fromRank-toRank))
		return false;
	int filesDirection = toFile > fromFile ? 1 : -1;
	int ranksDirection = toRank > fromRank ? 1 : -1;
	int currentFile = fromFile;
	int currentRank = fromRank;
	for (int i = 0; i < abs(toFile - fromFile); i++)
	{
		currentFile += filesDirection;
		currentRank += ranksDirection;
		if (currentFile == toFile)
			break;
		if (squares[currentFile][currentRank] != Piece::None)
			return false;
	}
	return true;
}

// Check that the way is free (regardless final position)
bool Board::IsQueenWayFree(int fromFile, int fromRank, int toFile, int toRank)
{
	return IsRookWayFree(fromFile, fromRank, toFile, toRank) &&
		 IsBishopWayFree(fromFile, fromRank, toFile, toRank);
}

// Check that the way is free (regardless final position)
bool Board::IsKingWayFree(int fromFile, int fromRank, int toFile, int toRank)
{
	if ((abs(toFile - fromFile)) <= 1 && (abs(toRank - fromRank) <= 1))
		return true;
	// Castling
	if (abs(toFile - fromFile) == 2)
	{
		int direction = (toFile - fromFile) / 2; // +1/-1

		for (int file = fromFile + direction; (file != 0) && (file != 8); file += direction)
			if (squares[file][fromRank] != Piece::None)
				return false;
		if (IsAttacked(fromFile, fromRank) || IsAttacked(fromFile + direction, fromRank))
			return false;
	}
	return false;
}

// return true if the move is valid
MoveResult Board::Move(int fromFile, int fromRank, int toFile, int toRank)
{
	if(!IsValidMove(fromFile, fromRank, toFile, toRank))
		return MoveResult::InvalidMove;

	squares[toFile][toRank] = squares[fromFile][fromRank];
	squares[fromFile][fromRank] = Piece::None;
	return MoveResult::Default;
}

bool Board::IsKingChecked(Piece color)
{
	color &= Piece::White | Piece::Black;

	if (color == Piece::White | Piece::Black) // Check both kings
		return IsKingChecked(Piece::White) | IsKingChecked(Piece::Black);

	// Find the king
	Piece king = Piece::King & color;
	int kingFile;
	int kingRank;
	for (kingFile = 0; kingFile < 8; kingFile++)
	for (kingRank = 0; kingRank < 8; kingRank++)
	{
		if (squares[kingFile][kingRank] == king)
			return IsAttacked(kingFile, kingRank);
	}
}

bool Board::IsAttacked(int file, int rank, Piece attackerColor)
{
	for (int fromFile = 0; fromFile < 8; fromFile++)
	for (int fromRank = 0; fromRank < 8; fromRank++)
	{
		if ((squares[fromFile][fromRank] & attackerColor) && IsWayFree(fromFile, fromRank, file, rank))
			return true;
	}
	return false;
}
