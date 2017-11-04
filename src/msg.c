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
 * @file msg.c
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

#include "msg.h"
#include <stdint.h>
#include <fcntl.h>
#include <mqueue.h>

uint8_t msg_init(void) {

    /* Set attributes */
    for (int i = 0; i < MSG_QUEUE_TOTAL; i++) {
        msg_attrs[i].mq_flags = 0;
        msg_attrs[i].mq_maxmsg = MSG_MAXMSGS;
        msg_attrs[i].mq_msgsize = MSG_SIZE;
        msg_attrs[i].mq_curmsgs = 0;
    }
    msg_attrs[MSG_QUEUE_LOG].mq_msgsize = MSG_LOGSIZE;

    /* Open queues */
    for (int i = 0; i < MSG_QUEUE_TOTAL; i++) {
        if ((msg_queues[i] = mq_open(msg_main_name[i], O_RDWR | O_CREAT, MSG_QUEUE_PERM, &msg_attrs[i])) == -1) {
            return MSG_ERR_INIT;   
        }
    }

    return MSG_SUCCESS;
}
