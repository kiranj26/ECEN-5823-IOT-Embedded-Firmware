/*
  gpio.c
 
   Created on: Dec 12, 2018
       Author: Dan Walkes
   Updated by Dave Sluiter Dec 31, 2020. Minor edits with #defines.

   March 17
   Dave Sluiter: Use this file to define functions that set up or control GPIOs.
   
   Jan 24, 2023
   Dave Sluiter: Cleaned up gpioInit() to make it less confusing for students regarding
                 drive strength setting. 

 *
 * Student edit: Add your name and email address here:
 * @student    Kiran Jojare, kiran.jojare@colorado.edu
 *
 
 */


// *****************************************************************************
// Students:
// We will be creating additional functions that configure and manipulate GPIOs.
// For any new GPIO function you create, place that function in this file.
// *****************************************************************************

#include <stdbool.h>
#include "em_gpio.h"
#include <string.h>

#include "gpio.h"


// Student Edit: Define these, 0's are placeholder values.
//
// See the radio board user guide at https://www.silabs.com/documents/login/user-guides/ug279-brd4104a-user-guide.pdf
// and GPIO documentation at https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/group__GPIO.html
// to determine the correct values for these.
// If these links have gone bad, consult the reference manual and/or the datasheet for the MCU.
// Change to correct port and pins:

#define LED_port   (gpioPortF)   //Port gpioPortF == 5
#define LED0_pin   (4)           //PF4 for LED0
#define LED1_pin   (5)           //PF5 for LED1



// Set GPIO drive strengths and modes of operation
void gpioInit()
{
    // Student Edit:

    // Set the port's drive strength. In this MCU implementation, all GPIO cells
    // in a "Port" share the same drive strength setting. 

  // for final submission setting GPIO drive to gpioDriveStrengthWeakAlternateWeak
	// GPIO_DriveStrengthSet(LED_port, gpioDriveStrengthStrongAlternateStrong); // Strong, 10mA
	GPIO_DriveStrengthSet(LED_port, gpioDriveStrengthWeakAlternateWeak); // Weak, 1mA
	
	// Set the 2 GPIOs mode of operation
	GPIO_PinModeSet(LED_port, LED0_pin, gpioModePushPull, false);
	GPIO_PinModeSet(LED_port, LED1_pin, gpioModePushPull, false);


} // gpioInit()

// Set LED0 to ON by setting its associated GPIO pin to high
void gpioLed0SetOn()
{
	GPIO_PinOutSet(LED_port, LED0_pin);
}

// Set LED0 to OFF by setting its associated GPIO pin to low
void gpioLed0SetOff()
{
	GPIO_PinOutClear(LED_port, LED0_pin);
}

// Set LED1 to ON by setting its associated GPIO pin to high
void gpioLed1SetOn()
{
	GPIO_PinOutSet(LED_port, LED1_pin);
}

// Set LED1 to OFF by setting its associated GPIO pin to low
void gpioLed1SetOff()
{
	GPIO_PinOutClear(LED_port, LED1_pin);
}






