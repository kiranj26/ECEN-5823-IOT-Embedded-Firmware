/*
   gpio.h
  
    Created on: Dec 12, 2018
        Author: Dan Walkes

    Updated by Dave Sluiter Sept 7, 2020. moved #defines from .c to .h file.
    Updated by Dave Sluiter Dec 31, 2020. Minor edits with #defines.

    Editor: Feb 26, 2022, Dave Sluiter
    Change: Added comment about use of .h files.

 *
 * Student edit: Add your name and email address here:
 * @student    Kiran Jojare, kiran.jojare@colorado.edu
 *
 
 */


// Students: Remember, a header file (a .h file) generally defines an interface
//           for functions defined within an implementation file (a .c file).
//           The .h file defines what a caller (a user) of a .c file requires.
//           At a minimum, the .h file should define the publicly callable
//           functions, i.e. define the function prototypes. #define and type
//           definitions can be added if the caller requires theses.


#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_




// Function prototypes

/**
 * @brief Initialize GPIO settings.
 *
 * This function initializes the GPIO pins used in the project.
 * It sets up the pins as input or output, configures pull-up or pull-down resistors,
 * and configures any other necessary settings.
 */
void gpioInit();

/**
 * @brief Turn on LED0.
 *
 * This function sets the GPIO pin connected to LED0 to a high voltage level,
 * which will turn on LED0.
 */
void gpioLed0SetOn();

/**
 * @brief Turn off LED0.
 *
 * This function sets the GPIO pin connected to LED0 to a low voltage level,
 * which will turn off LED0.
 */
void gpioLed0SetOff();

/**
 * @brief Turn on LED1.
 *
 * This function sets the GPIO pin connected to LED1 to a high voltage level,
 * which will turn on LED1.
 */
void gpioLed1SetOn();

/**
 * @brief Turn off LED1.
 *
 * This function sets the GPIO pin connected to LED1 to a low voltage level,
 * which will turn off LED1.
 */
void gpioLed1SetOff();





#endif /* SRC_GPIO_H_ */
