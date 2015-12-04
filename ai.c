#include <limits.h>
#include <stddef.h>

#include "ai.h"
#include "log.h"

const int value_pawn   = 1000;
const int value_knight = 3000;
const int value_bishop = 3100;
const int value_rook   = 5000;
const int value_queen  = 9000;
const int value_king   = 200000;
const int value_move   = 50; // the more, the more positional is playing

const enum piece promotions[] = {
    EMPTY,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
};

int perft; // Number of positions at the specified depth. Not thread safe.

int evaluate(struct game *game, enum piece color)
{
    enum piece actual_side_to_move = game->side_to_move;
    game->side_to_move = color;
    int result = 0;

    struct square square;
    break_debugger();
    for (square.file = 0; square.file < 8; square.file++)
    for (square.rank = 0; square.rank < 8; square.rank++) {
        int piece_value = 0;
        const enum piece piece = piece_at(game, square);
        if ((piece == EMPTY) || (piece & COLOR != color))
            continue;
        enum piece piece_type = piece & PIECE_TYPE;

        switch (piece_type) {
        case PAWN:   piece_value = value_pawn;   break;
        case KNIGHT: piece_value = value_knight; break;
        case BISHOP: piece_value = value_bishop; break;
        case ROOK:   piece_value = value_rook;   break;
        case QUEEN:  piece_value = value_queen;  break;
        }

        // count possible moves
        if (piece_type != PAWN && piece_type != KING) {
            struct square to;
            for (to.file = 0; to.file < 8; to.file++)
            for (to.rank = 0; to.rank < 8; to.rank++)
                if (is_legal_move(game, square, to, EMPTY) || is_legal_move(game, square, to, QUEEN))
                    result += value_move;
        }

        result += piece_value;
    }
    game->side_to_move = actual_side_to_move;
    return result;
}

/*
 * Negamax search. Returns the score of the current position;
 * returns the best move in 'from' and 'to'.
 */
int best_move(struct game *game, int depth,
        struct square *best_from, struct square *best_to, enum piece *best_promotion)
{
    if (best_to != NULL) // root call
        perft = 0;

    if (depth == 0) {
        perft++;
        enum piece op_color = (game->side_to_move == WHITE) ? BLACK : WHITE;
        return evaluate(game, game->side_to_move) - evaluate(game, op_color);
    }

    int score_max = INT_MIN;
    struct square from, to;
    struct game further_game = *game;
    for (from.file = 0; from.file < 8; from.file++)
    for (from.rank = 0; from.rank < 8; from.rank++)
    for (to.file = 0; to.file < 8; to.file++)
    for (to.rank = 0; to.rank < 8; to.rank++)
    for (int n_promotion = 0; n_promotion < 5; n_promotion++) {
        int score;
        enum move_result move_result = move(&further_game, from, to, promotions[n_promotion]);
        if (move_result == ILLEGAL)
            continue;
        else if (move_result == DRAW)
            score = 0; 
        else if (move_result == CHECKMATE)
            score =  value_king;
        else
            score = best_move(&further_game, depth - 1, NULL, NULL, NULL);
        if (score > score_max) {
            score_max = score;
            // return the best move only in the root call
            if (best_from != NULL) {
                *best_from = from;
                *best_to = to;
                *best_promotion = promotions[n_promotion];
            }
        }
        further_game = *game;
        if (n_promotion == 0)
            break; // no need to check any promotions
    }
    if (best_from != NULL)
        log_notice("Move %c%d%c%d %d scores %d", best_from->file + 'a', best_from->rank + 1,
                best_to->file + 'a', best_to->rank + 1, *best_promotion, score_max);
    return score_max;
}
