/*-----------------------------------------------------------------------------
'                       CC1101_RX_Demo_WOR
'                       ------------------
'  - needs PinChangeInt library
'  - *.eep file must flashed to Arduino first -> use eeprom Tool
'  - put cc1100_arduino.h in your sketch folder. dont't install it as library 
'    for this example.
'  
'-----------------------------------------------------------------------------*/
#include <avr/sleep.h>
#include <cc1100.h>
#include <PinChangeInt.h>
//#include <EnableInterrupt.h>

//---------------------------=[Global variables]=----------------------------
volatile int sleep_enable = 0;
volatile int cc1100_wor_enable_flag = 0;
uint32_t rf_timecode = 0;
uint32_t rf_timecode_backup = 0;
//--------------------------[Global Task variables]--------------------------

//--------------------------[Global CC1100 variables]------------------------
uint8_t Tx_fifo[FIFOBUFFER], Rx_fifo[FIFOBUFFER], Patable[8];
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
  Serial.begin(38400);Serial.println();
  
  // init CC1101 RF-module and get My_address from EEPROM
  cc1100.begin(My_addr);                   //inits RF module with main default settings. returns My_addr from Arduino EEPROM setting
  
  cc1100.sidle();                          //set to IDLE first
  
  cc1100.set_mode(0x03);                   //set modulation mode 1 = GFSK_1_2_kb; 2 = GFSK_38_4_kb; 3 = GFSK_100_kb; 4 = MSK_250_kb; 5 = MSK_500_kb; 6 = OOK_4_8_kb
  cc1100.set_ISM(0x02);                    //set ISM Band 1=315MHz; 2=433MHz; 3=868MHz; 4=915MHz
  cc1100.set_channel(0x01);                //set channel
  cc1100.set_myaddr(0x03);                 //overwrites my own address

  cc1100.set_output_power_level(0);        //set PA level in dbm
  
  cc1100.spi_read_burst(PATABLE_BURST, Patable, 8); //backup selected PA Table settings
  //cc1100.spi_write_register(MCSM2,0);
  cc1100.show_main_settings();             //shows setting debug messages to UART    
  cc1100.show_register_settings();         //shows current CC1101 register values
   
  cc1100.wor_enable();                     //enable WOR mode
  cc1100_wor_enable_flag = 1;              //set WOR enable flag
  
  // init interrrupt function for available packet
  attachPinChangeInterrupt(GDO2, rf_available_int, RISING);
  
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
    
    cc1101_packet_available = FALSE;              //resets packet available flag
  }
  
  //enable CC1101 WOR mode and set Arduino in standby mode after 1 sec.
  
  if(millis() - rf_timecode_backup > 1000){
    rf_timecode_backup = millis();
    
    if(cc1100_wor_enable_flag == 0){              //enable WOR mode after 5 seconds if WOR is not enabled
      cc1100.wor_enable();                        
      cc1100_wor_enable_flag = 1;                 //set WOR flag
    }
    
    sleep_enable = 1;                             //set Arduino in sleep mode till incomming packet interrupt
    Serial.println(F("...sleep MCU"));           
    delay(200);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    cli();
    sleep_enable();
    sei();
    sleep_cpu();
    sleep_disable();                              //restart MCU after standby mode
    
    attachPinChangeInterrupt(GDO2, rf_available_int, RISING);           //enable pin change interrupt
    //enableInterrupt(GDO2, rf_available_int, HIGH);    
  }
  
}
//--------------------------[end loop]----------------------------

//---------------------[ check incomming RF packet ]-----------------------
void rf_available_int(void) 
{
  detachPinChangeInterrupt(GDO2);
  //disableInterrupt(GDO2);
  sei();                                                                 //for millis()
  uint32_t rf_time_stamp = millis();                                     //generate time stamp
  
  sleep_disable();
  sleep_enable = 0;

  if(cc1100_wor_enable_flag == 1){                                       //only disable and update PA Table if CC1101 is in WOR mode
    //Serial.println(F("disable WOR and update PA_Table: "));
    cc1100.wor_disable();
    cc1100_wor_enable_flag = 0;
    cc1100.spi_write_burst(PATABLE_BURST, Patable, 8);                   //bugfix, CC1101 PA Table is lost after SLEEP mode
  }
                                                                         //cc1101 is in continous RX mode
  if(cc1100.packet_available() == TRUE){                                 //checks packet available flag
    cc1100.get_payload(Rx_fifo, pktlen, rx_addr, sender, rssi_dbm, lqi); //stores the payload data to Rx_fifo
    cc1101_packet_available = TRUE;                                      //set flag that an package is in RX buffer
  }
  Serial.print(F("rx_time: "));Serial.print(millis()-rf_time_stamp);Serial.println(F("ms"));
  
  attachPinChangeInterrupt(GDO2, rf_available_int, RISING);
  //enableInterrupt(GDO2, rf_available_int, HIGH);
}
