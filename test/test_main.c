/******************************************************************************
* Copyright (C) 2017 by Ben Heberlein
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. This file
* was created for the University of Colorado Boulder course Advanced Practical
* Embedded Software Development. Ben Heberlein and the University of Colorado 
* are not liable for any misuse of this material.
*
*******************************************************************************/
/**
 * @file test_main
 * @brief Test main function for all unit tests.
 *
 * The unit tests in this folder use the Cmocka test framework, which can be 
 * found at https://cmocka.org.
 *
 * @author Ben Heberlein
 * @date Nov 5 2017
 * @version 1.0
 *
 */

#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <limits.h>

void test_temp_conv(void);
void test_temp_rw(void);
void test_light_conv(void);
void test_light_rw(void);

int main(void) {

    struct CMUnitTest tests[] = {
        cmocka_unit_test(test_temp_conv),
        cmocka_unit_test(test_temp_rw),
        cmocka_unit_test(test_light_conv),
        cmocka_unit_test(test_light_rw),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
