#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "game.h"
#include "log.h"

const struct option long_options[] = {
    { "help", no_argument, NULL, 'h' },
    { "log-level", required_argument, NULL, 'l' },
    { },
};

const char usage[] =
    "Dharma Chess v0.0.1\n"
#ifndef NDEBUG
    "DEBUG VERSION\n"
#endif
    "Usage: dchess [OPTION...]\n"
    "  -h, --help               display this help and exit\n"
    "  -l, --log-level=LEVEL    console logging verbosity, from -1 (none) to 7 (debug)\n"
    "The game is still PvP only. Enter the moves in format \"e2 e4\"";

void run_game()
{
    puts("Enter moves like e2 e4.");
    log_print(6, "Game started");
    struct game game = setup;
    do {
        if (game.side_to_move == WHITE)
            printf("White's move: ");
        else
            printf("Black's move: ");

        char from_line[256], to_line[256];
        // TODO: support promotions, make the input format less strict
        int count = scanf("%s %s", &from_line, &to_line);
        if (count != 2 || strlen(from_line) != 2 || strlen(to_line) != 2) {
            puts("Wrong input format.");
            continue;
        }
        struct square from;
        struct square to;
        from.file = from_line[0] - 'a';
        from.rank = from_line[1] - '1';
        to.file = to_line[0] - 'a'; 
        to.rank = to_line[1] - '1';

        enum move_result result = move(&game, from, to, EMPTY);
        if (result == CHECK) {
            puts("Check!");
        } else if (result == CHECKMATE) {
            if (game.side_to_move == WHITE)
                puts("Black won!");
            else
                puts("White won!");
            break;
        } else if (result == DRAW) {
            puts("Draw!");
            break;
        } else if (result == ILLEGAL) {
            puts("Illegal move.");
            continue;
        }
    } while (true);
}

int main(int argc, char **argv)
{
    // Parse the command line arguments
    int arg = 0;
    do {
        arg = getopt_long(argc, argv, "hl:", long_options, NULL);
        switch (arg) {
        case -1:
            break; 

        case 'h':
            puts(usage);
            exit(0); 

        case 'l':
            logging_level = atoi(optarg);
            break;

        default:
            puts(usage);
            exit(1);
        }
    }
    while (arg != -1);

    run_game();

    return 0;
}
