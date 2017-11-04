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

#define MAIN_RSP(x, y) ((x) & MSG_FROM_MASK) >> 8 | ((y) & MSG_CMD_MASK)

static pthread_t main_tasks[MAIN_THREAD_TOTAL];

static uint8_t __main_pthread_init(void) {

    // TODO for all tasks...
    if (pthread_create(&main_tasks[MAIN_THREAD_TEMP], NULL, temp_task, NULL)) {
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

    /* Test read command */
	tx.from = MAIN_THREAD_MAIN;
	tx.cmd = TEMP_READREG;
	tx.data[0] = TEMP_REG_HIGH;
	tx.data[1] = 0;
    msg_send(&tx, MAIN_THREAD_TEMP);

    tx.from = MAIN_THREAD_MAIN;
	tx.cmd = TEMP_WRITEREG;
	tx.data[0] = TEMP_REG_HIGH;
	tx.data[1] = 0xa5;
    tx.data[2] = 0xa5;
    msg_send(&tx, MAIN_THREAD_TEMP);

    tx.from = MAIN_THREAD_MAIN;
	tx.cmd = TEMP_READREG;
	tx.data[0] = TEMP_REG_HIGH;
	tx.data[1] = 0;
    msg_send(&tx, MAIN_THREAD_TEMP);


    mqd_t rxq = mq_open(msg_names[MAIN_THREAD_MAIN], O_RDONLY);
	msg_t rx;
    while(1) {
        
        mq_receive(rxq, (char *) &rx, MSG_SIZE+10, NULL);
        if (rx.from & MSG_RSP_MASK) {
            /* Handle response data */
			uint16_t rx_fc = MAIN_RSP(rx.from, rx.cmd);
			switch(rx_fc) {
                case MAIN_RSP(MAIN_THREAD_TEMP, TEMP_READREG):
                    printf("read value: %d\n", rx.data[0] | rx.data[1] << 8);
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
