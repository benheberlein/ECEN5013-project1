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
 * @file log.c
 * @brief Log task
 * 
 * This task will log messages to a file. The messages can be of several levels
 * (LOG_LEVEL_ERROR, LOG_LEVEL_WARNING, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG).
 *
 * @author Ben Heberlein
 * @date Nov 2 2017
 * @version 1.0
 *
 */

#include "log.h"
#include <stdint.h>

void *log_task(void *data) {

	return NULL;
}

uint8_t log_init(void) {
	
	return LOG_ERR_STUB;
}

uint8_t log_log(uint8_t *data) {

	return LOG_ERR_STUB;
}

uint8_t log_setpath(uint8_t *data) {

	return LOG_ERR_STUB;
}

uint8_t log_alive(void) {

	return LOG_ERR_STUB;
}

uint8_t log_kill(void) {

	return LOG_ERR_STUB;
}

