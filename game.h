#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

enum piece {
    EMPTY       = 0x00,

    WHITE       = 0x01,
    BLACK       = 0x02,
    COLOR_MASK  = WHITE|BLACK,

    PAWN        = 0x04,
    KNIGHT      = 0x08,
    BISHOP      = 0x10,
    ROOK        = 0x20,
    QUEEN       = 0x40,
    KING        = 0x80,
    PIECE_MASK  = PAWN|KNIGHT|BISHOP|ROOK|QUEEN|KING,
};

enum move_result {
    DEFAULT = 0,
    CHECK,
    CHECKMATE,
    DRAW,
    ILLEGAL,
};

struct game {
    enum piece board[8][8];
    int side_to_move;
    enum piece white_castling_avail; // QUEEN|KING for kingside and queenside
    enum piece black_castling_avail;
    int en_passant_file;
};

struct square {
    int rank;
    int file;
};

extern const struct game setup;

enum move_result make_move(struct game *game, struct square from, struct square to);

#endif // GAME_H
