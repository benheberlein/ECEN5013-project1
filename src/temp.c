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

#include "temp.h"
#include "msg.h"
#include "mraa.h"
#include "main.h"
#include <stdint.h>
#include <byteswap.h>
#include <mqueue.h>

/**
 * @brief Private variables
 */
static mraa_i2c_context i2c;

void *temp_task(void *data) {

    /* Command loop */
    mqd_t rxq = mq_open(msg_names[MAIN_THREAD_TEMP], O_RDONLY);
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
                case TEMP_INIT:
                    temp_init(&rx);
                    break;
                case TEMP_READREG:
                    temp_readreg(&rx);
                    break;
                case TEMP_WRITEREG:
                    temp_writereg(&rx);
                    break;
                default:
                    break;
            }
        }
    }

    return NULL;
}

uint8_t temp_init(msg_t *rx) {

    mraa_init();
    i2c = mraa_i2c_init_raw(TEMP_I2C_BUS);
    mraa_i2c_address(i2c, TEMP_I2C_ADDR);

    return TEMP_SUCCESS;
}

uint8_t temp_readreg(msg_t *rx) {

    uint16_t data;
    uint8_t address = rx->data[0];
    data = __bswap_16((mraa_i2c_read_word_data(i2c, address)));

    /* Send Response*/
    msg_t tx;
    tx.from = MSG_RSP_MASK | MAIN_THREAD_TEMP;
    tx.cmd = TEMP_READREG;
    tx.data[0] = data & 0xff;
    tx.data[1] = data >> 8;
    tx.data[2] = 0;
    msg_send(&tx, rx->from);

    return TEMP_SUCCESS;
}

uint8_t temp_writereg(msg_t *rx) {

    uint8_t address = rx->data[0];
    uint16_t data = rx->data[1] | rx->data[2] << 8;

    mraa_i2c_write_word_data(i2c, __bswap_16(data), address);

    return TEMP_SUCCESS;
}

uint8_t temp_writeconfig(msg_t *rx) {

    return TEMP_ERR_STUB;
}

uint8_t temp_writeptr(msg_t *rx) {

    return TEMP_ERR_STUB;
}

uint8_t temp_gettemp(msg_t *rx) {

    return TEMP_ERR_STUB;
}

uint8_t temp_setres(msg_t *rx) {
    return TEMP_ERR_STUB;
}

uint8_t temp_shutdown(msg_t *rx) {

    return TEMP_ERR_STUB;
}

uint8_t temp_wakeup(msg_t *rx) {

    return TEMP_ERR_STUB;
}

uint8_t temp_alive(msg_t *rx) {

    return TEMP_ERR_STUB;
}

uint8_t temp_kill(msg_t *rx) {

    return TEMP_ERR_STUB;
}
