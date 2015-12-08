#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ai.h"
#include "game.h"

const char delimiters[]  = " \t\r\n";
const size_t buffer_size = 256;

void uci_position(struct game *game, char *command)
{
    // input command is now like:
    // position\0startpos moves e2e4
    struct game new_game;
    command += sizeof "position"; // skip "position\0"
    while (strchr(delimiters, command[0])) // skip whitespaces
        command++;
    char *moves = strstr(command, "moves");
    char *position_end = moves - 1;
    // terminate position substring
    while (strchr(delimiters, position_end[0]))
        position_end--;
    position_end[1] = '\0';

    // load position
    if (strcmp(command, "startpos") == 0) {
        new_game = setup;
    } else {
        struct game *fen_game = fen_to_game(command);
        if (fen_game == NULL)
            return;
        new_game = *fen_game;
        free(fen_game);
    }

    // load moves
    if (moves != NULL) {
        strtok(moves, delimiters); // skip "moves"
        enum move_result move_result = DEFAULT;
        while (moves = strtok(NULL, delimiters))
            if (parse_move(&new_game, moves) == ILLEGAL)
                return;
    }

    *game = new_game;
}

void uci_go(struct game *game, char *command)
{
    char *token;
    while (token = strtok(NULL, delimiters)) {
        if (strcmp(token, "searchmoves") == 0) {
            // not supported
            break;

        } else if (strcmp(token, "ponder") == 0) {
            //not supported

        } else if (strcmp(token, "wtime") == 0) {
            //not supported
            break;

        } else if (strcmp(token, "btime") == 0) {
            //not supported
            break;

        } else if (strcmp(token, "winc") == 0) {
            //not supported
            break;

        } else if (strcmp(token, "binc") == 0) {
            //not supported
            break;

        } else if (strcmp(token, "movestogo") == 0) {
            //not supported
            break;

        } else if (strcmp(token, "depth") == 0) {
            //not supported
            break;

        } else if (strcmp(token, "nodes") == 0) {
            //not supported
            break;

        } else if (strcmp(token, "mate") == 0) {
            //not supported
            break;

        } else if (strcmp(token, "movetime") == 0) {
            //not supported
            break;

        } else if (strcmp(token, "infinite") == 0) {
            //not supported

        }
    }

    struct square from, to;
    enum piece promotion;
    best_move(game, 2, &from, &to, &promotion);
    char move[6];
    sprintf(move, "%c%d%c%d ", from.file + 'a', from.rank + 1,
        to.file + 'a', to.rank + 1);
    switch (promotion) {
    case EMPTY:  move[4] = '\0'; break;
    case KNIGHT: move[4] = 'n'; break;
    case BISHOP: move[4] = 'b'; break;
    case ROOK:   move[4] = 'r'; break;
    case QUEEN:  move[4] = 'q'; break;
    }
    printf("bestmove %s\n", move);
}

void uci_loop()
{
    struct game game = setup;
    char buffer[buffer_size];
    bool quit = false;

    while (!quit) {
        // read user's input
        fgets(buffer, buffer_size, stdin);
        char *token = strtok(buffer, delimiters); 
        do {
            if (token == NULL) {
                // do nothing

            } else if (strcmp(token, "uci") == 0) {
                puts("id name Dharma Chess");
                puts("id author Dmitry Fedorkov"); 
                puts("uciok"); 

            } else if (strcmp(token, "debug") == 0) {
                // not implemented yet

            } else if (strcmp(token, "isready") == 0) {
                puts("readyok");

            } else if (strcmp(token, "setoption") == 0) {
                // no options supported

            } else if (strcmp(token, "register") == 0) {
                // no registration

            } else if (strcmp(token, "ucinewgame") == 0) {
                // do nothing

            } else if (strcmp(token, "position") == 0) {
                uci_position(&game, buffer);

            } else if (strcmp(token, "go") == 0) {
                uci_go(&game, buffer);

            } else if (strcmp(token, "stop") == 0) {
                // do nothing

            } else if (strcmp(token, "ponderhit") == 0) {
                // do nothing

            } else if (strcmp(token, "quit") == 0) {
                quit = true;

            } else {
                // skip tokens until EOL or a valid command is found
                if (token = strtok(NULL, delimiters))
                    continue;
                // ignore invalid commands
            } 
        } while(false);
    }
}
