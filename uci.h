#ifndef UCI_H
#define UCI_H

#include "game.h"

bool uci(struct game *game, char *command);
void uci_loop();

#endif // UCI_H
