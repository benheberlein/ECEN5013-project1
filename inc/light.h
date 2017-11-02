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

#endif /* __LIGHT_H__ */
