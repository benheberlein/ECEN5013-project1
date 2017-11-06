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
 * @file test_temp_conv.c
 * @brief Test suite for the temperature conversion function in temp.c
 *
 * @author Ben Heberlein
 * @date Nov 5 2017
 * @version 1.0
 *
 */

#include "temp.h"
#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <limits.h>

void test_temp_conv(void) {

    uint16_t data;
    float ret;

    /* From Table 2 in TMP102 data sheet */
    data = 0x7FF0;
    ret = __temp_conv(data);
    assert_true(ret == 127.9375);

    data = 0x6400;
    ret = __temp_conv(data);
    assert_true(ret == 100.0);

    data = 0x5000;
    ret = __temp_conv(data);
    assert_true(ret == 80.0);

    data = 0x4B00;
    ret = __temp_conv(data);
    assert_true(ret == 75.0);
    
    data = 0x3200;
    ret = __temp_conv(data);
    assert_true(ret == 50.0);
 
    data = 0x1900;
    ret = __temp_conv(data);
    assert_true(ret == 25.0);
 
    data = 0x0040;
    ret = __temp_conv(data);
    assert_true(ret == 0.25);

    data = 0x0000;
    ret = __temp_conv(data);
    assert_true(ret == 0);

    data = 0xFFC0;
    ret = __temp_conv(data);
    assert_true(ret == -0.25);
    
    data = 0xE700;
    ret = __temp_conv(data);
    assert_true(ret == -25);
 
    data = 0xC900;
    ret = __temp_conv(data);
    assert_true(ret == -55);
  
    return; 
}
