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
 * @file msg.h
 * @brief Message queues for interprocess communication
 * 
 * The interprocess communication is handled by POSIX message queues. Each task
 * has its own queue that is implemented here.
 *
 * @author Ben Heberlein
 * @date Nov 2 2017
 * @version 1.0
 *
 */

#ifndef __MSG_H__
#define __MSG_H__

#include <stdint.h>
#include <mqueue.h>

/**
 * @brief Message attributes
 */
#define MSG_SIZE    14
#define MSG_LOGSIZE 253
#define MSG_MAXMSGS 16

/**
 * @brief Error codes
 */
#define MSG_SUCCESS     0
#define MSG_ERR_INIT    1
#define MSG_ERR_STUB    126
#define MSG_ERR_UNKNOWN 127

/**
 * @brief Message structure for threads
 */
typedef struct msg_s {
    uint8_t from;       /* First bit is RSP field */
    uint8_t cmd;        /* Command */ 
    uint8_t data[MSG_SIZE];   /* NULL terminated data */
} msg_t;

/**
 * @brief Message structure for log thread
 */
typedef struct logmsg_s {
    uint8_t from;               /* First bit is RSP field */
    uint8_t cmd;                /* Command */ 
    uint8_t data[MSG_LOGSIZE];   /* NULL terminated data */
} logmsg_t;

/**
 * @brief Queue descriptions
 */
#define MSG_QUEUE_TOTAL 4
#define MSG_QUEUE_PERM  0660
#define MSG_QUEUE_MAIN  0
#define MSG_QUEUE_LIGHT 1
#define MSG_QUEUE_TEMP  2
#define MSG_QUEUE_LOG   3
mqd_t msg_queues[MSG_QUEUE_TOTAL];
struct mq_attr msg_attrs[MSG_QUEUE_TOTAL];
char *msg_main_name[] = {"main",
                         "light",
                         "temp",
                         "log"};

/**
 * @brief Initialize queues
 * 
 * Initialize the four message queues.
 *
 * @return Returns MSG_SUCCESS for successful init or error value
 */
uint8_t msg_init(void);

#endif /* __MSG_H__ */
