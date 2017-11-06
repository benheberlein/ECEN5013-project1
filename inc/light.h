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
 * @file light.h
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

#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "msg.h"
#include <stdint.h>
#include <stdio.h>

/**
 * @brief Error codes 
 */
#define LIGHT_SUCCESS       0
#define LIGHT_ERR_INIT      1
#define LIGHT_ERR_PARAM     2
#define LIGHT_ERR_STUB      126
#define LIGHT_ERR_UNKNOWN   127

/**
 * @brief Light API
 */
#define LIGHT_INIT          0
#define LIGHT_READREG       1
#define LIGHT_WRITEREG      2
#define LIGHT_WRITEIT       3
#define LIGHT_GETLUX        4
#define LIGHT_ENABLEINT     5
#define LIGHT_DISABLEINT    6
#define LIGHT_READID        7
#define LIGHT_ISDAY         8
#define LIGHT_ALIVE         9
#define LIGHT_KILL          10

/**
 * @brief I2C and register macros
 */
#define LIGHT_I2C_BUS       2
#define LIGHT_I2C_ADDR      0x39
#define LIGHT_CMD_READ      0xA0
#define LIGHT_CMD_WRITE     0x80
#define LIGHT_CMD_ADDR_MASK 0x0f
#define LIGHT_REG_CTRL      0
#define LIGHT_REG_TIME      1
#define LIGHT_REG_THRESHLL  2
#define LIGHT_REG_THRESHLH  3
#define LIGHT_REG_THRESHHL  4
#define LIGHT_REG_THRESHHH  5
#define LIGHT_REG_INT       6
#define LIGHT_REG_CRC       8
#define LIGHT_REG_ID        10
#define LIGHT_REG_DATA0L    12
#define LIGHT_REG_DATA0H    13
#define LIGHT_REG_DATA1L    14
#define LIGHT_REG_DATA1H    15

/**
 * @brief Interrupt options
 */
#define LIGHT_INT_EN (0x01 << 4)
#define LIGHT_INT_DIS (0x00 << 4)

/**
 * @brief Integration times in ms
 */
#define LIGHT_INT_13_7 0
#define LIGHT_INT_101  1
#define LIGHT_INT_402  2

/**
 * @brief Day or night calculation
 */
#define LIGHT_DAY_THRESH 3.0

/**
 * @brief Lux get timer
 */
#define LIGHT_TIMER_NS 200000000

/** 
 * @brief light task function
 *
 * @param data Data to send to task
 *
 * @return Return pointer
 */
void *light_task(void *data);

/**
 * @brief Initializes the light task
 *
 * DATA     none
 * RESPONSE none
 * 
 * @param rx Pointer to message
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_init(msg_t *rx);

/**
 * @brief Read register in light module
 *
 * DATA     (1) address to read
 * RESPONSE (1) read data
 * 
 * @param rx Pointer to message
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_readreg(msg_t *rx);

/**
 * @brief Write register in light module
 *
 * DATA     (1) address to write
			(1) data to write
 * RESPONSE none
 * 
 * @param rx Pointer to message
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_writereg(msg_t *rx);

/**
 * @brief Write integration time in light module
 *
 * DATA     (1) integration time
 * RESPONSE none
 * 
 * @param rx Pointer to message
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_writeit(msg_t *rx);

/**
 * @brief Get the luminosity from the light module
 *
 * DATA     none
 * RESPONSE (4) lux data in float
 * 
 * @param rx Pointer to message
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_getlux(msg_t *rx);

/**
 * @brief Enable interrupts in the light module
 *
 * DATA     none
 * RESPONSE none
 * 
 * @param rx Pointer to message
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_enableint(msg_t *rx);

/**
 * @brief Disable interrupts in the light module
 *
 * DATA     none
 * RESPONSE none
 * 
 * @param rx Pointer to message
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_disableint(msg_t *rx);

/**
 * @brief Read identification register
 *
 * DATA     none
 * RESPONSE (1) contents of ID register
 * 
 * @param rx Pointer to message
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_readid(msg_t *rx);

/**
 * @brief Check if night or day
 *
 * DATA     none
 * RESPONSE (1) if day response is 1 else 0
 * 
 * @param rx Pointer to message
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_isday(msg_t *rx);

/**
 * @brief Check if the light task is still alive
 *
 * DATA     none
 * RESPONSE (1) alive signal 0xa5
 * 
 * @param rx Pointer to message
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_alive(msg_t *rx);

/**
 * @brief Kill the light task gracefully
 *
 * DATA     none
 * RESPONSE none
 * 
 * @param rx Pointer to message
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_kill(msg_t *rx);

/**
 *  * @brief Private functions
 *   */
void __light_terminate(void *arg);
uint8_t __light_i2c_read(uint8_t address);
void __light_i2c_write(uint8_t data, uint8_t address);
float __light_convert_lux(uint16_t ch0, uint16_t ch1);
void __light_check(union sigval arg);
uint8_t __light_timer_init(void);

#endif /* __LIGHT_H__ */
