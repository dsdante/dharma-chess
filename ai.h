#ifndef AI_H
#define AI_H

#include "game.h"

extern int perft;

int best_move(struct game *game, int depth,
        struct square *best_from, struct square *best_to, enum piece *best_promotion); 

#endif // AI_H
