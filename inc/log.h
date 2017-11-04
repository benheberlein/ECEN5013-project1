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

#include <stdint.h>
#include <stdio.h>

#define LOG_SUCCESS     0
#define LOG_ERR_STUB    126
#define LOG_ERR_UNKNOWN 127

/**
 * @brief Log format macro
 *
 * Can format strings like printf. Can be used by requesting threads to format
 * data before requesting a log.
 */
#define LOG_FMT 

/**
 * @brief Log task commands
 */
typedef enum log_cmd_e {
    LOG_INIT,
    LOG_LOG,
    LOG_SETPATH,
    LOG_ALIVE,
    LOG_KILL,
} log_cmd_t;

/**
 * @brief Log levels
 */
typedef enum log_level_e {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
} log_level_t;

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
 * @return Returns LOG_SUCCESS or error code
 */
uint8_t log_init(void);

/**
 * @brief Logs the message to the file at the desired level
 * 
 * @param data Pointer to log data (NULL terminated, log_level_t as first byte)
 *
 * @return Returns LOG_SUCCESS or error code
 */
uint8_t log_log(uint8_t *data);

/**
 * @brief Sets the log path
 * 
 * @param data Pointer to NULL terminated ASCII file path
 *
 * @return Returns LOG_SUCCESS or error code
 */
uint8_t log_setpath(uint8_t *data);

/**
 * @brief Checks if the log task is still alive
 * 
 * @return Returns LOG_SUCCESS or error code
 */
uint8_t log_alive(void);

/**
 * @brief Kills the task gracefully
 * 
 * @return Returns LOG_SUCCESS or error code
 */
uint8_t log_kill(void);

#endif /* __LOG_H */
