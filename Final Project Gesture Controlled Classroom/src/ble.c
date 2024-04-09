/***********************************************************************
* @file        ble.c
* @version     0.0.1
* @brief       Function implementation file.
*
*
* @date        Dec 12 2023
*
* @institution University of Colorado Boulder (UCB)
* @course      ECEN 5823-001: IoT Embedded Firmware (Fall 2023)
* @instructor  David Sluiter
*
* @assignment  ecen5823-course-project-kiranj26
* @due         Dec 14, 2023
*
* @resources   Lecture 13 and Client Command Table
*              int32_t FLOAT_TO_INT32() given by Professor
*
* @Creator/Author/Editor :   
*               Ayswariya Kannan, ayka9480@Colorado.edu
*               Kiran Jojare, kijo7257@colorado.edu
*
* @copyright   All rights reserved. Distribution allowed only for the
*              use of assignment grading. Use of code excerpts allowed at the
*              discretion of author. Contact for permission.
*/
#include "ble.h"
#include "sl_bt_api.h"
#include "gatt_db.h"
#include "lcd.h"
#include "ble_device_type.h"
#include "em_gpio.h"
#include <stdbool.h>
#include "gpio.h"
#include "scheduler.h"

#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"
#include "math.h" // need function prototype for pow()

// Macro definitions for specific events and configurations
#define EVTPB0        (8)     // Identifier for event triggered by PB0 press
#define EVTPB1        (16)    // Identifier for event triggered by PB1 press
#define CONFIG        (0x0F)  // Identifier for BLE SM (Security Manager) configuration

// Instance of BLE data structure to hold private BLE-related data
ble_data_struct_t ble_data;

// Global variable definitions
uint8_t global_gesture_value = 0; // Holds the last detected gesture value
uint8_t global_pir_value = 0;     // Holds the last detected PIR sensor value

// Function implementations

// Returns the last detected gesture value
uint8_t getGestureValue(void) {
    return global_gesture_value;
}

// Returns the last detected PIR sensor value
uint8_t getPIRValue(void) {
    return global_pir_value;
}


#if DEVICE_IS_BLE_SERVER
  bool confirm_action = false;  // for bonding confirmation for server

  // Declare memory for the queue/buffer/fifo,
  // and the write and read pointers
  queue_struct_t my_queue[QUEUE_DEPTH]; // This is the storage for your queue

  uint32_t wptr = 0; // write pointer
  uint32_t rptr = 0; // read pointer

  bool full_flag = false; // flag for checking full condition
#else
  bd_addr bt_addr_server = SERVER_BT_ADDRESS;//server address
  bool confirm_action_client = false;
  bool PB0_pressed   = false;  //set true is PB0 is pressed
  bool PB1_pressed   = false;  //set true if PB1 is pressed when PB0 pressed
  bool PB1_released  = false;  //set true if PB1 pressed and PB0 pressed and released

  // UUIDs for PIR and Gesture services and characteristics

  // PIR Sensor Service UUID: Unique identifier for the PIR sensor service in the server configuration
  static const uint8_t PIRService[16] = {0xc3,0x89,0x17,0xd7,0x00,0x2b,0x3b,0xbb,0xd7,0x43,0x78,0xf5,0xb5,0x22,0x70,0xec};

  // Gesture Service UUID: Unique identifier for the Gesture service in the server configuration
  static const uint8_t GestureService[16] = {0x2c,0xb7,0x3c,0x1c,0xc1,0x12,0xab,0xad,0x39,0x41,0x43,0xb2,0xaf,0x53,0xbe,0x88};

  // PIR Sensor Characteristic UUID: Unique identifier for the PIR sensor characteristic used in server configuration
  static const uint8_t PIRChar[16] = {0x26,0xb9,0x3a,0x7e,0x1a,0x01,0x2d,0xbb,0xe3,0x4d,0x49,0x45,0xc1,0x10,0x9f,0x38};

  // Gesture Sensor Characteristic UUID: Unique identifier for the Gesture sensor characteristic used in server configuration
  static const uint8_t GestureChar[16] = {0xb4,0xd5,0xa6,0xdd,0x64,0xd0,0x4a,0xb5,0xa3,0x42,0xed,0x10,0x57,0x7a,0x5b,0x36};

  // -----------------------------------------------
  // Private function, original from Dan Walkes. I fixed a sign extension bug.
  // We'll need this for Client A7 assignment to convert health thermometer
  // indications back to an integer. Convert IEEE-11073 32-bit float to signed integer.
  // Referred from Assignment -7 Question pdf
  //-----------------------------------------------
  /***************************************************************************//**
   * @brief
   * Private function, original from Dan Walkes. I fixed a sign extension bug.
   * We'll need this for Client A7 assignment to convert health thermometer
   * indications back to an integer. Convert IEEE-11073 32-bit float to signed integer.
   * Referred from Assignment -7 Question pdf
   *
   * @param[in] const uint8_t *value_start_little_endian :value to be converted
   *
   * @return [in] int32_t value after conversion
   ******************************************************************************/
  static int32_t FLOAT_TO_INT32(const uint8_t *value_start_little_endian)
  {
    uint8_t mantissaSignByte = 0; // these bits will become the upper 8-bits of the mantissa
    int32_t mantissa; // this holds the 24-bit mantissa value with the upper 8-bits as sign bits
    // input data format is:
    // [0] = flags byte
    // [3][2][1] = mantissa (2's complement)
    // [4] = exponent (2's complement)
    // BT value_start_little_endian[0] has the flags byte
    int8_t exponent = (int8_t)value_start_little_endian[4]; // the exponent is a signed 2’s comp value
    // sign extend the mantissa value if the mantissa is negative
    if (value_start_little_endian[3] & 0x80)
      { // msb of [3] is the sign of the mantissa
        mantissaSignByte = 0xFF;
      }
    // assemble the mantissa
    mantissa = (int32_t) (value_start_little_endian[1] << 0) |
                         (value_start_little_endian[2] << 8) |
                         (value_start_little_endian[3] << 16) |
                         (mantissaSignByte << 24) ;
    // value = 10^exponent * mantissa; pow() returns a double type
    return (int32_t) (pow( (double) 10, (double) exponent) * (double) mantissa);

  } // FLOAT_TO_INT32
#endif



/***************************************************************************//**
 * @brief
 *  function that returns a pointer to the BLE private data
 *
 * @param[in] none
 *
 * @return [in] none
 ******************************************************************************/
ble_data_struct_t* getBleDataPtr()
{

return (&ble_data);

} // getBleDataPtr()



/***************************************************************************//**
 * @brief
 *  function that handles various events in the BT Stack
 *
 * @param[in] sl_bt_msg_t *evt ,evt->header is a uint32 value which has the event id
 *            various data is stored in the union evt->data.
 *            Structure definitions are in sl_bt_api.h
 *
 * @return [in] none
 ******************************************************************************/
void handle_ble_event(sl_bt_msg_t *evt)
{

  sl_status_t sc; // status code

  switch (SL_BT_MSG_ID(evt->header))
  {
// ******************************************************
// Events common to both Servers and Clients
// ******************************************************
// --------------------------------------------------------
// This event indicates the device has started and the radio is ready.
// Do not call any stack API commands before receiving this boot event!
// Including starting BT stack soft timers!
// --------------------------------------------------------
/*
* @Creator/Author/Editor :   
*               Ayswariya Kannan, ayka9480@Colorado.edu
*/
#if DEVICE_IS_BLE_SERVER
    case sl_bt_evt_system_boot_id:
      LOG_INFO("Radio is ready!\n\r");
      // handle boot event
      displayInit(); //Init LCD for display, it will generate a periodic 1Hz event
      displayPrintf(DISPLAY_ROW_NAME,"Server");
      ble_data.bonded =false;                     //not bonded
      ble_data.indication_in_flight_gest = false;
      ble_data.indication_in_flight_pir = false;

      //sl_bt_system_get_identity_address() returns the unique BT device address which need to be saved in the BLE private data
      sc = sl_bt_system_get_identity_address(&ble_data.myAddress, &ble_data.myAddressType);
      if (sc != SL_STATUS_OK) //checking for error status
        {
          LOG_ERROR("sl_bt_system_get_identity_address() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
        }
      else
        {
        displayPrintf( DISPLAY_ROW_BTADDR,"%x:%x:%x:%x:%x:%x", //print Bt address
                       ble_data.myAddress.addr[0],
                       ble_data.myAddress.addr[1],
                       ble_data.myAddress.addr[2],
                       ble_data.myAddress.addr[3],
                       ble_data.myAddress.addr[4],
                       ble_data.myAddress.addr[5]);
        displayPrintf(DISPLAY_ROW_ASSIGNMENT,"Final_Project");
        }

      //Creates an advertising set to use when the slave device wants to advertise its presence.
      //The handle created by this call is only to be used for advertising API calls, which also need to be saved
      sc = sl_bt_advertiser_create_set(&ble_data.advertisingSetHandle);
      if (sc != SL_STATUS_OK)//checking for error status
        {
          LOG_ERROR("sl_bt_advertiser_create_set() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
        }

      //Sets the timing to transmit advertising packets
      sc = sl_bt_advertiser_set_timing(ble_data.advertisingSetHandle,   // Value in units of 0.625 ms
                                  400,  //Advertising minimum and maximum interval to 250mS (250/0.625=400)
                                  400,
                                  0,    //no Advertising duration limit
                                  0);   //no maximum advertising events number limit
      if (sc != SL_STATUS_OK)
        {
          LOG_ERROR("sl_bt_advertiser_set_timing() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
        }
      //Tells the device to start sending advertising packets
      sc = sl_bt_advertiser_start(ble_data.advertisingSetHandle,
                                  sl_bt_advertiser_general_discoverable,  //set to Discoverable using general discovery procedure
                                  sl_bt_advertiser_connectable_scannable);//Undirected connectable scannable.
      if (sc != SL_STATUS_OK)
        {
          LOG_ERROR(" sl_bt_advertiser_start() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
        }
      else
        displayPrintf(DISPLAY_ROW_CONNECTION,"Advertising"); //Set LCD Display state to Advertising

      //set the configuration for security configuration with  io_capabilities
      sc = sl_bt_sm_configure(CONFIG, sl_bt_sm_io_capability_displayyesno);
      if (sc != SL_STATUS_OK)
        {
          LOG_ERROR(" sl_bt_sm_configure() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
        }

      //Delete previous bondings
      sc = sl_bt_sm_delete_bondings();
      if (sc != SL_STATUS_OK)
        {
          LOG_ERROR(" sl_bt_sm_delete_bondings() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
        }
      sc = sl_bt_system_set_soft_timer(1638, //for 50 msec timer
                                       4,     //handle
                                       0);    //repeating mode
      if (sc != SL_STATUS_OK)
        {
          LOG_ERROR(" sl_bt_system_set_soft_timer() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
        }

      break;
// --------------------------------------------------------
//This event indicates that a new connection was opened.
// --------------------------------------------------------
    case sl_bt_evt_connection_opened_id:
      // handle open event
      LOG_INFO("Connection opened \n\r");
      ble_data.connection_open = true;  //set the Flag for open connection true

      //save the connection handle. Use this value in all subsequent calls that require a connect handle,
      //when this connection is open to the client.
      ble_data.connection_handle = evt->data.evt_connection_opened.connection;

      //Stop advertising packets
      sc = sl_bt_advertiser_stop(ble_data.advertisingSetHandle);
      if (sc != SL_STATUS_OK)
        {
          LOG_ERROR(" sl_bt_advertiser_start() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
        }

      //Send a request with a set of parameters to the master
      sc = sl_bt_connection_set_parameters(ble_data.connection_handle,//Time = Value x 1.25 ms
                                           60, //Connection Interval minimum to 75mS (75/1.25 =60)
                                           60, //Connection Interval maximum to 75mS (75/1.25 =60)
                                           4,  //connection intervals the peripheral can skip =300 ms/75ms =4
                                           80,//Supervision timeout set to a value greater than (1 + slave latency)*(connection_interval * 2)=750
                                           0,
                                           0);
      if (sc != SL_STATUS_OK)
        {
          LOG_ERROR(" sl_bt_connection_set_parameters() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
        }
      else if( ble_data.connection_open == true)
        {
          displayPrintf(DISPLAY_ROW_CONNECTION," ");         //clear state displayed in the LCD
          displayPrintf(DISPLAY_ROW_CONNECTION,"Connected"); //Set LCD Display state to Connected

          displayPrintf(DISPLAY_NUMBER_OF_ROWS-2," ");
          displayPrintf(DISPLAY_NUMBER_OF_ROWS-2, "PIR = %d Gesture = %d", global_pir_value, global_gesture_value);
        }
      break;
// --------------------------------------------------------
//This event indicates that a connection was closed.
// --------------------------------------------------------
    case sl_bt_evt_connection_closed_id:
      // handle close event
      LOG_INFO("Connection closed \n\r");
      ble_data.connection_open = false;                 //set the Flag for open connection false
      ble_data.ok_to_send_htm_indications_gest = false; //set all flags to false
      ble_data.ok_to_send_htm_indications_pir = false;
      ble_data.indication_in_flight_gest = false;
      ble_data.indication_in_flight_pir = false;
      ble_data.On_flag =0;
      gpioLed1SetOff();                                 //PIR indications Off
      gpioLed0SetOff();                                 //Getsure indications Off
      //Tells the device to start sending advertising packets
      sc = sl_bt_advertiser_start(ble_data.advertisingSetHandle,
                             sl_bt_advertiser_general_discoverable,  //set to Discoverable using general discovery procedure
                             sl_bt_advertiser_connectable_scannable);//Undirected connectable scannable.
      if (sc != SL_STATUS_OK)
        {
          LOG_ERROR(" sl_bt_advertiser_start() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
        }//if
      else
        {
          displayPrintf(DISPLAY_ROW_TEMPVALUE," ");            //clear state displayed in the LCD
          displayPrintf(DISPLAY_ROW_CONNECTION," ");           //clear state displayed in the LCD
          displayPrintf(DISPLAY_ROW_9," ");
          displayPrintf(DISPLAY_ROW_PASSKEY," ");
          displayPrintf(DISPLAY_ROW_ACTION, " ");
          displayPrintf(DISPLAY_ROW_CONNECTION,"Advertising"); //Set LCD Display state to Advertising
        }//else
      //delete previous bondings
      sc = sl_bt_sm_delete_bondings();
      if (sc != SL_STATUS_OK)
        {
          LOG_ERROR(" sl_bt_sm_delete_bondings() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
        }

      break;
// ---------------------------------------------------------------------------------------------------
//Triggered whenever the connection parameters are changed and at any time a connection is established
//----------------------------------------------------------------------------------------------------
    case sl_bt_evt_connection_parameters_id:
      //triggered when connection parameters are changed

      //sl_bt_connection_set_parameters() will generate a sl_bt_evt_connection_parameters_id
      //event that indicates how the master has set the connection parameters.we need to log these values
      LOG_INFO("Connection parameters set by Master :Interval: %d\n\r",evt->data.evt_connection_parameters.interval);
      LOG_INFO("Connection parameters set by Master :Latency: %d\n\r",evt->data.evt_connection_parameters.latency);
      LOG_INFO("Connection parameters set by Master :Timeout: %d\n\r",evt->data.evt_connection_parameters.timeout);
      break;
//-------------------------------------------------------------------------------------------------
//This event indicates that sl_bt_external_signal(myEvent) was called and returns the myEvent value
//in the event data structure:evt->data.evt_system_external_signal.extsignals
//-------------------------------------------------------------------------------------------------
    case sl_bt_evt_system_external_signal_id:
      //This event indicates that sl_bt_external_signal(myEvent) was called and returns the myEvent value
      ;
      uint32_t event = evt->data.evt_system_external_signal.extsignals; //get the event set in the Ext signal

      if((event & EVTPB0) == EVTPB0){ //Check for PB0 event

          uint8_t value = !(GPIO_PinInGet(gpioPortF,6)); //get the value of PB0

          if(confirm_action == true && value == 0x01) //for yes/no bonding passkey confirmation
            {
              sc = sl_bt_sm_passkey_confirm(ble_data.connection_handle,1);
              if (sc != SL_STATUS_OK)
                {
                  LOG_ERROR("sl_bt_sm_passkey_confirm() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
                }
              confirm_action = false; //set the flag false
            }
      }//event PB0
      break;
// ---------------------------------------------------------------------------------------------------
//This event indicates that a soft timer has expired.
//----------------------------------------------------------------------------------------------------
    case sl_bt_evt_system_soft_timer_id:
      if(evt->data.evt_system_soft_timer.handle == 2)
        displayUpdate(); //update the LCD screen

      else if (evt->data.evt_system_soft_timer.handle == 4)//check for indications queued in circular buffer
        {
          if((get_queue_depth() != 0) && ble_data.indication_in_flight_gest == false && ble_data.indication_in_flight_pir ==false)
            {
              uint16_t char_handle;
              size_t length;
              uint8_t buff[5];
              memset(buff, 0, 5);
              read_queue(&char_handle,&length,&buff[0]); //save to the corresponding variables
              sc = sl_bt_gatt_server_send_indication(  ble_data.connection_handle,
                                                       char_handle, // handle from gatt_db.h
                                                       length,       //1 bytes
                                                       &buff[0]);
              if (sc != SL_STATUS_OK)
                {
                  LOG_ERROR(" sl_bt_gatt_server_send_indication() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
                }
              else
                {
                  if(char_handle == gattdb_motion_detected )
                    ble_data.indication_in_flight_pir = true; //inflight condition is true
                  else
                    ble_data.indication_in_flight_gest = true; //inflight condition is true
                  }//if
              LOG_INFO("reading from queue %d of length\n\r",length);

            }
        }//check for indications queued in circular buffer
          break;

// ******************************************************
// Events for Only Servers
// ******************************************************
//-------------------------------------------------------------------------------------------------------------
//"Indicates either:
//A local Client Characteristic Configuration descriptor (CCCD) was changed by the remote GATT client, or
//That a confirmation from the remote GATT Client was received upon a successful reception of the indication
//I.e. we sent an indication from our server to the client with sl_bt_gatt_server_send_indication()"
//--------------------------------------------------------------------------------------------------------------
    case sl_bt_evt_gatt_server_characteristic_status_id:
      // Handle GATT server characteristic status events

      // Check if the event is related to the Gesture Value characteristic
      if(evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_gesture_value)
        {
          // Check for changes in indication status
          if(evt->data.evt_gatt_server_characteristic_status.status_flags == sl_bt_gatt_server_client_config)
            {
              // Check if the client has enabled indications
              if(evt->data.evt_gatt_server_characteristic_status.client_config_flags == sl_bt_gatt_server_indication)
                {
                  LOG_INFO("Indication turned On by client \n\r");
                  ble_data.ok_to_send_htm_indications_gest = true; // Enable sending gesture indications
                  gpioLed0SetOn(); // Turn on LED for Gesture indications
                }
              else
                {
                  ble_data.ok_to_send_htm_indications_gest = false; // Disable sending gesture indications
                  displayPrintf(DISPLAY_ROW_TEMPVALUE," "); // Clear the display
                  gpioLed0SetOff(); // Turn off LED for Gesture indications
                }
            }

          // Check for confirmation of indication reception by the client
          if(evt->data.evt_gatt_server_characteristic_status.status_flags == sl_bt_gatt_server_confirmation)
            {
              LOG_INFO("Indication received by client\n\r");
              ble_data.indication_in_flight_gest = false; // Indication has been successfully received
            }
        }

      // Check if the event is related to the Motion Detected characteristic
      else if(evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_motion_detected )
        {
          // Check for changes in indication status
          if(evt->data.evt_gatt_server_characteristic_status.status_flags == sl_bt_gatt_server_client_config)
            {
              // Check if the client has enabled indications
              if(evt->data.evt_gatt_server_characteristic_status.client_config_flags == sl_bt_gatt_server_indication)
                {
                  LOG_INFO("Indication turned On by client \n\r");
                  ble_data.ok_to_send_htm_indications_pir = true; // Enable sending PIR indications
                  gpioLed1SetOn(); // Turn on LED for PIR indications
                }
              else
                {
                  ble_data.ok_to_send_htm_indications_pir = false; // Disable sending PIR indications
                  gpioLed1SetOff(); // Turn off LED for PIR indications
                }
            }

          // Check for confirmation of indication reception by the client
          if(evt->data.evt_gatt_server_characteristic_status.status_flags == sl_bt_gatt_server_confirmation)
            {
              LOG_INFO("Indication received by client\n\r");
              ble_data.indication_in_flight_pir = false; // Indication has been successfully received
            }
        }
      break;

//-----------------------------------------------------------------------------
//Possible event from calling sl_bt_gatt_server_send_indication() -
//i.e. we never received a confirmation for a previously transmitted indication.
//-----------------------------------------------------------------------------
    case sl_bt_evt_gatt_server_indication_timeout_id:

      LOG_INFO("Confirmation of Indication not received from client.Indication Timeout \n\r");
      ble_data.indication_in_flight_gest = false; //since the prev indication timed out we can have the next indication
      ble_data.indication_in_flight_pir = false; //since the prev indication timed out we can have the next indication

      confirm_action = false;
      break;
//-----------------------------------------------------------------------------
//Indicates a user request to display that the new bonding request is
//received and for the user to confirm the request
//-----------------------------------------------------------------------------
    case sl_bt_evt_sm_confirm_bonding_id:
      LOG_INFO("Confirm Bonding\n\r");
      sc = sl_bt_sm_bonding_confirm(ble_data.connection_handle,1);//accept request
      if (sc != SL_STATUS_OK)
        {
          LOG_ERROR("sl_bt_sm_bonding_confirm() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
        }
      break;
//-----------------------------------------------------------------------------
//For Passkey Display so that user can authenticate the connection
//-----------------------------------------------------------------------------
    case sl_bt_evt_sm_confirm_passkey_id:
      LOG_INFO("Confirm passkey\n\r");
      //display passkey for confirmation
      uint32_t passkey= evt->data.evt_sm_passkey_display.passkey;
      displayPrintf(DISPLAY_ROW_PASSKEY,"Passkey %d",passkey);
      displayPrintf(DISPLAY_ROW_ACTION, "Confirm with PB0");
      confirm_action = true;

      break;
//--------------------------------------------------------------------
//Triggered after the pairing or bonding procedure is successfully
//--------------------------------------------------------------------
    case sl_bt_evt_sm_bonded_id:
      LOG_INFO("Bonded\n\r");
      displayPrintf(DISPLAY_ROW_PASSKEY," ");
      displayPrintf(DISPLAY_ROW_ACTION, " ");
      displayPrintf(DISPLAY_ROW_CONNECTION,"Bonded");
      ble_data.bonded = true;
      break;
//-------------------------------------------------------------------
//This event is triggered if the pairing or bonding procedure fails.
//-------------------------------------------------------------------
    case sl_bt_evt_sm_bonding_failed_id:
      LOG_INFO("Bonding Failed\n\r");
      sc = sl_bt_connection_close(evt->data.evt_sm_bonding_failed.connection);
      if (sc != SL_STATUS_OK)
        {
          LOG_ERROR("sl_bt_connection_close() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
        }
      break;



#else
// ******************************************************
// Events common to both Servers and Clients
// *****************************************************
// --------------------------------------------------------
// This event indicates the device has started and the radio is ready.
// Do not call any stack API commands before receiving this boot event!
// Including starting BT stack soft timers!
// --------------------------------------------------------
/*
* @ Creator/Author/Editor :   
*               Kiran Jojare, kijo7257@colorado.edu
*/
    case sl_bt_evt_system_boot_id:
      LOG_INFO("Radio is ready!\n\r");
      // handle boot event
      displayInit(); //Init LCD for display, it will generate a periodic 1Hz event
      displayPrintf(DISPLAY_ROW_NAME,"Client");
      ble_data.read_inflight =false;
      //sl_bt_system_get_identity_address() returns the unique BT device address which need to be saved in the BLE private data
      sc = sl_bt_system_get_identity_address(&ble_data.myAddress, &ble_data.myAddressType);
      if (sc != SL_STATUS_OK) //checking for error status
        {
          LOG_ERROR("sl_bt_system_get_identity_address() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
        }
      else
        {
          displayPrintf( DISPLAY_ROW_BTADDR,"%x:%x:%x:%x:%x:%x", //print Bt address
                         ble_data.myAddress.addr[0],
                         ble_data.myAddress.addr[1],
                         ble_data.myAddress.addr[2],
                         ble_data.myAddress.addr[3],
                         ble_data.myAddress.addr[4],
                         ble_data.myAddress.addr[5]);
          displayPrintf(DISPLAY_ROW_ASSIGNMENT,"Final_Project");
        }
      //Set Scanning mode of Client. Set phy to 1M and Passive scanning
      sc = sl_bt_scanner_set_mode(sl_bt_gap_1m_phy,0);
      if (sc != SL_STATUS_OK) //checking for error status
        {
          LOG_ERROR("sl_bt_scanner_set_mode() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
        }
      //Set timing for scanning.Set phy to 1M, scan interval to 50ms and scan window to 25ms
      sc = sl_bt_scanner_set_timing(sl_bt_gap_1m_phy,
                                    80,   // value = 50/0.625 =80
                                    40);  // value = 25/0.625 =40
      if (sc != SL_STATUS_OK) //checking for error status
        {
          LOG_ERROR("sl_bt_scanner_set_timing() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
        }
      //Set Connection parameters
      sc = sl_bt_connection_set_default_parameters(60, //75/1.25
                                                   60, //75/1.25
                                                   4,
                                                   85,// 850/10
                                                   0,
                                                   4);  //given values
      if (sc != SL_STATUS_OK) //checking for error status
        {
          LOG_ERROR(" sl_bt_connection_set_default_parameters() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
        }
      //Scanner Start
      sc = sl_bt_scanner_start(sl_bt_gap_1m_phy,sl_bt_scanner_discover_generic);
      if (sc != SL_STATUS_OK) //checking for error status
        {
          LOG_ERROR("sl_bt_scanner_start() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
        }
      else
        displayPrintf(DISPLAY_ROW_CONNECTION,"Discovering"); //Set LCD Display state to Discovering

      sc= sl_bt_sm_configure( CONFIG,sl_bt_sm_io_capability_displayyesno);
      if (sc != SL_STATUS_OK) //checking for error status
        {
          LOG_ERROR(" sl_bt_sm_configure() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
        }

      sc = sl_bt_sm_delete_bondings();
      if (sc != SL_STATUS_OK) //checking for error status
        {
          LOG_ERROR(" sl_bt_sm_delete_bondings() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
        }

      break;
// --------------------------------------------------------
//This event indicates that a new connection was opened.
// --------------------------------------------------------
    case sl_bt_evt_connection_opened_id:
      LOG_INFO("Connected to Server\n\r");
      displayPrintf(DISPLAY_ROW_CONNECTION," ");
      displayPrintf(DISPLAY_ROW_CONNECTION,"Connected"); //Set LCD Display state to Connected
      displayPrintf( DISPLAY_ROW_BTADDR2,"%x:%x:%x:%x:%x:%x", //print Bt address
                     bt_addr_server.addr[0],
                     bt_addr_server.addr[1],
                     bt_addr_server.addr[2],
                     bt_addr_server.addr[3],
                     bt_addr_server.addr[4],
                     bt_addr_server.addr[5]);
      break;

// --------------------------------------------------------
//This event indicates that a connection was closed.
// --------------------------------------------------------
    case sl_bt_evt_connection_closed_id:
      displayPrintf( DISPLAY_ROW_BTADDR2," ");//clear LCD values
      displayPrintf(DISPLAY_ROW_CONNECTION," ");
      displayPrintf(DISPLAY_ROW_TEMPVALUE," ");
      displayPrintf(DISPLAY_ROW_PASSKEY," ");
      displayPrintf(DISPLAY_ROW_ACTION, " ");
      displayPrintf(DISPLAY_ROW_9," ");
      displayPrintf(DISPLAY_ROW_CONNECTION,"Discovering"); //Set LCD Display state to Discovering
      //start scanning again
      sc = sl_bt_scanner_start(sl_bt_gap_1m_phy, sl_bt_scanner_discover_generic);
      if (sc != SL_STATUS_OK) //checking for error status
        {
          LOG_ERROR("sl_bt_scanner_start() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
        }

      sc = sl_bt_sm_delete_bondings();
      if (sc != SL_STATUS_OK) //checking for error status
        {
          LOG_ERROR(" sl_bt_sm_delete_bondings() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
        }
      ble_data.connection_open = false; //connection disconnected
      ble_data.read_inflight =false;    //read inflight of characteristic
      break;
// --------------------------------------------------------
//This event triggered when connection parameters are changed
// --------------------------------------------------------
    case sl_bt_evt_connection_parameters_id:
      break;
// --------------------------------------------------------
//This event triggered when connection parameters are changed
// --------------------------------------------------------
    case sl_bt_evt_system_external_signal_id:
      ;
      uint32_t event = evt->data.evt_system_external_signal.extsignals; //get the event set in the Ext signal
      uint8_t value = !(GPIO_PinInGet(gpioPortF,6)); //get the value of PB0
      uint8_t value1 = !(GPIO_PinInGet(gpioPortF,7)); //get the value of PB1

      //-----------------------------------------------------------------------------------
      //Sequence for toggling enable button indication
      //-----------------------------------------------------------------------------------

      if(((event & EVTPB0)== EVTPB0) && (value ==0x01) && (confirm_action_client == false) )
        {
          PB0_pressed = true; //when we get PB0 value 0x01
        }
      else if(((event & EVTPB0)== EVTPB0) && (value ==0x00) && (confirm_action_client == false) )
        {
          PB0_pressed = false; //when we get PB0 value 0x00
        }
      if(((event & EVTPB1)== EVTPB1) && (PB0_pressed== true) && (value1 ==0x01) && (ble_data.read_inflight == false) && (PB1_pressed == false))
        {
          PB1_pressed =true; //when still PB0 is pressed and we get PB1 pressed event of value 0x01
        }
      if(((event & EVTPB1)== EVTPB1) && (PB1_pressed == true) && (value1 ==0x00) && (ble_data.read_inflight == false))
        {
          PB1_released = true; //if still PB0 is pressed and PB1 is pressed and released with value 0x00
          PB1_pressed = false;
        }
      if(PB1_released == true && PB0_pressed == false)
        {
          //if finally PB0 is released as the last event to toggle the gesture indication
          if(ble_data.indication_enable == true)
            {
              sc = sl_bt_gatt_set_characteristic_notification(ble_data.connection_handle,
                                                              ble_data.characteristic_handle_gest,
                                                              sl_bt_gatt_disable);
              ble_data.indication_enable =false;
            }
          else
            {
              sc = sl_bt_gatt_set_characteristic_notification(ble_data.connection_handle,
                                                              ble_data.characteristic_handle_gest,
                                                              sl_bt_gatt_indication);
              ble_data.indication_enable = true;
            }//if
              PB1_pressed  = false;// set to default
              PB1_released = false;
      }//PB1_released == true && PB0_pressed == false
      //-------------------------------------------------------------------------------------------


      if((event & EVTPB0) == EVTPB0 && confirm_action_client == true && value == 0x01 )
        { //Check for PB0 event and for yes/no bonding passkey confirmation
          sc = sl_bt_sm_passkey_confirm(ble_data.connection_handle,1);
          if (sc != SL_STATUS_OK)
            {
              LOG_ERROR("sl_bt_sm_passkey_confirm() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
            }
          confirm_action_client = false; //set the flag false
          } //Check for PB0 event and for yes/no bonding passkey confirmation

      if((event & EVTPB1)== EVTPB1 && (value1 ==1) && (ble_data.read_inflight == false) && (PB0_pressed ==false))
        {
          //sending for read characteristic value
          sc = sl_bt_gatt_read_characteristic_value (ble_data.connection_handle,ble_data.characteristic_handle_gest);
          if (sc != SL_STATUS_OK)
            {
              LOG_ERROR("sl_bt_gatt_read_characteristic_value() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
            }
          else
            ble_data.read_inflight = true;
        }//sending for read characteristic value

      break;

// --------------------------------------------------------
//This event indicates that a soft timer has expired.
// --------------------------------------------------------
    case sl_bt_evt_system_soft_timer_id:
      displayUpdate();
      break;

// ******************************************************
// Events for Only Client
// ******************************************************
//---------------------------------------------------------------------------------------
//Received for advertising or scan response packets generated by: sl_bt_scanner_start().
//----------------------------------------------------------------------------------------
    case sl_bt_evt_scanner_scan_report_id:
      //check for packet type as Connectable scannable undirected advertising
      //check for address type as public address
      if(evt->data.evt_scanner_scan_report.packet_type == 0 &&
         evt->data.evt_scanner_scan_report.address_type == 0)
        { //compare the bt address with the hardcoded server address
          if(memcmp(evt->data.evt_scanner_scan_report.address.addr, bt_addr_server.addr,6) ==0)
            {
              //stop scanning if the server address is found
              sc = sl_bt_scanner_stop();
              if (sc != SL_STATUS_OK) //checking for error status
                {
                  LOG_ERROR("sl_bt_scanner_stop() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
                }
              //open the connection with the server
              sc = sl_bt_connection_open(evt->data.evt_scanner_scan_report.address,
                                         evt->data.evt_scanner_scan_report.address_type,
                                         sl_bt_gap_phy_1m,
                                         NULL);
              if (sc != SL_STATUS_OK) //checking for error status
                {
                  LOG_ERROR("sl_bt_connection_open() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
                }
            }
        }
      break;
//----------------------------------------------------------------------------------
//We get this event when it’s ok to call the next GATT command.
//Check for error code returned by sl_bt_gatt_read_characteristic_value() command
//----------------------------------------------------------------------------------
    case sl_bt_evt_gatt_procedure_completed_id:

      //when read characteristic failed with error code = 0x110F (SL_STATUS_BT_ATT_INSUFFICIENT_ENCRYPTION,
      if(evt->data.evt_gatt_procedure_completed.result == 0x110F)
        {
          sl_bt_sm_increase_security(ble_data.connection_handle); //raising security level
          LOG_INFO("Bonding process initiated\n\r");
          ble_data.read_inflight = false;
        }

      break;
//--------------------------------------------------------------------------------
//We get this event when GATT service in the remote GATT database was discovered
//--------------------------------------------------------------------------------
    case sl_bt_evt_gatt_service_id:
      // Handle GATT service discovery events

      // Check if the discovered service UUID matches the Gesture Service UUID
      if(memcmp(evt->data.evt_gatt_service.uuid.data, GestureService, 16) == 0)
        {
          // Store the service handle for the Gesture Service
          ble_data.service_handle_gest = evt->data.evt_gatt_service.service;
        }
      else
        {
          // Log an error if the discovered service is not the expected Gesture Service
          LOG_ERROR("sl_bt_evt_gatt_service_id not Gesture service\n\r");
        }

      // Check if the discovered service UUID matches the PIR Service UUID
      if(memcmp(evt->data.evt_gatt_service.uuid.data, PIRService, 16) == 0)
        {
          // Store the service handle for the PIR Service
          ble_data.service_handle_pir = evt->data.evt_gatt_service.service;
        }
      else
        {
          // Log an error if the discovered service is not the expected PIR Service
          LOG_ERROR("sl_bt_evt_gatt_service_id not PIR service\n\r");
        }
      break;

//--------------------------------------------------------------------------------
//We get this event when GATT characteristic was discovered in the remote GATT database
//--------------------------------------------------------------------------------
    case sl_bt_evt_gatt_characteristic_id:
      // Handle GATT characteristic discovery events

      // Check if the discovered characteristic UUID matches the Gesture Characteristic UUID
      if(memcmp(evt->data.evt_gatt_characteristic.uuid.data, GestureChar, 16) == 0)
        {
          // Store the characteristic handle for the Gesture Characteristic
          ble_data.characteristic_handle_gest = evt->data.evt_gatt_characteristic.characteristic;
        }
      else
        {
          // Log an error if the discovered characteristic is not the expected Gesture Characteristic
          LOG_ERROR("sl_bt_evt_gatt_characteristic_id not Gesture characteristic\n\r");
        }

      // Check if the discovered characteristic UUID matches the PIR Characteristic UUID
      if(memcmp(evt->data.evt_gatt_characteristic.uuid.data, PIRChar, 16) == 0)
        {
          // Store the characteristic handle for the PIR Characteristic
          ble_data.characteristic_handle_pir = evt->data.evt_gatt_characteristic.characteristic;
        }
      else
        {
          // Log an error if the discovered characteristic is not the expected PIR Characteristic
          LOG_ERROR("sl_bt_evt_gatt_characteristic_id not PIR characteristic\n\r");
        }
      break;

//-------------------------------------------------------------------------------------------------------
//If an indication or notification has been enabled for a characteristic,
//this event is triggered whenever an indication or notification is received from the remote GATT server.
//-------------------------------------------------------------------------------------------------------
    case sl_bt_evt_gatt_characteristic_value_id:
      // Handle GATT characteristic value events

      // Check for a gesture value update
      if (evt->data.evt_gatt_characteristic_value.value.len >= 1 &&
          evt->data.evt_gatt_characteristic_value.connection == ble_data.connection_handle &&
          evt->data.evt_gatt_characteristic_value.att_opcode == sl_bt_gatt_handle_value_indication &&
          evt->data.evt_gatt_characteristic_value.characteristic == ble_data.characteristic_handle_gest)
        {
          // Retrieve the transmitted gesture value
          uint8_t char_value = evt->data.evt_gatt_characteristic_value.value.data[0];
          // Update the global gesture value and display it
          global_gesture_value = char_value;
          displayPrintf(DISPLAY_NUMBER_OF_ROWS-1, "PIR = %d Gesture = %d", global_pir_value, global_gesture_value);

          // Send confirmation for receiving the indication
          sc = sl_bt_gatt_send_characteristic_confirmation(ble_data.connection_handle);
          if (sc != SL_STATUS_OK)
            {
              LOG_ERROR("Error in sending characteristic confirmation: 0x%04x\n\r", (unsigned int) sc);
            }
        }

      // Check for a PIR value update
      if (evt->data.evt_gatt_characteristic_value.value.len >= 1 &&
          evt->data.evt_gatt_characteristic_value.connection == ble_data.connection_handle &&
          evt->data.evt_gatt_characteristic_value.att_opcode == sl_bt_gatt_handle_value_indication &&
          evt->data.evt_gatt_characteristic_value.characteristic == ble_data.characteristic_handle_pir)
        {
          // Retrieve the transmitted PIR value
          uint8_t char_value = evt->data.evt_gatt_characteristic_value.value.data[0];
          // Update the global PIR value and display it
          global_pir_value = char_value;
          displayPrintf(DISPLAY_NUMBER_OF_ROWS-1, "PIR = %d Gesture = %d", global_pir_value, global_gesture_value);

          // Send confirmation for receiving the indication
          sc = sl_bt_gatt_send_characteristic_confirmation(ble_data.connection_handle);
          if (sc != SL_STATUS_OK)
            {
              LOG_ERROR("Error in sending characteristic confirmation: 0x%04x\n\r", (unsigned int) sc);
            }
        }

      break;

 //-----------------------------------------------------------------------------
 //For Passkey Display so that user can authenticate the connection
//-----------------------------------------------------------------------------
    case sl_bt_evt_sm_confirm_passkey_id:
      LOG_INFO("Confirm passkey\n\r");//display passkey for confirmation
      uint32_t passkey= evt->data.evt_sm_passkey_display.passkey;
      displayPrintf(DISPLAY_ROW_PASSKEY,"Passkey %d",passkey);
      displayPrintf(DISPLAY_ROW_ACTION, "Confirm with PB0");
      confirm_action_client = true;
      break;
//--------------------------------------------------------------------
//Triggered after the pairing or bonding procedure is successfully
//--------------------------------------------------------------------
    case sl_bt_evt_sm_bonded_id:
      LOG_INFO("Bonded\n\r");
      displayPrintf(DISPLAY_ROW_PASSKEY," ");
      displayPrintf(DISPLAY_ROW_ACTION, " ");
      displayPrintf(DISPLAY_ROW_CONNECTION,"Bonded");
      ble_data.bonded = true;
      break;
//-------------------------------------------------------------------
//This event is triggered if the pairing or bonding procedure fails.
//-------------------------------------------------------------------
    case sl_bt_evt_sm_bonding_failed_id:
      LOG_INFO("Bonding Failed\n\r");
      sc = sl_bt_connection_close(evt->data.evt_sm_bonding_failed.connection);
      if (sc != SL_STATUS_OK)
        {
          LOG_ERROR("sl_bt_connection_close() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
        }
      break;

#endif
    default:
      break;

      } //switch
  } // handle_ble_event()



/***************************************************************************//**
 * @brief
 *  function that saves the gesture reading to GATT Server DB and send indication of
 *  the same to server if all conditions satisfied
 *
 * @param[in] uint8_t gesture_value : gesture value from gesture sensor
 *
 * @return [in] none
 ******************************************************************************/
void Bt_gesture(uint8_t gesture_value )
{
  global_gesture_value = gesture_value;
  sl_status_t sc; // status code

  // -------------------------------
  // Write our local GATT DB
  // -------------------------------
  sc = sl_bt_gatt_server_write_attribute_value(gattdb_gesture_value, // handle from gatt_db.h
                                               0,                              // offset
                                               1,                              // length
                                               &gesture_value                  // pointer to buffer where data is
                                               );
  if (sc != SL_STATUS_OK)
    {
      LOG_ERROR("sl_bt_gatt_server_write_attribute_value() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
    }

  //Check conditions for sending indication
  if ( (ble_data.connection_open == true) && (ble_data.ok_to_send_htm_indications_gest == true)){
    if( (ble_data.indication_in_flight_gest == false) && (ble_data.indication_in_flight_pir == false))
      {
        sc = sl_bt_gatt_server_send_indication(ble_data.connection_handle,
                                               gattdb_gesture_value, // handle from gatt_db.h
                                               1,                    //1 bytes
                                               &gesture_value );     //in IEEE-11073 format
        displayPrintf(DISPLAY_NUMBER_OF_ROWS-2," ");
        displayPrintf(DISPLAY_NUMBER_OF_ROWS-2, "PIR = %d Gesture = %d", global_pir_value, global_gesture_value);

        if (sc != SL_STATUS_OK)//check for error condition
          {
            LOG_ERROR("sl_bt_gatt_server_send_indication() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
          }
        else
          {
            ble_data.indication_in_flight_gest = true; //if send indication was successful set this flag true
          }
      }
    else
      {
        LOG_INFO("Indication in flight\n\r");
        int val = write_queue(gattdb_gesture_value,1,&gesture_value);
        if(val ==1)
          LOG_INFO("Buffer full\n\r");
      }
    } // if


}//Bt_gesture()



/***************************************************************************//**
 * @brief
 *  function that saves the PIR reading to GATT Server DB and send indication of
 *  the same to server if all conditions satisfied
 *
 * @param[in] uint8_t pir_value : PIR value from PIR Motion sensor
 *
 * @return [in] none
 ******************************************************************************/
void send_pir_value(uint8_t pir_value )
{

  sl_status_t sc; // status code
  global_pir_value = pir_value;

  // -------------------------------
  // Write our local GATT DB
  // -------------------------------
  sc = sl_bt_gatt_server_write_attribute_value(gattdb_motion_detected, // handle from gatt_db.h
                                               0,                      // offset
                                               1,                      // length
                                               &pir_value              // pointer to buffer where data is
                                               );
  if (sc != SL_STATUS_OK)
    {
      LOG_ERROR("sl_bt_gatt_server_write_attribute_value() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
    }

  //Check conditions for sending indication
  if ( (ble_data.connection_open == true) && (ble_data.ok_to_send_htm_indications_pir == true)){
    if( (ble_data.indication_in_flight_gest == false) && (ble_data.indication_in_flight_pir == false))
      {
        sc = sl_bt_gatt_server_send_indication(ble_data.connection_handle,
                                               gattdb_motion_detected, // handle from gatt_db.h
                                               1,                      //1 bytes
                                               &pir_value );           // in IEEE-11073 format
        displayPrintf(DISPLAY_NUMBER_OF_ROWS-2," ");
        displayPrintf(DISPLAY_NUMBER_OF_ROWS-2, "PIR = %d Gesture = %d", global_pir_value, global_gesture_value);
        if (sc != SL_STATUS_OK)//check for error condition
          {
            LOG_ERROR("sl_bt_gatt_server_send_indication() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
          }
        else
          {
            ble_data.indication_in_flight_pir = true; //if send indication was successful set this flag true
          }
      }
    else
      {
        LOG_INFO("Indication in flight\n\r");
        int val = write_queue(gattdb_motion_detected,1,&pir_value);
        if(val ==1)
          LOG_INFO("Buffer full\n\r");
      }
    } // if
}//send_pir_value()



#if DEVICE_IS_BLE_SERVER
/***************************************************************************//**
 * @brief
 *  Compute the next ptr value. Given a valid ptr value, compute the next valid
 *  value of the ptr and return it.
 *
 * @param[in] uint32_t ptr
 *
 * @return [in] uint32_t ptr
 ******************************************************************************/
static uint32_t nextPtr(uint32_t ptr)
{

  ptr = ((ptr + 1) % (QUEUE_DEPTH)); // moves pointer in a circular order wraps back to 0
  return ptr;

} // nextPtr()



/***************************************************************************//**
 * @brief
 * Writing to circular buffer
 *
 * @param[in] uint16_t a, size_t b,uint8_t* buffer : items to be added to struct
 *
 * @return [in] bool true/false for full condition
 ******************************************************************************/
bool write_queue(uint16_t a, size_t b,uint8_t* buffer)
{

  // full condition
  if (full_flag == true)
  {
    return true; // if full returns error status
  }

  // else save the data to the write pointer where the write pointer is
  queue_struct_t *temp = &(my_queue[wptr]);          // to get the address of array
  temp->charHandle = a;                              // store respective data
  temp->bufferLength = b;
  for(int i =0;i<(int)b;i++)
    {
      temp->buffer[i]    = *buffer;
      buffer++;
    }
  wptr = nextPtr(wptr); // move the head to next ptr

  // Handling full condition
  if (nextPtr(wptr) == rptr) // i.e, if write ptr +1 = read ptr means circular buffer is full
  {
    full_flag = true; // this will leave the last index of the array empty
  }
  return false; // return false if successful

} // write_queue()



/***************************************************************************//**
 * @brief
 * reading from circular buffer
 *
 * @param[in] uint16_t *a, size_t *b,uint8_t* buffer : items to be added to read
 *
 * @return [in] bool true/false for empty condition
 ******************************************************************************/
bool read_queue(uint16_t *a, size_t *b,uint8_t* buffer)
{

  // Handling Empty Condition
  if (rptr == wptr) // if write pointer and read pointer are same then the buffer is empty
  {
    return true; // if empty returns error status
  }

  queue_struct_t *temp = &(my_queue[rptr]); // getting array address
  *a = temp->charHandle;                             // store data
  *b = temp->bufferLength;
  for(int i=0;i<(int)temp->bufferLength;i++){
      *buffer = temp->buffer[i];
      buffer++;

  }

  if (full_flag == true) // clears the full flag once read occurs
    full_flag = false;

  rptr = nextPtr(rptr); // move to next pointer

  return false;

} // read_queue()



/***************************************************************************//**
 * @brief
 *  Function that computes the number of written entries currently in the queue
 *
 * @param[in]
 *
 * @return [in]  uint32_t entries : no of entries
 ******************************************************************************/
uint32_t get_queue_depth()
{


  uint32_t entries;

  if (rptr == wptr)
    return 0;

  // handling acccording to the position of wptr and rptr
  if (wptr > rptr)
    entries = wptr - rptr;
  else
    entries = QUEUE_DEPTH - (rptr - wptr);

  return entries;

} // get_queue_depth()
#endif







