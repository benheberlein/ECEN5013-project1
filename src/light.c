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
 * @file light.c
 * @brief Light sensor task
 * 
 * This task is responsible for managing interactions with the APDS-9301 light
 * sensor. It contains commands to initialize, read, write, enable/disable 
 * interrupts, check if it is day/night, and check health. This task will use
 * a timer to wake up periodically and read the luminosity from the module,
 * storing it in local data.
 *
 * @author Ben Heberlein
 * @date Nov 2 2017
 * @version 1.0
 *
 */

#include "light.h"
#include <stdint.h>

void *light_task(void *data) {

	return NULL;
}

uint8_t light_init(void) {

	return LIGHT_ERR_STUB;
}

uint8_t light_readreg(uint8_t address) {

	return LIGHT_ERR_STUB;
}

uint8_t light_writereg(uint8_t address) {

	return LIGHT_ERR_STUB;
}

uint8_t light_writeit(uint8_t time) {

	return LIGHT_ERR_STUB;
}

uint8_t light_getlux(void) {

	return LIGHT_ERR_STUB;
}

uint8_t light_enableint(void) {

	return LIGHT_ERR_STUB;
}

uint8_t light_disableint(void) {

	return LIGHT_ERR_STUB;
}

uint8_t light_readid(void) {

	return LIGHT_ERR_STUB;
}

uint8_t light_isday(void) {

	return LIGHT_ERR_STUB;
}

uint8_t light_alive(void) {

	return LIGHT_ERR_STUB;
}

uint8_t light_kill(void) {

	return LIGHT_ERR_STUB;
}

