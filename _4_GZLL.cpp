#include "_3_GZLL.h"

/****************************************************************************** 
  ------------------------------- Variables -------------------------------
******************************************************************************/
char* data_hacked = "$STARTM2$";


/****************************************************************************** 
  ------------------------------- Static Functions -------------------------------
******************************************************************************/


/*************************************************** 
There seems to be one excessive character when receiving the command.
This functions corrects it.
***************************************************/
static void CorrectCommand(char* data, int len){
  data[len] = '\0';
}

/****************************************************************************** 
  ------------------------------- Functions -------------------------------
******************************************************************************/

/*************************************************** 
Initializes the BLE (-GZLL) protocol
***************************************************/
void InitGZLL(){
  RFduinoBLE.deviceName = "Subject B";
  RFduinoBLE.begin();
  pinMode(LED2, OUTPUT);
  if(TEST)
  {
    /*
    for(int i =0; i<3; ++i)
    {
      digitalWrite(LED2, HIGH);
      delay(300);
      digitalWrite(LED2, LOW);
      delay(300);
    }
    */
  }
}

/*void InitBLE(){
  RFduinoBLE.begin();
  pinMode(LED2, OUTPUT);
}*/

void RFduinoBLE_onConnect()
{
  //Serial.write("RFduinoBLE_onConnect");
  
  for(int i =0; i<3; ++i)
  {
    digitalWrite(LED2, HIGH);
    delay(300);
    digitalWrite(LED2, LOW);
    delay(300);
  }
  
}

void RFduinoBLE_onDisconnect()
{
  // back to IDLE...
  //Serial.println("RFduinoBLE_onDisconnect");
  for(int i =0; i<5; ++i)
  {
    digitalWrite(LED2, HIGH);
    delay(300);
    digitalWrite(LED2, LOW);
    delay(300);
  }
}
/*************************************************** 
Receives data from Host
***************************************************/

void RFduinoBLE_onReceive( char *data, int len)
{
  //Serial.println("RFduinoBLE_onReceive");
  //Serial.println(data[0]);
  int cmd = atoi(data);
  if(TEST){RFduinoBLE.sendInt(40);delay(DELAY_BEFORE_SENDING_COMMAND);}
  if(TEST){RFduinoBLE.sendInt(cmd);delay(DELAY_BEFORE_SENDING_COMMAND);}
  if(TEST){RFduinoBLE.sendInt(40);delay(DELAY_BEFORE_SENDING_COMMAND);}
  //Serial.read()
  
  if(len == 1) // !!!
  { 
    if(cmd >= 0 && cmd <= 8) // 
    {

      if(cmd == 7) FlashLed(cmd);
      
      if(TEST){RFduinoBLE.sendInt(41);delay(DELAY_BEFORE_SENDING_COMMAND);}
      ExecuteCommand(cmd);
    }
    else
    {
      if(TEST){RFduinoBLE.sendInt(42);delay(DELAY_BEFORE_SENDING_COMMAND);}  
    }
  /*
    switch(data[0]){
      case COMMAND_CHARACTER: // -> $
      {
        if(TEST){RFduinoBLE.sendInt(44);}
        
        //Serial.println("COMMAND_CHARACTER");
        //todo: use $0...5 format
        CorrectCommand(data_hacked,len);
        int cmd = VerifyCommand(data_hacked);
        ExecuteCommand(cmd);
        //RFduinoBLE.sendInt(1);
        break;
       }
       case '0':
       {
        //Serial.println("LED1, HIGH");
        if(TEST){RFduinoBLE.sendInt(41);}
        digitalWrite(LED1, HIGH);
        ExecuteCommand(CMD_STARTM2);
        break;
       }
       case '1':
       {
        if(TEST){RFduinoBLE.sendInt(42);}
        //Serial.println("LED1, LOW");
        digitalWrite(LED1, LOW);
        break;
       }
      default:
      {
        if(TEST){RFduinoBLE.sendInt(43);}
        break;
      }
*/
  }
  else
  {
    if(TEST){RFduinoBLE.sendInt(43);}
  }
}
 
  
/*
  if(len > 0){

    switch(data[0]){
      
      case COMMAND_CHARACTER:{
        CorrectCommand(data,len);
        int cmd = VerifyCommand(data);
        ExecuteCommand(cmd);
        break;
      }

      case PARAM_CHAR:{
        CorrectCommand(data,len);
        SaveParamString(data, len);
        event = PARAMS_EVENT;
        break;
      }

      default: break;
    }
}
*/

/*************************************************** 
Sends host a 0 byte payload to receive the binary_state. 
Also pings the MCU when not receiving serial data.
***************************************************/
void Ping(){/*
  
  char temp[20];
  snprintf(temp, 20, "sDEVICE State: %d\r\n", state);
  
 // request the binary_state from the Host (send a 0 byte payload)
  RFduinoGZLL.sendToHost(temp);
  delay(5);
  if(state == DEBUG_STATE)  DeepSleepDelay(DEBUG_PING_INTERVAL);
  else                      DeepSleepDelay(PING_INTERVAL);
  delay(5);
  */
}

void FlashLed(unsigned int n){
  for(int i =0; i<n; ++i)
  {
    digitalWrite(LED2, HIGH);
    delay(300);
    digitalWrite(LED2, LOW);
    delay(300);
  }
}







