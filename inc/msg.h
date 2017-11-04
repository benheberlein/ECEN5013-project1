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

#include "main.h"
#include <stdint.h>
#include <mqueue.h>

/**
 * @brief Message attributes
 */
#define MSG_SIZE        16
#define MSG_LOGSIZE     256
#define MSG_DATASIZE    14
#define MSG_LOGDATASIZE 253
#define MSG_MAXMSGS     8
#define MSG_RSP_MASK    0x80
#define MSG_FROM_MASK   0x7f
#define MSG_CMD_MASK    0xff

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
typedef struct __attribute((packed)) msg_s {
    uint8_t from;       /* First bit is RSP field */
    uint8_t cmd;        /* Command */ 
    uint8_t data[MSG_DATASIZE];   /* NULL terminated data */
} msg_t;

/**
 * @brief Message structure for log thread
 */
typedef struct __attribute((packed)) logmsg_s {
    uint8_t from;               /* First bit is RSP field */
    uint8_t cmd;                /* Command */ 
    uint8_t data[MSG_LOGDATASIZE];   /* NULL terminated data */
} logmsg_t;

/**
 * @brief Queue descriptions
 */
#define MSG_QUEUE_TOTAL MAIN_THREAD_TOTAL
#define MSG_QUEUE_PERM  0666
#define MSG_QUEUE_MAIN  MAIN_THREAD_MAIN
#define MSG_QUEUE_LIGHT MAIN_THREAD_LIGHT
#define MSG_QUEUE_TEMP  MAIN_THREAD_TEMP
#define MSG_QUEUE_LOG   MAIN_THREAD_LOG
mqd_t msg_queues[MSG_QUEUE_TOTAL];
struct mq_attr msg_attrs[MSG_QUEUE_TOTAL];
static const char *msg_names[] = {"/mainqueue",
      		                        "/lightqueue",
            	    	            "/tempqueue",
                          		    "/logqueue"};

/**
 * @brief Temperature messages
 */ 
#define MSG_CMD_TEMP_INIT 0
#define MSG_CMD_TEMP_READREG 1
#define MSG_CMD_TEMP_WRITEREG 2
#define MSG_CMD_TEMP_WRITECONFIG 3
//#define MSG_CMD_TEMP_WRITEPTR
#define MSG_CMD_TEMP_GETTEMP 4
#define MSG_CMD_TEMP_SETRES 5
#define MSG_CMD_TEMP_SHUTDOWN 6
#define MSG_CMD_TEMP_WAKEUP 7
#define MSG_CMD_TEMP_ALIVE 8
#define MSG_CMD_TEMP_KILL 9

/**
 * @brief Send a message to a queue
 *
 * @param tx The message to send
 * @param to The queue to send the message to
 *
 * @return MSG_SUCCESS or error value
 */ 
uint8_t msg_send(msg_t *tx, uint8_t to);

/**
 * @brief Initialize queues
 * 
 * Initialize the four message queues.
 *
 * @return Returns MSG_SUCCESS for successful init or error value
 */
uint8_t msg_init(void);

#endif /* __MSG_H__ */
