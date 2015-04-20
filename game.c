#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "log.h"

bool is_checked(const struct game *game, enum piece color);

// Starting position
const struct game setup = {
    .board = {
        { WHITE|ROOK,   WHITE|PAWN, 0, 0, 0, 0, BLACK|PAWN, BLACK|ROOK   },
        { WHITE|KNIGHT, WHITE|PAWN, 0, 0, 0, 0, BLACK|PAWN, BLACK|KNIGHT },
        { WHITE|BISHOP, WHITE|PAWN, 0, 0, 0, 0, BLACK|PAWN, BLACK|BISHOP },
        { WHITE|QUEEN,  WHITE|PAWN, 0, 0, 0, 0, BLACK|PAWN, BLACK|QUEEN  },
        { WHITE|KING,   WHITE|PAWN, 0, 0, 0, 0, BLACK|PAWN, BLACK|KING   },
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

bool can_move_pawn(const struct game *game, struct square from, struct square to)
{
    int direction = (game->side_to_move == WHITE) ? 1 : -1;
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
    if (abs(from.file - to.file) == 1) {
        if (advance != direction)
            return false;
        if (piece_at(game, to) != EMPTY)
            return true;
        int en_passant_rank = (game->side_to_move == WHITE) ? 5 : 2;
        if (to.file == game->en_passant_file && to.rank == en_passant_rank)
            return true;
        return false;
    }

    return false;
}

bool can_move_knight(struct square from, struct square to)
{
    int file_move = abs(from.file - to.file);
    int rank_move = abs(from.rank - to.rank);
    if ((file_move == 1 && rank_move == 2) || (file_move == 2 && rank_move == 1))
        return true;
    return false; 
}

bool can_move_bishop(const struct game *game, struct square from, struct square to)
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

bool can_move_rook(const struct game *game, struct square from, struct square to)
{
    if (from.file != to.file || from.rank != to.rank)
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

bool can_move_queen(const struct game *game, struct square from, struct square to)
{
    return can_move_bishop(game, from, to) || can_move_rook(game, from, to);
}

bool can_move_king(const struct game *game, struct square from, struct square to)
{
    int file_move = abs(from.file - to.file);
    int rank_move = abs(from.rank - to.rank);

    // castling
    if (file_move == 2 && rank_move == 0) {
        // didn't the king and the rook move?
        enum piece castling_side = (to.file > from.file) ? KING : QUEEN;
        if (game->side_to_move == WHITE) {
            if (!(castling_side & game->white_castling_avail))
                return false;
        } else {
            if (!(castling_side & game->black_castling_avail))
                return false;
        }
        // free squares
        int direction = (castling_side == KING) ? 1 : -1;
        int file = from.file + direction;
        int rook_file = (direction == 1) ? 7 : 0;
        for (int file = from.file + direction; file != rook_file; file += direction)
            if (game->board[file][from.rank] != EMPTY)
                return false;
        // cannot castle when checked
        if (is_checked(game, game->side_to_move))
            return false;
        // cannot castle over an attacked square
        struct game mediate_pos = *game;
        mediate_pos.board[to.file][to.rank] = game->board[from.file][from.rank];
        mediate_pos.board[from.file][from.rank] = EMPTY;
        if (is_checked(&mediate_pos, game->side_to_move))
            return false;
    }

    if (file_move > 1 || rank_move > 1)
        return false;

    // A move into check will be checked later

    return true;
}

bool can_move_piece(const struct game *game, struct square from, struct square to)
{
    switch(piece_at(game, from) & PIECE_TYPE) {
    case PAWN:
        return can_move_pawn(game, from, to);

    case KNIGHT:
        return can_move_knight(from, to);

    case BISHOP:
        return can_move_bishop(game, from, to);

    case ROOK:
        return can_move_rook(game, from, to);

    case QUEEN:
        return can_move_queen(game, from, to);

    case KING:
        return can_move_king(game, from, to);
    }
    assert(false && "can_move_piece()");
    return false;
}

bool is_checked(const struct game *game, enum piece color)
{
    struct square king;
    for (king.file = 0; king.file < 8; king.file++)
        for (king.rank = 0; king.rank < 8; king.rank++)
            if (game->board[king.file][king.rank] == (KING | color))
                goto king_found; // break(2)
king_found:
    assert(king.file < 8 && "king not found");

    enum piece opp_color = (color == WHITE) ? BLACK : WHITE;
    struct square square;
    for (square.file = 0; square.file < 8; square.file++) {
        for (square.rank = 0; square.rank < 8; square.rank++) {
            enum piece piece = piece_at(game, square);
            if ((piece & opp_color) && (can_move_piece(game, square, king))) {
                return true;
            }
        }
    }

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
        return false;
    }
    
    // Must move own piece
    if ((piece_at(game, from) & COLOR) != game->side_to_move)
        return false;

    // Cannot capture own piece
    if ((piece_at(game, to) & COLOR) == game->side_to_move)
        return false;

    if (!can_move_piece(game, from, to))
        return false;

    // Need promotion
    int last_rank = (game->side_to_move == WHITE) ? 7 : 0;
    if ((piece_at(game, from) & PAWN) && (to.rank == last_rank)) {
        if ((promotion & COLOR) != game->side_to_move)
            return false;
        switch (promotion & PIECE_TYPE) {
        case KNIGHT:
        case BISHOP:
        case ROOK:
        case QUEEN:
            break;
        default:
            return false;
        }
    }

    // Isn't own king checked?
    struct game new_position = *game;
    new_position.board[to.file][to.rank] = game->board[from.file][from.rank];
    new_position.board[from.file][from.rank] = EMPTY;
    if (is_checked(&new_position, game->side_to_move))
        return false;

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
            for (to.rank = 0; to.rank < 8; to.rank++)
                if (is_legal_move(game, from, to, QUEEN))
                    return true;
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

    // move the piece
    game->board[to.file][to.rank] = game->board[from.file][from.rank];
    game->board[from.file][from.rank] = EMPTY;
    if (promotion != EMPTY)
        game->board[to.file][to.rank] = ((promotion & ~COLOR) | game->side_to_move);
    game->side_to_move = (game->side_to_move == WHITE) ? BLACK : WHITE;

    // FIXME: remove a pawn taken en passant

    // en passant
    game->en_passant_file = -1;
    if ((piece_at(game, from) & PAWN) && abs(from.rank - to.rank) == 2) {
        log_info("Available en passant at file %c", 'a' + from.file);
        game->en_passant_file = from.file;
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
