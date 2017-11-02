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

#define MSG_SIZE 14
#define MSG_LOGSIZE 253

#define MSG_SUCCESS 0
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
typedef struct msg_s {
    uint8_t from;               /* First bit is RSP field */
    uint8_t cmd;                /* Command */ 
    uint8_t data[MSG_LOGSIZE];   /* NULL terminated data */
} msg_t;

/**
 * @brief Queue instances
 */
mqd_t msg_main_queue;
struct mq_attr msg_main_attr;
mqd_t msg_light_queue;
struct mq_attr msg_light_attr;
mqd_t msg_temp_queue;
struct mq_attr msg_temp_attr;
mqd_t msg_log_queue;
struct mq_attr msg_log_attr;

/**
 * @brief Initialize queues
 * 
 * Initialize the four message queues.
 *
 * @return Returns MSG_SUCCESS for successful init or error value
 */
uint8_t msg_init(void);

#endif /* __MSG_H__ */
