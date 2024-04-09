/*
   gpio.h
  
    Created on: Dec 12, 2018
        Author: Dan Walkes

    Updated by Dave Sluiter Sept 7, 2020. moved #defines from .c to .h file.
    Updated by Dave Sluiter Dec 31, 2020. Minor edits with #defines.

    Editor: Feb 26, 2022, Dave Sluiter
    Change: Added comment about use of .h files.

 *
 * @Creator/Author/Editor :   
 *               Ayswariya Kannan, ayka9480@Colorado.edu
 *               Kiran Jojare, kijo7257@colorado.edu
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

#include "stdbool.h"

#define PIR_pin (10)
#define PIR_port (3)
// gesture sensor port and pin PF Port and Pin 3
#define APDS9960_port   gpioPortF
#define APDS9960_pin    3
// Function prototypes
void gpioInit();
void gpioLed0SetOn();
void gpioLed0SetOff();
void gpioLed1SetOn();
void gpioLed1SetOff();
void gpioSi7021SetOff();
void gpioSetDisplayExtcomin(bool value);
void gpioSensorEnSetOn();
void gpioPIROn();
void gpioPIROff();


#endif /* SRC_GPIO_H_ */
