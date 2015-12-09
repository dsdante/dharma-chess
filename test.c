#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ai.h"
#include "log.h"
#include "test.h"
#include "uci.h"

/*
 * Create a game, open raw move file, play the game,
 * place the last move's result to [result], return the number of moves made
 */
int run_raw_file(const char *filename, enum move_result *result)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        log_err("Cannot open file '%s': %s", filename, strerror(errno));
        return errno;
    }

    *result = DEFAULT;
    int halfmoves = 0;
    struct game game = setup;
    char move[6];
    while (fscanf(file, "%5s", move) == 1) {
        printf("%s ", move); 
        if (halfmoves == 39)
            break_debugger();
        *result = parse_move(&game, move);
        halfmoves++;
        if (*result != DEFAULT)
            printf("\n%s ", move_result_text[*result]);
        if (*result == ILLEGAL || *result == CHECKMATE || *result == DRAW)
            break;
    }
    puts("");

    fclose(file);
    return halfmoves;
}

int test_game(const char *test_name, int moves_expected, enum move_result result_expected)
{
    printf("Running test '%s'\n", test_name);
    char filename[256] = "tests/";
    strcat(filename, test_name);
    enum move_result result;
    int number_of_moves = run_raw_file(filename, &result);
    if (number_of_moves < 0) {
        log_err("Test '%s' failed", test_name);
        return -1;
    } else if (number_of_moves != moves_expected) {
        log_err("Test '%s' failed: expected %d moves, actual is %d.", test_name,
            moves_expected, number_of_moves);
        return -1;
    } else if (result != result_expected) {
        log_err("Test '%s' failed at move %d: expected %s, actual is %s.", test_name,
            number_of_moves, move_result_text[result_expected], move_result_text[result]);
        return -1;
    } else {
        log_notice("Test '%s' passed.", test_name);
        return 0;
    }
}

int test_perft(struct game *game, int depth, int result_expected)
{
    struct square from, to;
    enum piece promotion;
    int score = best_move(game, depth, &from, &to, &promotion);
    printf("score %d, move %c%d%c%d %d\n", score, from.file + 'a', from.rank + 1, to.file + 'a', to.rank + 1, promotion);
    if (perft == result_expected) {
        log_notice("A perft(%d) test passed.", depth);
        return 0;
    } else {
        log_err("A perft(%d) test failed.", depth);
        return -1;
    }
}

int test_uci(const char *test_name, int commands_expected)
{
    printf("Running test '%s'\n", test_name);
    char filename[256] = "tests/";
    strcat(filename, test_name);
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        log_err("Cannot open file '%s': %s", filename, strerror(errno));
        return -1;
    }

    struct game game = setup;
    char command[256];
    int commands_actual = 0;
    while (fgets(command, 256, file) != NULL) {
        commands_actual++;
        if (uci(&game, command)) 
            break;
    }

    fclose(file);
    if (commands_actual == commands_expected) {
        log_notice("Test '%s' passed.", test_name);
        return 0;
    } else {
        log_err("Test '%s' failed: expected %d commands, actual is %d.", test_name,
                commands_expected, commands_actual);
        return -1;
    }
}

int test_all()
{
    int result = 0;

    // games
    result -= test_game("castling", 10, DEFAULT);
    result -= test_game("castling_queenside", 16, DEFAULT);
    result -= test_game("en_passant", 5, DEFAULT);
    result -= test_game("promotion", 11, DEFAULT);
    result -= test_game("check_can_run_away", 6, CHECK);
    result -= test_game("check_can_block", 4, CHECK);
    result -= test_game("check_can_capture", 6, CHECK);
    result -= test_game("checkmate", 4, CHECKMATE);
    result -= test_game("insufficient_material", 40, DRAW);
    result -= test_game("threefold", 8, DRAW);
    result -= test_game("threefold_enpassant", 12, DEFAULT);
    result -= test_game("threefold_enpassant_cannot_capture", 9, DRAW);
    result -= test_game("threefold_castling_availability", 20, DEFAULT);
    result -= test_game("fifty-move", 100, DRAW);
    result -= test_game("fifty-move_checkmate", 104, CHECKMATE);

    // UCI
    result -= test_uci("uci_basic", 5);

    // perft
    struct game game = setup;
    result -= test_perft(&game, 0, 1);
    result -= test_perft(&game, 1, 20);
    result -= test_perft(&game, 2, 400);
//    result -= test_perft(&game, 3, 8902);

    if (result == 0)
        log_notice("--- All tests passed. ---");
    else if (result == 1)
        log_err("--- 1 test failed. ---");
    else
        log_err("--- %d tests failed. ---", result);
    return result;
}
