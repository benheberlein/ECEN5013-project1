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
 * @file main.h
 * @brief Main task
 * 
 * This task is responsible for creating, initializing, and monitoring the 
 * other tasks.
 *
 * @author Ben Heberlein
 * @date Nov 2 2017
 * @version 1.0
 *
 */

#include "msg.h"
#include "main.h"
#include "light.h"
#include "temp.h"
#include "log.h"
#include <stdint.h>
#include <pthread.h>
#include <mraa.h>

static pthread_t main_tasks[MAIN_THREAD_TOTAL];

static uint8_t __main_pthread_init(void) {

    // TODO for all tasks...
    if (pthread_create(&main_tasks[MAIN_THREAD_TEMP], NULL, temp_task, NULL)) {
       return MAIN_ERR_INIT; 
    }
    if (pthread_create(&main_tasks[MAIN_THREAD_LIGHT], NULL, light_task, NULL)) {
       return MAIN_ERR_INIT; 
    }
    
    return MAIN_ERR_STUB;
}

uint8_t main_ex(msg_t *rx) {

    return MAIN_ERR_STUB;
}

int main(void) {
    msg_init();
    __main_pthread_init();
    
	/* Initialize temperature module */
	msg_t tx;
	tx.from = MAIN_THREAD_MAIN;
	tx.cmd = TEMP_INIT;
	tx.data[0] = 0;
    msg_send(&tx, MAIN_THREAD_TEMP);

	/* Initialize light module */
	tx.from = MAIN_THREAD_MAIN;
	tx.cmd = LIGHT_INIT;
	tx.data[0] = 0;
    msg_send(&tx, MAIN_THREAD_LIGHT);

    /* Test read temp command */
	tx.from = MAIN_THREAD_MAIN;
	tx.cmd = TEMP_READREG;
	tx.data[0] = TEMP_REG_HIGH;
	tx.data[1] = 0;
    msg_send(&tx, MAIN_THREAD_TEMP);

    tx.from = MAIN_THREAD_MAIN;
	tx.cmd = TEMP_WRITEREG;
	tx.data[0] = TEMP_REG_HIGH;
	tx.data[1] = 0x00;
    tx.data[2] = 0xa5;
    msg_send(&tx, MAIN_THREAD_TEMP);

    tx.from = MAIN_THREAD_MAIN;
	tx.cmd = TEMP_READREG;
	tx.data[0] = TEMP_REG_HIGH;
	tx.data[1] = 0;
    msg_send(&tx, MAIN_THREAD_TEMP);

    /* Test read light command */
	tx.from = MAIN_THREAD_MAIN;
	tx.cmd = LIGHT_READREG;
	tx.data[0] = LIGHT_REG_TIME;
	tx.data[1] = 0;
    msg_send(&tx, MAIN_THREAD_LIGHT);

    tx.from = MAIN_THREAD_MAIN;
	tx.cmd = LIGHT_WRITEREG;
	tx.data[0] = LIGHT_REG_TIME;
	tx.data[1] = 0x01;
    tx.data[2] = 0;
    msg_send(&tx, MAIN_THREAD_LIGHT);

    tx.from = MAIN_THREAD_MAIN;
	tx.cmd = LIGHT_READREG;
	tx.data[0] = LIGHT_REG_TIME;
	tx.data[1] = 0;
    msg_send(&tx, MAIN_THREAD_LIGHT);

    /* Command loop */
    mqd_t rxq = mq_open(msg_names[MAIN_THREAD_MAIN], O_RDONLY);
	msg_t rx;
    while(1) {
        
        mq_receive(rxq, (char *) &rx, MSG_SIZE+10, NULL);
        if (rx.from & MSG_RSP_MASK) {
            /* Handle response data */
			uint16_t rx_fc = MSG_RSP(rx.from, rx.cmd);
			switch(rx_fc) {
                case MSG_RSP(MAIN_THREAD_TEMP, TEMP_READREG):
                    printf("temp read value: %d\n", rx.data[0] | rx.data[1] << 8);
                    break;
                case MSG_RSP(MAIN_THREAD_LIGHT, LIGHT_READREG):
                    printf("light read value: %d\n", rx.data[0]);
                    break;
                default:
					break;
			}
        } else {
            /* Handle command data */
            switch(rx.cmd) {
                default:
                    break;
            }
        }
        
    }

    return 0;
}
