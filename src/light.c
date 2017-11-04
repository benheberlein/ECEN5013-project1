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
 * @file light.c
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

#include "light.h"
#include "msg.h"
#include "main.h"
#include <stdint.h>
#include <mraa.h>

static mraa_i2c_context i2c;

void *light_task(void *data) {

    /* Command loop */
    mqd_t rxq = mq_open(msg_names[MAIN_THREAD_LIGHT], O_RDONLY);
    msg_t rx;
    while(1) {
        mq_receive(rxq, (char *) &rx, MSG_SIZE+1, NULL);
        if (rx.from & MSG_RSP_MASK) {
            /* Handle response data */
            uint16_t rx_fc = MSG_RSP(rx.from, rx.cmd);
            switch(rx_fc) {
                default:
                    break;
            }

        } else {
            /* Handle command data */
            switch(rx.cmd) {
                case LIGHT_ALIVE:
                    light_alive(&rx);
                    break;
                case LIGHT_INIT:
                    light_init(&rx);
                    break;
                case LIGHT_READREG:
                    light_readreg(&rx);
                    break;
                case LIGHT_WRITEREG:
                    light_writereg(&rx);
                    break;
                default:
                    break;
            }
        }
    }

	return NULL;
}

uint8_t light_init(msg_t *rx) {

    mraa_init();
    i2c = mraa_i2c_init_raw(LIGHT_I2C_BUS);
    mraa_i2c_address(i2c, LIGHT_I2C_ADDR);

	return LIGHT_SUCCESS;
}

uint8_t light_readreg(msg_t *rx) {
    uint8_t data;
    uint8_t address;
    address = rx->data[0];
        
    mraa_i2c_write_byte(i2c, LIGHT_CMD_READ | (address & LIGHT_CMD_ADDR_MASK));
    data = mraa_i2c_read_byte(i2c);

    /* Send Response*/
    msg_t tx;
    tx.from = MSG_RSP_MASK | MAIN_THREAD_LIGHT;
    tx.cmd = LIGHT_READREG;
    tx.data[0] = data & 0xff;
    tx.data[1] = 0;
    msg_send(&tx, rx->from);

	return LIGHT_SUCCESS;
}

uint8_t light_writereg(msg_t *rx) {

    uint8_t address, data;
    address = rx->data[0];
    data = rx->data[1];

    mraa_i2c_write_byte(i2c, LIGHT_CMD_WRITE | (address & LIGHT_CMD_ADDR_MASK));
    mraa_i2c_write_byte(i2c, data);

	return LIGHT_SUCCESS;
}

uint8_t light_writeit(msg_t *rx) {

	return LIGHT_ERR_STUB;
}

uint8_t light_getlux(msg_t *rx) {

	return LIGHT_ERR_STUB;
}

uint8_t light_enableint(msg_t *rx) {

	return LIGHT_ERR_STUB;
}

uint8_t light_disableint(msg_t *rx) {

	return LIGHT_ERR_STUB;
}

uint8_t light_readid(msg_t *rx) {

	return LIGHT_ERR_STUB;
}

uint8_t light_isday(msg_t *rx) {

	return LIGHT_ERR_STUB;
}

uint8_t light_alive(msg_t *rx) {

    /* Send alive */
    msg_t tx;
    tx.from = MSG_RSP_MASK | MAIN_THREAD_LIGHT;
    tx.cmd = LIGHT_ALIVE;
    tx.data[0] = 0xa5;
    msg_send(&tx, rx->from);

	return LIGHT_SUCCESS;
}

uint8_t light_kill(msg_t *rx) {

	return LIGHT_ERR_STUB;
}

