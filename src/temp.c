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
 * @file temp.c
 * @brief Temperature sensor task
 * 
 * This task is responsible for managing interactions with the TMP106 sensor
 * sensor. It contains commands to initialize, read, write, sleep, wake up,
 * check health, and get temperature. This task will use a timer to wake up 
 * periodically and read the temperature from the module.
 *
 * @author Ben Heberlein
 * @date Nov 2 2017
 * @version 1.0
 *
 */


