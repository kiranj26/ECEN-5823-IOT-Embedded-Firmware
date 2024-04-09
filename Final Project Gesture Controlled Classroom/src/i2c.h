/***********************************************************************
* @file        i2c.h
* @version     0.0.1
header*
* Functions for initializing I2C, sending commands, and reading data
* from I2C devices like the Si7021 sensor and the APDS-9960 gesture sensor.
*
* @Creator/Author/Editor: Kiran Jojare, kijo7257@colorado.edu
*
* @institution University of Colorado Boulder (UCB)
* @course      ECEN 5823-001: IoT Embedded Firmware (Fall 2023)
* @instructor  David Sluiter
*
* @assignment  ecen5823-course-project-kiranj26
* @due         Dec 14, 2023
*
* @resources   Lecture 8, slides 12, 13 & 14: Assignment 4 - LPM Part 2
*              Interrupt Driven I2C
*              APDS-9960 I2C implementation reference:
*              https://github.com/zhaohaiyi/i2cdevlib/tree/13b4927551311a1168bcdebe7c40930cb821d1ba/STM32HAL/APDS9960
*
* @copyright   All rights reserved. Distribution allowed only for the
*              use of assignment grading. Use of code excerpts allowed at the
*              discretion of author. Contact for permission.
*/

#ifndef SRC_I2C_H_
#define SRC_I2C_H_
#include "stdint.h"
#include "em_device.h"
#include "sl_i2cspm.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    I2C_TypeDef *i2c;
    uint8_t addr;
  } si114x_i2c_t;

//function prototypes
uint32_t  I2C_temp_reading();
void Sensor_Init();
void Init_I2C();
void APDS9960_I2C_Init();

void send_I2C_command ();
void read_I2C_command ();

uint32_t APDS9960_Write_Register (I2C_TypeDef *i2c,uint8_t addr, uint8_t reg, uint8_t data);
uint32_t APDS9960_Write_Block_Register (I2C_TypeDef *i2c,uint8_t addr, uint8_t reg, uint8_t length, uint8_t const *data);
uint32_t APDS9960_Read_Block_Register (I2C_TypeDef *i2c,uint8_t addr, uint8_t reg, uint8_t length, uint8_t  *data);
uint32_t APDS9960_Read_Register (I2C_TypeDef *i2c,uint8_t addr, uint8_t reg, uint8_t *data);


#endif /* SRC_I2C_H_ */
