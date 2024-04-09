/***********************************************************************
* @file        irq.c
* @version     0.0.1
* @brief       Function implementation file.
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
* @resources   Assignment 9
*
* @copyright   All rights reserved. Distribution allowed only for the
*              use of assignment grading. Use of code excerpts allowed at the
*              discretion of author. Contact for permission.
*/
#include "irq.h"
#include "em_letimer.h"
#include "em_gpio.h"
#include "scheduler.h"
#include "timer.h"
#include "app.h"
#include <sl_i2cspm.h>
#include <em_i2c.h>
#include "gpio.h"

// Include logging specifically for this .c file
//#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"
#define EVTPB0        (8)     //for event PB0 press
#define EVTPB1        (16)

uint32_t roller_count=0; //for keeping count of UFs

/***************************************************************************//**
 * @brief
 *   Low Energy Timer Interrupt Handler : Setting LEDs based on UF/COMP1 interrupt
 *
 * @param[in] none
 *
 * @return [in] none
 ******************************************************************************/
void LETIMER0_IRQHandler(void) {

  uint32_t flags;
 // step 1: determine source of interrupt
 //LETIMER_IntGetEnabled function ANDs IF & IEN bits to get only enabled interrupt
  flags = LETIMER_IntGetEnabled(LETIMER0);
 // step 2: clear source of interrupt
  LETIMER_IntClear(LETIMER0,flags);
 // step 3: perform work (handle) for the interrupt
  if (flags & LETIMER_IF_UF)
    {
      schedulerSetEventUF(); //set the UF Event
      roller_count+=1;       //increment the roller count
    }
  if (flags & LETIMER_IF_COMP1)
    {
      schedulerSetEventCOMP1();                          //set the COMP1 Event
      LETIMER_IntDisable(LETIMER0, LETIMER_IEN_COMP1);   //disable the COMP1 INterrupt Enable
    }

} //LETIMER0_IRQHandler()



/***************************************************************************//**
 * @brief
 *   I2C Interrupt handler will set the I2C Event
 *
 * @param[in] none
 *
 * @return [in] none
 ******************************************************************************/
void I2C0_IRQHandler(void) {

I2C_TransferReturn_TypeDef transferStatus;
// Interrupt based I2C transfer
transferStatus = I2C_Transfer(I2C0);

if (transferStatus == i2cTransferDone)
  {
    NVIC_DisableIRQ(I2C0_IRQn); //Disable I2C Interrupt
    schedulerSetEventI2C();     //set the I2C Event
  }

if (transferStatus < 0)
  {
LOG_ERROR("%d", transferStatus); //Error condition
  }

} // I2C0_IRQHandler()



/***************************************************************************//**
 * @brief
 *   Function to calculate the count of milliseconds since power up
 *
 * @param[in] none
 *
 * @return [in] uint32_t actual time : time calculated with UF interrupt
 *                                     and current LETIMER Count
 ******************************************************************************/
uint32_t letimerMilliseconds()
{
  uint32_t current_count = LETIMER_CounterGet(LETIMER0);
  uint32_t actual_time  = (roller_count*LETIMER_PERIOD_MS) +((MAX_COUNTER_VALUE -current_count) * ACTUAL_CLK_FREQ)/1000;
  return actual_time;
}//letimerMilliseconds()



/***************************************************************************//**
 * @brief
 *   GPIO Even interrupt handler. This handler is responsible for responding
 *   to interrupts on even-numbered GPIO pins. It sets events for push button 0
 *   and the PIR sensor based on the source of the interrupt.
 *
 * @param[in] none
 *
 * @return [in] none
 ******************************************************************************/
void GPIO_EVEN_IRQHandler(void)
{
  uint32_t flags = GPIO_IntGetEnabled(); // Get the source of the interrupt
  GPIO_IntClear(flags);                  // Clear the interrupt flags

  // Check if the interrupt was caused by push button 0 (PB0)
  if (flags & (1 << 6)) // PB0 is connected to pin 6
  {
    schedulerSetEventPB0(); // Set an event for PB0 in the scheduler
  }
  LOG_INFO("GPIO Even Interrupt\n\r");

  // Check for PIR sensor interrupt on its specified pin
  if ((flags & (1 << PIR_pin)) && (GPIO_PinInGet(PIR_port, PIR_pin) == 1))
  {
    schedulerSetEventPIR(); // Set an event for PIR sensor in the scheduler
  }
} // GPIO_EVEN_IRQHandler()


/***************************************************************************//**
 * @brief
 *   GPIO Odd interrupt handler. This handler is responsible for responding
 *   to interrupts on odd-numbered GPIO pins. It sets events for push button 1
 *   and the Gesture sensor based on the source of the interrupt.
 *
 * @param[in] none
 *
 * @return [in] none
 ******************************************************************************/
void GPIO_ODD_IRQHandler(void)
{
  uint32_t flags = GPIO_IntGetEnabled(); // Get the source of the interrupt
  GPIO_IntClear(flags);                  // Clear the interrupt flags

  // Check if the interrupt was caused by push button 1 (PB1)
  if (flags & (1 << 7)) // PB1 is connected to pin 7
  {
    schedulerSetEventPB1(); // Set an event for PB1 in the scheduler
  }
  LOG_INFO("GPIO Odd Interrupt\n\r");

  // Check if the interrupt was caused by the Gesture sensor
  if (flags == (1 << APDS9960_pin)) // Gesture sensor is connected to its specific pin
  {
    schedulerSetEventGesture(); // Set an event for the Gesture sensor in the scheduler
  }
} // GPIO_ODD_IRQHandler()
