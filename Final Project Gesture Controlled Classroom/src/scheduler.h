/***********************************************************************
* @file        scheduler.h
* @version     0.0.1
* @brief       Function header file.
*
* @Creator/Author/Editor :   
*               Ayswariya Kannan, ayka9480@Colorado.edu
*               Kiran Jojare, kijo7257@colorado.edu
*
* @institution University of Colorado Boulder (UCB)
* @course      ECEN 5823-001: IoT Embedded Firmware (Fall 2023)
* @instructor  David Sluiter
*
* @assignment  ecen5823-course-project-kiranj26
* @due         Dec 14, 2023
*
* @resources
* @copyright   All rights reserved. Distribution allowed only for the
*              use of assignment grading. Use of code excerpts allowed at the
*              discretion of author. Contact for permission.
*/

#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_
#include "stdint.h"
#include "sl_bt_api.h"


// function prototypes
void schedulerSetEventUF();
void schedulerSetEventCOMP1();
void schedulerSetEventI2C();
void schedulerSetEventPB0();
void schedulerSetEventPB1();
void schedulerSetEventPIR();
void schedulerSetEventGesture();
//uint32_t getNextEvent();
void gesture_state_machine(sl_bt_msg_t *evt);
void discovery_state_machine(sl_bt_msg_t *evt);
void PIR_state_machine(sl_bt_msg_t *evt);
uint32_t On_flag();
#endif /* SRC_SCHEDULER_H_ */
