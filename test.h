#ifndef TEST_H
#define TEST_H

#include "game.h"

int test(const char *test_name, int moves_expected, enum move_result result_expected);
int test_all();

#endif // TEST_H
