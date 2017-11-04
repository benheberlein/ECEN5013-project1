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
 * @brief Macro for building respsonse case statements
 */
#define MSG_RSP(x, y) ((x) & MSG_FROM_MASK) << 8 | ((y) & MSG_CMD_MASK)

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
#define MSG_QUEUE_NUM 4
#define MSG_QUEUE_PERM  0666
mqd_t msg_queues[MSG_QUEUE_NUM];
struct mq_attr msg_attrs[MSG_QUEUE_NUM];
static const char *msg_names[] = {"/mainqueue",
      		                        "/lightqueue",
            	    	            "/tempqueue",
                          		    "/logqueue"};

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
 * @brief Send a message to the log queue
 *
 * @param tx The message to send
 * @param to The queue to send the message to
 *
 * @return MSG_SUCCESS or error value
 */ 
uint8_t logmsg_send(logmsg_t *tx, uint8_t to);


/**
 * @brief Initialize queues
 * 
 * Initialize the four message queues.
 *
 * @return Returns MSG_SUCCESS for successful init or error value
 */
uint8_t msg_init(void);

#endif /* __MSG_H__ */
