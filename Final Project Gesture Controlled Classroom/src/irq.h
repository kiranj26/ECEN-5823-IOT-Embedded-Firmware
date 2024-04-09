/***********************************************************************
* @file        irq.h
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
* @resources   Lecture 5, slide 34
* @copyright   All rights reserved. Distribution allowed only for the
*              use of assignment grading. Use of code excerpts allowed at the
*              discretion of author. Contact for permission.
*/

#ifndef SRC_IRQ_H_
#define SRC_IRQ_H_
#include "stdint.h"

// function prototypes
void LETIMER0_IRQHandler(void);
void I2C0_IRQHandler(void);
void GPIO_EVEN_IRQHandler(void);
void GPIO_ODD_IRQHandler(void);
uint32_t letimerMilliseconds();

#endif /* SRC_IRQ_H_ */
