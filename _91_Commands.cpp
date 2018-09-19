#include "_90_Commands.h"

/****************************************************************************** 
  ------------------------------- Variables -------------------------------
******************************************************************************/

char* CMD[NUMBER_OF_COMMANDS];
char param_str[PARAM_STRING_BUFFER_SIZE];


/****************************************************************************** 
  ------------------------------- Functions -------------------------------
******************************************************************************/
/*************************************************** 
Initializes the command strings
***************************************************/
void InitCMD(){
   CMD[CMD_START]       =   "$START$";
   CMD[CMD_STOP]        =   "$STOP$";
   CMD[CMD_SLEEP]       =   "$SLEEP$";
   CMD[CMD_ACK]         =   "$ACK$";
   CMD[CMD_STARTM1]     =   "$STARTM1$";
   CMD[CMD_SLEEP_DEBUG] =   "$SLEEP_DEBUG$";
   CMD[CMD_DEBUG]       =   "$DEBUG$";
   CMD[CMD_STARTM2]     =   "$STARTM2$";
}


/*************************************************** 
Executes the given command
***************************************************/
void ExecuteCommand(int cmd){

  switch(cmd){
    
    case CMD_START:{
      event = START_EVENT;
      if(TEST) {RFduinoBLE.sendInt(91);delay(DELAY_BEFORE_SENDING_COMMAND);}
      break;
    }

    case CMD_STOP:{
      event = STOP_EVENT;
      if(TEST) {RFduinoBLE.sendInt(92);delay(DELAY_BEFORE_SENDING_COMMAND);}
      break;
    }

    case CMD_SLEEP:{
      event = SLEEP_EVENT;
      if(TEST) {RFduinoBLE.sendInt(93);delay(DELAY_BEFORE_SENDING_COMMAND);}
      break;
    }

    case CMD_STARTM1:{
      event = STARTM1_EVENT;
      if(TEST) {RFduinoBLE.sendInt(96);delay(DELAY_BEFORE_SENDING_COMMAND);}
      break;
    }

     case CMD_STARTM2:{
      event = STARTM2_EVENT;
      if(TEST) {RFduinoBLE.sendInt(97);delay(DELAY_BEFORE_SENDING_COMMAND);}
      break;
    }

     case CMD_SLEEP_DEBUG:{
      if(TEST) {RFduinoBLE.sendInt(98);delay(DELAY_BEFORE_SENDING_COMMAND);}
      event = SLEEP_DEBUG_EVENT;
      break;
    }

    case CMD_DEBUG:{
      event = DEBUG_EVENT;
      if(TEST) {RFduinoBLE.sendInt(111);delay(DELAY_BEFORE_SENDING_COMMAND);}
      break;
    }

    default: {
      event = STOP_EVENT; 
      if(TEST){RFduinoBLE.sendInt(92);delay(DELAY_BEFORE_SENDING_COMMAND);} 
      break;
      }
    
  }
  
}


/*************************************************** 
Verifies the command string
***************************************************/
int VerifyCommand(char* data){
  if(TEST) {RFduinoBLE.sendInt(113);delay(DELAY_BEFORE_SENDING_COMMAND);}
  for(int i=0; i<NUMBER_OF_COMMANDS; ++i){
    if(strcmp(data, CMD[i])==0) return i;
  }
  return -1;
}


/*************************************************** 
Saves the parameters string for future use
***************************************************/
void SaveParamString(char* data, int len){
  if(TEST) {RFduinoBLE.sendInt(114);delay(DELAY_BEFORE_SENDING_COMMAND);}
  for(int i=0; i<len; ++i){
    param_str[i] = data[i];
  }
}

