/***********************************************************************
* @file        ble.h
* @version     0.0.1
* @brief       Function header file.
*
*
* @institution University of Colorado Boulder (UCB)
* @course      ECEN 5823-001: IoT Embedded Firmware (Fall 2023)
* @instructor  David Sluiter
*
* @assignment  ecen5823-course-project-kiranj26
* @due         Dec 14, 2023
*
* @resources   Lecture 10, Slide 28
*
* @Creator/Author/Editor :   
*               Ayswariya Kannan, ayka9480@Colorado.edu
*               Kiran Jojare, kijo7257@colorado.edu
*
* @copyright   All rights reserved. Distribution allowed only for the
*              use of assignment grading. Use of code excerpts allowed at the
*              discretion of author. Contact for permission.
*/

#ifndef SRC_BLE_H_
#define SRC_BLE_H_

#include "stdbool.h"
#include "stdint.h"
#include "sl_bt_api.h"
#define UINT8_TO_BITSTREAM(p, n) { *(p)++ = (uint8_t)(n); }

#define UINT32_TO_BITSTREAM(p, n) { *(p)++ = (uint8_t)(n); *(p)++ = (uint8_t)((n) >> 8); \
                                    *(p)++ = (uint8_t)((n) >> 16); *(p)++ = (uint8_t)((n) >> 24); }

#define UINT32_TO_FLOAT(m, e)     (((uint32_t)(m) & 0x00FFFFFFU) | (uint32_t)((int32_t)(e) << 24))

// BLE Data Structure, save all of our private BT data in here.
// Modern C (circa 2021 does it this way)
// typedef ble_data_struct_t is referred to as an anonymous struct definition
// This is the number of entries in the queue. Please leave
// this value set to 16.
#define QUEUE_DEPTH      (16)
// Student edit:
//   define this to 1 if your design uses all array entries
//   define this to 0 if your design leaves 1 array entry empty
#define USE_ALL_ENTRIES  (0)


typedef struct {
  // Common values for both servers and clients:
  bd_addr myAddress; // Device's Bluetooth address.
  uint8_t myAddressType; // Type of Bluetooth address.

  // Values unique to the server:
  uint8_t advertisingSetHandle; // Handle for the advertising set.
  bool connection_open; // Indicates if there is an open connection.
  bool ok_to_send_htm_indications_gest; // True if client enabled indications for the gesture service.
  bool ok_to_send_htm_indications_pir; // True if client enabled indications for the PIR service.
  bool indication_in_flight_gest; // True if a gesture service indication is currently being sent.
  bool indication_in_flight_pir; // True if a PIR service indication is currently being sent.
  bool bonded; // Indicates if the device is bonded.
  bool read_inflight; // True if a read operation of the encrypted button_state characteristic is ongoing.
  bool indication_enable; // True if indications are enabled for the gesture state.
  uint8_t connection_handle; // Handle for the current connection.
  uint32_t service_handle_gest; // Handle for the Gesture Encrypted Test service.
  uint16_t characteristic_handle_gest; // Handle for the Gesture Encrypted Test characteristic.
  uint32_t service_handle_pir; // Handle for the PIR Encrypted Test service.
  uint16_t characteristic_handle_pir; // Handle for the PIR Encrypted Test characteristic.
  uint32_t On_flag; // Flag indicating the ON state.

  // Values unique to the client:
} ble_data_struct_t;

typedef struct {
  uint16_t charHandle; // Characteristic handle from gatt_db.h.
  size_t   bufferLength; // Length of the data buffer in bytes.
  uint8_t  buffer[5]; // Data buffer for indications, with room for HTM (5 bytes) and button_state (2 bytes).
  // The first element of the array holds the flags byte.
} queue_struct_t;



// Function Prototypes

// Writes data to a queue.
bool write_queue(uint16_t a, size_t b, uint8_t* buffer);

// Reads data from a queue.
bool read_queue(uint16_t *a, size_t *b, uint8_t* buffer);

// Returns the current depth of the queue.
uint32_t get_queue_depth(void);

// Retrieves a pointer to the BLE data structure.
ble_data_struct_t* getBleDataPtr(void);

// Handles incoming BLE events.
void handle_ble_event(sl_bt_msg_t *evt);

// Sends a gesture value over Bluetooth.
void Bt_gesture(uint8_t gesture_value);

// Sends a PIR sensor value over Bluetooth.
void send_pir_value(uint8_t pir_value);

#endif /* SRC_BLE_H_ */

#if !DEVICE_IS_BLE_SERVER
  // Function implementations

  // Retrieves the last gesture value detected.
  uint8_t getGestureValue(void);

  // Retrieves the last PIR sensor value.
  uint8_t getPIRValue(void);
#endif
