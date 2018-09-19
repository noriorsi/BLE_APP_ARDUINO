#include "_7_StateMachine.h"

/****************************************************************************** 
  ------------------------------- Variables -------------------------------
******************************************************************************/

state_enum state = IDLE_STATE;
state_enum next_state;
event_enum event = NO_EVENT;

unsigned DEBUG_active = false;
unsigned DEBUG_receive = false;
unsigned no_stop_when_timeout = false;

unsigned long current_state_time = 0;

//A 2 dimensional array of function pointers. The functions return state_enum type and have no parameters.
state_enum (*state_table[MAX_STATES][MAX_EVENTS])(void)={
  
    //NO_EVENT             // START_EVENT            // STOP_EVENT             // SLEEP_EVENT         //SERIAL_TIMEOUT_EVENT           //WAKE_UP_EVENT          //STARTM1_EVENT           //STARTM2_EVENT            //SLEEP_DEBUG_EVENT         //PARAMS_EVENT        //TIMEOUT_EVENT           //DEBUG_EVENT
        
  { No_Event_Handler    ,    Start_Event_Handler  ,   Error_Event_Handler  ,   Sleep_Event_Handler  , Error_Event_Handler           ,  Wake_Up_Event_Handler  ,  StartM1_Event_Handler  , StartM2_Event_Handler   ,   Sleep_Debug_Event_Handler , Error_Event_Handler  , Error_Event_Handler    , Debug_Event_Handler   },   //IDLE_STATE
  { No_Event_Handler    ,    Stop_Event_Handler   ,   Stop_Event_Handler   ,   Sleep_Event_Handler  , Serial_Timeout_Event_Handler  ,  Wake_Up_Event_Handler  ,  Stop_Event_Handler     , Stop_Event_Handler      ,   Error_Event_Handler       , Error_Event_Handler  , Error_Event_Handler    , Error_Event_Handler     },   //RECEIVE_N_SEND_STATE
  { Error_Event_Handler  ,   Error_Event_Handler  ,   Error_Event_Handler  ,   Error_Event_Handler  , Error_Event_Handler           ,  Error_Event_Handler    ,  Error_Event_Handler    , Error_Event_Handler     ,   Error_Event_Handler       , Error_Event_Handler  , Error_Event_Handler    , Error_Event_Handler    },    //SHUTOFF_STATE
  { No_Event_Handler    ,   Error_Event_Handler  ,   Error_Event_Handler  ,   Error_Event_Handler  , Error_Event_Handler           ,  Error_Event_Handler    ,  Error_Event_Handler     , Error_Event_Handler     ,   Error_Event_Handler       , Params_Event_Handler , Timeout_Event_Handler  , Error_Event_Handler   },    //WAITING_FOR_PARAMS_STATE
  { No_Event_Handler    ,    Start_Event_Handler  ,   Stop_Event_Handler   ,   Sleep_Event_Handler  , Error_Event_Handler           ,  Wake_Up_Event_Handler  ,  StartM1_Event_Handler  , StartM2_Event_Handler   ,   Sleep_Debug_Event_Handler , Params_Event_Handler  , Error_Event_Handler   , EndDebug_Event_Handler   }    //DEBUG_STATE

};



/****************************************************************************** 
  ------------------------------- Functions -------------------------------
******************************************************************************/


/*************************************************** 
When there is no event
***************************************************/
state_enum No_Event_Handler(void){
  //if(TEST) {RFduinoBLE.sendInt(80);delay(DELAY_BEFORE_SENDING_COMMAND);}

  //char test = Serial.read();
  //RFduinoBLE.sendInt(int(test));
  
  switch(state){
  
    case IDLE_STATE:{
     // Ping();
      return IDLE_STATE;
      break;
    }

    case RECEIVE_N_SEND_STATE:{
      return RECEIVE_N_SEND_STATE;
      break;
    }

    case DEBUG_STATE:{/*
      if(!DEBUG_receive) Ping();
      return DEBUG_STATE;
      break;*/
    }

    case WAITING_FOR_PARAMS_STATE:{
     // Ping();
      return WAITING_FOR_PARAMS_STATE;
      break;
    }

    default: return IDLE_STATE;
    
  }

}


/*************************************************** 
This is what happens when we receive a start command
***************************************************/
state_enum Start_Event_Handler(void){
  if(state == DEBUG_STATE){
    //RFduinoBLE.sendInt(80);
    event = NO_EVENT;
    return DEBUG_STATE;
  }
  RFduinoBLE.sendInt(81);
  current_state_time = millis();
  event = NO_EVENT;
  return WAITING_FOR_PARAMS_STATE;
}




/*************************************************** 
This is what happens when we receive a stop command
***************************************************/
state_enum Stop_Event_Handler(void){
  //RFduinoGZLL.sendToHost(CMD[CMD_ACK]);
  if(TEST) {RFduinoBLE.sendInt(82);delay(DELAY_BEFORE_SENDING_COMMAND);}
  digitalWrite(LED2, LOW);
  Serial.write(CMD[CMD_STOP]);
  no_stop_when_timeout = false;
  if(event==STOP_EVENT) event = NO_EVENT;
  if(state==DEBUG_STATE){event = NO_EVENT; DEBUG_receive = false; return DEBUG_STATE; }
  return IDLE_STATE;
}





/*************************************************** 
Handling errors
***************************************************/
state_enum Error_Event_Handler(void){
  if(TEST) {RFduinoBLE.sendInt(83);delay(DELAY_BEFORE_SENDING_COMMAND);}
  DEBUG_active = false;
  DEBUG_receive = false;
  no_stop_when_timeout = false;
  event = NO_EVENT;
  return Stop_Event_Handler();
}


/*************************************************** 
Handles the sleep command
***************************************************/
state_enum Sleep_Event_Handler(void){
  if(TEST) {RFduinoBLE.sendInt(84);delay(DELAY_BEFORE_SENDING_COMMAND);}
  DEBUG_active = false;
  DEBUG_receive = false;
  SystemOff();
  return SHUTOFF_STATE;
}


/*************************************************** 
Handles the serial timeout
***************************************************/
state_enum Serial_Timeout_Event_Handler(void){
  if(TEST) {RFduinoBLE.sendInt(85);delay(DELAY_BEFORE_SENDING_COMMAND);}
  if(event==SERIAL_TIMEOUT_EVENT) event = STOP_EVENT;

  if(no_stop_when_timeout){//If no_stop_when_timeout don't send a stop command
    event = NO_EVENT;
    //return RECEIVE_N_SEND_STATE;
  }
  return IDLE_STATE;
}



/*************************************************** 
Checks if a state has reached its time limit
***************************************************/
void Timeout(state_enum state){
  switch(state){
    
    case RECEIVE_N_SEND_STATE:{
      delay(5000);              // changed: from 1000 to 5000
      if(millis()-SerialIdleTime > SERIAL_IDLE_TIMEOUT) event = SERIAL_TIMEOUT_EVENT;
      break;
    }
    case WAITING_FOR_PARAMS_STATE:{
      if( millis() - current_state_time >= WAITING_FOR_PARAMS_STATE_TIMEOUT) event = TIMEOUT_EVENT;
      break;
    }
    default: break;
    
  }

}


/*************************************************** 
Handles waking up the device by the wake up pin interrupt
***************************************************/
state_enum Wake_Up_Event_Handler(void){
  
  //NRF_TWI1->ENABLE = (TWI_ENABLE_ENABLE_Enabled << TWI_ENABLE_ENABLE_Pos);
  //NRF_UART0->ENABLE = (UART_ENABLE_ENABLE_Enabled << UART_ENABLE_ENABLE_Pos);
  if(TEST) {RFduinoBLE.sendInt(86);delay(DELAY_BEFORE_SENDING_COMMAND); }
  if(event == WAKE_UP_EVENT) event = MAX_EVENTS;
  if(state==DEBUG_STATE){ event = NO_EVENT; return DEBUG_STATE;}
  return RECEIVE_N_SEND_STATE;
}


/*************************************************** 
Starts Mode1
***************************************************/
state_enum StartM1_Event_Handler(void){
  //if(state == DEBUG_STATE) RFduinoGZLL.sendToHost("DEVICE: Start mode1 event\n\r");
  //RFduinoGZLL.sendToHost(CMD[CMD_ACK]);
  delay(DELAY_BEFORE_SENDING_COMMAND);
  if(TEST) {RFduinoBLE.sendInt(87);delay(DELAY_BEFORE_SENDING_COMMAND);}
  digitalWrite(LED2, HIGH);
  Serial.write(CMD[CMD_STARTM1]);
  SerialIdleTime = millis();
  if(event==STARTM1_EVENT) event = MAX_EVENTS;
  if(state==DEBUG_STATE) {event = NO_EVENT; DEBUG_receive = true; return DEBUG_STATE;}
  return RECEIVE_N_SEND_STATE;
}

/*************************************************** 
Starts Mode2
***************************************************/
state_enum StartM2_Event_Handler(void){
  no_stop_when_timeout = true;
 // RFduinoBLE.send(CMD[CMD_ACK]);
  if(TEST) {RFduinoBLE.sendInt(88);delay(DELAY_BEFORE_SENDING_COMMAND);}
  digitalWrite(LED2, HIGH);
  for(int i = 0; i<100; ++i){
    Serial.write(CMD[CMD_STARTM2]);
    Serial.write("$222222$");
    
  }
  //digitalWrite(LED1, HIGH);
  SerialIdleTime = millis();


  if(event==STARTM2_EVENT) event = MAX_EVENTS;
  if(state==DEBUG_STATE) {event = NO_EVENT; DEBUG_receive = true; return DEBUG_STATE;if(TEST) {RFduinoBLE.sendInt(124);delay(DELAY_BEFORE_SENDING_COMMAND);}}
  if(TEST) {RFduinoBLE.sendInt(125);delay(DELAY_BEFORE_SENDING_COMMAND);}

  digitalWrite(LED2, LOW);
  
  return RECEIVE_N_SEND_STATE;
}

/*************************************************** 
Send the MCU a command to send back the sleep command
***************************************************/
state_enum Sleep_Debug_Event_Handler(void){
  //if(state == DEBUG_STATE) RFduinoGZLL.sendToHost("DEVICE: Sleep debug event\n\r");
  //RFduinoGZLL.sendToHost(CMD[CMD_ACK]);
  if(TEST) {RFduinoBLE.sendInt(89);delay(DELAY_BEFORE_SENDING_COMMAND);}
  Serial.write(CMD[CMD_SLEEP_DEBUG]);
  event = MAX_EVENTS;
  return RECEIVE_N_SEND_STATE;
}

/*************************************************** 
When we receive parameters
***************************************************/
state_enum Params_Event_Handler(void){
  no_stop_when_timeout = true;
  digitalWrite(LED2, HIGH);
  Serial.write(CMD[CMD_START]);
  Serial.write(param_str);
  SerialIdleTime = millis();
  //RFduinoGZLL.sendToHost(CMD[CMD_ACK]);
  if(TEST) {RFduinoBLE.sendInt(120);delay(DELAY_BEFORE_SENDING_COMMAND);}
  if(event==PARAMS_EVENT) event = MAX_EVENTS;
  if(state == DEBUG_STATE){
    //RFduinoGZLL.sendToHost("DEVICE: Params event, params: ");
   // RFduinoGZLL.sendToHost(param_str);
    DEBUG_receive = true;
    event = NO_EVENT;
    return DEBUG_STATE;
  }
  
  return RECEIVE_N_SEND_STATE;
}

/*************************************************** 
In case of a timeout
***************************************************/
state_enum Timeout_Event_Handler(void){
  if(TEST) {RFduinoBLE.sendInt(121);delay(DELAY_BEFORE_SENDING_COMMAND);}
  //if(state == DEBUG_STATE) RFduinoBLE.sendInt(80);
  event = NO_EVENT;
  return IDLE_STATE;
}


/*************************************************** 
Entering debug state
***************************************************/
state_enum Debug_Event_Handler(void){
  //RFduinoGZLL.sendToHost("DEVICE: Entering DEBUG state.\n\r");
  if(TEST) {RFduinoBLE.sendInt(122);delay(DELAY_BEFORE_SENDING_COMMAND);}
  DEBUG_active = true;
  event = NO_EVENT;
  return DEBUG_STATE;
}


/*************************************************** 
Leaving debug state
***************************************************/
state_enum EndDebug_Event_Handler(void){
  //RFduinoGZLL.sendToHost("DEVICE: Leaving DEBUG state.\n\r");
  if(TEST) {RFduinoBLE.sendInt(123);delay(DELAY_BEFORE_SENDING_COMMAND);}
  DEBUG_active = false;
  DEBUG_receive = false;
  event = NO_EVENT;
  return IDLE_STATE;
}

