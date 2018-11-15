/*-----------------------------------------------------------------------------
'                       CC1101_RX_Demo_WOR
'                       ------------------
'
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
volatile int mcu_sleep_enable = 0;
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
  Serial.begin(115200);Serial.println();
  
  // init CC1101 RF-module and get My_address from EEPROM
  cc1100.begin(My_addr);                   //inits RF module with main default settings. returns My_addr from Arduino EEPROM setting
  
  cc1100.sidle();                          //set to IDLE first
  
  cc1100.set_mode(0x04);                   //set modulation mode 1 = GFSK_1_2_kb; 2 = GFSK_38_4_kb; 3 = GFSK_100_kb; 4 = MSK_250_kb; 5 = MSK_500_kb; 6 = OOK_4_8_kb
  cc1100.set_ISM(0x02);                    //set ISM Band 1=315MHz; 2=433MHz; 3=868MHz; 4=915MHz
  cc1100.set_channel(0x01);                //set channel
  cc1100.set_myaddr(0x03);                 //overwrites my own address

  cc1100.spi_read_burst(PATABLE_BURST, Patable, 8); //backup selected PA Table settings
  cc1100.set_output_power_level(0);        //set PA level in dbm
  
  cc1100.spi_write_register(IOCFG2,0x06);
  
  cc1100.show_main_settings();             //shows setting debug messages to UART    
  cc1100.show_register_settings();         //shows current CC1101 register values

  cc1100.set_debug_level(1);               //enable/disable debug outputs
  
  cc1100.wor_enable();                     //enable WOR mode
  cc1100_wor_enable_flag = 1;              //set WOR enable flag
  //cc1100.receive();
  
  // init interrrupt function for available packet
  enableInterrupt(GDO2, rf_available_int, RISING); 
  
  Serial.println(F("CC1101 RX Demo WOR"));   //welcome message
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
      cc1100.wor_reset();                        
      cc1100_wor_enable_flag = 1;                 //set WOR flag
      Serial.println(F("...CC1101 WOR enabled"));
    }
    
    mcu_sleep_enable = 1;                         //set Arduino in sleep mode till incomming packet interrupt
    Serial.println(F("...sleep MCU")); Serial.println();          
    delay(100);
    set_sleep_mode(SLEEP_MODE_STANDBY);           //important to be in STANDBY Mode
    cli();
    sleep_enable();
    sei();
    sleep_cpu();
    sleep_disable();                              //restart MCU after standby mode
    
    enableInterrupt(GDO2, rf_available_int, RISING);     
  }
  
}
//--------------------------[end loop]----------------------------

//---------------------[ check incomming RF packet ]-----------------------
void rf_available_int(void) 
{
  disableInterrupt(GDO2);
  sei();                                                                 //for millis()
  uint32_t rf_time_stamp = millis();                                     //generate time stamp
  mcu_sleep_enable = 0;

  
  if(cc1100.packet_available() == TRUE){                                 //checks packet available flag

    if(cc1100_wor_enable_flag == 1){                                     //only disable and update PA Table if CC1101 is in WOR mode
      cc1100.wor_disable();
      cc1100_wor_enable_flag = 0;
      cc1100.spi_write_burst(PATABLE_BURST, Patable, 8);                 //bugfix, CC1101 PA Table is lost after SLEEP mode
    }
    if(cc1100.get_payload(Rx_fifo, pktlen, rx_addr, sender, rssi_dbm, lqi) == TRUE) //stores the payload data to Rx_fifo
    {
        cc1101_packet_available = TRUE;                                //set flag that a package is in RX buffer
        Serial.print(F("rx_time: "));Serial.print(millis()-rf_time_stamp);Serial.println(F("ms"));
    }
    else
    {
        cc1101_packet_available = FALSE;                               //set flag that an package is corrupted
    }
  }

  enableInterrupt(GDO2, rf_available_int, RISING);
}

//-----------------------------------------------------------------------------------
