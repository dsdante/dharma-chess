#include <limits.h>
#include <stddef.h>

#include "ai.h"

const int value_pawn = 1000;
const int value_knight = 3000;
const int value_bishop = 3100;
const int value_rook = 5000;
const int value_queen = 9000;
const int value_king = 20000;
const int value_move = 50; // the more, the more positional is playing

const enum piece promotions[] = {
    EMPTY,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
};

int evaluate(struct game *game)
{
    int result = 0;

    struct square square;
    for (square.file = 0; square.file < 8; square.file++)
    for (square.rank = 0; square.rank < 8; square.rank++) {
        int piece_value = 0;
       
        // count piece values
        switch (piece_at(game, square) & PIECE_TYPE) {
        case PAWN:   piece_value = value_pawn; break;
        case KNIGHT: piece_value = value_knight; break;
        case BISHOP: piece_value = value_bishop; break;
        case ROOK:   piece_value = value_rook; break;
        case QUEEN:  piece_value = value_queen; break;
        }

        // count possible moves
        struct square to;
        for (to.file = 0; to.file < 8; to.file++)
        for (to.rank = 0; to.rank < 8; to.rank++)
            if (piece_has_way(game, square, to))
                result += value_move;

        if (piece_at(game, square) & COLOR != game->side_to_move)
            piece_value = -piece_value;
        result += piece_value;
    }
    return result;
}

/*
 * Negamax search. Returns the score of the current position;
 * returns the best move in 'from' and 'to'.
 */
int best_move(struct game *game, int depth,
        struct square *best_from, struct square *best_to, enum piece *best_promotion)
{
    if (depth == 0)
        return evaluate(game);

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
    return score_max;
}
