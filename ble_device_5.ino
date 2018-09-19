#include <RFduinoBLE.h>
#include <stdio.h>
#include "_1_Serial.h"
#include "_3_GZLL.h"
#include "_7_StateMachine.h"


void setup()
{
  InitCMD();
  InitSerial();
  InitGZLL();
}


void loop()
{

 //Timeout(state);

// send data 1/sec
   if(millis()-SerialIdleTime > 1000){
    SendBuffer_();
    //SendCntr();
  }

  // ki lehet venni... vagy mégse?...
  if( (event >= 0) && (event < MAX_EVENTS) ){
    next_state = state_table[state][event]();
    state = next_state;
  }


}




