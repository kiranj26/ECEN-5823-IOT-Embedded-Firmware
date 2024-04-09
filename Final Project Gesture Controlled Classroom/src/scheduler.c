/***********************************************************************
* @file        scheduler.c
* @version     0.0.1
* @brief       Function implementation file.
*
* @Creator/Author/Editor :   
*               Ayswariya Kannan, ayka9480@Colorado.edu
*               Kiran Jojare, kijo7257@colorado.edu
*
* @institution University of Colorado Boulder (UCB)
* @course      ECEN 5823-001: IoT Embedded Firmware (Fall 2023)
* @instructor  David Sluiter
*
* @assignment  ecen5823-course-project-kiranj26
* @due         Dec 14, 2023
*
* @resources   Assignment-9
*
* @copyright   All rights reserved. Distribution allowed only for the
*              use of assignment grading. Use of code excerpts allowed at the
*              discretion of author. Contact for permission.
*/
#include "sl_power_manager.h"
#include "scheduler.h"
#include "em_core.h"
#include "i2c.h"
#include "timer.h"
#include "gpio.h"
#include "sl_bt_api.h"
#include "ble.h"
#include "ble_device_type.h"
#include "lcd.h"
#include "src/SparkFun_APDS9960.h"
#include "irq.h"
#include "src/powerpoint.h"

#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"

//*****Global Variables******************//
uint32_t count;     //counter for gesture state machine
uint32_t flag=0;    //flag for initializing timer to power OFF PIR On_Flag=0; //Set it 1/0 based on Motion detection
uint32_t flag_2=0;
uint32_t pir_off_for_long_count = 0;
uint32_t turn_client_off_command = 8;
// Gesture Sensor Vlaues
#define GESTURE_LEFT  1
#define GESTURE_RIGHT 2
#define GESTURE_UP    3
#define GESTURE_DOWN  4

// PIR Sensor UID
static const uint8_t PIRService[16] = {0xc3,0x89,0x17,0xd7,0x00,0x2b,0x3b,0xbb,0xd7,0x43,0x78,0xf5,0xb5,0x22,0x70,0xec};
// Encryption Test Service UUID defined by user in server configuration
static const uint8_t GestureService[16] = {0x2c,0xb7,0x3c,0x1c,0xc1,0x12,0xab,0xad,0x39,0x41,0x43,0xb2,0xaf,0x53,0xbe,0x88};
// Gesture Sensor UUID
static const uint8_t PIRChar[16] = {0x26,0xb9,0x3a,0x7e,0x1a,0x01,0x2d,0xbb,0xe3,0x4d,0x49,0x45,0xc1,0x10,0x9f,0x38};
//Button State characteristic UUID defined by user in server configuration
static const uint8_t GestureChar[16] = {0xb4,0xd5,0xa6,0xdd,0x64,0xd0,0x4a,0xb5,0xa3,0x42,0xed,0x10,0x57,0x7a,0x5b,0x36};

static int previous_gest_value = -1; // Initialize to an invalid value

//Enum for various states in Discovery State machine
typedef enum{
  state1_Idle,
  state2_Discover_Service,
  state3_Discover_Characteristic,
  state4_Indication,
  state5_Discover_PIRService,
  state6_Discover_Characteristic_PIR,
  state7_Indication_PIR,
  state8_InitialiseLCD,
  state9_UpdateSlides,
}client_state;

//Enum for PIR & gesture state machine events
typedef enum {
  evtLETIMER0_UF = 1,
  evtLETIMER0_COMP1 = 2,
  evtLETIMER0_I2C =4,
  evtPB0 = 8,
  evtPB1 = 16,
  evtPIR =32,
  evtGesture =64
} events;

 //Enum for various states in PIR state machine
typedef enum uint32_t {
State1Idle,
State2_PIR_On,
State3_Gesture_Idle,
} State_t;

//Enum for various states in Gesture control state machine
typedef enum {
  Gesture_Idle,
  Gesture_Enable
}gesture;



/***************************************************************************//**
 * @brief
 *   Scheduler for setting event for UF according to their occurrence using
 *
 * @param[in] none
 *
 * @return [in] none
 ******************************************************************************/
void schedulerSetEventUF() {

  CORE_DECLARE_IRQ_STATE; //for Critical Section handling

  CORE_ENTER_CRITICAL(); // enter critical, turn off interrupts in NVIC
  sl_bt_external_signal(evtLETIMER0_UF);
  CORE_EXIT_CRITICAL(); // exit critical, re-enable interrupts in NVIC

} // schedulerSetEventUF()



/***************************************************************************//**
 * @brief
 *   Scheduler for setting event for COMP1 according to their occurrence using sl_bt_external_signal()
 *
 * @param[in] none
 *
 * @return [in] none
 ******************************************************************************/
void schedulerSetEventCOMP1() {

  CORE_DECLARE_IRQ_STATE; //for Critical Section handling

  CORE_ENTER_CRITICAL(); // enter critical, turn off interrupts in NVIC
  sl_bt_external_signal(evtLETIMER0_COMP1);
  CORE_EXIT_CRITICAL(); // exit critical, re-enable interrupts in NVIC

} // schedulerSetEventCOMP1()



/***************************************************************************//**
 * @brief
 *   Scheduler for setting event for I2C according to their occurrence using sl_bt_external_signal()
 * @param[in] none
 *
 * @return [in] none
 ******************************************************************************/
void schedulerSetEventI2C() {

  CORE_DECLARE_IRQ_STATE; //for Critical Section handling

  CORE_ENTER_CRITICAL(); // enter critical, turn off interrupts in NVIC
  sl_bt_external_signal(evtLETIMER0_I2C);
  CORE_EXIT_CRITICAL(); // exit critical, re-enable interrupts in NVIC

} // schedulerSetEventI2C()



/***************************************************************************//**
 * @brief
 *   Scheduler for setting event for PIR
 * @param[in] none
 *
 * @return [in] none
 ******************************************************************************/
void schedulerSetEventPIR(){

  CORE_DECLARE_IRQ_STATE; //for Critical Section handling
  CORE_ENTER_CRITICAL(); // enter critical, turn off interrupts in NVIC
  sl_bt_external_signal(evtPIR);
  CORE_EXIT_CRITICAL(); // exit critical, re-enable interrupts in NVIC

} // schedulerSetEventPIR()



/***************************************************************************//**
 * @brief
 *   Scheduler for setting event for Gesture
 * @param[in] none
 *
 * @return [in] none
 ******************************************************************************/
void schedulerSetEventGesture(){

  CORE_DECLARE_IRQ_STATE; //for Critical Section handling
  CORE_ENTER_CRITICAL(); // enter critical, turn off interrupts in NVIC
  sl_bt_external_signal(evtGesture);
  CORE_EXIT_CRITICAL(); // exit critical, re-enable interrupts in NVIC

  } // schedulerSetEventGesture()



/***************************************************************************//**
 * @brief
 *   Scheduler for setting event for PB0
 * @param[in] none
 *
 * @return [in] none
 ******************************************************************************/
void schedulerSetEventPB0(){

  CORE_DECLARE_IRQ_STATE; //for Critical Section handling
  CORE_ENTER_CRITICAL(); // enter critical, turn off interrupts in NVIC
  sl_bt_external_signal(evtPB0);
  CORE_EXIT_CRITICAL(); // exit critical, re-enable interrupts in NVIC

  } // schedulerSetEventPB0()



/***************************************************************************//**
 * @brief
 *   Scheduler for setting event for PB1
 * @param[in] none
 *
 * @return [in] none
 ******************************************************************************/
void schedulerSetEventPB1(){

  CORE_DECLARE_IRQ_STATE; //for Critical Section handling
  CORE_ENTER_CRITICAL(); // enter critical, turn off interrupts in NVIC
  sl_bt_external_signal(evtPB1);
  CORE_EXIT_CRITICAL(); // exit critical, re-enable interrupts in NVIC

  } // schedulerSetEventPB1()



/***************************************************************************//**
 * @brief
 *    PIR state machine to implement the sequence of events for PIR sensor
 *
 * @param[in] sl_bt_msg_t *evt : evt from the BT Stack
 *
 * @return [in] none
 ******************************************************************************/
void PIR_state_machine(sl_bt_msg_t *evt){

  ble_data_struct_t* ble_data;
  ble_data = getBleDataPtr(); //get the BLE Private data
  if(SL_BT_MSG_ID(evt->header) != sl_bt_evt_system_external_signal_id) //check for external signal event ID
    {
        return;
    }
  //Proceed with state machine only if the connection open and indications have been enabled
 if(ble_data->connection_open == true && ble_data->ok_to_send_htm_indications_pir == true)
    {
      uint32_t event = evt->data.evt_system_external_signal.extsignals; //get the event set in the Ext signal
      State_t currentState;                                             //current State
      static State_t nextState = State1Idle;                            //Initialize the state to IDLE state
      currentState = nextState;

      switch (currentState)
      {
        case State1Idle:
          nextState =State1Idle; // default
          if ((event & evtLETIMER0_UF) == evtLETIMER0_UF)// UnderFlow Event
            {
             //NVIC_DisableIRQ(GPIO_EVEN_IRQn); //Disable Gpio Even IRQ to prevent false triggering during startup mode
              if(flag_2 ==0){
              gpioPIROn();                    //Initialize sensor
              //flag_2 =1;
              LOG_INFO("PIR_on\n\r");
              }
              timerWaitUs_irq(7500000);       //wait for it to power up

              nextState = State2_PIR_On;
            }
          break;
        case State2_PIR_On:
          nextState = State2_PIR_On; // default
          if ((event & evtLETIMER0_COMP1) == evtLETIMER0_COMP1) //COMP1 timer interrupt
            {
             // NVIC_EnableIRQ(GPIO_EVEN_IRQn);                   //Enable the Gpio Even IRQ for PIR interrupt to occur
              nextState =State3_Gesture_Idle;
            }
          break;
        case State3_Gesture_Idle:
          nextState = State3_Gesture_Idle; // default
          if (flag == 0)
            {
              timerWaitUs_irq(4000000);    // wait for it to power up
              flag =1;                     // Keep track of entry into the state
            }

          if ((event &  evtPIR) == evtPIR)
            { //when motion detected
              pir_off_for_long_count = 0; // reset if motion detected
              LOG_INFO("Motion detected\n\r");
              ble_data->On_flag=1 ;                  //Set On_Flag to advance gesture state machine
              send_pir_value(1);            //sending PIR as an indication for GATT service
              gpioPIROff();
              flag_2 =1;
              flag=0;
              // send_pir_value(0);            //send the off value
              nextState = State1Idle;
            }
          //If no motion detected and the 5 seconds expires turn PIR off
          else if((event & evtLETIMER0_COMP1) == evtLETIMER0_COMP1)
            {
              pir_off_for_long_count++;
              gpioPIROff();
              send_pir_value(0);
              nextState = State1Idle;

              // if the PIR was off for more than expected time send a comman to cleint to tuern off the presentation
              if (pir_off_for_long_count > 3){
                  send_pir_value(turn_client_off_command);
                  pir_off_for_long_count = 0;
              }
              flag=0;
            }
          break;
        default:
          break;
      } // switch
    }//if
 }//PIR_state_machine()



/*********************************************************************************//**
 * @brief
 *    gesture state machine to implement the sequence of events for gesture sensor
 *
 * @param[in] sl_bt_msg_t *evt : evt from the BT Stack
 *
 * @return [in] none
 ************************************************************************************/
void gesture_state_machine(sl_bt_msg_t *evt)
{
  ble_data_struct_t* ble_data;
  ble_data = getBleDataPtr(); //get the BLE Private data
  if(SL_BT_MSG_ID(evt->header) != sl_bt_evt_system_external_signal_id) //check for external signal event ID
     {
         return;
     }
   //Proceed with state machine only if the connection open and indications have been enabled
  if(ble_data->connection_open == true && ble_data->ok_to_send_htm_indications_gest == true)
     {
       uint32_t event = evt->data.evt_system_external_signal.extsignals; //get the event set in the Ext signal
       gesture currentState;                                             //current State
       static gesture nextState = Gesture_Idle;                          //Initialize the state to IDLE state
       currentState = nextState;

       switch (currentState)
       {
         case Gesture_Idle:
           nextState = Gesture_Idle;          // default
          // Once motion is detected, gesture sensor is enabled
          if((ble_data->On_flag ==1))
              {
              enableGestureSensor(true);      //enable the gesture sensor
              LOG_INFO("Gesture Enabled\n\r");
              nextState = Gesture_Enable;
              }
          break;
         case Gesture_Enable:
           nextState = Gesture_Enable; // default
           count++;                    //Counter to disable gesture sensor
           //Check for gesture event from external signals
           if( (event & evtGesture) == evtGesture)
             {
               //Disable the gesture GPIO Interrupt till we read the gesture data
                NVIC_DisableIRQ(GPIO_ODD_IRQn);

                //get the value corresponding to each gestures from 1 to 6
                uint8_t gesture_value = readGesture();
                if(gesture_value > 0)
                  {
                    LOG_INFO("Gesture = %d\r\n", gesture_value);
                    //send the gesture value as indication for GATT service
                    Bt_gesture(gesture_value);
                  }
                //wait for sometime to process the data
                timerWaitUs_polled(100000);
                NVIC_EnableIRQ(GPIO_ODD_IRQn);
             }//if evtGesture
           //If no gestures detected, we will disable gesture sensor once counter reaches threshold value
           else if(count>100)
             {
               disableGestureSensor();
               count=0;              //start counter again
               ble_data->On_flag =0;  //start Motion detection from PIR
               // gpioPIROff();
               flag_2 = 0;
               nextState = Gesture_Idle;
           } //else if
           break;
         default:
           // LOG_INFO("Gesture Default\n\r");
           break;
       }//switch
     }//if
  }// gesture_state_machine()



/***************************************************************************//**
 * @brief
 *   Discovery state machine to implement the discovery-related client events
 *   in the Bluetooth stack. This function manages the various states involved in
 *   discovering and setting up services and characteristics from the Bluetooth server.
 *
 * @param[in] evt
 *   Event from the Bluetooth stack.
 *
 * @return [in] none
 ******************************************************************************/

void discovery_state_machine(sl_bt_msg_t *evt){

  sl_status_t sc; // Status code
  ble_data_struct_t* ble_data = getBleDataPtr(); // Get the BLE private data

  uint32_t evt_id = SL_BT_MSG_ID(evt->header);
  client_state currentState; // Current state of the state machine
  static client_state nextState = state1_Idle; // Initialize the state to IDLE
  currentState = nextState;

  switch (currentState)
  {
    case state1_Idle:
      // Handle Idle state
      nextState = state1_Idle;
      if (evt_id == sl_bt_evt_connection_opened_id) {
        ble_data->connection_open = true;
        // Start discovering primary services by UUID
        sc = sl_bt_gatt_discover_primary_services_by_uuid(
          evt->data.evt_connection_opened.connection,
          sizeof(GestureService),
          (const uint8_t*)GestureService
        );
        ble_data->connection_handle = evt->data.evt_connection_opened.connection;
        if (sc != SL_STATUS_OK) {
          LOG_ERROR("Discover primary services error: 0x%04x\n\r", (unsigned int)sc);
        }
        nextState = state2_Discover_Service;
      }
      if (!ble_data->connection_open) nextState = state1_Idle;
      break;

    // State 2: Discover the Gesture Service
    case state2_Discover_Service:
      nextState = state2_Discover_Service;

      // Check if the GATT procedure is completed to send the next command
      if (evt_id == sl_bt_evt_gatt_procedure_completed_id) {
        // Start discovering characteristics for the Gesture Service
        sc = sl_bt_gatt_discover_characteristics_by_uuid(
          ble_data->connection_handle,
          ble_data->service_handle_gest,
          sizeof(GestureChar),
          (const uint8_t*)GestureChar
        );

        // Check for errors in discovering characteristics
        if (sc != SL_STATUS_OK) {
          LOG_ERROR("Discover characteristics error: 0x%04x\n\r", (unsigned int)sc);
        }

        // Advance to the next state to discover characteristics
        nextState = state3_Discover_Characteristic;
      }

      // If the connection is closed, return to the Idle state
      if (!ble_data->connection_open) {
        nextState = state1_Idle;
      }
      break;


    // State 3: Discover Characteristics of the Gesture Service
    case state3_Discover_Characteristic:
      nextState = state3_Discover_Characteristic;

      // Check if the GATT procedure is completed to send the next command
      if (evt_id == sl_bt_evt_gatt_procedure_completed_id) {
        // Enable indication from the server for the Gesture characteristic
        sc = sl_bt_gatt_set_characteristic_notification(
          ble_data->connection_handle,
          ble_data->characteristic_handle_gest,
          sl_bt_gatt_indication
        );

        // Check for errors in setting the characteristic notification
        if (sc != SL_STATUS_OK) {
          LOG_ERROR("Setting characteristic notification error: 0x%04x\n\r", (unsigned int)sc);
        }

        // Advance to the next state to handle indications
        nextState = state4_Indication;
      }

      // If the connection is closed, return to the Idle state
      if (!ble_data->connection_open) {
        nextState = state1_Idle;
      }
      break;


    // State 4: Handle Indications for the Gesture Characteristic
    case state4_Indication:
      nextState = state4_Indication;

      // Check if the GATT procedure is completed to send the next command
      if (evt_id == sl_bt_evt_gatt_procedure_completed_id) {
        // Update the display to indicate that the device is handling indications
        displayPrintf(DISPLAY_ROW_CONNECTION, " ");
        displayPrintf(DISPLAY_ROW_CONNECTION, "Handling Indication");

        // Start discovering the PIR (Passive Infrared Sensor) Service by its UUID
        sc = sl_bt_gatt_discover_primary_services_by_uuid(
          ble_data->connection_handle,
          sizeof(PIRService),
          (const uint8_t*)PIRService
        );

        // Check for errors in discovering the PIR Service
        if (sc != SL_STATUS_OK) {
          LOG_ERROR("Discover PIR Service error: 0x%04x\n\r", (unsigned int)sc);
        }

        // Advance to the next state to discover the PIR Service
        nextState = state5_Discover_PIRService;
      }

      // If the connection is closed, return to the Idle state
      if (!ble_data->connection_open) {
        nextState = state1_Idle;
      }
      break;


      // State 5: Discover the PIR (Passive Infrared Sensor) Service
      case state5_Discover_PIRService:
        nextState = state5_Discover_PIRService;

        // Check if the GATT procedure is completed to send the next command
        if (evt_id == sl_bt_evt_gatt_procedure_completed_id) {
          // Start discovering characteristics for the PIR Service
          sc = sl_bt_gatt_discover_characteristics_by_uuid(
            ble_data->connection_handle,
            ble_data->service_handle_pir,
            sizeof(PIRChar),
            (const uint8_t*)PIRChar
          );

          // Check for errors in discovering characteristics for the PIR Service
          if (sc != SL_STATUS_OK) {
            LOG_ERROR("Discover PIR characteristics error: 0x%04x\n\r", (unsigned int)sc);
          }

          // Advance to the next state to discover characteristics for the PIR Service
          nextState = state6_Discover_Characteristic_PIR;
        }

        // If the connection is closed, return to the Idle state
        if (!ble_data->connection_open) {
          nextState = state1_Idle;
        }
        break;


      // State 6: Discover Characteristics of the PIR Service
      case state6_Discover_Characteristic_PIR:
        nextState = state6_Discover_Characteristic_PIR;

        // Check if the GATT procedure is completed to send the next command
        if (evt_id == sl_bt_evt_gatt_procedure_completed_id) {
          // Enable indication from the server for the PIR characteristic
          sc = sl_bt_gatt_set_characteristic_notification(
            ble_data->connection_handle,
            ble_data->characteristic_handle_pir,
            sl_bt_gatt_indication
          );

          // Check for errors in setting the characteristic notification
          if (sc != SL_STATUS_OK) {
            LOG_ERROR("Setting PIR characteristic notification error: 0x%04x\n\r", (unsigned int)sc);
          } else {
            ble_data->indication_enable = true; // Mark indication as enabled for tracking
          }

          // Advance to the next state to handle PIR indications
          nextState = state7_Indication_PIR;
        }

        // If the connection is closed, return to the Idle state
        if (!ble_data->connection_open) {
          nextState = state1_Idle;
        }
        break;

      // State 7: Handle Indications for the PIR Characteristic
      case state7_Indication_PIR:
        nextState = state7_Indication_PIR;

        // Check if the GATT procedure is completed to send the next command
        if (evt_id == sl_bt_evt_gatt_procedure_completed_id) {
          // Transition to the state to initialize the LCD for handling indications
          nextState = state8_InitialiseLCD;
        }

        // If the connection is closed, return to the Idle state
        if (!ble_data->connection_open) {
          nextState = state1_Idle;
        }
        break;


      // State 9: Initialize LCD Display
      case state8_InitialiseLCD:
        // Check if the device is bonded
        if (ble_data->bonded == true) {
          // Display the current slide on the LCD
          displayCurrentSlide();
          // Transition to the state for updating slides based on gestures
          nextState = state9_UpdateSlides;
        }
         // If the connection is closed, return to the Idle state
      if (!ble_data->connection_open) {
        nextState = state1_Idle;
      }
      break;

    // State 10: Update Slides based on Gesture Values
    case state9_UpdateSlides:
    {
      int gest_val = getGestureValue();  // Retrieve the current gesture value
      int pir_value = getPIRValue();     // Retrieve the current PIR sensor value

      // Check if no motion is detected for a long time
      if (pir_value == 8) {
        clearDisplay();
        displayPrintf(10, "No Motion Detected");
        displayPrintf(11, "For a Long Time");
        displayPrintf(12, "Display OFF");
        break;
      }

      // Check if the gesture value has changed
      if (previous_gest_value != gest_val) {
        switch (gest_val) {
          case GESTURE_LEFT:
            nextPage();
            displayCurrentSlide();
            break;

          case GESTURE_RIGHT:
            prevPage();
            displayCurrentSlide();
            break;

          case GESTURE_UP:
            scroll(-1);
            displayCurrentSlide();
            break;

          case GESTURE_DOWN:
            scroll(1);
            displayCurrentSlide();
            break;
        }
        previous_gest_value = gest_val; // Update the previous gesture value
      }
    }
    // If the connection is closed, return to the Idle state
    if (!ble_data->connection_open) {
        nextState = state1_Idle;
    }
    break;

    default :
      break;

  }//switch


}//discovery_state_machine()

