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

int run_raw_file(const char *filename, enum move_result *result)
{
    FILE *file = fopen(filename, "rb");
    if (!file) {
        log_err("Cannot open file '%s': %s", filename, strerror(errno));
        return errno;
    }

    *result = DEFAULT;
    int number_of_moves = 0;
    struct game game = setup;
    char move[6];
    while (fscanf(file, "%5s", move) == 1) {
        puts(move); 
        *result = parse_move(&game, move);
        number_of_moves++;
        if (*result != DEFAULT)
            puts(move_result_text[*result]);
        if (*result == ILLEGAL || *result == CHECKMATE || *result == DRAW)
            break;
    }

    fclose(file);
    return number_of_moves;
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
    } else if (result != result_expected) {
        log_err("Test '%s' failed at move %d: expected %s, actual is %s. ", test_name,
            number_of_moves, move_result_text[result_expected], move_result_text[result]);
        return -1;
    } else if (number_of_moves != moves_expected) {
        log_err("Test '%s' failed: expected %d moves, actual is %d. ", test_name,
            moves_expected, number_of_moves);
        return -1;
    } else {
        log_notice("Test '%s' passed.", test_name);
        return 0;
    }
}

int test_all()
{
    int result = 0;
    result -= test("fool", 4, CHECKMATE);
    result -= test("en_passant", 5, DEFAULT);
    return result;
}
