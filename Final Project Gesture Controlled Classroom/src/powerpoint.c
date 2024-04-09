/***********************************************************************
* @file        powerpoint.c
* @version     0.0.1
* @brief       Function implementation file for LCD Slide Navigation
*
* This file contains the implementation of functions declared in the
* powerpoint.h header file. It includes the logic for managing slides,
* navigating through them, and displaying them on an LCD screen.
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
************************************************************************
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "powerpoint.h"  // Include the corresponding header file
#include "ble.h"

#define DISPLAY_ROW_LEN 20
#define DISPLAY_NUMBER_OF_ROW 13


// Slide data
char *slides[] = {
    // Slide 1 (20 rows)
    "Welcome to C Prog\n"
    "Lecture Series 1:\n"
    "-----------------\n"
    "Topics:\n"
    "1. Basics of C\n"
    "2. Variables\n"
    "3. Control Flow\n"
    "4. Functions\n"
    "5. Arrays\n"
    "6. Pointers\n"
    "7. Structs\n"
    "8. File I/O\n"
    "9. Loops\n"
    "10. Conditionals\n"
    "11. Data Types\n"
    "12. Operators\n"
    //"13. Preprocessor\n"
    //"14. Command Line\n"
    //"15. Compilation\n"
    "End of Slide 1\n"
    "-----------------\n",

    // Slide 2 (20 rows)
    "Lecture Series 2:\n"
    "-----------------\n"
    "Advanced Topics:\n"
    "1. Memory Mgmt\n"
    "2. Dynamic Alloc\n"
    "3. Linked Lists\n"
    "4. Trees\n"
    "5. Graphs\n"
    "6. Sorting Algos\n"
    "7. Search Algos\n"
    "8. C Libraries\n"
    "9. Debugging\n"
    "10. Multithreading\n"
    "11. Optimization\n"
    "12. File Systems\n"
    //"13. Network Prog\n"
    //"14. DBMS Interfacing\n"
    //"15. System Calls\n"
    "End of Slide 2\n"
    "-----------------\n",
};

int numberOfSlides = sizeof(slides) / sizeof(char *);  // Calculate the total number of slides

// Current slide and position
int currentSlideIndex = 0;  // Index of the currently displayed slide
int currentRow = 0;         // Row position within the current slide

// Variable to hold the content of the last row
// char lastRow[DISPLAY_ROW_LEN + 1] = "PIR = 0  Gesture = 0";
// char lastRow[DISPLAY_ROW_LEN + 1] = "             ";

void clearDisplay() {
    // Clears the display by writing 20 spaces on each line
    for (int i = 0; i < DISPLAY_NUMBER_OF_ROW; i++) {
        displayPrintf(i, "                    ");
    }
}

// Function to display current Slide over LCD
void displayCurrentSlide() {
    clearDisplay();         // Call clearDisplay to clear the entire LCD screen before displaying the new slide.

    // Get the pointer to the current line of the current slide.
    const char *currentLine = slides[currentSlideIndex];

    int lineCount = 0;      // Initialize a counter for the number of lines displayed on the LCD.

    // Loop to skip lines until reaching the currentRow.
    for (int i = 0; i < currentRow && *currentLine; i++) {
        currentLine = strchr(currentLine, '\n');  // Find the next newline character in the slide's string.
        if (currentLine) currentLine++;           // If newline is found, move to the start of the next line.
    }

    // Loop to display the slide starting from the currentRow but not including the last row.
    while (*currentLine && lineCount < DISPLAY_NUMBER_OF_ROW - 1) {
        char buffer[DISPLAY_ROW_LEN + 1];         // Create a buffer to hold a single line of text.

        // Find the next newline character to determine the end of the current line.
        const char *nextLine = strchr(currentLine, '\n');

        // Calculate the length of the line to copy into the buffer.
        int copyLength = nextLine ? (nextLine - currentLine) : strlen(currentLine);

        // Ensure copyLength does not exceed DISPLAY_ROW_LEN.
        copyLength = (copyLength > DISPLAY_ROW_LEN) ? DISPLAY_ROW_LEN : copyLength;

        strncpy(buffer, currentLine, copyLength);   // Copy the line into the buffer.
        buffer[copyLength] = '\0';                  // Null-terminate the buffer to make it a valid C string.

        // Display the buffer on the LCD, increment lineCount for each displayed line.
        displayPrintf(lineCount++, buffer);

        // If there's a next line, move the currentLine pointer to the start of the next line.
        if (nextLine) currentLine = nextLine + 1;
        else break;  // If there's no next line, break out of the loop.
    }

    // Display static content on the last row of the LCD.
    displayPrintf(DISPLAY_NUMBER_OF_ROWS," ");

    displayPrintf(DISPLAY_NUMBER_OF_ROWS, "PIR = %d Gesture = %d", getPIRValue(), getGestureValue());
}

// Scrolls the current slide up or down based on the direction parameter.
void scroll(int direction) {
    currentRow += direction * 3;  // Adjust the currentRow based on the scroll direction (up or down) and scroll speed (3 rows at a time).

    if (currentRow < 0) currentRow = 0;  // Ensure currentRow doesn't go below 0 (beginning of the slide).

    // Code block to prevent scrolling past the end of the slide
    int maxRow = 0;                                 // Variable to keep track of the maximum row number in the current slide.
    const char *line = slides[currentSlideIndex];   // Pointer to the start of the current slide's text.
    while (*line) {                                 // Iterate over each character in the slide's text.
        if (strchr(line, '\n')) maxRow++;           // Increment maxRow for each newline found (indicating a new row).
        line = strchr(line, '\n');                  // Move the pointer to the next newline character.
        if (line) line++;                           // If a newline character is found, move to the start of the next line.
    }

    // Adjust currentRow to ensure it doesn't exceed the number of rows that can be displayed.
    if (currentRow > maxRow - DISPLAY_NUMBER_OF_ROW) currentRow = maxRow - DISPLAY_NUMBER_OF_ROW;

    // displayCurrentSlide();  // Refresh the display to show the new position after scrolling.
}

// Advances the presentation to the next slide, if available.
void nextPage() {
    // Check if there is a next slide available.
    if (currentSlideIndex < numberOfSlides - 1) {
        currentSlideIndex++;                        // Move to the next slide.
        currentRow = 0;                             // Reset currentRow to start at the beginning of the new slide.
        // displayCurrentSlide();                      // Display the new current slide.
    }
    // If the current slide is the last one, this function does nothing.
}

// Returns to the previous slide, if available.
void prevPage() {
    // Check if there is a previous slide available.
    if (currentSlideIndex > 0) {
        currentSlideIndex--;                        // Move to the previous slide.
        currentRow = 0;                             // Reset currentRow to start at the beginning of the previous slide.
        // displayCurrentSlide();                      // Display the new current slide.
    }
    // If the current slide is the first one, this function does nothing.
}
