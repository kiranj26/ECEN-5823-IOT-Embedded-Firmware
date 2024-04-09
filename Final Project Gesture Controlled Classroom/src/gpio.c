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
 * @Creator/Author/Editor :   
 *               Ayswariya Kannan, ayka9480@Colorado.edu
 *               Kiran Jojare, kijo7257@colorado.edu
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
// Macro definitions for GPIO pin and port configurations

#define LED_port    (5)          // Port number for LEDs
#define LED0_pin    (4)          // Pin number for LED0
#define LED1_pin    (5)          // Pin number for LED1

#define Sensor_port (3)          // Port number for the Relative Humidity and Temperature Sensor (Si7021)
#define Si7021_pin  (15)         // Pin number for Si7021 sensor

#define LCD_pin     (13)         // Pin number for the LCD

#define PB_port     (5)          // Port number for Push Buttons
#define PB0_pin     (6)          // Pin number for Push Button 0
#define PB1_pin     (7)          // Pin number for Push Button 1

#define PIR_powerpin  (2)        // Pin number for controlling the power to the PIR sensor
#define PIR_powerport (gpioPortA)// Port number for the PIR sensor power control






// Set GPIO drive strengths and modes of operation
void gpioInit()
{
    // Student Edit:

    // Set the port's drive strength. In this MCU implementation, all GPIO cells
    // in a "Port" share the same drive strength setting. 
	//GPIO_DriveStrengthSet(LED_port, gpioDriveStrengthStrongAlternateStrong); // Strong, 10mA
	GPIO_DriveStrengthSet(LED_port, gpioDriveStrengthWeakAlternateWeak); // Weak, 1mA
	
	// Set the 2 GPIOs mode of operation
	GPIO_PinModeSet(LED_port, LED0_pin, gpioModePushPull, false);
	GPIO_PinModeSet(LED_port, LED1_pin, gpioModePushPull, false);
	GPIO_PinModeSet( Sensor_port, Si7021_pin, gpioModePushPull, true); //set push pull true for Enable pin
	GPIO_PinModeSet (PB_port,PB0_pin,gpioModeInputPullFilter,true);   //set input glitch filter enabled
	GPIO_PinModeSet (PB_port,PB1_pin,gpioModeInputPullFilter,true);   //set input glitch filter enabled
	GPIO_ExtIntConfig (PB_port, PB0_pin, PB0_pin, true, true, true);  //enable interrupt PB0
	GPIO_ExtIntConfig (PB_port, PB1_pin, PB1_pin, true, true, true);  //enable interrupt PB1

	//Enable IRQ for PIR sensor
	GPIO_PinModeSet(PIR_powerport, PIR_powerpin, gpioModePushPull, false);
	GPIO_PinModeSet (PIR_port, PIR_pin,gpioModeInputPullFilter,false);
	GPIO_ExtIntConfig (PIR_port, PIR_pin, PIR_pin, true, false, true);  //enable interrupt PB1

  // Enable IRQ for Gesture sensor
  GPIO_PinModeSet(APDS9960_port, APDS9960_pin, gpioModeInputPullFilter, true);
  GPIO_ExtIntConfig(APDS9960_port, APDS9960_pin, APDS9960_pin, false, true, true); // PF0 (Pin24 on gecko board) configured for external interrupt APDS9960

} // gpioInit()

void gpioLed0SetOn()
{
	GPIO_PinOutSet(LED_port, LED0_pin);
}


void gpioLed0SetOff()
{
	GPIO_PinOutClear(LED_port, LED0_pin);
}


void gpioLed1SetOn()
{
	GPIO_PinOutSet(LED_port, LED1_pin);
}


void gpioLed1SetOff()
{
	GPIO_PinOutClear(LED_port, LED1_pin);
}


// GPIO Enabled for Si7021 sensor
void gpioSensorEnSetOn()
{
  GPIO_PinOutSet( Sensor_port,  Si7021_pin);
}


// GPIO disabled for Si7021 sensor
void gpioSi7021SetOff()
{
  GPIO_PinOutClear( Sensor_port,  Si7021_pin);
}

void gpioSetDisplayExtcomin(bool value)
{
  if( value == true)
    GPIO_PinOutSet( Sensor_port,  LCD_pin);
  else if( value == false)
    GPIO_PinOutClear( Sensor_port,  LCD_pin);
}

//GPIO Enabled for PIR
void gpioPIROn()
{
  GPIO_PinOutSet( PIR_powerport, PIR_powerpin);
}

// GPIO disabled for PIR
void gpioPIROff()
{
  GPIO_PinOutClear( PIR_powerport, PIR_powerpin);
}

