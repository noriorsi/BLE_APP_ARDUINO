#ifndef GZLL_FLAG
#define GZLL_FLAG


/****************************************************************************** 
  ------------------------------- Includes -------------------------------
******************************************************************************/


#include <RFduinoBLE.h>
#include <Arduino.h>
#include "_1_Serial.h"
#include "_5_EnergyManagement.h"
#include "_7_StateMachine.h"
#include "_90_Commands.h"


/****************************************************************************** 
  ------------------------------- Globals -------------------------------
******************************************************************************/

#define DEVICE_ROLE DEVICE0

//The time needed between receiving 2 commands from HOST
#define PING_INTERVAL       2000 //[ms]
#define DEBUG_PING_INTERVAL 500 //[ms]

#define LED2                6
#define TEST                0 // if >0 send feedback information trough BLE


/****************************************************************************** 
  ------------------------------- Functions -------------------------------
******************************************************************************/

void InitGZLL();
// void InitBLE();
void RFduinoBLE_onReceive( char *data, int len);
//void Ping();
void FlashLed(unsigned int n);

#endif
