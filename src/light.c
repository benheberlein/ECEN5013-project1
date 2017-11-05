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
#include "log.h"
#include "main.h"
#include <stdint.h>
#include <mraa.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Private variables
 */
static mraa_i2c_context i2c;
static float current_lux = 0.0;

/**
 * @brief Private functions
 */
static void __light_terminate(void *arg);
static uint8_t __light_i2c_read(uint8_t address);
static void __light_i2c_write(uint8_t data, uint8_t address);
static float __light_convert_lux(uint16_t ch0, uint16_t ch1);
static void __light_check(union sigval arg);
static uint8_t __light_timer_init(void);

static uint8_t __light_timer_init(void) {

    timer_t tmr;
    struct itimerspec ts;
    struct sigevent se;

    se.sigev_notify = SIGEV_THREAD;
    se.sigev_value.sival_ptr = &tmr;
    se.sigev_notify_function = __light_check;
    se.sigev_notify_attributes = NULL;

    ts.it_value.tv_sec = 0;
    ts.it_value.tv_nsec = LIGHT_TIMER_NS;
    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 0;

    if (timer_create(CLOCK_REALTIME, &se, &tmr) == -1) {
        logmsg_t ltx;
        LOG_FMT(MAIN_THREAD_LIGHT, LOG_LEVEL_ERROR, ltx, "Failed to start light check");

        logmsg_send(&ltx, MAIN_THREAD_LOG);
    }

    if (timer_settime(tmr, 0, &ts, 0) == -1) {
        logmsg_t ltx;
        LOG_FMT(MAIN_THREAD_LIGHT, LOG_LEVEL_ERROR, ltx, "Failed to set light check timer");
        logmsg_send(&ltx, MAIN_THREAD_LOG);
    }

    return MAIN_SUCCESS;
}

static void __light_check(union sigval arg) {
    uint8_t temp;
	uint16_t ch0, ch1;



	/* Get CH0 */
    temp = __light_i2c_read(LIGHT_REG_DATA0L);
    ch0 = temp;
    temp = __light_i2c_read(LIGHT_REG_DATA0H);
    ch0 |= (temp << 8);

    /* Get CH1 */
    temp = __light_i2c_read(LIGHT_REG_DATA1L);
    ch1 = temp;
    temp = __light_i2c_read(LIGHT_REG_DATA1H);
    ch1 |= (temp << 8);

    float old_lux = current_lux;

    /* Calculate lux */
    current_lux = __light_convert_lux(ch0, ch1);

    /* Log if there was a large change */
    logmsg_t ltx;
    if ((current_lux != 0.0 && old_lux != 0.0 ) && (current_lux / old_lux > 2 || old_lux / current_lux > 2)) {
        LOG_FMT(MAIN_THREAD_LIGHT, LOG_LEVEL_INFO, ltx, "Lux changed from %f to %f!", old_lux, current_lux);
        logmsg_send(&ltx, MAIN_THREAD_LOG);
    }

	__light_timer_init();
}

static float __light_convert_lux(uint16_t ch0, uint16_t ch1) {
    float ret = 0.0;
    float div = 0.0;

    /* Taken from APDS-9301 data sheet page 4 */
    div = ((float)ch1)/((float)ch0);
    if (div < 0 && div <= 0.5) {
        ret = (0.0304 * ch0) - (0.062 * ch0 *pow(div, 1.4));
    } else if (div < 0.5 && div <= 0.61) {
        ret = (0.0224 * ch0) - (0.031 * ch1);
    } else if (div < 0.61 && div <= 0.8) {
        ret = (0.0128 * ch0) - (.0153 * ch1);
    } else if (div < 0.8 && div <= 1.30) {
        ret = (0.00146 * ch0) - (0.00112 * ch1);
    } else {
        ret = 0.0;
    }

    return ret;
}

static uint8_t __light_i2c_read(uint8_t address) {

    uint8_t data;

    mraa_i2c_write_byte(i2c, LIGHT_CMD_READ | (address & LIGHT_CMD_ADDR_MASK));
    data = mraa_i2c_read_byte(i2c);

    logmsg_t ltx;
    LOG_FMT(MAIN_THREAD_LIGHT, LOG_LEVEL_INFO, ltx, "Register %d is %d", address, data);
    logmsg_send(&ltx, MAIN_THREAD_LOG);

    return data;

}

static void __light_i2c_write(uint8_t data, uint8_t address) {
    mraa_i2c_write_byte(i2c, LIGHT_CMD_WRITE | (address & LIGHT_CMD_ADDR_MASK));
    mraa_i2c_write_byte(i2c, data);

}

static void __light_terminate(void *arg) {
    logmsg_t ltx;
    LOG_FMT(MAIN_THREAD_TEMP, LOG_LEVEL_WARN, ltx, "Killing light module gracefully");
    logmsg_send(&ltx, MAIN_THREAD_LOG);
}

void *light_task(void *data) {

    /* Register exit handler */
    pthread_cleanup_push(__light_terminate, "light");

	/* Initialize lux check timer */
	__light_timer_init();

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
                case LIGHT_WRITEIT:
                    light_writeit(&rx);
                    break;
                case LIGHT_GETLUX:
                    light_getlux(&rx);
                    break;
                case LIGHT_ENABLEINT:
                    light_enableint(&rx);
                    break;
                case LIGHT_DISABLEINT:
                    light_disableint(&rx);
                    break;
                case LIGHT_READID:
                    light_readid(&rx);
                    break;
                case LIGHT_KILL:
                    light_kill(&rx);
                    break;                   
                default:
                    break;
            }
        }
    }

    pthread_cleanup_pop(1);

	return NULL;
}

uint8_t light_init(msg_t *rx) {

    mraa_init();
    i2c = mraa_i2c_init_raw(LIGHT_I2C_BUS);
    mraa_i2c_address(i2c, LIGHT_I2C_ADDR);

    logmsg_t ltx;
    LOG_FMT(MAIN_THREAD_LIGHT, LOG_LEVEL_INFO, ltx, "Initialized light module");
    logmsg_send(&ltx, MAIN_THREAD_LOG);

	return LIGHT_SUCCESS;
}

uint8_t light_readreg(msg_t *rx) {
    uint8_t data;
    uint8_t address;
    address = rx->data[0];
        
    data = __light_i2c_read(address); 

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

    __light_i2c_write(data, address);

	return LIGHT_SUCCESS;
}

uint8_t light_writeit(msg_t *rx) {

    uint8_t data = rx->data[0];
    if (data > 2) {
        logmsg_t ltx;
        LOG_FMT(MAIN_THREAD_LIGHT, LOG_LEVEL_INFO, ltx, "Invalid integration time");
        logmsg_send(&ltx, MAIN_THREAD_LOG);
        return LIGHT_ERR_PARAM;
    }

    /* Get and set integration time */
    uint8_t integ = __light_i2c_read(LIGHT_REG_TIME);
    integ &= ~0x03;
    integ |= data;
    __light_i2c_write(integ, LIGHT_REG_TIME);

	return LIGHT_SUCCESS;
}

uint8_t light_getlux(msg_t *rx) {

    /* Send Response from lux variable*/
    msg_t tx;
    tx.from = MSG_RSP_MASK | MAIN_THREAD_LIGHT;
    tx.cmd = LIGHT_GETLUX;
    memcpy(tx.data, &current_lux, 4);
    tx.data[4] = 0;
    msg_send(&tx, rx->from);

	return LIGHT_SUCCESS;
}

uint8_t light_enableint(msg_t *rx) {

    uint8_t intreg = __light_i2c_read(LIGHT_REG_INT);
    intreg &= ~(0x03 << 4);
    intreg |= (LIGHT_INT_EN);
    __light_i2c_write(intreg, LIGHT_REG_INT);

	return LIGHT_SUCCESS;
}

uint8_t light_disableint(msg_t *rx) {

    uint8_t intreg = __light_i2c_read(LIGHT_REG_INT);
    intreg &= ~(0x03 << 4);
    intreg |= (LIGHT_INT_DIS);
    __light_i2c_write(intreg, LIGHT_REG_INT);

	return LIGHT_SUCCESS;
}

uint8_t light_readid(msg_t *rx) {

    uint8_t id = __light_i2c_read(LIGHT_REG_ID);

    /* Send Response*/
    msg_t tx;
    tx.from = MSG_RSP_MASK | MAIN_THREAD_LIGHT;
    tx.cmd = LIGHT_READID;
    tx.data[0] = id;
    tx.data[1] = 0;
    msg_send(&tx, rx->from);

	return LIGHT_SUCCESS;
}

uint8_t light_isday(msg_t *rx) {

  	uint8_t day = 0;
	if (current_lux > LIGHT_DAY_THRESH) {
		day = 1;
	}

    msg_t tx;
    tx.from = MSG_RSP_MASK | MAIN_THREAD_LIGHT;
    tx.cmd = LIGHT_ISDAY;
    tx.data[0] = day;
    tx.data[1] = 0;
    msg_send(&tx, rx->from);

	return LIGHT_SUCCESS;
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

	pthread_exit(0);

	return LIGHT_SUCCESS;
}

