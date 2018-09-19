#ifndef SERIAL_FLAG
#define SERIAL_FLAG

/****************************************************************************** 
  ------------------------------- Includes -------------------------------
******************************************************************************/

#include <Arduino.h>
#include <RFduinoBLE.h>
#include "_3_GZLL.h"
#include "_5_EnergyManagement.h"
#include "_90_Commands.h"
#include "_7_StateMachine.h"

/****************************************************************************** 
  ------------------------------- Constants -------------------------------
******************************************************************************/

#define RX_PIN        2 //0 for serial monitor // 2 communicate with gecco
#define TX_PIN        3 //1 // 3 
#define WAKE_PIN      4
#define LED1          5


#define BAUD          9600

#define BUFFER_SIZE   200

#define DOUBLE_ID         'f'
#define INT_ID            'd'
#define STRING_ID         's'  
#define ERROR_CHARACTER   'X'


extern unsigned long SerialIdleTime;

#define   SERIAL_IDLE_TIMEOUT   100 //ms
#define   LED_BLINK_TIME   100 //ms


/****************************************************************************** 
  ------------------------------- Structures -------------------------------
******************************************************************************/

typedef struct BufferStruct{
  char data[BUFFER_SIZE];
  int head;
  
}buffer_struct;


/****************************************************************************** 
  ------------------------------- Functions -------------------------------
******************************************************************************/

void InitSerial();
void serialEvent(void);
void BlinkLed(unsigned ms);
void SendBuffer();
void SendBuffer_();
void SendCntr();

#endif
