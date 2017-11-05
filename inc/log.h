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
 * @file log.h
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

#ifndef __LOG_H__
#define __LOG_H__

#include "msg.h"
#include <stdint.h>
#include <stdio.h>

#define LOG_SUCCESS     0
#define LOG_ERR_FILE    1
#define LOG_ERR_UNINIT  2
#define LOG_ERR_STUB    126
#define LOG_ERR_UNKNOWN 127

/**
 * @brief Log format macro
 *
 * Can format strings with sprintf. Can be used by requesting threads to format
 * data before requesting a log. Arguments are FROM, LOG_LEVEL, and the command
 * packet TX to put data into.
 */
#define LOG_FMT(fr, lvl, tx, ...) do { (tx).from = (fr); \
                                       (tx).cmd = LOG_LOG; \
                                       (tx).data[0] = (lvl); \
                                       sprintf((char *) &((tx).data[1]), __VA_ARGS__); \
                                      } while (0)

/**
 * @brief Log task API
 */
#define LOG_INIT    0
#define LOG_LOG     1
#define LOG_SETPATH 2
#define LOG_ALIVE   3
#define LOG_KILL    4

/**
 * @brief Log levels
 */
#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_WARN  2
#define LOG_LEVEL_ERROR 3

/**
 * @brief Log string arrays
 */
static char *log_level_strings[] = {"DEBUG", "INFO", "WARN", "ERROR"}; 
static char *log_task_strings[] = {"MAIN", "LIGHT", "TEMP", "LOG"}; 

/** 
 * @brief log task function
 *
 * @param data Data to send to task
 *
 * @return Return pointer
 */
void *log_task(void *data);

/**
 * @brief Initialize log function
 * 
 * DATA     (1)     1 for new file 0 for append
 *          (...)   ASCII string for path
 * RESPONSE none
 * 
 * @param rx Pointer to message
 * @return Returns LOG_SUCCESS or error code
 */
uint8_t log_init(logmsg_t *rx);

/**
 * @brief Logs the message to the file at the desired level
 * 
 * DATA     (1) 	log level
			(...) 	variable string to print
 * RESPONSE (none)
 * 			
 * 
 * @param rx Pointer to message
 *
 * @return Returns LOG_SUCCESS or error code
 */
uint8_t log_log(logmsg_t *rx);

/**
 * @brief Sets the log path
 * 
 * DATA     (...)	ASCII string for new path
 * RESPONSE none
 * 
 * @param rx Pointer to message
 *
 * @return Returns LOG_SUCCESS or error code
 */
uint8_t log_setpath(logmsg_t *rx);

/**
 * @brief Checks if the log task is still alive
 * 
 * DATA     none
 * RESPONSE (1) alive packet 0xa5
 * 
 * @param rx Pointer to message
 *
 * @return Returns LOG_SUCCESS or error code
 */
uint8_t log_alive(logmsg_t *rx);

/**
 * @brief Kills the task gracefully
 * 
 * DATA     none
 * RESPONSE none
 * 
 * @param rx Pointer to message
 *
 * @return Returns LOG_SUCCESS or error code
 */
uint8_t log_kill(logmsg_t *rx);

#endif /* __LOG_H */
