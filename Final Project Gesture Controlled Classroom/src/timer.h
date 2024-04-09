/***********************************************************************
* @file        timer.h
* @version     0.0.1
* @brief       Function header file.
*
* @author      Ayswariya Kannan, ayka9480@Colorado.edu
* @date        Sep 29, 2023
*
* @institution University of Colorado Boulder (UCB)
* @course      ECEN 5823-001: IoT Embedded Firmware (Fall 2023)
* @instructor  David Sluiter
*
* @assignment  ecen5823-assignment4-Ayswariya Kannan
* @due         Sep 29, 2023
*
* @resources
* @copyright   All rights reserved. Distribution allowed only for the
*              use of assignment grading. Use of code excerpts allowed at the
*              discretion of author. Contact for permission.
*/
#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_
#include "stdint.h"
#include "app.h"

// define anything a caller needs
/*****************************************************/
//Setting frequency according to energy modes
//ULFRCO: 1000 Hz used for EM3 mode
//LFXO  :32768 Hz used for EM0,EM1,EM2
#if (LOWEST_ENERGY_MODE == EM3)
#define FREQ (1000)
#else
#define FREQ (32768)
#endif
/*****************************************************/

//defining parameters required to set the counter preload value
#define PRESCALER_VALUE (4)
//CALCULATING ACTUAL CLK FREQ
#define ACTUAL_CLK_FREQ (FREQ/PRESCALER_VALUE)
//for value to load into COMP0
#define MAX_COUNTER_VALUE (LETIMER_PERIOD_MS*ACTUAL_CLK_FREQ)/1000
//#define TIME_PERIOD_MS (1000/ACTUAL_CLK_FREQ)

// function prototypes
void initLETIMER0 ();
void timerWaitUs_polled(uint32_t us_wait);
void timerWaitUs_irq(uint32_t us_wait);
void unit_test();

#endif /* SRC_TIMER_H_ */
