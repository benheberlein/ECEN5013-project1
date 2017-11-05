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

#include "msg.h"
#include <stdint.h>
#include <stdio.h>

/* Error codes */
#define TEMP_SUCCESS        0
#define TEMP_ERR_STUB       126
#define TEMP_ERR_UNKNOWN    127

/**
 * @brief Temperature API
 */
#define TEMP_INIT           0
#define TEMP_READREG        1
#define TEMP_WRITEREG       2
#define TEMP_WRITECONFIG    3
#define TEMP_GETTEMP        4
#define TEMP_SETCONV        5
#define TEMP_SHUTDOWN       6
#define TEMP_WAKEUP         7
#define TEMP_ALIVE          8
#define TEMP_KILL           9
#define TEMP_WRITEPTR       10

/**
 * @brief I2C and sensor macros
 */
#define TEMP_I2C_BUS  2
#define TEMP_I2C_ADDR 0x48
#define TEMP_REG_TEMP 0x00
#define TEMP_REG_CTRL 0x01
#define TEMP_REG_HIGH 0x02
#define TEMP_REG_LOW  0x03

#define TEMP_REG_CTRL_OS    (1<<15)
#define TEMP_REG_CTRL_R1    (1<<14)
#define TEMP_REG_CTRL_R0    (1<<13)
#define TEMP_REG_CTRL_F1    (1<<12)
#define TEMP_REG_CTRL_F0    (1<<11)
#define TEMP_REG_CTRL_POL   (1<<10)
#define TEMP_REG_CTRL_TM    (1<<9)
#define TEMP_REG_CTRL_SD    (1<<8)
#define TEMP_REG_CTRL_CR1   (1<<7)
#define TEMP_REG_CTRL_CR2   (1<<6)
#define TEMP_REG_CTRL_AL    (1<<5)
#define TEMP_REG_CTRL_EM    (1<<4)

/**
 * @brief Temperature formats
 */
#define TEMP_FMT_CEL 0
#define TEMP_FMT_FAR 1
#define TEMP_FMT_KEL 2

/**
 * @brief Conversion rates
 */
#define TEMP_CONV_0_25HZ 0
#define TEMP_CONV_1HZ    1
#define TEMP_CONV_4HZ    2
#define TEMP_CONV_8Hz    3

/**
 * @brief Resolution per DN (C)
 */
#define TEMP_RES 0.0625

/**
 * @brief Temperature update timer
 */
#define TEMP_TIMER_NS 260000000

/**
 * @brief Format strings
 */
static const char *temp_fmt_strings[] = {"Celcius", "Farenheit", "Kelvin"};

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
 * DATA     none
 * RESPONSE none
 * 
 * @param rx Pointer to message
 *
 * @return Return TEMP_SUCCESS or error code
 */
uint8_t temp_init(msg_t *rx);

/**
 * @brief Read a register in the temperature module
 * 
 * DATA     (2) address to read
 * RESPONSE (1) read data
 *          (2) address that was read
 * 
 * @param rx Pointer to message
 *
 * @return Return TEMP_SUCCESS or error code
 */
uint8_t temp_readreg(msg_t *rx);

/**
 * @brief Write a register in the temperature module
 * 
 * DATA     (1) address to write
 *          (1) write data
 * RESPONSE none
 * 
 * @param rx Pointer to message
 *
 * @return Return TEMP_SUCCESS or error code
 */
uint8_t temp_writereg(msg_t *rx);

/**
 * @brief Write the configuration register
 * 
 * DATA     (2) data to write
 * RESPONSE none
 * 
 * @param rx Pointer to message
 *
 * @return Return TEMP_SUCCESS or error code
 */
uint8_t temp_writeconfig(msg_t *rx);

/**
 * @brief Write the pointer register
 * 
 * DATA     (1) data to write     
 * RESPONSE none
 * 
 * @param rx Pointer to message
 *
 * @return Return TEMP_SUCCESS or error code
 */
uint8_t temp_writeptr(msg_t *rx);

/**
 * @brief Get the temperature 
 * 
 * DATA     (1) format type
 * RESPONSE (4) data as float
 *          (1) format type
 * 
 * @param rx Pointer to message
 *
 * @return Return TEMP_SUCCESS or error code
 */
uint8_t temp_gettemp(msg_t *rx);

/**
 * @brief Configures the sensor conversion rate to the specified amount
 * 
 * DATA     (1) Conversion rate
 * RESPONSE none
 * 
 * @param rx Pointer to message
 *
 * @return Return TEMP_SUCCESS or error code
 */
uint8_t temp_setconv(msg_t *rx);

/**
 * @brief Puts the temperature module to sleep
 *
 * DATA     none    
 * RESPONSE none
 * 
 * @param rx Pointer to message
 * @return Return TEMP_SUCCESS or error code
 */
uint8_t temp_shutdown(msg_t *rx);

/**
 * @brief Wakes the temperature module back up from sleep
 *
 * DATA     none
 * RESPONSE none
 * 
 * @param rx Pointer to message
 * @return Return TEMP_SUCCESS or error code
 */
uint8_t temp_wakeup(msg_t *rx);

/**
 * @brief Checks if the temperature task is still alive 
 *
 * DATA     none
 * RESPONSE (1) alive packet 0xa5
 * 
 * @param rx Pointer to message
 * @return Return TEMP_SUCCESS or error code
 */
uint8_t temp_alive(msg_t *rx);

/**
 * @brief Kills the task gracefully
 *
 * DATA     none
 * RESPONSE none
 * 
 * @param rx Pointer to message
 * @return Return TEMP_SUCCESS or error code
 */
uint8_t temp_kill(msg_t *rx);

#endif /* __TEMP_H__ */
