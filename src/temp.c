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
#include "log.h"
#include "main.h"
#include <mraa.h>
#include <stdint.h>
#include <byteswap.h>
#include <mqueue.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Private variables
 */
static mraa_i2c_context i2c;
static float temperature_c = 123.456;

static uint16_t  __temp_i2c_read(uint8_t address);
static void __temp_i2c_write(uint16_t data, uint8_t address);
static void __temp_check(union sigval arg);
static uint8_t __temp_timer_init(void);
static void __temp_terminate(void *arg);

static uint16_t  __temp_i2c_read(uint8_t address) {

    uint16_t data = __bswap_16((mraa_i2c_read_word_data(i2c, address)));

    logmsg_t ltx;
    LOG_FMT(MAIN_THREAD_TEMP, LOG_LEVEL_INFO, ltx, "Register %d is %d", address, data);
    logmsg_send(&ltx, MAIN_THREAD_LOG);

    return data;
}

static void __temp_i2c_write(uint16_t data, uint8_t address) {
     mraa_i2c_write_word_data(i2c, __bswap_16(data), address);
}

static uint8_t __temp_timer_init(void) {

    timer_t tmr;
    struct itimerspec ts;
    struct sigevent se;

    se.sigev_notify = SIGEV_THREAD;
    se.sigev_value.sival_ptr = &tmr;
    se.sigev_notify_function = __temp_check;
    se.sigev_notify_attributes = NULL;

    ts.it_value.tv_sec = 0;
    ts.it_value.tv_nsec = 260000000;
    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 0;

    if (timer_create(CLOCK_REALTIME, &se, &tmr) == -1) {
        logmsg_t ltx;
        LOG_FMT(MAIN_THREAD_TEMP, LOG_LEVEL_ERROR, ltx, "Failed to start temp check timer");

        logmsg_send(&ltx, MAIN_THREAD_LOG);
    }

    if (timer_settime(tmr, 0, &ts, 0) == -1) {
        logmsg_t ltx;
        LOG_FMT(MAIN_THREAD_MAIN, LOG_LEVEL_ERROR, ltx, "Failed to set temp check timer");
        logmsg_send(&ltx, MAIN_THREAD_LOG);
    }

    return MAIN_SUCCESS;
}

static void __temp_check(union sigval arg) {

#if 0
    logmsg_t ltx;
    LOG_FMT(MAIN_THREAD_TEMP, LOG_LEVEL_INFO, ltx, "Temp check");
    logmsg_send(&ltx, MAIN_THREAD_LOG); */
#endif 

    /* Get temperature */
    uint16_t data = __temp_i2c_read(TEMP_REG_TEMP) >> 4;
    float c = 0;

    /* Convert to deg C */
    if (data & 0x0800) {
        data = ((~data) & 0x0fff) + 1;
        c = data * -TEMP_RES;
    } else {
        c = data * TEMP_RES;
    }
    temperature_c = c;

    __temp_timer_init();

}

static void __temp_terminate(void *arg) {
    logmsg_t ltx;
    LOG_FMT(MAIN_THREAD_TEMP, LOG_LEVEL_WARN, ltx, "Killing temperature module gracefully");
    logmsg_send(&ltx, MAIN_THREAD_LOG);
}

void *temp_task(void *data) {

    /* Register exit handler */
    pthread_cleanup_push(__temp_terminate, "temp");

    /* Initialize temp check timer */
    __temp_timer_init();

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
                case TEMP_ALIVE:
                    temp_alive(&rx);
                    break;
                case TEMP_INIT:
                    temp_init(&rx);
                    break;
                case TEMP_READREG:
                    temp_readreg(&rx);
                    break;
                case TEMP_WRITEREG:
                    temp_writereg(&rx);
                    break;
                case TEMP_GETTEMP:
                    temp_gettemp(&rx);
                    break;
                case TEMP_WRITECONFIG:
                    temp_writeconfig(&rx);
                    break;
                case TEMP_SETCONV:
                    temp_setconv(&rx);
                    break;
                case TEMP_SHUTDOWN:
                    temp_shutdown(&rx);
                    break;
                case TEMP_WAKEUP:
                    temp_wakeup(&rx);
                    break;
                case TEMP_WRITEPTR:
                    temp_writeptr(&rx);
                    break;
                case TEMP_KILL:
                    temp_kill(&rx);
                    break;
                default:
                    break;
            }
        }
    }

    pthread_cleanup_pop(1);

    return NULL;
}

uint8_t temp_init(msg_t *rx) {

    mraa_init();
    i2c = mraa_i2c_init_raw(TEMP_I2C_BUS);
    mraa_i2c_address(i2c, TEMP_I2C_ADDR);

    logmsg_t ltx;
    LOG_FMT(MAIN_THREAD_TEMP, LOG_LEVEL_INFO, ltx, "Initialized temperature module");
    logmsg_send(&ltx, MAIN_THREAD_LOG);

    return TEMP_SUCCESS;
}

uint8_t temp_readreg(msg_t *rx) {

    uint16_t data;
    uint8_t address = rx->data[0];
    data = __temp_i2c_read(address);

    /* Send Response*/
    msg_t tx;
    tx.from = MSG_RSP_MASK | MAIN_THREAD_TEMP;
    tx.cmd = TEMP_READREG;
    tx.data[0] = data & 0xff;
    tx.data[1] = data >> 8;
    tx.data[2] = address;
    msg_send(&tx, rx->from);

    return TEMP_SUCCESS;
}

uint8_t temp_writereg(msg_t *rx) {

    uint8_t address = rx->data[0];
    uint16_t data = rx->data[1] | rx->data[2] << 8;

    __temp_i2c_write(data, address);

    return TEMP_SUCCESS;
}

uint8_t temp_writeconfig(msg_t *rx) {

    uint16_t data = rx->data[0] | rx->data[1] << 8;
    __temp_i2c_write(data, TEMP_REG_CTRL);

    return TEMP_SUCCESS;
}

uint8_t temp_writeptr(msg_t *rx) {

    uint8_t data = rx->data[0];
    
    /* Write just the pointer register */
    mraa_i2c_write_byte(i2c, data);

    return TEMP_SUCCESS;
}

uint8_t temp_gettemp(msg_t *rx) {

    float ret = temperature_c;
    if (rx->data[0] == TEMP_FMT_KEL) {
        ret = temperature_c + 273.15;
    } else if (rx->data[0] == TEMP_FMT_FAR) {
        ret = temperature_c * 1.8 + 32;
    } else {
        ret = temperature_c;
    }

    /* Send response */
    msg_t tx;
    tx.from = MSG_RSP_MASK | MAIN_THREAD_TEMP;
    tx.cmd = TEMP_GETTEMP;
    memcpy(tx.data, &ret, 4);
    tx.data[4] = rx->data[0];
    tx.data[5] = 0;
    msg_send(&tx, rx->from);

    return TEMP_ERR_STUB;
}

uint8_t temp_setconv(msg_t *rx) {
    uint16_t data = __temp_i2c_read(TEMP_REG_CTRL);
    data &= ~(TEMP_REG_CTRL_CR1 | TEMP_REG_CTRL_CR2);
    data |= rx->data[0] << 7;

    __temp_i2c_write(TEMP_REG_CTRL, data);

    return TEMP_SUCCESS;
}

uint8_t temp_shutdown(msg_t *rx) {
    uint16_t data = __temp_i2c_read(TEMP_REG_CTRL);
    data |= TEMP_REG_CTRL_SD;

    __temp_i2c_write(TEMP_REG_CTRL, data);

    return TEMP_SUCCESS;
}

uint8_t temp_wakeup(msg_t *rx) {
    uint16_t data = __temp_i2c_read(TEMP_REG_CTRL);
    data &= ~TEMP_REG_CTRL_SD;

    __temp_i2c_write(TEMP_REG_CTRL, data);

    return TEMP_ERR_STUB;
}

uint8_t temp_alive(msg_t *rx) {

    /* Send alive */
    msg_t tx;
    tx.from = MSG_RSP_MASK | MAIN_THREAD_TEMP;
    tx.cmd = TEMP_ALIVE;
    tx.data[0] = 0xa5;
    msg_send(&tx, rx->from);

    return TEMP_SUCCESS;
}

uint8_t temp_kill(msg_t *rx) {

    pthread_exit(0);

    return TEMP_ERR_STUB;
}
