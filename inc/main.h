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

#include <stdint.h>

#define MAIN_SUCCESS		0
#define MAIN_ERR_STUB 		126
#define MAIN_ERR_UNKNOWN	127

/**
 * @brief Main task commands
 */
typedef enum main_cmd_e {
    MAIN_EXIT,
} main_cmd_t;

/**
 * @brief Main task function
 *
 * @return Returns program exit code 
 */
int main(void);

/**
 * @brief Kills all tasks and exits
 *
 * @return MAIN_SUCCESS or error code 
 */
uint8_t main_exit(void);

#endif /* __MAIN_H__ */
