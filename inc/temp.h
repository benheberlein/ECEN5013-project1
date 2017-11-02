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
 * @file temp.h
 * @brief Temperature sensor task
 * 
 * This task is responsible for managing interactions with the TMP106
 * temperature module. It contains commands to initialize, read, write, sleep,
 * wake up, check health, and get temperature. This task will use a timer to 
 * periodically wake up and read the temperature from the TMP106 module.
 *
 * @author Ben Heberlein
 * @date Nov 2 2017
 * @version 1.0
 *
 */

#ifndef __TEMP_H__
#define __TEMP_H__

#define TEMP_SUCCESS        0
#define TEMP_ERR_STUB       126
#define TEMP_ERR_UNKNOWN    127

/**
 * @brief Temp task commands
 */
typedef enum temp_cmd_e {
    TEMP_INIT,
    TEMP_READREG,
    TEMP_WRITEREG,
    TEMP_WRITECONFIG,
    TEMP_WRITEPTR,
    TEMP_GETTEMP,
    TEMP_WRITERES,
    TEMP_SHUTDOWN,
    TEMP_WAKEUP,
    TEMP_ALIVE,
    TEMP_KILL
} temp_cmd_t;

/**
 * @brief Temperature formats
 */
typedef enum temp_fmt_e {
    TEMP_FMT_CELC,
    TEMP_FMT_FAR,
    TEMP_FMT_KEL,
} temp_fmt_t;

/** 
 * @brief temp task function
 *
 * @param data Data to send to task
 *
 * @return Return pointer
 */
void *temp_task(void *data);

/**
 * @brief Initialize temperature task
 * 
 * @return Return TEMP_SUCCESS or error code
 */
uint8_t temp_init(void);

/**
 * @brief Read a register in the temperature module
 * 
 * @param address Address of the register to read
 *
 * @return Return TEMP_SUCCESS or error code
 */
uint8_t temp_readreg(uint8_t address);

/**
 * @brief Write a register in the temperature module
 * 
 * @param address Address of the register to write
 *
 * @return Return TEMP_SUCCESS or error code
 */
uint8_t temp_writereg(uint8_t address);

/**
 * @brief Write the configuration register
 * 
 * @param data The data to write
 *
 * @return Return TEMP_SUCCESS or error code
 */
uint8_t temp_writeconfig(uint8_t data);

/**
 * @brief Write the pointer register
 * 
 * @param data The data to write
 *
 * @return Return TEMP_SUCCESS or error code
 */
uint8_t temp_writeptr(uint8_t data);

/**
 * @brief Get the temperature 
 * 
 * @param fmt The format of type temp_fmt_t
 *
 * @return Return TEMP_SUCCESS or error code
 */
uint8_t temp_gettemp(temp_fmt_t fmt);

/**
 * @brief Configures the sensor resolution to the specified amount
 * 
 * @param res The resolution of the sensor
 *
 * @return Return TEMP_SUCCESS or error code
 */
uint8_t temp_gettemp(float res);

/**
 * @brief Puts the temperature module to sleep
 *
 * @return Return TEMP_SUCCESS or error code
 */
uint8_t temp_shutdown(void);

/**
 * @brief Wakes the temperature module back up from sleep
 *
 * @return Return TEMP_SUCCESS or error code
 */
uint8_t temp_wakeup(void);

/**
 * @brief Checks if the temperature task is still alive 
 *
 * @return Return TEMP_SUCCESS or error code
 */
uint8_t temp_alive(void);

/**
 * @brief Kills the task gracefully
 *
 * @return Return TEMP_SUCCESS or error code
 */
uint8_t temp_kill(void);

#endif /* __TEMP_H__ */
