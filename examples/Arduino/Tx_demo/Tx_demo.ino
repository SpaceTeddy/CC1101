/*-----------------------------------------------------------------------------
'                     TX_DEMO
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

//--------------------------[Global Task variables]--------------------------
uint32_t prev_millis_1s_timer = 0;

const uint16_t INTERVAL_1S_TIMER = 1000;        // interval at which to blink (milliseconds)

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
  cc1100.set_ISM(0x02);                    //set frequency 1=315MHz; 2=433MHz; 3=868MHz; 4=915MHz
  cc1100.set_channel(0x01);                //set channel
  cc1100.set_output_power_level(0);        //set PA level in dbm
  cc1100.set_myaddr(0x01);                 //set my own address
  
  //cc1100.spi_write_register(IOCFG2, 0x06); //set module in sync mode detection mode
  //cc1100.set_modulation_type(7);           //set Modulation type 2-FSK=0; GFSK=1; ASK/OOK=3; 4-FSK=4; MSK=7
  //cc1100.set_datarate(0x8b, 0xf8, 0x44);   //set datarate parameters, calculated by python tool
  
  cc1100.show_main_settings();             //shows setting debug messages to UART
  cc1100.show_register_settings();         //shows current CC1101 register values
  cc1100.receive();                        //set to RECEIVE mode

  // init interrrupt function for available packet
  enableInterrupt(GDO2, rf_available_int, RISING); 
  
  Serial.println(F("CC1101 TX Demo"));   //welcome message
}
//---------------------------------[LOOP]-----------------------------------
void loop()
{ 
 
  // one second update timer
  if (millis() - prev_millis_1s_timer >= INTERVAL_1S_TIMER) 
  {
    Rx_addr = 0x03;                                              //receiver address

    uint32_t time_stamp = millis();                              //generate time stamp
  
    Tx_fifo[3] = (uint8_t)(time_stamp >> 24);                    //split 32-Bit timestamp to 4 byte array
    Tx_fifo[4] = (uint8_t)(time_stamp >> 16);
    Tx_fifo[5] = (uint8_t)(time_stamp >> 8);
    Tx_fifo[6] = (uint8_t)(time_stamp);

    Pktlen = 0x07;                                               //set packet len to 0x13

    detachPinChangeInterrupt(GDO2);                              //disable pin change interrupt
    cc1100.sent_packet(My_addr, Rx_addr, Tx_fifo, Pktlen, 40);   //sents package over air. ACK is received via GPIO polling    
    attachPinChangeInterrupt(GDO2, rf_available_int, RISING);    //enable pin change interrupt           
    
    Serial.print(F("tx_time: "));Serial.print(millis()-time_stamp);Serial.println(F("ms"));
    prev_millis_1s_timer = millis();
  }
  
}
//--------------------------[end loop]----------------------------

//-----interrrupt function not needed for this demo
//---------------------[ check incomming RF packet ]-----------------------
void rf_available_int(void) 
{
  disableInterrupt(GDO2);
  
  if(cc1100.packet_available() == TRUE){
    if(cc1100.get_payload(Rx_fifo, pktlen, rx_addr, sender, rssi_dbm, lqi) == TRUE) //stores the payload data to Rx_fifo
    {
        cc1101_packet_available = TRUE;                                //set flag that a package is in RX buffer
    }
    else
    {
        cc1101_packet_available = FALSE;                               //set flag that an package is corrupted
    }
  }
  
  enableInterrupt(GDO2, rf_available_int, RISING); 
}
