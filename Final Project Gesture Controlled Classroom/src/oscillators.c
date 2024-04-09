/***********************************************************************
* @file        oscillators.c
* @version     0.0.1
* @brief       Function implementation file.
*
* @author      Ayswariya Kannan, ayka9480@Colorado.edu
* @date        Sep 13, 2023
*
* @institution University of Colorado Boulder (UCB)
* @course      ECEN 5823-001: IoT Embedded Firmware (Fall 2023)
* @instructor  David Sluiter
*
* @assignment  ecen5823-course-project-kiranj26
* @due         Dec 14, 2023
*
* @resources   Lecture 5, slide 12
*
* @copyright   All rights reserved. Distribution allowed only for the
*              use of assignment grading. Use of code excerpts allowed at the
*              discretion of author. Contact for permission.
*/

#include "em_cmu.h"
#include "oscillators.h"
#include "app.h"

// Include logging for this file
//#define INCLUDE_LOG_DEBUG (1)
#include "log.h"



/***************************************************************************//**
 * @brief
 *   Initializing Oscillator for Low Energy Timer
 *
 * @param[in] none
 *
 * @return [in] none
 ******************************************************************************/
void initOscillator(){

  //enable the low freq clk according to the energy mode
  #if (LOWEST_ENERGY_MODE == EM3)
    //EM3 should use ULFRCO 1KHz clock
    CMU_OscillatorEnable(cmuOsc_ULFRCO,true,true);
    //selecting LFA clock select
    CMU_ClockSelectSet(cmuClock_LFA , cmuSelect_ULFRCO);
  #else
    //all other energy modes use LFX0 32768Hz
    CMU_OscillatorEnable(cmuOsc_LFXO,true,false);
    //selecting LFA clock select
    CMU_ClockSelectSet(cmuClock_LFA , cmuSelect_LFXO);
  #endif

    // Enable LFA clock
    CMU_ClockEnable(cmuClock_LFA,true);
    //set prescaler value
    CMU_ClockDivSet(cmuClock_LETIMER0, cmuClkDiv_4);
    //enable LETIMER
    CMU_ClockEnable(cmuClock_LETIMER0,true);

    //for checking if clock is set correct
    CMU_ClockSelectGet(cmuClock_LFA);
    CMU_ClockDivGet(cmuClock_LFA);

    //check the frequency value using LOG function
    uint32_t temp = CMU_ClockFreqGet(cmuClock_LFA);
    LOG_INFO("Low frequency Clock :%d\n",temp);

    temp = CMU_ClockFreqGet(cmuClock_LETIMER0);
    LOG_INFO("LETIMER frequency :%d\n",temp);

}//initOscillator()


