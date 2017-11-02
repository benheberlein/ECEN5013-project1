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
#include <stdint.h>

void *temp_task(void *data) {

    return NULL;
}

uint8_t temp_init(void) {
    
    return TEMP_ERR_STUB;
}

uint8_t temp_readreg(uint8_t address) {

    return TEMP_ERR_STUB;
}

uint8_t temp_writereg(uint8_t address) {

    return TEMP_ERR_STUB;
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

uint8_t temp_gettemp(float res) {

    return TEMP_ERR_STUB;
}

uint8_t temp_shutdown(void) {

    return TEMP_ERR_STUB;
}

uint8_t temp_wakeup(void) {

    return TEMP_ERR_STUB;
}

uint8_t temp_alive(void);

    return TEMP_ERR_STUB;
}

uint8_t temp_kill(void);

    return TEMP_ERR_STUB;
}
