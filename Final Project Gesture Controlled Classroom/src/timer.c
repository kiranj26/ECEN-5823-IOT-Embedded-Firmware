/***********************************************************************
* @file        timer.c
* @version     0.0.1
* @brief       Function implementation file.
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
* @resources   Lecture 8, slide 9
*
* @copyright   All rights reserved. Distribution allowed only for the
*              use of assignment grading. Use of code excerpts allowed at the
*              discretion of author. Contact for permission.
*/

#include "em_letimer.h"
#include "timer.h"
#include "app.h"

// Include logging specifically for this .c file
//#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"

//for value to load into COMP0
#define VALUE_TO_LOAD_COMP0 (LETIMER_PERIOD_MS*ACTUAL_CLK_FREQ)/1000
//value to load to COMP1
//#define VALUE_TO_LOAD_COMP1 (LETIMER_ON_TIME_MS*ACTUAL_CLK_FREQ)/1000
#define PERIOD_MIN (1)

/***************************************************************************//**
 * @brief
 *   Initializing Low energy timer
 *   Preloading COMP 0 & 1,Enabling interrupts for UF & COMP1
 * @param[in] none
 *
 * @return [in] none
 ******************************************************************************/
void initLETIMER0 ()
{
  uint32_t temp;

  const LETIMER_Init_TypeDef letimerInitData = {
  false, // enable; don't enable when init completes, we'll enable last
  false, // debugRun; useful to have the timer running when single-stepping in the debugger
  true,  // comp0Top; load COMP0 into CNT on underflow
  false, // bufTop; don't load COMP1 into COMP0 when REP0==0
  0,     // out0Pol; 0 default output pin value
  0,     // out1Pol; 0 default output pin value
  letimerUFOANone, // ufoa0; no underflow output action
  letimerUFOANone, // ufoa1; no underflow output action
  letimerRepeatFree, // repMode; free running mode i.e. load & go forever
  0 // COMP0(top) Value, I calculate this below
  };

  //initialize the LETIMER
  LETIMER_Init(LETIMER0,&letimerInitData);

  // calculate and load COMP0 (top)
  LETIMER_CompareSet(LETIMER0,0,VALUE_TO_LOAD_COMP0);
  // calculate and load COMP1
  //LETIMER_CompareSet(LETIMER0,1,VALUE_TO_LOAD_COMP1);

  // Clear all IRQ flags in the LETIMER0 IF status register
  LETIMER_IntClear (LETIMER0, 0xFFFFFFFF); // punch them all down

  // Set UF in LETIMER0_IEN, so that the timer will generate IRQs to the NVIC.
  temp = LETIMER_IEN_UF ;

  LETIMER_IntEnable (LETIMER0, temp); // defined the ISR routine LETIMER0_IRQHandler()

  // Enable the timer to starting counting down, set LETIMER0_CMD[START] bit, see LETIMER0_STATUS[RUNNING] bit
  LETIMER_Enable (LETIMER0, true);

  // Test code:
  // read it a few times to make sure it's running within the range of values we expect
  temp = LETIMER_CounterGet (LETIMER0);
  //LOG_INFO("TIMER VALUE %d\n",temp);

} //initLETIMER0()




/***************************************************************************//**
 * @brief
 *   Polled waiting for us_wait microseconds, using LETIMER0 tick counts as a reference
 *
 * @param[in] uint32_t us_wait : wait time in micro seconds
 *
 * @return [in] none
 ******************************************************************************/
void timerWaitUs_polled(uint32_t us_wait)
{

  uint32_t ms_wait = us_wait/1000 ; //converting to milli seconds

  // Range Checking between tick time period and LETIMER period
  if( ms_wait > LETIMER_PERIOD_MS)
    {
    LOG_ERROR(" Range outside limit\n\r");
    return ; // return Error logs if outside range
    }

  uint32_t tick_count  = ms_wait * ACTUAL_CLK_FREQ/1000;//no of ticks needed
  uint32_t timer_value =  LETIMER_CounterGet(LETIMER0); //get instant letimer counter value

  //if LETIMER COUNTER is greater than or equal to required ticks
  if(timer_value>=tick_count)
    {
      while((timer_value- LETIMER_CounterGet (LETIMER0))<tick_count);
    }
  else
    { //if tick value is more than LETIMER Counter value
      while(!(LETIMER_CounterGet(LETIMER0) ==(MAX_COUNTER_VALUE-(tick_count-timer_value))));
    }

}//timerWaitUs_polled()



/***************************************************************************//**
 * @brief
 *   IRQ based waiting for us_wait microseconds, using LETIMER0 COMP1 Interrupt
 *
 * @param[in] uint32_t us_wait : wait time in micro seconds
 *
 * @return [in] none
 ******************************************************************************/
void timerWaitUs_irq(uint32_t us_wait)
{

  uint32_t ms_wait = us_wait/1000 ; //converting to milli seconds

  // Range Checking between tick time period and LETIMER period
  if( ms_wait > LETIMER_PERIOD_MS || ms_wait < PERIOD_MIN)
    {
    LOG_ERROR(" Range outside limit\n\r");
    return ; // return Error logs if outside range
    }

  uint32_t tick_count  = ms_wait * ACTUAL_CLK_FREQ/1000;       //no of ticks needed
  uint32_t timer_value = LETIMER_CounterGet(LETIMER0); //get instant letimer counter value
  uint32_t comp1_value;                                //comp1 value to be loaded

  //if LETIMER COUNTER is greater than or equal to required ticks
  if(timer_value >= tick_count)
    {
      comp1_value = timer_value - tick_count;
    }
  else
    { //if tick value is more than LETIMER Counter value
      comp1_value = (MAX_COUNTER_VALUE-(tick_count-timer_value));
    }

  LETIMER_CompareSet(LETIMER0, 1, comp1_value);   // load comp1 value
  LETIMER_IntClear (LETIMER0, LETIMER_IFC_COMP1); //Clear COMP1 IRQ Flag
  LETIMER_IntEnable (LETIMER0, LETIMER_IEN_COMP1);//Enable COMP1 IRQ

}//timerWaitUs_irq()



