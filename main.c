#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ai.h"
#include "game.h"
#include "log.h"
#include "test.h"
#include "uci.h"

const struct option long_options[] = {
    { "help", no_argument, NULL, 'h' },
    { "console", no_argument, NULL, 'c' },
    { "test", optional_argument, NULL, 't' },
    { "log-level", required_argument, NULL, 'l' },
    { },
};

const char usage[] =
    "Dharma Chess v0.0.2\n"
#ifndef NDEBUG
    "DEBUG VERSION\n"
#endif
    "Usage: dchess [OPTION...]\n"
    "  -h, --help               display this help and exit\n"
    "  -c, --console            console user interface (UCI protocol otherwise)\n"
    "  -t, --test               run tests and benchmarks\n"
    "  -l, --log-level=LEVEL    console logging verbosity, from -1 (none) to 7 (debug)\n"
    "Enter moves like e2e4 or e7e8q (with promotion).";

const int max_move_length = 256;

/*
// Parse fuzzy format. Examples:
// e2 e4
// e2-e4
// Be2
// Rfe1
// cd
// 0-0

const char move_filter[] = "abcdefgh12345678rnbqk0o";
const char file_chars[] = "abcdefgh";
const char rank_chars[] = "12345678";
const char castling_chars[] = "0o";
const char piece_chars[] = "rnbqk";
const enum piece piece_values[] = { ROOK, KNIGHT, BISHOP, QUEEN, KING };

enum move_result parse_move(struct game *game, char *str_move)
{
    enum piece piece = EMPTY, promotion = EMPTY, castling = EMPTY;
    struct square from = { -1, -1 }, to = { -1, -1 };

    for (int i = 0; str_move[i]; i++)
        str_move[i] = tolower(str_move[i]);

    // remove chars like 'x', ':', '+', '#', '-', '='
    char filtered[6] = { 0 };
    int i_filtered = 0;
    for (int i_move = 0; str_move[i_move] != '\0'; i_move++)
        if (strchr(move_filter, str_move[i_move])) {
            if (i_filtered == 4) {
                log_err("Too long notation '%s'", str_move);
                return ILLEGAL;
            }
            filtered[i_filtered++] = str_move[i_move];
        }

    //piece
    i_filtered = 0;
    char *p_piece = strchr(piece_chars, filtered[i_filtered++]);
    if (p_piece)
        piece = piece_values[p_piece - piece_chars];

    // squares
    from.file = filtered[i_filtered++] - 'a';
    if (from.file < 0 || from.file > 7) {
        log_err("from.rank expected in '%s'", filtered);
        return ILLEGAL;
    }
    if (strchr(rank_chars, filtered[i_filtered]))
        from.rank = filtered[i_filtered++] - '1';
    if (strchr(file_chars, filtered[i_filtered])) {
        to.file = filtered[i_filtered++] - 'a';
        if (strchr(rank_chars, filtered[i_filtered]))
            to.rank = filtered[i_filtered++] - '1';
    } else {
        to = from;
        from = (struct square){ -1, -1 };
    }

    // promotion
    p_piece = strchr(piece_chars, filtered[i_filtered++]);
    if (p_piece)
        promotion = piece_values[p_piece - piece_chars];

    // castling
    if (strchr(castling_chars, filtered[1]))
        castling = KING;
    if (strchr(castling_chars, filtered[2]))
        castling = QUEEN;
    if (castling) {
        from.file = 4;
        to.file = (castling == QUEEN) ? 2 : 6;
        from.rank = to.rank = (game->side_to_move == WHITE) ? 0 : 7;
    }
        
}
*/

void run_game()
{
    puts("Enter moves like e2e4 or e7e8q (with promotion).");
    puts("Enter q to quit");
    log_info("Game started");
    struct game game = setup;
    do {
        enum move_result result;
        if (game.side_to_move == WHITE) {
            puts("Your move: ");
            char move_str[max_move_length];
            fgets(move_str, max_move_length, stdin);
            if (move_str[0] == 'q')  // quit
                break;
            result = parse_move(&game, move_str);
        } else {
            struct square from, to;
            enum piece promotion;
            best_move(&game, 2, &from, &to, &promotion);
            char promotion_char;
            switch (promotion) {
            case EMPTY:  promotion_char = ' '; break;
            case KNIGHT: promotion_char = 'N'; break;
            case BISHOP: promotion_char = 'B'; break;
            case ROOK:   promotion_char = 'R'; break;
            case QUEEN:  promotion_char = 'Q'; break;
            }
            printf("Computer's move: %c%d%c%d%c\n", from.file + 'a', from.rank + 1,
                    to.file + 'a', to.rank + 1, promotion_char);
            result = move(&game, from, to, promotion);
        }

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
        arg = getopt_long(argc, argv, "hcl:t::", long_options, NULL);
        switch (arg) {
        case -1:
            break; 

        case 'h':
            puts(usage);
            exit(0); 

        case 'c':
            run_game();
            return 0;

        case 't':
            exit(test_all());

        case 'l':
            logging_level = atoi(optarg);
            break;

        default:
            puts(usage);
            exit(1);
        }
    }
    while (arg != -1);

    uci_loop();

    return 0;
}
