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
#include <string.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <time.h>
#include <stdlib.h>

static const char *MAIN_USAGE = "One optional argument for log file name.\n";

static pthread_t main_tasks[MAIN_THREAD_TOTAL];
static uint8_t main_alive[MAIN_THREAD_TOTAL];

static void __main_heartbeat(union sigval arg);
static uint8_t __main_timer_init(void);
static uint8_t __main_pthread_init(void);

static uint8_t __main_timer_init(void) {

    timer_t tmr;
    struct itimerspec ts;
    struct sigevent se;

    se.sigev_notify = SIGEV_THREAD;
    se.sigev_value.sival_ptr = &tmr;
    se.sigev_notify_function = __main_heartbeat;
    se.sigev_notify_attributes = NULL;

    ts.it_value.tv_sec = 0;
    ts.it_value.tv_nsec = 100000000;
    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 0;

    if (timer_create(CLOCK_REALTIME, &se, &tmr) == -1) {
        logmsg_t ltx;
        LOG_FMT(MAIN_THREAD_MAIN, LOG_LEVEL_ERROR, ltx, "Failed to start heartbeat timer");
        
        logmsg_send(&ltx, MAIN_THREAD_LOG);
    }

    if (timer_settime(tmr, 0, &ts, 0) == -1) {
        logmsg_t ltx;
        LOG_FMT(MAIN_THREAD_MAIN, LOG_LEVEL_ERROR, ltx, "Failed to set heartbeat timer");
        logmsg_send(&ltx, MAIN_THREAD_LOG);
    }

    /* Send out alive packets */                  
    msg_t tx;            
    tx.from = MAIN_THREAD_MAIN;
    tx.cmd = LIGHT_ALIVE;
    tx.data[0] = 0;
    msg_send(&tx, MAIN_THREAD_LIGHT);

    tx.from = MAIN_THREAD_MAIN;
    tx.cmd = TEMP_ALIVE;
    tx.data[0] = 0;
    msg_send(&tx, MAIN_THREAD_TEMP);

    logmsg_t ltx;
    ltx.from = MAIN_THREAD_MAIN;
    ltx.cmd = LOG_ALIVE;
    ltx.data[0] = 0;
    logmsg_send(&ltx, MAIN_THREAD_LOG);

    return MAIN_SUCCESS;
}

static void __main_heartbeat(union sigval arg) {    

    /* Send aliveness requests */
    for (int i = 1; i < MAIN_THREAD_TOTAL; i++) {
        /* Check if we have recieved a confirmation from the last time */
        if (main_alive[i] != 0xa5) {
            logmsg_t ltx;
            LOG_FMT(MAIN_THREAD_MAIN, LOG_LEVEL_ERROR, ltx, "%s missed a heartbeat check, restarting thread", log_task_strings[i]);
            logmsg_send(&ltx, MAIN_THREAD_LOG);

            /* Kill thread and restart */
            pthread_cancel(main_tasks[i]);
            if (i == MAIN_THREAD_TEMP) {
                if (pthread_create(&main_tasks[MAIN_THREAD_TEMP], NULL, temp_task, NULL)) {
            	   	logmsg_t ltx;
			        LOG_FMT(MAIN_THREAD_MAIN, LOG_LEVEL_ERROR, ltx, "Couldn't restart thread");
            		logmsg_send(&ltx, MAIN_THREAD_LOG);
 
                }         
            } else if (i == MAIN_THREAD_LIGHT) {
                if (pthread_create(&main_tasks[MAIN_THREAD_LIGHT], NULL, light_task, NULL)) {
                    logmsg_t ltx;
                    LOG_FMT(MAIN_THREAD_MAIN, LOG_LEVEL_ERROR, ltx, "Couldn't restart thread");
                    logmsg_send(&ltx, MAIN_THREAD_LOG);
    
                }
            } else if (i == MAIN_THREAD_LOG) {
                if (pthread_create(&main_tasks[MAIN_THREAD_LOG], NULL, log_task, NULL)) {
                    logmsg_t ltx;
                    LOG_FMT(MAIN_THREAD_MAIN, LOG_LEVEL_ERROR, ltx, "Couldn't restart thread");
                    logmsg_send(&ltx, MAIN_THREAD_LOG);    
                }           
            }
            
        } else {
            main_alive[i] = 0;
        }
    }

    /* Restart timer and send alive packets*/
    __main_timer_init();
}

static uint8_t __main_pthread_init(void) {

    /* Open all threads */
    if (pthread_create(&main_tasks[MAIN_THREAD_TEMP], NULL, temp_task, NULL)) {
       return MAIN_ERR_INIT; 
    }
    if (pthread_create(&main_tasks[MAIN_THREAD_LIGHT], NULL, light_task, NULL)) {
       return MAIN_ERR_INIT; 
    }
    if (pthread_create(&main_tasks[MAIN_THREAD_LOG], NULL, log_task, NULL)) {
       return MAIN_ERR_INIT; 
    }

    return MAIN_SUCCESS;
}

uint8_t main_exit(msg_t *rx) {

    return MAIN_ERR_STUB;
}

int main(int argc, char **argv) {
    
    if (argc > 3) {
        printf("%s", MAIN_USAGE);
    }            
   
    msg_init();
    __main_pthread_init();

    /* Initialize logger */
    char *log_file;
    if (argc == 2) {
        log_file = argv[1];
    } else {
        log_file = "project1.log";
    }
    
    logmsg_t ltx;
    ltx.from = MAIN_THREAD_MAIN;
    ltx.cmd = LOG_INIT;
    strcpy((char *) ltx.data, log_file);
    logmsg_send(&ltx, MAIN_THREAD_LOG);

    LOG_FMT(MAIN_THREAD_MAIN, LOG_LEVEL_INFO, ltx, "Initialized logger");
    logmsg_send(&ltx, MAIN_THREAD_LOG);

	/* Initialize temperature module */
	msg_t tx;
	tx.from = MAIN_THREAD_MAIN;
	tx.cmd = TEMP_INIT;
	tx.data[0] = 0;
    msg_send(&tx, MAIN_THREAD_TEMP);

    LOG_FMT(MAIN_THREAD_MAIN, LOG_LEVEL_INFO, ltx, "Initialized temperature module");
    logmsg_send(&ltx, MAIN_THREAD_LOG);

	/* Initialize light module */
	tx.from = MAIN_THREAD_MAIN;
	tx.cmd = LIGHT_INIT;
	tx.data[0] = 0;
    msg_send(&tx, MAIN_THREAD_LIGHT);

    LOG_FMT(MAIN_THREAD_MAIN, LOG_LEVEL_INFO, ltx, "Initialized light module");
    logmsg_send(&ltx, MAIN_THREAD_LOG);

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

    /* Initialize heartbeat timer */
    __main_timer_init();

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

                    LOG_FMT(MAIN_THREAD_MAIN, LOG_LEVEL_INFO, ltx, "Register value is %d", rx.data[1] << 8 | rx.data[0]);
                    logmsg_send(&ltx, MAIN_THREAD_LOG);
                    break;
                case MSG_RSP(MAIN_THREAD_LIGHT, LIGHT_READREG):
                    LOG_FMT(MAIN_THREAD_MAIN, LOG_LEVEL_INFO, ltx, "Register value is %d", rx.data[0]);
                    logmsg_send(&ltx, MAIN_THREAD_LOG);
                    break;
                case MSG_RSP(MAIN_THREAD_TEMP, TEMP_ALIVE):
                    main_alive[MAIN_THREAD_TEMP] = rx.data[0];    
                    break;
                case MSG_RSP(MAIN_THREAD_LIGHT, LIGHT_ALIVE):
                    main_alive[MAIN_THREAD_LIGHT] = rx.data[0];    
                    break;
                case MSG_RSP(MAIN_THREAD_LOG, LOG_ALIVE):
                    main_alive[MAIN_THREAD_LOG] = rx.data[0];    
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
