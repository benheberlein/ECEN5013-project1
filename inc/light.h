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

#define LIGHT_SUCCESS       0
#define LIGHT_ERR_STUB      126
#define LIGHT_ERR_UNKNOWN   127

/**
 * @brief Light task commands
 */
typedef enum light_cmd_e {
    LIGHT_INIT,
    LIGHT_READREG,
    LIGHT_WRITEREG,
    LIGHT_WRITEIT,
    LIGHT_GETLUX,
    LIGHT_ENABLEINT,
    LIGHT_DISABLEINT,
    LIGHT_READID,
    LIGHT_ISDAY,
    LIGHT_ALIVE,
    LIGHT_KILL,
} light_cmd_t;

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
uint8_t light_init(void);

/**
 * @brief Read register in light module
 *
 * @param address The register to read
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_readreg(uint8_t address);

/**
 * @brief Write register in light module
 *
 * @param address The register to write
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_writereg(uint8_t address);

/**
 * @brief Write integration time in light module
 *
 * @param time New integration time
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_writeit(uint8_t time);

/**
 * @brief Get the luminosity from the light module
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_getlux(void);

/**
 * @brief Enable interrupts in the light module
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_enableint(void);

/**
 * @brief Disable interrupts in the light module
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_disableint(void);

/**
 * @brief Read identification register
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_readid(void);

/**
 * @brief Check if night or day
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_isday(void);

/**
 * @brief Check if the light task is still alive
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_alive(void);

/**
 * @brief Kill the light task gracefully
 *
 * @return Returns LIGHT_SUCCESS or error code
 */
uint8_t light_kill(void);

#endif /* __LIGHT_H__ */
