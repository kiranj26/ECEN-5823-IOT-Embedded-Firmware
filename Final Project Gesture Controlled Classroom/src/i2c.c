/***********************************************************************
* @file        i2c.c
* @version     0.0.1
* @brief       Function implementation file for I2C communication.
*
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


#include "i2c.h"
#include <sl_i2cspm.h>
#include <em_i2c.h>
#include "gpio.h"
#include "timer.h"

#define SI7021_DEVICE_ADDR (0x40) //slave address
#define MEASURE_TEMP_CMD   (0xF3) //No hold Master mode temperature Measurement cmd

// Include logging specifically for this .c file
//#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"

//I2C typedefs & global Variables
 I2C_TransferReturn_TypeDef transferStatus;
 I2C_TransferSeq_TypeDef transferSequence;
 uint8_t cmd_data;     //sending commands
 uint8_t read_data[2]; //for reading data from buffer



 /***************************************************************************//**
  * @brief
  *   Init function for powering up Si7021 sensor
  *
  * @param[in] none
  *
  * @return [in] none
  ******************************************************************************/
void Sensor_Init()
{
  gpioSensorEnSetOn();
}



/***************************************************************************//**
 * @brief
 *   Initialize the general I2C interface with specific configurations.
 *   This function sets up the I2C communication for devices not requiring
 *   specialized initialization.
 *
 * @param[in] none
 *
 * @return [in] none
 ******************************************************************************/
void Init_I2C()
{
  // Define and initialize the I2C configuration structure
  I2CSPM_Init_TypeDef I2C_Config = {
    .port = I2C0,                  // Use I2C instance 0
    .sclPort = gpioPortC,          // Port C for SCL line
    .sclPin = 10,                  // Pin 10 for SCL
    .sdaPort = gpioPortC,          // Port C for SDA line
    .sdaPin = 11,                  // Pin 11 for SDA
    .portLocationScl = 14,         // Port location of SCL
    .portLocationSda = 16,         // Port location of SDA
    .i2cRefFreq = 0,               // Use the currently configured reference frequency
    .i2cMaxFreq = I2C_FREQ_STANDARD_MAX, // Set to standard maximum frequency
    .i2cClhr = i2cClockHLRStandard // Clock low/high ratio standard
  };
  I2CSPM_Init(&I2C_Config);        // Initialize I2C with the specified configuration
}

// Configuration structure for I2C communication with APDS-9960 gesture sensor
I2CSPM_Init_TypeDef apds9960_gesture;

/***************************************************************************//**
 * @brief
 *   Initialize the I2C interface specifically for the APDS-9960 gesture sensor.
 *   This function configures the I2C communication tailored for the
 *   requirements of the gesture sensor.
 *
 * @param[in] none
 *
 * @return [in] none
 ******************************************************************************/
void APDS9960_I2C_Init()
{
  // Define and initialize the I2C configuration for APDS-9960 gesture sensor
  apds9960_gesture.port       = I2C0;
  apds9960_gesture.sclPort    = gpioPortC;
  apds9960_gesture.sclPin     = 10;
  apds9960_gesture.sdaPort    = gpioPortC;
  apds9960_gesture.sdaPin     = 11;
  apds9960_gesture.portLocationScl = 14;
  apds9960_gesture.portLocationSda = 16;
  apds9960_gesture.i2cRefFreq = 0;
  apds9960_gesture.i2cMaxFreq = I2C_FREQ_STANDARD_MAX;
  apds9960_gesture.i2cClhr    = i2cClockHLRStandard;

  I2CSPM_Init(&apds9960_gesture); // Initialize I2C with the gesture sensor configuration

  LOG_INFO("Gesture Sensor I2C initialized\r\n"); // Log initialization completion
}



/***************************************************************************//**
 * @brief
 *   Isend_I2C_command function used to write commands to the sensor
 *
 * @param[in] none
 *
 * @return [in] none
 ******************************************************************************/
void send_I2C_command (void) {

  I2C_TransferReturn_TypeDef transferStatus; // this can be locally defined
  Init_I2C();
  cmd_data = MEASURE_TEMP_CMD;
  transferSequence.addr = SI7021_DEVICE_ADDR << 1; //slave address
  transferSequence.flags = I2C_FLAG_WRITE;         //setting flags for write
  transferSequence.buf[0].data = &cmd_data;        // pointer to data to write
  transferSequence.buf[0].len = sizeof(cmd_data);

  // configuring the NVIC to generate an IRQ for the I2C0 module.
  // we will disable this NVIC interrupt when the I2C transfer has completed
  NVIC_EnableIRQ(I2C0_IRQn);

  transferStatus = I2C_TransferInit (I2C0, &transferSequence); //Initialize transfer
  //check for Error
  if (transferStatus < 0)
    {
      LOG_ERROR("I2C_TransferInit() Write error = %d", transferStatus);
    }
} // send_I2C_command()



/***************************************************************************//**
 * @brief
 *   read_I2C_command :will return data to uint8_t read_data[2]
 *
 * @param[in] none
 *
 * @return [in] none
 ******************************************************************************/
void read_I2C_command (void) {

  I2C_TransferReturn_TypeDef transferStatus; // this can be locally defined
  Init_I2C();
  transferSequence.addr  = SI7021_DEVICE_ADDR << 1; //slave address
  transferSequence.flags = I2C_FLAG_READ;           //set Read Flag
  transferSequence.buf[0].data = &read_data[0];     // pointer to data to read
  transferSequence.buf[0].len = sizeof(read_data);

  // configuring the NVIC to generate an IRQ for the I2C0 module.
  // we will disable this NVIC interrupt when the I2C transfer has completed
  NVIC_EnableIRQ(I2C0_IRQn);

  transferStatus = I2C_TransferInit (I2C0, &transferSequence);
  if (transferStatus < 0)
    {
      LOG_ERROR("I2C_TransferInit() Write error = %d", transferStatus);
    }
} // read_I2C_command()



/***************************************************************************//**
 * @brief
 *   I2C temperature readings taken by sensor is logged in this function
 *
 * @param[in] none
 *
 * @return [in] none
 ******************************************************************************/
uint32_t I2C_temp_reading()
{
  //converting data to temp value
  uint32_t temp_data =( 175.72 *(read_data[0]<<8 | read_data[1]))/65536 -(46.85);
  //logging data info
  LOG_INFO("TEMP_DATA %d Deg Celcius\n\r",temp_data);
  return temp_data;

}//I2C_temp_reading()




/***************************************************************************//**
 * @brief
 *   Read a single byte from a specified register of the APDS-9960 gesture sensor
 *   using I2C communication.
 *
 * @param[in] i2c
 *   Pointer to the I2C peripheral to be used for communication.
 *
 * @param[in] addr
 *   I2C address of the APDS-9960 sensor.
 *
 * @param[in] reg
 *   Register address from which to read data.
 *
 * @param[out] data
 *   Pointer to a variable where the read data will be stored.
 *
 * @return
 *   Returns 1 if the read is successful, a negative value if there is an error.
 ******************************************************************************/
uint32_t APDS9960_Read_Register(I2C_TypeDef *i2c, uint8_t addr, uint8_t reg, uint8_t *data)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t i2c_write_data[1];

  // Configure the I2C transfer for reading
  seq.addr  = (addr << 1);                  // Set the sensor's address
  seq.flags = I2C_FLAG_WRITE_READ;          // Indicate a write followed by read operation
  i2c_write_data[0] = reg;                  // Specify the register to read from
  seq.buf[0].data = i2c_write_data;         // Assign the write buffer
  seq.buf[0].len  = 1;                      // Length of data to write (register address)
  seq.buf[1].data = data;                   // Assign the read buffer
  seq.buf[1].len  = 1;                      // Length of data to read

  // Perform the I2C transfer
  ret = I2CSPM_Transfer(i2c, &seq);

  // Handle transfer result
  if(ret != i2cTransferDone)
  {
    *data = 0xff;                           // Set data to 0xff in case of error
    LOG_ERROR("I2C read error: %d in APDS9960_Read_Register\r\n", ret);
    return (uint32_t)ret;
  }

  return (uint32_t)1; // Indicate successful read
}


/***************************************************************************//**
 * @brief
 *   Write a single byte to a specified register of the APDS-9960 gesture sensor
 *   using I2C communication.
 *
 * @param[in] i2c
 *   Pointer to the I2C peripheral to be used for communication.
 *
 * @param[in] addr
 *   I2C address of the APDS-9960 sensor.
 *
 * @param[in] reg
 *   Register address where the data will be written.
 *
 * @param[in] data
 *   Byte of data to be written to the register.
 *
 * @return
 *   Returns 1 if the write is successful, a negative value if there is an error.
 ******************************************************************************/
uint32_t APDS9960_Write_Register(I2C_TypeDef *i2c, uint8_t addr, uint8_t reg, uint8_t data)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t i2c_write_data[2]; // Buffer for the register address and data
  uint8_t i2c_read_data[1];  // Buffer for read operation, not used in this case

  // Set up the I2C transfer for writing
  seq.addr  = (addr << 1);        // Set the sensor's address
  seq.flags = I2C_FLAG_WRITE;     // Indicate a write operation
  i2c_write_data[0] = reg;        // Assign the register address
  i2c_write_data[1] = data;       // Assign the data to write
  seq.buf[0].data = i2c_write_data;// Assign the write buffer
  seq.buf[0].len  = 2;            // Length of data to write (register address + data)
  seq.buf[1].data = i2c_read_data; // Assign the read buffer (not used)
  seq.buf[1].len  = 0;            // Length of data to read (none in this case)

  // Perform the I2C transfer
  ret = I2CSPM_Transfer(i2c, &seq);

  // Handle transfer result
  if(ret != i2cTransferDone)
  {
    LOG_ERROR("I2C write error: %d in APDS9960_Write_Register\r\n", ret);
    return (uint32_t)ret;
  }

  return (uint32_t)1; // Indicate successful write
}



/***************************************************************************//**
 * @brief
 *   Write a block of data to consecutive registers of the APDS-9960 gesture sensor
 *   using I2C communication. This function is useful for initializing multiple
 *   registers in a single I2C transaction.
 *
 * @param[in] i2c
 *   Pointer to the I2C peripheral to be used for communication.
 *
 * @param[in] addr
 *   I2C address of the APDS-9960 sensor.
 *
 * @param[in] reg
 *   Starting register address where the block write will begin.
 *
 * @param[in] length
 *   Number of bytes to write.
 *
 * @param[in] data
 *   Pointer to the block of data to be written to the registers.
 *
 * @return
 *   Returns 1 if the block write is successful, a negative value if there is an error.
 ******************************************************************************/
uint32_t APDS9960_Write_Block_Registers(I2C_TypeDef *i2c, uint8_t addr, uint8_t reg, uint8_t length, uint8_t const *data)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t i2c_write_data[10]; // Buffer to store the register address and data
  uint8_t i2c_read_data[1];   // Buffer for read operation, not used in this case
  int i;

  // Set up the I2C transfer for block writing
  seq.addr  = addr << 1;           // Set the sensor's address
  seq.flags = I2C_FLAG_WRITE;      // Indicate a write operation
  i2c_write_data[0] = reg;         // First byte is the starting register address
  for (i = 0; i < length; i++)     // Copy the data to be written into the buffer
  {
    i2c_write_data[i + 1] = data[i];
  }
  seq.buf[0].data = i2c_write_data;// Assign the write buffer
  seq.buf[0].len  = 1 + length;    // Length of data to write (register address + data)
  seq.buf[1].data = i2c_read_data; // Assign the read buffer (not used)
  seq.buf[1].len  = 0;             // Length of data to read (none in this case)

  // Perform the I2C transfer
  ret = I2CSPM_Transfer(i2c, &seq);

  // Handle transfer result
  if (ret != i2cTransferDone)
  {
    LOG_ERROR("I2C block write error: %d in APDS9960_Write_Block_Registers\r\n", ret);
    return (uint32_t)ret;
  }

  return (uint32_t)1; // Indicate successful block write
}

/***************************************************************************//**
 * @brief
 *   Read a block of data from consecutive registers of the APDS-9960 gesture sensor
 *   using I2C communication. This function is useful for reading multiple
 *   consecutive registers in a single I2C transaction.
 *
 * @param[in] i2c
 *   Pointer to the I2C peripheral to be used for communication.
 *
 * @param[in] addr
 *   I2C address of the APDS-9960 sensor.
 *
 * @param[in] reg
 *   Starting register address from which the block read will begin.
 *
 * @param[in] length
 *   Number of bytes to read.
 *
 * @param[out] data
 *   Pointer to the buffer where the read data will be stored.
 *
 * @return
 *   Returns the length of the data read if the operation is successful,
 *   or a negative value if there is an error.
 ******************************************************************************/
uint32_t APDS9960_Read_Block_Register(I2C_TypeDef *i2c, uint8_t addr, uint8_t reg, uint8_t length, uint8_t *data)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t i2c_write_data[1]; // Buffer for the register address

  // Set up the I2C transfer for block reading
  seq.addr  = addr << 1;            // Set the sensor's address
  seq.flags = I2C_FLAG_WRITE_READ;  // Indicate a write followed by read operation
  i2c_write_data[0] = reg;          // First byte is the starting register address
  seq.buf[0].data = i2c_write_data; // Assign the write buffer
  seq.buf[0].len  = 1;              // Length of data to write (register address)
  seq.buf[1].data = data;           // Assign the read buffer
  seq.buf[1].len  = length;         // Length of data to read

  // Perform the I2C transfer
  ret = I2CSPM_Transfer(i2c, &seq);

  // Handle transfer result
  if (ret != i2cTransferDone)
  {
    LOG_ERROR("I2C block read error: %d in APDS9960_Read_Block_Register\r\n", ret);
    return (uint32_t)ret;
  }

  return (uint32_t)length; // Indicate the length of data successfully read
}
