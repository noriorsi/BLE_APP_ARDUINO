#include "_5_EnergyManagement.h"


/****************************************************************************** 
  ------------------------------- Variables -------------------------------
******************************************************************************/



/****************************************************************************** 
  ------------------------------- Functions -------------------------------
******************************************************************************/

/*************************************************** 
Enters deep sleep mode and wakes up after a delay
***************************************************/
void DeepSleepDelay(uint64_t ms){
  if(TEST){RFduinoBLE.sendInt(60);delay(DELAY_BEFORE_SENDING_COMMAND);}

  //Update: Disabling these causes some unwanted errors and the power comsumption doesnt seem the decrease.
  
  // Disable the Serial and the Wire for low power mode
  //NRF_UART0->ENABLE = (UART_ENABLE_ENABLE_Disabled << UART_ENABLE_ENABLE_Pos);
  //NRF_TWI1->ENABLE = (TWI_ENABLE_ENABLE_Disabled << TWI_ENABLE_ENABLE_Pos);

  //Swtich off leds
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  Serial.end();
  //RFduinoBLE.end();
  
  //RFduino_ULPDelay(ms);

  // Re-Enable the Serial and the Wire
  //NRF_TWI1->ENABLE = (TWI_ENABLE_ENABLE_Enabled << TWI_ENABLE_ENABLE_Pos);
  //NRF_UART0->ENABLE = (UART_ENABLE_ENABLE_Enabled << UART_ENABLE_ENABLE_Pos);

  Serial.begin(BAUD, RX_PIN, TX_PIN);
  //RFduinoBLE.begin();
}


/*************************************************** 
Wakes up the device
***************************************************/
/*int WakeUp(long unsigned int a){
  Serial.begin(BAUD, RX_PIN, TX_PIN);
  RFduinoBLE.begin();

  digitalWrite(LED2, HIGH);
  event = WAKE_UP_EVENT;
  if(TEST){RFduinoBLE.sendInt(61);delay(DELAY_BEFORE_SENDING_COMMAND);}
  return 0;
}*/


/*************************************************** 
Enters deep sleep mode
***************************************************/
void SystemOff(){
  /*if(TEST){}

  RFduino_pinWakeCallback(WAKE_PIN, DISABLE, WakeUp);
  
  // Disable the Serial and the Wire for low power mode
  NRF_UART0->ENABLE = (UART_ENABLE_ENABLE_Disabled << UART_ENABLE_ENABLE_Pos);
  NRF_TWI1->ENABLE = (TWI_ENABLE_ENABLE_Disabled << TWI_ENABLE_ENABLE_Pos);

  //Swtich off leds
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  Serial.end();
  RFduinoBLE.end(); 

  RFduino_systemOff();*/
}

