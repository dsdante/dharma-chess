#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "log.h"
#include "test.h"

int run_pgn(const char *filename)
{
    log_err("PGN parsing not implemented yet.");
    return -1;
}

/*
 * Create a game, open raw moves file, play the game,
 * place last move's result to [result], return the number of moves made
 */
int run_raw_file(const char *filename, enum move_result *result)
{
    FILE *file = fopen(filename, "rb");
    if (!file) {
        log_err("Cannot open file '%s': %s", filename, strerror(errno));
        return errno;
    }

    *result = DEFAULT;
    int halfmoves = 0;
    struct game game = setup;
    char move[6];
    while (fscanf(file, "%5s", move) == 1) {
        printf("%s ", move); 
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

int test(const char *test_name, int moves_expected, enum move_result result_expected)
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
        log_err("Test '%s' failed: expected %d moves, actual is %d. ", test_name,
            moves_expected, number_of_moves);
        return -1;
    } else if (result != result_expected) {
        log_err("Test '%s' failed at move %d: expected %s, actual is %s. ", test_name,
            number_of_moves, move_result_text[result_expected], move_result_text[result]);
        return -1;
    } else {
        log_notice("Test '%s' passed.", test_name);
        return 0;
    }
}

int test_all()
{
    int result = 0;
    result -= test("castling", 10, DEFAULT);
    result -= test("queenside_castling", 12, DEFAULT);
    result -= test("en_passant", 5, DEFAULT);
    result -= test("promotion", 11, DEFAULT);
    result -= test("check_can_run_away", 6, CHECK);
    result -= test("check_can_block", 4, CHECK);
    result -= test("check_can_capture", 6, CHECK);
    result -= test("checkmate", 4, CHECKMATE);
    result -= test("threefold_repetition", 8, DRAW);
    log_warning("Threefold repetition not implemented yet");
    result -= test("fifty-move", 100, DRAW);
    result -= test("fifty-move_checkmate", 104, CHECKMATE);
    if (result == 0)
        log_notice("--- All tests passed. ---");
    else if (result == 1)
        log_err("--- 1 test failed. ---");
    else
        log_err("--- %d tests failed. ---", result);
    return result;
}
