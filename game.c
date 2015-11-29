#include <assert.h>
#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "log.h"

bool is_attacked_by(const struct game *game, struct square square, enum piece color);
bool is_attacked(const struct game *game, struct square square);
bool is_checked(const struct game *game, enum piece color);

const char *move_result_text[] = {
    "default",
    "check",
    "checkmate",
    "draw",
    "illegal",
};

// Starting position
const struct game setup = {
    .board = {
        { WHITE|ROOK,   WHITE|PAWN, 0, 0, 0, 0, BLACK|PAWN, BLACK|ROOK   },
        { WHITE|KNIGHT, WHITE|PAWN, 0, 0, 0, 0, BLACK|PAWN, BLACK|KNIGHT },
        { WHITE|BISHOP, WHITE|PAWN, 0, 0, 0, 0, BLACK|PAWN, BLACK|BISHOP },
        { WHITE|QUEEN,  WHITE|PAWN, 0, 0, 0, 0, BLACK|PAWN, BLACK|QUEEN  },
        { WHITE|KING,   WHITE|PAWN, 0, 0, 0, 0, BLACK|PAWN, BLACK|KING   },
        //{ WHITE|KING,   WHITE|PAWN, 0, BLACK|KING, 0, 0, BLACK|PAWN,   },
        { WHITE|BISHOP, WHITE|PAWN, 0, 0, 0, 0, BLACK|PAWN, BLACK|BISHOP },
        { WHITE|KNIGHT, WHITE|PAWN, 0, 0, 0, 0, BLACK|PAWN, BLACK|KNIGHT },
        { WHITE|ROOK,   WHITE|PAWN, 0, 0, 0, 0, BLACK|PAWN, BLACK|ROOK   } },
    
    .side_to_move = WHITE,
    .white_castling_avail = KING | QUEEN,
    .black_castling_avail = KING | QUEEN,
    .en_passant_file = -1, 
};

enum piece piece_at(const struct game *game, struct square square)
{
    return game->board[square.file][square.rank]; 
} 

/*
 * Check the destination correctness and the free way to it.
 * We already know that there is no own piece in the destination.
 */

bool pawn_has_way(const struct game *game, struct square from, struct square to)
{
    assert((piece_at(game, from) & PIECE_TYPE) == PAWN && "checking not pawn");
    int direction = ((piece_at(game, from) & COLOR) == WHITE) ? 1 : -1;
    int advance = to.rank - from.rank;

    // Just move, no capture
    if (from.file == to.file) {
        if (piece_at(game, to) != EMPTY)
            return false;
        if (advance == direction && piece_at(game, to) == EMPTY)
            return true;
        if (advance == 2 * direction) {
            int pawn_start_rank = (game->side_to_move == WHITE) ? 1 : 6;
            if (from.rank != pawn_start_rank)
                return false;
            if (game->board[from.file][pawn_start_rank + direction] != EMPTY)
                return false;
            return true;
        }
        return false;
    }

    // Capture
    if (abs(from.file - to.file) != 1)
        return false;
    if (advance != direction)
        return false;
    if (piece_at(game, to) != EMPTY)
        return true;
    int en_passant_rank = (game->side_to_move == WHITE) ? 5 : 2;
    if (to.file == game->en_passant_file && to.rank == en_passant_rank)
        return true;
    return false;
}

bool knight_has_way(struct square from, struct square to)
{
    int file_move = abs(from.file - to.file);
    int rank_move = abs(from.rank - to.rank);
    if ((file_move == 1 && rank_move == 2) || (file_move == 2 && rank_move == 1))
        return true;
    return false; 
}

bool bishop_has_way(const struct game *game, struct square from, struct square to)
{
    int file_move = abs(from.file - to.file);
    int rank_move = abs(from.rank - to.rank);
    if (file_move != rank_move)
        return false;
    int file_direction = (to.file > from.file) ? 1 : -1;
    int rank_direction = (to.rank > from.rank) ? 1 : -1;
    int file = from.file;
    int rank = from.rank;
    for (int i = 1; i < file_move; i++) {
        file += file_direction;
        rank += rank_direction;
        if (game->board[file][rank] != EMPTY)
            return false;
    }
    return true;
}

bool rook_has_way(const struct game *game, struct square from, struct square to)
{
    if (from.file != to.file && from.rank != to.rank)
        return false;

    if (from.file == to.file) {
        int direction = (to.rank > from.rank) ? 1 : -1;
        for (int rank = from.rank + direction; rank != to.rank; rank += direction)
            if (game->board[from.file][rank] != EMPTY)
                return false;
    }

    if (from.rank == to.rank) {
        int direction = (to.file > from.file) ? 1 : -1;
        for (int file = from.file + direction; file != to.file; file += direction)
            if (game->board[file][from.rank] != EMPTY)
                return false;
    } 

    return true;
}

bool queen_has_way(const struct game *game, struct square from, struct square to)
{
    assert((piece_at(game, from) & PIECE_TYPE) == QUEEN && "checking not queen");
    return bishop_has_way(game, from, to) || rook_has_way(game, from, to);
}

bool king_has_way(const struct game *game, struct square from, struct square to)
{
    assert((piece_at(game, from) & PIECE_TYPE) == KING && "checking not king");

    int file_move = abs(from.file - to.file);
    int rank_move = abs(from.rank - to.rank);

    // castling
    if (file_move == 2 && rank_move == 0) {
        // didn't the king and the rook move?
        enum piece castling_side = (to.file > from.file) ? KING : QUEEN;
        enum piece color = piece_at(game, from) & COLOR;
        if (color == WHITE) {
            if (!(castling_side & game->white_castling_avail))
                return false;
        } else {
            if (!(castling_side & game->black_castling_avail))
                return false;
        }
        // free squares
        int direction = (castling_side == QUEEN) ? -1 : 1;
        struct square rook = {(castling_side == QUEEN) ? 0 : 7, from.rank};
        struct square rook_to = {(castling_side == QUEEN) ? 3 : 5, from.rank};
        enum piece opp_color = (color == WHITE) ? BLACK : WHITE;
        for (int file = from.file + direction; file != rook.file; file += direction)
            if (game->board[file][from.rank] != EMPTY)
                return false;
        // cannot castle when the king, the rook, the new rook position,
        // or the intermediate king position is checked
        if (is_attacked(game, from) ||
            is_attacked(game, rook) ||
            is_attacked_by(game, to, opp_color) ||
            is_attacked_by(game, rook_to, opp_color))
        {
            return false;
        }
        return true;
    }

    if (file_move > 1 || rank_move > 1)
        return false;

    // A move into check will be checked later

    return true;
}

bool piece_has_way(const struct game *game, struct square from, struct square to)
{
    switch(piece_at(game, from) & PIECE_TYPE) {
    case PAWN:
        return pawn_has_way(game, from, to);

    case KNIGHT:
        return knight_has_way(from, to);

    case BISHOP:
        return bishop_has_way(game, from, to);

    case ROOK:
        return rook_has_way(game, from, to);

    case QUEEN:
        return queen_has_way(game, from, to);

    case KING:
        return king_has_way(game, from, to);
    }
    assert(false && "piece_has_way()");
    return false;
}

bool is_attacked_by(const struct game *game, struct square square, enum piece color)
{
    struct square from;
    for (from.file = 0; from.file < 8; from.file++) {
        for (from.rank = 0; from.rank < 8; from.rank++) {
            enum piece piece = piece_at(game, from);
            if ((piece & color) && (piece_has_way(game, from, square))) {
                log_debug("%c%d is attacked by %c%d",
                    'a' + square.file, 1 + square.rank, 'a' + from.file, 1 + from.rank);
                return true;
            }
        }
    }
    return false;
}

bool is_attacked(const struct game *game, struct square square)
{
    assert(piece_at(game, square) != EMPTY && "is_attacked() empty square");
    enum piece opp_color = ((piece_at(game, square) & COLOR) == WHITE) ? BLACK : WHITE;
    return is_attacked_by(game, square, opp_color);
}

bool is_checked(const struct game *game, enum piece color)
{
    struct square king;
    for (king.file = 0; king.file < 8; king.file++)
        for (king.rank = 0; king.rank < 8; king.rank++)
            if (game->board[king.file][king.rank] == (KING | color)) {
                log_debug("king found at %c%d", 'a' + king.file, 0 + king.rank);
                return is_attacked(game, king);
            }
    assert(false && "king not found");
    return false;
}

/*
 * Generic movement restrictions
 */

bool is_legal_move(const struct game *game, struct square from,
                   struct square to, enum piece promotion)
{
    if (from.rank < 0 || from.rank > 7 ||
        from.file < 0 || from.file > 7 ||
        to.rank   < 0 || to.rank   > 7 ||
        to.file   < 0 || to.file   > 7)
    {
        log_warning("Can't move out of the board");
        return false;
    }
    
    if ((piece_at(game, from) & COLOR) != game->side_to_move) {
        log_warning("Must move own piece");
        return false;
    }

    if ((piece_at(game, to) & COLOR) == game->side_to_move) {
        //log_warning("Can't capture own piece");
        return false;
    }

    if (!piece_has_way(game, from, to)) {
        //log_warning("No way from %c%d to %c%d",
        //    'a' + from.file, 1 + from.rank, 'a' + to.file, 1 + to.rank);
        return false;
    }

    int last_rank = (game->side_to_move == WHITE) ? 7 : 0;
    if ((piece_at(game, from) & PAWN) && (to.rank == last_rank)) {
        switch (promotion & PIECE_TYPE) {
        case KNIGHT:
        case BISHOP:
        case ROOK:
        case QUEEN:
            break;
        default:
            log_warning("Promotion not specified");
            return false;
        }
    }
    else
        if (promotion != EMPTY) {
            //log_warning("Can't promote");
            return false;
        }

    // Isn't own king checked?
    struct game new_position = *game;
    new_position.board[to.file][to.rank] = piece_at(game, from);
    new_position.board[from.file][from.rank] = EMPTY;
    new_position.side_to_move = (game->side_to_move == WHITE) ? BLACK : WHITE;
    new_position.en_passant_file = -1;
    if (is_checked(&new_position, game->side_to_move)) {
        log_debug("Can't move into check");
        return false;
    }

    return true;
}

bool can_make_any_move(const struct game *game)
{
    // Not optimal, but neither is performance-critical
    struct square from;
    struct square to;
    for (from.file = 0; from.file < 8; from.file++)
    for (from.rank = 0; from.rank < 8; from.rank++)
        if (piece_at(game, from) & game->side_to_move)
            for (to.file = 0; to.file < 8; to.file++)
            for (to.rank = 0; to.rank < 8; to.rank++) {
                enum piece promotion = EMPTY;
                if ((piece_at(game, from) & PAWN) && (to.rank == 0 || to.rank == 7))
                    promotion = QUEEN;
                if (is_legal_move(game, from, to, promotion))
                    return true;
            }
    return false;
}

/*
 * Make a move, modifying the input game structure (if the move is legal) and
 * returning the result (default, check, checkmate, draw, or illegal move).
 */

enum move_result move(struct game *game, struct square from, struct square to,
                      enum piece promotion)
{
    if (!is_legal_move(game, from, to, promotion))
        return ILLEGAL;

    // disabling castling
    if (from.file == 0 && from.rank == 0)
        game->white_castling_avail &= ~QUEEN;
    if (from.file == 4 && from.rank == 0)
        game->white_castling_avail = EMPTY;
    if (from.file == 7 && from.rank == 0)
        game->white_castling_avail &= ~KING;
    if (from.file == 0 && from.rank == 7)
        game->black_castling_avail &= ~QUEEN;
    if (from.file == 4 && from.rank == 7)
        game->black_castling_avail = EMPTY;
    if (from.file == 7 && from.rank == 7)
        game->black_castling_avail &= ~KING;

    // moving the rook for castling
    if ((piece_at(game, from) & KING) && (to.file - from.file == 2)) {
        game->board[5][from.rank] = game->board[7][from.rank];
        game->board[7][from.rank] = EMPTY;
    }
    if ((piece_at(game, from) & KING) && (to.file - from.file == -2)) {
        game->board[3][from.rank] = game->board[0][from.rank];
        game->board[0][from.rank] = EMPTY;
    }

    // en passant availability
    game->en_passant_file = -1;
    if ((piece_at(game, from) & PAWN) && abs(from.rank - to.rank) == 2) {
        log_debug("Available en passant at file %c", 'a' + from.file);
        game->en_passant_file = from.file;
    }

    // move the piece
    game->board[to.file][to.rank] = game->board[from.file][from.rank];
    game->board[from.file][from.rank] = EMPTY;
    if (promotion != EMPTY)
        game->board[to.file][to.rank] = ((promotion & ~COLOR) | game->side_to_move);
    game->side_to_move = (game->side_to_move == WHITE) ? BLACK : WHITE;

    // remove a pawn taken en passant
    if ((piece_at(game, from) & PAWN) && (from.file != to.file) &&
            (piece_at(game, to) == EMPTY)) {
        game->board[to.file][from.rank] == EMPTY;
    }

    if (!can_make_any_move(game)) {
        if (is_checked(game, game->side_to_move))
            return CHECKMATE;
        else
            return DRAW;
    }
    if (is_checked(game, game->side_to_move))
        return CHECK;
        
    return DEFAULT;
} 

enum move_result parse_move(struct game *game, char *move_str)
{
    for (int i = 0; move_str[i]; i++)  // lower case
        move_str[i] = tolower(move_str[i]);

    // TODO: make the input format less strict
    struct square from, to;
    enum piece promotion = EMPTY;
    from.file = move_str[0] - 'a';
    from.rank = move_str[1] - '1';
    to.file = move_str[2] - 'a'; 
    to.rank = move_str[3] - '1';
    switch (move_str[4]) {
    case 'n': promotion = KNIGHT; break;
    case 'b': promotion = BISHOP; break;
    case 'r': promotion = ROOK; break;
    case 'q': promotion = QUEEN; break;
    }

    return move(game, from, to, promotion);
}
