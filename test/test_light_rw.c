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
 * @file test_light_rw.c
 * @brief Test suite for reading and writing TMP102 registers
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
#include <mraa.h>
#include <stdio.h>

static mraa_i2c_context i2c;

void test_light_rw(void) {

    uint8_t data = 0;
    uint8_t data_new = 0;
    uint8_t address = 0;

    /* Initialize */
    light_init(NULL);

    /* Set power on */
    address = LIGHT_REG_CTRL;
    data = 0x03;
    __light_i2c_write(data, address);
    data_new  = __light_i2c_read(address);
    assert_true(data_new & 0x03 == 0x03);

    /* Read timing register */
    address = LIGHT_REG_TIME;
    data = __light_i2c_read(address);
    data |= 0x03;
    __light_i2c_write(data, address);
    data_new = __light_i2c_read(address);
    assert_true(data == data_new);

    return;

}
