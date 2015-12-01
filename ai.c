#include <limits.h>
#include <stddef.h>

#include "ai.h"

const enum piece promotions[] = {
    EMPTY,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
};

int evaluate(struct game *game)
{
    // detect checkmates and draws
    // count pieces
    // count all possible moves
    return 0;
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
        enum move_result move_result = move(&further_game, from, to, promotions[n_promotion]);
        if (move_result == ILLEGAL)
            continue;
        int score = best_move(&further_game, depth - 1, NULL, NULL, NULL);
        further_game = *game;
        if (score > score_max) {
            score_max = score;
            // return the best move only in the root call
            if (best_from != NULL) {
                *best_from = from;
                *best_to = to;
                *best_promotion = promotions[n_promotion];
            }
        }
        if (n_promotion == 0)
            break; // no need to check any promotions
    }
    return score_max;
}
