/***************************************************************************//**
 * @file
 * @brief Core application logic.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * Date:        02-25-2022
 * Author:      Dave Sluiter
 * Description: This code was created by the Silicon Labs application wizard
 *              and started as "Bluetooth - SoC Empty".
 *              It is to be used only for ECEN 5823 "IoT Embedded Firmware".
 *              The MSLA referenced above is in effect.
 *
 *
 *
 * @ Creator/Author/Editor :   
 * Ayswariya Kannan, ayka9480@Colorado.edu
 * Kiran Jojare, kijo7257@colorado.edu
 *
 *
 *
 ******************************************************************************/
#include "em_common.h"
#include "app_assert.h"
#include "sl_bluetooth.h"
#include "gatt_db.h"
#include "app.h"
#include"src/powerpoint.h"




// *************************************************
// Students: It is OK to modify this file.
//           Make edits appropriate for each
//           assignment.
// *************************************************

#include "sl_status.h"             // for sl_status_print()

#include "src/ble_device_type.h"
#include "src/gpio.h"
#include "src/lcd.h"
#include "src/timer.h"
#include "src/oscillators.h"
#include "src/scheduler.h"
#include "src/i2c.h"
#include "src/ble.h"
#include "src/SparkFun_APDS9960.h"


// Students: Here is an example of how to correctly include logging functions in
//           each .c file.
//           Apply this technique to your other .c files.
//           Do not #include "src/log.h" in any .h file! This logging scheme is
//           designed to be included at the top of each .c file that you want
//           to call one of the LOG_***() functions from.

// Include logging specifically for this .c file
#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"



// *************************************************
// Power Manager
// *************************************************

// See: https://docs.silabs.com/gecko-platform/latest/service/power_manager/overview
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)

// -----------------------------------------------------------------------------
// defines for power manager callbacks
// -----------------------------------------------------------------------------
// Return values for app_is_ok_to_sleep():
//   Return false to keep sl_power_manager_sleep() from sleeping the MCU.
//   Return true to allow system to sleep when you expect/want an IRQ to wake
//   up the MCU from the call to sl_power_manager_sleep() in the main while (1)
//   loop.
//
// Students: We'll need to modify this for A2 onward so that compile time we
//           control what the lowest EM (energy mode) the MCU sleeps to. So
//           think "#if (expression)".

//Change this according to the energy mode
#if (LOWEST_ENERGY_MODE == EM0)
#define APP_IS_OK_TO_SLEEP      (false)
#else
//Sleep allowed for EM1,2,3
#define APP_IS_OK_TO_SLEEP      (true)
#endif

#define EVTPB0        (8)     //for event PB0 press
#define EVTPB1        (16)


// Return values for app_sleep_on_isr_exit():
//   SL_POWER_MANAGER_IGNORE; // The module did not trigger an ISR and it doesn't want to contribute to the decision
//   SL_POWER_MANAGER_SLEEP;  // The module was the one that caused the system wakeup and the system SHOULD go back to sleep
//   SL_POWER_MANAGER_WAKEUP; // The module was the one that caused the system wakeup and the system MUST NOT go back to sleep
//
// Notes:
//       SL_POWER_MANAGER_IGNORE, we see calls to app_process_action() on each IRQ. This is the
//       expected "normal" behavior.
//
//       SL_POWER_MANAGER_SLEEP, the function app_process_action()
//       in the main while(1) loop will not be called! It would seem that sl_power_manager_sleep()
//       does not return in this case.
//
//       SL_POWER_MANAGER_WAKEUP, doesn't seem to allow ISRs to run. Main while loop is
//       running continuously, flooding the VCOM port with printf text with LETIMER0 IRQs
//       disabled somehow, LED0 is not flashing.

#define APP_SLEEP_ON_ISR_EXIT   (SL_POWER_MANAGER_IGNORE)
//#define APP_SLEEP_ON_ISR_EXIT   (SL_POWER_MANAGER_SLEEP)
//#define APP_SLEEP_ON_ISR_EXIT   (SL_POWER_MANAGER_WAKEUP)

#endif // defined(SL_CATALOG_POWER_MANAGER_PRESENT)




// *************************************************
// Power Manager Callbacks
// The values returned by these 2 functions AND
// adding and removing power manage requirements is
// how we control when EM mode the MCU goes to when
// sl_power_manager_sleep() is called in the main
// while (1) loop.
// *************************************************

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)

bool app_is_ok_to_sleep(void)
{
  return APP_IS_OK_TO_SLEEP;
} // app_is_ok_to_sleep()

sl_power_manager_on_isr_exit_t app_sleep_on_isr_exit(void)
{
  return APP_SLEEP_ON_ISR_EXIT;
} // app_sleep_on_isr_exit()

#endif // defined(SL_CATALOG_POWER_MANAGER_PRESENT)




/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
SL_WEAK void app_init(void)
{
  // Put your application 1-time initialization code here.
  // This is called once during start-up.
  // Don't call any Bluetooth API functions until after the boot event.

  // Initialize various components and subsystems of the application:
  gpioInit();       // Initialize GPIO for LED control.
  initOscillator(); // Start the system oscillator for timing operations.
  initLETIMER0();   // Initialize the Low Energy Timer (LETIMER) for scheduled events.
  APDS9960_init();  // Initialize the APDS-9960 gesture sensor.

  //Add power requirements for EM1 and EM2
#if (LOWEST_ENERGY_MODE == EM1)
  sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
#elif (LOWEST_ENERGY_MODE == EM2)
  sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM2);
#endif

  // Finalize the initialization by configuring NVIC (Nested Vectored Interrupt Controller)
  // to handle various interrupts from peripherals.
  NVIC_ClearPendingIRQ (LETIMER0_IRQn); // Clear any pending interrupts from LETIMER0.
  NVIC_EnableIRQ(LETIMER0_IRQn);        // Enable NVIC to handle interrupts from LETIMER0.

  NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn); // Clear any pending even GPIO interrupts.
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);       // Enable NVIC to handle even GPIO interrupts.

  NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);  // Clear any pending odd GPIO interrupts.
  NVIC_EnableIRQ(GPIO_ODD_IRQn);        // Enable NVIC to handle odd GPIO interrupts.

} // app_init()




/*****************************************************************************
 * delayApprox(), private to this file.
 * A value of 3500000 is ~ 1 second. After assignment 1 you can delete or
 * comment out this function. Wait loops are a bad idea in general.
 * We'll discuss how to do this a better way in the next assignment.
 *****************************************************************************/
//static void delayApprox(int delay)
//{
//  volatile int i;
//
//  for (i = 0; i < delay; ) {
//      i=i+1;
//  }
//
//} // delayApprox()





/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
SL_WEAK void app_process_action(void)
{
  // Put your application code here for A1 to A4.
  // This is called repeatedly from the main while(1) loop
  // Notice: This function is not passed or has access to Bluetooth stack events.
  //         We will create/use a scheme that is far more energy efficient in
  //         later assignments.

  //  uint32_t evt;                   // for event
  //  evt = getNextEvent();           // get the events logged
  //  temperature_state_machine(evt); //start the state machine

} // app_process_action()


/**************************************************************************//**
 * Bluetooth stack event handler.
 * This overrides the dummy weak implementation.
 *
 * @param[in] evt Event coming from the Bluetooth stack.
 *
 * The code here will process events from the Bluetooth stack. This is the only
 * opportunity we will get to act on an event.
 *
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt)
{

  // A trick to suppress compiler warnings for the unused parameter 'evt'.
  //(void) evt;

  // Handling specific BLE events that require responses or actions
  // from the application but do not directly advance state machines.
  // This function call is critical for processing BLE-related events
  // and should be included in the ble.c file.
  handle_ble_event(evt); // Function defined in ble.c/.h

  #if DEVICE_IS_BLE_SERVER
   // For the SERVER:
   // Progress through various states in the state machine, driven by BLE events.
   // The PIR_state_machine function manages the PIR sensor's state transitions
   // based on incoming events. This function should be placed in scheduler.c.
   PIR_state_machine(evt); // Function defined in scheduler.c/.h

   // The gesture_state_machine function handles state transitions for the gesture sensor.
   // This function is responsible for interpreting gesture-related events and managing
   // state changes accordingly. It should also be located in scheduler.c.
   gesture_state_machine(evt);

  #else
   // For the CLIENT:
   // Manage the discovery process for services and characteristics.
   // The discovery_state_machine function sequences through the discovery
   // of BLE services and characteristics, crucial for client-side operations.
   // This function should be implemented in src/scheduler.c.
   discovery_state_machine(evt); // Function defined in src/scheduler.c/.h
  #endif

} // end of sl_bt_on_event()


