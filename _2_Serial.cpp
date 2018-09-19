#include "_1_Serial.h"

/****************************************************************************** 
  ------------------------------- Variables -------------------------------
******************************************************************************/

buffer_struct Buffer;

char T_buffer[36] = {'x','x','x','x','x','x', 'x', 'x', 'x','x', 'x', 'x', 'x','x','x','x','x','x', 'x', 'x', 'x','x', 'x', 'x', 'x','x','x','x','x','x', 'x', 'x', 'x','x', 'x', 'x'};
int T_buffer_index = 0;
int T_buffer_size = 0;
bool T_buffer_flag = false;
char T_buffer2[12] = {'x','x','x','x','x','x', 'x', 'x', 'x','x', 'x', 'x'}; //, 'x','x','x','x','x','x', 'x', 'x', 'x','x', 'x', 'x', 'x','x','x','x','x','x', 'x', 'x', 'x','x', 'x', 'x'};
int T_buffer2_index = 0;
int T_buffer2_size = 0;
char T_buffer3[12] = {'x','x','x','x','x','x', 'x', 'x', 'x','x', 'x', 'x'};
int T_buffer3_index = 0;
int T_buffer3_size = 0;
char T_buffer4[12] = {'x','x','x','x','x','x', 'x', 'x', 'x','x', 'x', 'x'};
int T_buffer4_index = 0;
int T_buffer4_size = 0;
int B2cntr = 0;
bool T_buffer2_flag_send = false;
bool T_buffer2_flag_reset = false;

bool flag_ = false;

int rec_cntr = 0;
bool rec_cntr_flag = true;

char commandchar = 0;
int commandchar_isfree = true; 

unsigned long SerialIdleTime = 0;
unsigned long LedBlinkTime = 0;

int LedState = 0;

unsigned cmd_char_counter = 0;

/****************************************************************************** 
  ----------------------------- Static Functions -----------------------------
******************************************************************************/

static void ResetBuffer(buffer_struct *buffstr){
  buffstr->head = 0;
}


static void InitBuffer(buffer_struct buffstr){
  buffstr.head = 0;
}

/*************************************************** 
Checks if a character is a number or not
***************************************************/
static int isNum(char ch){
  int temp = ch - '0';
  if(temp<0 || temp>9) return false;
  return true;
}

/*************************************************** 
Checks if a number is a double or not
If not then it replaces the characters with X
***************************************************/
static void DataFormatCheck_DOUBLE(buffer_struct *Buffer){
  /*
   * 
   int wrong_format = false;

  if(Buffer->data[0] != DOUBLE_ID) wrong_format = true;

  for(int i = 1; i<(Buffer->head); ++i){
    char c = Buffer->data[i];
    if(!isNum(c) && (c != '.') && (c != '\n') && (c != '\r') ) Buffer->data[i] = ERROR_CHARACTER;
  }

  if(wrong_format){
    for(int i = 1; i<(Buffer->head); ++i){
      if( (Buffer->data[i] != '\n') && (Buffer->data[i] != '\r') ){
        Buffer->data[i] = 'X';
      }
    }
  }
  */
}


/*************************************************** 
Checks if a number is an integer or not.
If not then it replaces the characters with X
***************************************************/
static void DataFormatCheck_INT(buffer_struct *Buffer){
  /*
  int wrong_format = false;

  //If there is no format type character at the beginning it is wrong
  if(Buffer->data[0] != INT_ID) wrong_format = true;
  
  for(int i = 1; i<(Buffer->head); ++i){
    char c = Buffer->data[i];
    if(!isNum(c) && (c != '-') && (c != '\n') && (c != '\r') ) Buffer->data[i] = ERROR_CHARACTER;
  }

  if(wrong_format){
    for(int i = 1; i<(Buffer->head); ++i){
      if( (Buffer->data[i] != '\n') && (Buffer->data[i] != '\r') ){
        Buffer->data[i] = 'X';
      }
    }
  }
  */
}


/*************************************************** 
Checks if a number is a string
If not then it replaces the characters with X
***************************************************/
static void DataFormatCheck_STRING(buffer_struct *Buffer){
  /*
   * 
  int wrong_format = false;

  if(Buffer->data[0] != STRING_ID) wrong_format = true;

  if(wrong_format){
    for(int i = 1; i<(Buffer->head); ++i){
      if( (Buffer->data[i] != '\n') && (Buffer->data[i] != '\r') ){
        Buffer->data[i] = 'X';
      }
    }
  }
  */
}


/*************************************************** 
Stores the characters and sends them
***************************************************/
static void SendBuffer(buffer_struct *Buffer, char datatype){
  //if(TEST){RFduinoBLE.sendInt(20);delay(DELAY_BEFORE_SENDING_COMMAND);}

  switch(datatype){
    case DOUBLE_ID: DataFormatCheck_DOUBLE(Buffer); break;
    case INT_ID:    DataFormatCheck_INT(Buffer);    break;
    case STRING_ID: DataFormatCheck_STRING(Buffer); break;
    default: break;
  }
  
  //RFduinoGZLL.sendToHost(Buffer->data, Buffer->head); -> Buffer->head = hossz
  //if(TEST){RFduinoBLE.sendInt(21);}
  //RFduinoBLE.send(Buffer->data, Buffer->head);
  //RFduinoBLE.sendByte('c');
  
  //ResetBuffer(Buffer);
  
  commandchar = 0;
  commandchar_isfree = true; //The next data may arrive
}



/*************************************************** 
Stores the characters and sends them
***************************************************/
static void StoreAndSendData(char ch, char datatype){
  //if(TEST){RFduinoBLE.sendInt(21);delay(DELAY_BEFORE_SENDING_COMMAND);}
  if(ch != 0){
    Buffer.data[Buffer.head] = ch;
    Buffer.head++;
    
    if(Buffer.head >= BUFFER_SIZE) ResetBuffer(&Buffer);
    
    if(ch == '\n'){
      Buffer.data[Buffer.head] = '\r';
      Buffer.head++;
      SendBuffer(&Buffer, datatype);
    }
    
  }
}


/*************************************************** 
Sets up the command character
***************************************************/
static void SetCommandChar(char ch){
  //if(TEST){RFduinoBLE.sendInt(22);delay(DELAY_BEFORE_SENDING_COMMAND);}
  
  if(commandchar_isfree){
    
    switch(ch){
      case DOUBLE_ID:         commandchar = DOUBLE_ID;  commandchar_isfree = false;   break;
      case INT_ID:            commandchar = INT_ID;     commandchar_isfree = false;   break;
      case STRING_ID:         commandchar = STRING_ID;  commandchar_isfree = false;   break;
      case COMMAND_CHARACTER: commandchar = COMMAND_CHARACTER; commandchar_isfree = false; break;
      default: break;
    }
    
  
  }
  
}



/*************************************************** 
Stores the command string
***************************************************/
static void GetCommand(char ch){
  //if(TEST){RFduinoBLE.sendInt(23);delay(DELAY_BEFORE_SENDING_COMMAND);}
 
  if(ch != 0){
    if(ch == COMMAND_CHARACTER) cmd_char_counter++;

    Buffer.data[Buffer.head] = ch;
    Buffer.head++;
    
    if(Buffer.head >= BUFFER_SIZE) ResetBuffer(&Buffer);
    
    if(cmd_char_counter == 2){
      Buffer.data[Buffer.head] = '\0';
      int cmd = VerifyCommand(Buffer.data);
      ExecuteCommand(cmd);
      
      ResetBuffer(&Buffer);
      commandchar = 0;
      commandchar_isfree = true;
      cmd_char_counter = 0;
    }
    
  }
 
}





/****************************************************************************** 
  ------------------------------- Functions -------------------------------
******************************************************************************/


/*************************************************** 
Initializes the serial communication
***************************************************/
void InitSerial(){
  pinMode(WAKE_PIN, INPUT);
  //RFduino_pinWakeCallback(WAKE_PIN, HIGH, WakeUp);
  
  pinMode(RX_PIN,INPUT);
  pinMode(TX_PIN,OUTPUT);
  pinMode(LED1, OUTPUT);

  InitBuffer(Buffer);

  Serial.begin(BAUD, RX_PIN, TX_PIN);
}


/*************************************************** 
Serial event interrupt handler
***************************************************/
void serialEvent(void){

    //RFduinoBLE.sendByte(25);
    //if(TEST){RFduinoBLE.sendInt(25);} delay(DELAY_BEFORE_SENDING_COMMAND);}
    if(state == RECEIVE_N_SEND_STATE || event == WAKE_UP_EVENT){

      SerialIdleTime = millis();
       
      char ch = Serial.read();

      if(ch != '\n')
      {
        switch(B2cntr)
        {
          case 0:
          {
            T_buffer2[T_buffer2_index] = ch;
            T_buffer2_index++;
            break;
          }
          case 1:
          {
            T_buffer3[T_buffer3_index] = ch;
            T_buffer3_index++;
            break;
          }
          case 2:
          {
            T_buffer4[T_buffer4_index] = ch;
            T_buffer4_index++;
            break;
          }
          /*case 3:
          {
            T_buffer2_flag_send = true;
            T_buffer2_size = T_buffer2_index;
            T_buffer3_size = T_buffer3_index;
            T_buffer4_size = T_buffer4_index;
            T_buffer2_index = 0;
            T_buffer3_index = 0;
            T_buffer4_index = 0;
            B2cntr = 0;
            break;
          }*/
        }
      }
      else
      {
        B2cntr++;
        if(B2cntr >=3){
          T_buffer2_flag_send = true;
            T_buffer2_size = T_buffer2_index;
            T_buffer3_size = T_buffer3_index;
            T_buffer4_size = T_buffer4_index;
            T_buffer2_index = 0;
            T_buffer3_index = 0;
            T_buffer4_index = 0;
            B2cntr = 0;
        }
      }
/*
      if(ch != '\n')
      {
        T_buffer2[T_buffer2_index] = ch;
        T_buffer2_index++;
        T_buffer2_flag_send = false;
      }
      else
      {
        if(B2cntr < 3)
        {
          T_buffer2[T_buffer2_index] = ch;
          T_buffer2_index++;
          B2cntr++;
          T_buffer2_flag_send = false;
        }
        else
        {
          T_buffer2[T_buffer2_index] = ch;
          T_buffer2_size = T_buffer2_index + 1;
          T_buffer2_index = 0;
          T_buffer2_flag_send = true;
          B2cntr = 0;
        }
      }
*/
/*
      T_buffer[T_buffer_index] = ch;
      T_buffer_index++;

      if(T_buffer_index >= 6)
      {
        T_buffer_index = 0;
        T_buffer_flag = true;
      }
      */
      
      
/*
      if(T_buffer_index <= 50)
      {
        //rec_cntr++;
        T_buffer[T_buffer_index] = ch;
        T_buffer_index++;
      }
      else
      {
        flag_ = true;
      }
*/
     /*
      if(T_buffer_index >= 9)
      {
        T_buffer_index = 0;
      }
      */
      
      //rec_cntr_flag = true;
      
      
      
      
      /*
      if(!Serial.available()){
        T_buffer_size = T_buffer_index + 1;
        T_buffer_index = 0;
      }*/

    /*T_buffer[T_buffer_index] = ch;
    T_buffer_index++;
    if(T_buffer_index == 49)
    {
      T_buffer_index = 0;
    }*/

      BlinkLed(LED_BLINK_TIME);

      //RFduinoGZLL.sendToHost(ch);
      //RFduinoBLE.sendInt(100);
      //RFduinoBLE.sendInt(int(ch));
      //SetCommandChar(ch); // MA VETTEM KI
  
/* MA VETTEM KI
      switch(commandchar){
        case DOUBLE_ID:   StoreAndSendData(ch, DOUBLE_ID);   break;
        case INT_ID:      StoreAndSendData(ch, INT_ID);      break;
        case STRING_ID:   StoreAndSendData(ch, STRING_ID);   break;
        case COMMAND_CHARACTER: GetCommand(ch); break;
        default:  break;
      }
      */
  }
  /*else if(state == DEBUG_STATE){
      //RFduinoBLE.send(Serial.read());
      RFduinoBLE.sendInt(100);
  }*/


}

/*************************************************** 
Toggles LED1 with ms period
***************************************************/
void BlinkLed(unsigned ms){
  if(millis()-LedBlinkTime > ms){
    LedState = !LedState;
    digitalWrite(LED1, LedState);
    LedBlinkTime = millis();
  }
}

/*************************************************** 
Sends buffer
***************************************************/
void SendBuffer_(){
  /*for(int i=0; i<8; ++i){
   // 2 byte egy adat!
    //RFduinoBLE.sendByte(Buffer.data[i]);
    RFduinoBLE.sendByte(T_buffer[i]);
    delay(100);
  }
  RFduinoBLE.sendByte(-1);
  */
  /*int buffer_size = 10;
  //RFduinoBLE.send();
  for(int i = 0; i<10; i++)
  {
    if(T_buffer[i] = 'x'){
      buffer_size = i;
      break;
    }
  }*/
  //RFduinoBLE.send(T_buffer, 36);
  if(T_buffer2_flag_send)
  {
    RFduinoBLE.send(T_buffer2, T_buffer2_size);
    //T_buffer2_flag_send = false;
    for(int idx = 0; idx < 12; ++idx)
    {
      T_buffer2[idx] = 'x';
    }
    delay(100);
    RFduinoBLE.send(T_buffer3, T_buffer3_size);
    //T_buffer2_flag_send = false;
    for(int idx = 0; idx < 12; ++idx)
    {
      T_buffer3[idx] = 'x';
    }
    delay(100);
    RFduinoBLE.send(T_buffer4, T_buffer4_size);
    T_buffer2_flag_send = false;
    for(int idx = 0; idx < 12; ++idx)
    {
      T_buffer4[idx] = 'x';
    }
    
  }
  // T_buffer2 = {'x','x','x','x','x','x', 'x', 'x', 'x','x'};
  
  
  /*
  if(T_buffer_flag)
  {
    RFduinoBLE.send(T_buffer2, 10); //, T_buffer_size);
    T_buffer_flag = false;
    }
  
  /*if(flag_)
  {
    for(int i = 0; i < 50; i++)
    {
      RFduinoBLE.send(T_buffer[i]);
      delay(100);
    }
    RFduinoBLE.send(-1);
    flag_ = false;
  }*/
}

/*************************************************** 
Sends counter value
***************************************************/
void SendCntr(){
  if(rec_cntr_flag)
  {
    RFduinoBLE.sendInt(T_buffer_index);
    rec_cntr_flag = false;
  }
}
