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
 * @file test_light_conv.c
 * @brief Test suite for the lux conversion function in light.c
 *
 * @author Ben Heberlein
 * @date Nov 5 2017
 * @version 1.0
 *
 */

#include "light.h"
#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

void test_light_conv(void) {

    uint16_t ch0, ch1;
    float ret;

    /* Divide by zero handling */
    ch0 = 0;
    ch1 = 0;
    ret = __light_convert_lux(ch0, ch1);
    assert_true(ret == 0.0);

    /* First range from data sheet */
    ch0 = 3;
    ch1 = 1;
    ret = __light_convert_lux(ch0, ch1);
    assert_true(ret > 0.05 && ret < 0.052); 

    ch0 = 25;
    ch1 = 4;
    ret = __light_convert_lux(ch0, ch1);
    assert_true(ret > 0.64 && ret < 0.65); 

    /* Second range from data sheet */
    ch0 = 20;
    ch1 = 11;
    ret = __light_convert_lux(ch0, ch1);
    assert_true(ret > 0.1 && ret < 0.11); 

    ch0 = 41;
    ch1 = 22;
    ret = __light_convert_lux(ch0, ch1);
    assert_true(ret > 0.23 && ret < 0.24); 

    /* Third range from data sheet */
    ch0 = 10;
    ch1 = 7;
    ret = __light_convert_lux(ch0, ch1);
    assert_true(ret > 0.02 && ret < 0.022); 

    ch0 = 20;
    ch1 = 15;
    ret = __light_convert_lux(ch0, ch1);
    assert_true(ret > 0.025 && ret < 0.028); 

    /* Fourth range from data sheet */
    ch0 = 10;
    ch1 = 12;
    ret = __light_convert_lux(ch0, ch1);
    assert_true(ret > 0.001 && ret < 0.0012); 

    ch0 = 25;
    ch1 = 25;
    ret = __light_convert_lux(ch0, ch1);
    assert_true(ret > 0.008 && ret < 0.009); 

    /* Fifth range from data sheet */
    ch0 = 3;
    ch1 = 12;
    ret = __light_convert_lux(ch0, ch1);
    assert_true(ret == 0.0); 

    ch0 = 11;
    ch1 = 30;
    ret = __light_convert_lux(ch0, ch1);
    assert_true(ret == 0.0); 

}
