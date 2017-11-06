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
 * @file test_temp_rw.c
 * @brief Test suite for reading and writing APDS-9301 registers.
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
#include <stdio.h>
#include <mraa.h>

static mraa_i2c_context i2c;

void test_temp_rw(void) {

    uint16_t data = 0;
    uint16_t data_new = 0;
    uint8_t address = 0;

    /* Initialize */
    temp_init(NULL);

    /* Set conversion rate */
    address = TEMP_REG_CTRL;
    data = TEMP_REG_CTRL_CR1 | TEMP_REG_CTRL_CR1;
    data |= __temp_i2c_read(address);
    __temp_i2c_write(data, address);
    data_new  = __temp_i2c_read(address);
    assert_true(data == data_new);

    /* Reset conversion rate */
    address = TEMP_REG_CTRL;
    data = __temp_i2c_read(address);
    data |= TEMP_REG_CTRL_CR1;
    data &= ~TEMP_REG_CTRL_CR2;
    __temp_i2c_write(data, address);
    data_new = __temp_i2c_read(address);
    assert_true(data == data_new);

    return;
}
