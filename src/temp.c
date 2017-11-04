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
 * @file temp.c
 * @brief Temperature sensor task
 * 
 * This task is responsible for managing interactions with the TMP106 sensor
 * sensor. It contains commands to initialize, read, write, sleep, wake up,
 * check health, and get temperature. This task will use a timer to wake up 
 * periodically and read the temperature from the module.
 *
 * @author Ben Heberlein
 * @date Nov 2 2017
 * @version 1.0
 *
 */

#include "temp.h"
#include "mraa.h"
#include <stdint.h>
#include <byteswap.h>

/**
 * @brief Private variables
 */
static mraa_i2c_context i2c;

void *temp_task(void *data) {

    return NULL;
}

uint8_t temp_init(void) {

    mraa_init();
    i2c = mraa_i2c_init_raw(TEMP_I2C_BUS);
    mraa_i2c_address(i2c, TEMP_I2C_ADDR);

    return TEMP_SUCCESS;
}

uint8_t temp_readreg(uint8_t address) {

    uint16_t data = 0;
    data = __bswap_16((mraa_i2c_read_word_data(i2c, address)));

    /* TODO: Send response msg */

    return TEMP_ERR_STUB;
}

uint8_t temp_writereg(uint8_t address, uint16_t data) {

    mraa_i2c_write_word_data(i2c, __bswap_16(data), address);

    return TEMP_SUCCESS;
}

uint8_t temp_writeconfig(uint8_t data) {

    return TEMP_ERR_STUB;
}

uint8_t temp_writeptr(uint8_t data) {

    return TEMP_ERR_STUB;
}

uint8_t temp_gettemp(temp_fmt_t fmt) {

    return TEMP_ERR_STUB;
}

uint8_t temp_setres(float res) {

    return TEMP_ERR_STUB;
}

uint8_t temp_shutdown(void) {

    return TEMP_ERR_STUB;
}

uint8_t temp_wakeup(void) {

    return TEMP_ERR_STUB;
}

uint8_t temp_alive(void) {

    return TEMP_ERR_STUB;
}

uint8_t temp_kill(void) {

    return TEMP_ERR_STUB;
}
