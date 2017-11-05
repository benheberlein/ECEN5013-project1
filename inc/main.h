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
 * @file main.h
 * @brief Main task
 * 
 * This task is responsible for creating, initializing, and monitoring the 
 * other tasks.
 *
 * @author Ben Heberlein
 * @date Nov 2 2017
 * @version 1.0
 *
 */

#ifndef __MAIN_H___
#define __MAIN_H__

#include "msg.h"
#include <stdint.h>
#include <stdio.h>
#include <sys/signal.h>

/**
 * @brief Error codes
 */
#define MAIN_SUCCESS		0
#define MAIN_ERR_INIT       1
#define MAIN_ERR_PARAM      2
#define MAIN_ERR_STUB 		126
#define MAIN_ERR_UNKNOWN	127

/**
 * @brief Thread IDs
 */
#define MAIN_THREAD_TOTAL 4
#define MAIN_THREAD_MAIN  0
#define MAIN_THREAD_LIGHT 1
#define MAIN_THREAD_TEMP  2
#define MAIN_THREAD_LOG   3

/**
 * @brief Main task API
 */
#define MAIN_EXIT 0

/**
 * @brief Main timer durations
 */
#define MAIN_TIMER_HEARTBEAT_NS 500000000
#define MAIN_TIMER_LOGIC_NS     400000000

/**
 * @brief LEDs
 */ 
#define MAIN_LED0       0
#define MAIN_LED1       1
#define MAIN_LED2       2
#define MAIN_LED3       3
#define MAIN_LED_ON     1
#define MAIN_LED_OFF    0

/**
 * @brief Logic for leds
 */
#define MAIN_TOOCOLD    10.0
#define MAIN_TOOHOT     30.0
#define MAIN_TOOBRIGHT  50.0

/**
 * @brief Main task function
 *
 * @return Returns program exit code 
 */
int main(int argc, char **argv);

/**
 * @brief Kills all tasks and exits
 *
 * DATA     none
 * RESPONSE none
 *
 * @param rx Pointer to message
 *
 * @return MAIN_SUCCESS or error code 
 */
uint8_t main_exit(msg_t *rx);

#endif /* __MAIN_H__ */
