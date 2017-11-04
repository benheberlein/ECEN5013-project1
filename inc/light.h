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
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_init(msg_t *rx);

/**
 * @brief Read register in light module
 *
 * @param address The register to read
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_readreg(msg_t *rx, uint8_t address);

/**
 * @brief Write register in light module
 *
 * @param address The register to write
 * @param data The data to write
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_writereg(msg_t *rx, uint8_t address, uint8_t data);

/**
 * @brief Write integration time in light module
 *
 * @param time New integration time
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_writeit(msg_t *rx, uint8_t time);

/**
 * @brief Get the luminosity from the light module
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_getlux(msg_t *rx);

/**
 * @brief Enable interrupts in the light module
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_enableint(msg_t *rx);

/**
 * @brief Disable interrupts in the light module
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_disableint(msg_t *rx);

/**
 * @brief Read identification register
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_readid(msg_t *rx);

/**
 * @brief Check if night or day
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_isday(msg_t *rx);

/**
 * @brief Check if the light task is still alive
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_alive(msg_t *rx);

/**
 * @brief Kill the light task gracefully
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_kill(msg_t *rx);

#endif /* __LIGHT_H__ */
