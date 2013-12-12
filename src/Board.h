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
/*
struct Square
{
	unsigned char column;
	unsigned char row;

	Square(column, row)
	{
		if (column < 0 || column > 7 || row < 0 || row > 7)
			throw "invalid square coordinates";
		this->column = column;
		this->row = row;
	}
};

class Board
{
	bool IsCorrectMove(Square from, Square to);

public:
	Piece squares[8][8] = { };

	Board();
	MoveResult Move(Square from, Square to);
};
*/
#endif /* BOARD_H_ */
