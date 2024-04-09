/***********************************************************************
* @file        powerpoint.h
* @version     0.0.1
* @brief       Function header file for LCD Slide Navigation
*
* @author      Kiran Jojare, kijo7257Colorado.edu
* @date        Sep 29, 2023
*
* @institution University of Colorado Boulder (UCB)
* @course      ECEN 5823-001: IoT Embedded Firmware (Fall 2023)
* @instructor  David Sluiter
*
* @assignment  ecen5823-course-project-kiranj26
* @due         Dec 14, 2023
*
* @resources   NA
*
* @copyright   All rights reserved. Distribution allowed only for the
*              use of assignment grading. Use of code excerpts allowed at the
*              discretion of author. Contact for permission.
*/

#ifndef POWERPOINT_H
#define POWERPOINT_H

#include "lcd.h"      // Include LCD control functions
#include "string.h"   // Standard string manipulation functions
#include "stdlib.h"   // Standard library for general utilities

// Constant definitions
#define DISPLAY_ROW_LEN 20          // Number of characters per row in the LCD display
#define DISPLAY_NUMBER_OF_ROWS 12   // Number of rows in the LCD display

/**
 * @brief Clears the entire display screen.
 *
 * This function is used to clear the contents of the LCD display. It's typically
 * called before rendering new content to ensure the screen is blank.
 */
void clearDisplay();

/**
 * @brief Displays the current slide on the LCD.
 *
 * This function is responsible for rendering the current slide's content
 * onto the LCD screen. It fetches the current slide data and formats it
 * for display.
 */
void displayCurrentSlide();

/**
 * @brief Scrolls the content in a specified direction.
 *
 * @param direction The direction to scroll, typically represented by an integer.
 * Positive values scroll forward, while negative values scroll backward.
 */
void scroll(int direction);

/**
 * @brief Advances to the next slide or page.
 *
 * This function changes the current view to the next slide. It updates
 * the state to reflect this change and triggers a display refresh.
 */
void nextPage();

/**
 * @brief Returns to the previous slide or page.
 *
 * Similar to nextPage, but in the reverse direction. This function takes
 * the user back to the previous slide and updates the display accordingly.
 */
void prevPage();

#endif // POWERPOINT_H
