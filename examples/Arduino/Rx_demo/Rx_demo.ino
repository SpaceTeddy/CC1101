/*-----------------------------------------------------------------------------
'                     RX_DEMO
'                     -------
'  - needs EnableInterrupt (former PinChangeInt) library
'  - *.eep file must flashed to Arduino first -> use eeprom Tool
'  - install cc1100_arduino.h and cc1100_arduino.cpp as a library (create subdirectory "cc1100"
'    in your existing sketchbook/libraries directory and copy cc1100_arduino.* into the cc1100 
'    directory)
'  
'-----------------------------------------------------------------------------*/
#include <avr/sleep.h>
#include <cc1100_arduino.h>
#include <EnableInterrupt.h>

//---------------------------=[Global variables]=----------------------------
volatile int sleep_enable = 0;
uint32_t rf_timecode = 0;
uint32_t rf_timecode_backup = 0;
//--------------------------[Global Task variables]--------------------------

//--------------------------[Global CC1100 variables]------------------------
uint8_t Tx_fifo[FIFOBUFFER], Rx_fifo[FIFOBUFFER];
uint8_t My_addr, Tx_addr, Rx_addr, Pktlen, pktlen, Lqi, Rssi;
uint8_t rx_addr,sender,lqi;
 int8_t rssi_dbm;
volatile uint8_t cc1101_packet_available;

//--------------------------[class constructors]-----------------------------
//init CC1100 constructor
CC1100 cc1100;

//---------------------------------[SETUP]-----------------------------------
void setup() 
{
  // init serial Port for debugging
  Serial.begin(115200);Serial.println();
  
  // init CC1101 RF-module and get My_address from EEPROM
  cc1100.begin(My_addr);                   //inits RF module with main default settings
  
  cc1100.sidle();                          //set to ILDE first
  
  cc1100.set_mode(0x04);                   //set modulation mode 1 = GFSK_1_2_kb; 2 = GFSK_38_4_kb; 3 = GFSK_100_kb; 4 = MSK_250_kb; 5 = MSK_500_kb; 6 = OOK_4_8_kb
  cc1100.set_ISM(0x02);                    //set ISM Band 1=315MHz; 2=433MHz; 3=868MHz; 4=915MHz
  cc1100.set_channel(0x01);                //set channel
  cc1100.set_output_power_level(0);        //set PA level in dbm
  cc1100.set_myaddr(0x03);                 //set my own address
  
  //cc1100.spi_write_register(IOCFG0, 0x24); //set module in sync mode detection mode
  cc1100.spi_write_register(IOCFG2,0x06);

  cc1100.show_main_settings();             //shows setting debug messages to UART    
  cc1100.show_register_settings();         //shows current CC1101 register values
   
  cc1100.receive();                        //set to RECEIVE mode
  
  // init interrrupt function for available packet
  enableInterrupt(GDO2, rf_available_int, RISING); 
  
  Serial.println(F("CC1101 RX Demo"));   //welcome message
}

//---------------------------------[LOOP]-----------------------------------
void loop() 
{ 
  //if valid package is received
  if(cc1101_packet_available == TRUE){
    rf_timecode = ((uint32_t)Rx_fifo[3] << 24) + 
                  ((uint32_t)Rx_fifo[4] << 16) + 
                  ((uint16_t)Rx_fifo[5] <<  8) + 
                             Rx_fifo[6];
    Serial.print(F("TX_Timecode: "));Serial.print(rf_timecode);Serial.println(F("ms\n"));

    rf_timecode_backup = millis();
    
    cc1101_packet_available = FALSE;
  }
  
  //set Arduino in sleep mode if no package is received within 5 sec.
  if(millis() - rf_timecode_backup > 5000){
    rf_timecode_backup = millis();
    
    sleep_enable = 1;
    Serial.println(F("...sleep MCU"));   //welcome message
    delay(100);
    set_sleep_mode(SLEEP_MODE_STANDBY);
    cli();
    sleep_enable();
    sei();
    sleep_cpu();
    sleep_disable();
    
    enableInterrupt(GDO2, rf_available_int, RISING); 
  }
  
}
//--------------------------[end loop]----------------------------

//---------------------[ check incomming RF packet ]-----------------------
void rf_available_int(void) 
{
  disableInterrupt(GDO2);
  sei();                                                            //for millis()
  uint32_t time_stamp = millis();                                   //generate time stamp
  
  sleep_disable();
  sleep_enable = 0;

  if(cc1100.packet_available() == TRUE)
  {
    if(cc1100.get_payload(Rx_fifo, pktlen, rx_addr, sender, rssi_dbm, lqi) == TRUE) //stores the payload data to Rx_fifo
    {
        cc1101_packet_available = TRUE;                                //set flag that a package is in RX buffer
        Serial.print(F("rx_time: "));Serial.print(millis()-time_stamp);Serial.println(F("ms"));
    }
    else
    {
        cc1101_packet_available = FALSE;                               //set flag that an package is corrupted
    }
  }
  
  enableInterrupt(GDO2, rf_available_int, RISING); 
}
