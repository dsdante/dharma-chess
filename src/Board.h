#ifndef BOARD_H_
#define BOARD_H_

enum Piece
{
	None = 0x00,

	King = 0x01,
	Queen = 0x02,
	Rook = 0x03,
	Bishop = 0x04,
	Knight = 0x05,
	Pawn = 0x06,

	White = 0x10,
	Black = 0x20,

	WKing = King | White,
	WQueen = Queen | White,
	WRook = Rook | White,
	WBishop = Bishop | White,
	WKnight = Knight | White,
	WPawn = Pawn | White,
	BKing = King | Black,
	BQueen = Queen | Black,
	BRook = Rook | Black,
	BBishop = Bishop | Black,
	BKnight = Knight | Black,
	BPawn = Pawn | Black,
};

enum MoveResult
{
	Default = 0x00,

	IncorrectInput = 0x01,
	InvalidMove = 0x02,

	WhiteWins = 0x10,
	BlackWins = 0x20,
	Draw = 0x40,
};

class Board
{
private:
	bool whiteToMove;
	bool whiteKingsideCastlingAvaible;
	bool whiteQueensideCastlingAvaible;
	bool blackKingsideCastlingAvaible;
	bool blackQueensideCastlingAvaible;
	int enPassantFile;

	bool IsValidMove	(int fromFile, int fromRank, int toFile, int toRank);
	bool IsWayFree		(int fromFile, int fromRank, int toFile, int toRank);
	bool IsPawnWayFree	(int fromFile, int fromRank, int toFile, int toRank);
	bool IsRookWayFree	(int fromFile, int fromRank, int toFile, int toRank);
	bool IsKnightWayFree(int fromFile, int fromRank, int toFile, int toRank);
	bool IsBishopWayFree(int fromFile, int fromRank, int toFile, int toRank);
	bool IsQueenWayFree	(int fromFile, int fromRank, int toFile, int toRank);
	bool IsKingWayFree	(int fromFile, int fromRank, int toFile, int toRank);
	bool IsKingChecked	(Piece color);

public:
	Piece squares[8][8] = { };

	Board();
	MoveResult Move(int from, int fromRank, int to, int toRank);
	bool IsWhiteToMove();
};

#endif /* BOARD_H_ */
