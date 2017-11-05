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
 * @file log.c
 * @brief Log task
 * 
 * This task will log messages to a file. The messages can be of several levels
 * (LOG_LEVEL_ERROR, LOG_LEVEL_WARNING, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG).
 *
 * @author Ben Heberlein
 * @date Nov 2 2017
 * @version 1.0
 *
 */

#include "log.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

/**
 * @brief Private variables
 */ 
static FILE *log_file;

/**
 * @brief Private functions
 */
static void __log_terminate(void *arg) {
    if (log_file != NULL) {
        /* Get time */
        time_t t;
        struct tm *ti;
        time(&t);
        ti = localtime(&t);

        /* Log */
        char *p = asctime(ti);
        p[strlen(p) - 1] = 0;
        fprintf(log_file, "%s\t", p);
        fprintf(log_file, "%s\t", log_task_strings[MAIN_THREAD_LOG]);
        fprintf(log_file, "%s\t", log_level_strings[LOG_LEVEL_WARN]);
        fprintf(log_file, "'%s'\n", "Closing log thread gracefully");
        fflush(log_file);

        fclose(log_file);
        log_file = NULL;
    }
}

void *log_task(void *data) {

    /* Register exit handler */
    pthread_cleanup_push(__log_terminate, "log");

    /* Command loop */
    mqd_t rxq = mq_open(msg_names[MAIN_THREAD_LOG], O_RDONLY);
    logmsg_t rx;
    while(1) {
        mq_receive(rxq, (char *) &rx, MSG_LOGSIZE+1, NULL);
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
                case LOG_ALIVE:
                    log_alive(&rx);
                    break;
                case LOG_INIT:
                    log_init(&rx);
                    break;
                case LOG_LOG:
                    log_log(&rx);
                    break;
                case LOG_SETPATH:
                    log_setpath(&rx);
                    break;
                case LOG_KILL:
                    log_kill(&rx);
                    break;
                default:
                    break;
            }
        }
    }

    pthread_cleanup_pop(1);

	return NULL;
}

uint8_t log_init(logmsg_t *rx) {

    if (log_file != NULL) {
        fclose(log_file);
        log_file = NULL;
    }

    if (rx->data[0] == 1) {
        log_file = fopen((char *)(rx->data+1), "w+");
    } else {
        log_file = fopen((char *)(rx->data+1), "a+");
    }
    if (log_file == NULL) {
        return LOG_ERR_FILE;
    }

    logmsg_t ltx;
    LOG_FMT(MAIN_THREAD_LOG, LOG_LEVEL_INFO, ltx, "Initialized logger");
    logmsg_send(&ltx, MAIN_THREAD_LOG);

	return LOG_SUCCESS;
}

uint8_t log_log(logmsg_t *rx) {

    if (log_file == NULL) {
        return LOG_ERR_UNINIT;   
    }

    /* Get time */
    time_t t;
    struct tm *ti;
    time(&t);
    ti = localtime(&t);

    /* Log */
    char *p = asctime(ti);
    p[strlen(p) - 1] = 0;
    fprintf(log_file, "%s\t", p);
    fprintf(log_file, "%s\t", log_task_strings[rx->from]);
    fprintf(log_file, "%s\t", log_level_strings[rx->data[0]]);
    fprintf(log_file, "'%s'\n", &rx->data[1]);
    fflush(log_file);

	return LOG_SUCCESS;
}

uint8_t log_setpath(logmsg_t *rx) {

    if (log_file != NULL) {
        fclose(log_file);
    }
    log_file = fopen((char *)rx->data, "w+");

	return LOG_SUCCESS;
}

uint8_t log_alive(logmsg_t *rx) {

    /* Send alive */
    msg_t tx;
    tx.from = MSG_RSP_MASK | MAIN_THREAD_LOG;
    tx.cmd = LOG_ALIVE;
    tx.data[0] = 0xa5;
    msg_send(&tx, rx->from);

	return LOG_SUCCESS;
}

uint8_t log_kill(logmsg_t *rx) {

    pthread_exit(0);

	return LOG_ERR_STUB;
}
