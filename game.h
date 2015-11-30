#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

enum piece {
    EMPTY      = 0x00,

    WHITE      = 0x01,
    BLACK      = 0x02,
    COLOR      = WHITE|BLACK,

    PAWN       = 0x04,
    KNIGHT     = 0x08,
    BISHOP     = 0x10,
    ROOK       = 0x20,
    QUEEN      = 0x40,
    KING       = 0x80,
    PIECE_TYPE = PAWN|KNIGHT|BISHOP|ROOK|QUEEN|KING,
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
    enum piece side_to_move; // WHITE or BLACK
    enum piece white_castling_avail; // QUEEN|KING for kingside and queenside
    enum piece black_castling_avail;
    int en_passant_file;
    int halfmove_clock; // track fifty-move rule
    int position_history[256];
};

struct square {
    int file;
    int rank;
};

extern const struct game setup; // starting position
extern const char *move_result_text[];

enum move_result move(struct game *game, struct square from,
                      struct square to, enum piece promotion);
enum move_result parse_move(struct game *game, char *move);
char* move_result_to_string(enum move_result move_result);
#endif // GAME_H
