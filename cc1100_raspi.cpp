/*-------------------------------------------------------------------------------
'                     CC1100 Raspberry Pi Library
'                     ---------------------------
'
'
'
'
'
'  module contains helper code from other people. Thx for that
'-----------------------------------------------------------------------------*/
#include "cc1100_raspi.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

extern uint8_t cc1100_debug;
//-------------------[global default settings 868 Mhz]---------------------------------

static uint8_t cc1100_GFSK_1_2_kb[CFG_REGISTER] = {
                    0x07,  // IOCFG2        GDO2 Output Pin Configuration
                    0x2E,  // IOCFG1        GDO1 Output Pin Configuration
                    0x80,  // IOCFG0        GDO0 Output Pin Configuration
                    0x07,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
                    0x57,  // SYNC1         Sync Word, High Byte
                    0x43,  // SYNC0         Sync Word, Low Byte
                    0x3E,  // PKTLEN        Packet Length
                    0x0E,  // PKTCTRL1      Packet Automation Control
                    0x45,  // PKTCTRL0      Packet Automation Control
                    0xFF,  // ADDR          Device Address
                    0x00,  // CHANNR        Channel Number
                    0x08,  // FSCTRL1       Frequency Synthesizer Control
                    0x00,  // FSCTRL0       Frequency Synthesizer Control
                    0x21,  // FREQ2         Frequency Control Word, High Byte
                    0x65,  // FREQ1         Frequency Control Word, Middle Byte
                    0x6A,  // FREQ0         Frequency Control Word, Low Byte
                    0xF5,  // MDMCFG4       Modem Configuration
                    0x83,  // MDMCFG3       Modem Configuration
                    0x13,  // MDMCFG2       Modem Configuration
                    0xA0,  // MDMCFG1       Modem Configuration
                    0xF8,  // MDMCFG0       Modem Configuration
                    0x15,  // DEVIATN       Modem Deviation Setting
                    0x07,  // MCSM2         Main Radio Control State Machine Configuration
                    0x0C,  // MCSM1         Main Radio Control State Machine Configuration
                    0x18,  // MCSM0         Main Radio Control State Machine Configuration
                    0x16,  // FOCCFG        Frequency Offset Compensation Configuration
                    0x6C,  // BSCFG         Bit Synchronization Configuration
                    0x03,  // AGCCTRL2      AGC Control
                    0x40,  // AGCCTRL1      AGC Control
                    0x91,  // AGCCTRL0      AGC Control
                    0x02,  // WOREVT1       High Byte Event0 Timeout
                    0x26,  // WOREVT0       Low Byte Event0 Timeout
                    0x09,  // WORCTRL       Wake On Radio Control
                    0x56,  // FREND1        Front End RX Configuration
                    0x17,  // FREND0        Front End TX Configuration
                    0xA9,  // FSCAL3        Frequency Synthesizer Calibration
                    0x0A,  // FSCAL2        Frequency Synthesizer Calibration
                    0x00,  // FSCAL1        Frequency Synthesizer Calibration
                    0x11,  // FSCAL0        Frequency Synthesizer Calibration
                    0x41,  // RCCTRL1       RC Oscillator Configuration
                    0x00,  // RCCTRL0       RC Oscillator Configuration
                    0x59,  // FSTEST        Frequency Synthesizer Calibration Control,
                    0x7F,  // PTEST         Production Test
                    0x3F,  // AGCTEST       AGC Test
                    0x81,  // TEST2         Various Test Settings
                    0x3F,  // TEST1         Various Test Settings
                    0x0B   // TEST0         Various Test Settings
               };

static uint8_t cc1100_GFSK_38_4_kb[CFG_REGISTER] = {
                    0x07,  // IOCFG2        GDO2 Output Pin Configuration
                    0x2E,  // IOCFG1        GDO1 Output Pin Configuration
                    0x80,  // IOCFG0        GDO0 Output Pin Configuration
                    0x07,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
                    0x57,  // SYNC1         Sync Word, High Byte
                    0x43,  // SYNC0         Sync Word, Low Byte
                    0x3E,  // PKTLEN        Packet Length
                    0x0E,  // PKTCTRL1      Packet Automation Control
                    0x45,  // PKTCTRL0      Packet Automation Control
                    0xFF,  // ADDR          Device Address
                    0x00,  // CHANNR        Channel Number
                    0x06,  // FSCTRL1       Frequency Synthesizer Control
                    0x00,  // FSCTRL0       Frequency Synthesizer Control
                    0x21,  // FREQ2         Frequency Control Word, High Byte
                    0x65,  // FREQ1         Frequency Control Word, Middle Byte
                    0x6A,  // FREQ0         Frequency Control Word, Low Byte
                    0xCA,  // MDMCFG4       Modem Configuration
                    0x83,  // MDMCFG3       Modem Configuration
                    0x13,  // MDMCFG2       Modem Configuration
                    0xA0,  // MDMCFG1       Modem Configuration
                    0xF8,  // MDMCFG0       Modem Configuration
                    0x34,  // DEVIATN       Modem Deviation Setting
                    0x07,  // MCSM2         Main Radio Control State Machine Configuration
                    0x0C,  // MCSM1         Main Radio Control State Machine Configuration
                    0x18,  // MCSM0         Main Radio Control State Machine Configuration
                    0x16,  // FOCCFG        Frequency Offset Compensation Configuration
                    0x6C,  // BSCFG         Bit Synchronization Configuration
                    0x43,  // AGCCTRL2      AGC Control
                    0x40,  // AGCCTRL1      AGC Control
                    0x91,  // AGCCTRL0      AGC Control
                    0x02,  // WOREVT1       High Byte Event0 Timeout
                    0x26,  // WOREVT0       Low Byte Event0 Timeout
                    0x09,  // WORCTRL       Wake On Radio Control
                    0x56,  // FREND1        Front End RX Configuration
                    0x17,  // FREND0        Front End TX Configuration
                    0xA9,  // FSCAL3        Frequency Synthesizer Calibration
                    0x0A,  // FSCAL2        Frequency Synthesizer Calibration
                    0x00,  // FSCAL1        Frequency Synthesizer Calibration
                    0x11,  // FSCAL0        Frequency Synthesizer Calibration
                    0x41,  // RCCTRL1       RC Oscillator Configuration
                    0x00,  // RCCTRL0       RC Oscillator Configuration
                    0x59,  // FSTEST        Frequency Synthesizer Calibration Control,
                    0x7F,  // PTEST         Production Test
                    0x3F,  // AGCTEST       AGC Test
                    0x81,  // TEST2         Various Test Settings
                    0x3F,  // TEST1         Various Test Settings
                    0x0B   // TEST0         Various Test Settings
               };

static uint8_t cc1100_GFSK_100_kb[CFG_REGISTER] = {
                    0x07,  // IOCFG2        GDO2 Output Pin Configuration
                    0x2E,  // IOCFG1        GDO1 Output Pin Configuration
                    0x80,  // IOCFG0        GDO0 Output Pin Configuration
                    0x07,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
                    0x57,  // SYNC1         Sync Word, High Byte
                    0x43,  // SYNC0         Sync Word, Low Byte
                    0x3E,  // PKTLEN        Packet Length
                    0x0E,  // PKTCTRL1      Packet Automation Control
                    0x45,  // PKTCTRL0      Packet Automation Control
                    0xFF,  // ADDR          Device Address
                    0x00,  // CHANNR        Channel Number
                    0x08,  // FSCTRL1       Frequency Synthesizer Control
                    0x00,  // FSCTRL0       Frequency Synthesizer Control
                    0x21,  // FREQ2         Frequency Control Word, High Byte
                    0x65,  // FREQ1         Frequency Control Word, Middle Byte
                    0x6A,  // FREQ0         Frequency Control Word, Low Byte
                    0x5B,  // MDMCFG4       Modem Configuration
                    0xF8,  // MDMCFG3       Modem Configuration
                    0x13,  // MDMCFG2       Modem Configuration
                    0xA0,  // MDMCFG1       Modem Configuration
                    0xF8,  // MDMCFG0       Modem Configuration
                    0x47,  // DEVIATN       Modem Deviation Setting
                    0x07,  // MCSM2         Main Radio Control State Machine Configuration
                    0x0C,  // MCSM1         Main Radio Control State Machine Configuration
                    0x18,  // MCSM0         Main Radio Control State Machine Configuration
                    0x1D,  // FOCCFG        Frequency Offset Compensation Configuration
                    0x1C,  // BSCFG         Bit Synchronization Configuration
                    0xC7,  // AGCCTRL2      AGC Control
                    0x00,  // AGCCTRL1      AGC Control
                    0xB2,  // AGCCTRL0      AGC Control
                    0x02,  // WOREVT1       High Byte Event0 Timeout
                    0x26,  // WOREVT0       Low Byte Event0 Timeout
                    0x09,  // WORCTRL       Wake On Radio Control
                    0xB6,  // FREND1        Front End RX Configuration
                    0x17,  // FREND0        Front End TX Configuration
                    0xEA,  // FSCAL3        Frequency Synthesizer Calibration
                    0x0A,  // FSCAL2        Frequency Synthesizer Calibration
                    0x00,  // FSCAL1        Frequency Synthesizer Calibration
                    0x11,  // FSCAL0        Frequency Synthesizer Calibration
                    0x41,  // RCCTRL1       RC Oscillator Configuration
                    0x00,  // RCCTRL0       RC Oscillator Configuration
                    0x59,  // FSTEST        Frequency Synthesizer Calibration Control,
                    0x7F,  // PTEST         Production Test
                    0x3F,  // AGCTEST       AGC Test
                    0x81,  // TEST2         Various Test Settings
                    0x3F,  // TEST1         Various Test Settings
                    0x0B   // TEST0         Various Test Settings
               };

static uint8_t cc1100_MSK_250_kb[CFG_REGISTER] = {
                    0x07,  // IOCFG2        GDO2 Output Pin Configuration
                    0x2E,  // IOCFG1        GDO1 Output Pin Configuration
                    0x80,  // IOCFG0        GDO0 Output Pin Configuration
                    0x07,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
                    0x57,  // SYNC1         Sync Word, High Byte
                    0x43,  // SYNC0         Sync Word, Low Byte
                    0x3E,  // PKTLEN        Packet Length
                    0x0E,  // PKTCTRL1      Packet Automation Control
                    0x45,  // PKTCTRL0      Packet Automation Control
                    0xFF,  // ADDR          Device Address
                    0x00,  // CHANNR        Channel Number
                    0x0B,  // FSCTRL1       Frequency Synthesizer Control
                    0x00,  // FSCTRL0       Frequency Synthesizer Control
                    0x21,  // FREQ2         Frequency Control Word, High Byte
                    0x65,  // FREQ1         Frequency Control Word, Middle Byte
                    0x6A,  // FREQ0         Frequency Control Word, Low Byte
                    0x2D,  // MDMCFG4       Modem Configuration
                    0x3B,  // MDMCFG3       Modem Configuration
                    0x73,  // MDMCFG2       Modem Configuration
                    0xA0,  // MDMCFG1       Modem Configuration
                    0xF8,  // MDMCFG0       Modem Configuration
                    0x00,  // DEVIATN       Modem Deviation Setting
                    0x07,  // MCSM2         Main Radio Control State Machine Configuration
                    0x0C,  // MCSM1         Main Radio Control State Machine Configuration
                    0x18,  // MCSM0         Main Radio Control State Machine Configuration
                    0x1D,  // FOCCFG        Frequency Offset Compensation Configuration
                    0x1C,  // BSCFG         Bit Synchronization Configuration
                    0xC7,  // AGCCTRL2      AGC Control
                    0x00,  // AGCCTRL1      AGC Control
                    0xB2,  // AGCCTRL0      AGC Control
                    0x02,  // WOREVT1       High Byte Event0 Timeout
                    0x26,  // WOREVT0       Low Byte Event0 Timeout
                    0x09,  // WORCTRL       Wake On Radio Control
                    0xB6,  // FREND1        Front End RX Configuration
                    0x17,  // FREND0        Front End TX Configuration
                    0xEA,  // FSCAL3        Frequency Synthesizer Calibration
                    0x0A,  // FSCAL2        Frequency Synthesizer Calibration
                    0x00,  // FSCAL1        Frequency Synthesizer Calibration
                    0x11,  // FSCAL0        Frequency Synthesizer Calibration
                    0x41,  // RCCTRL1       RC Oscillator Configuration
                    0x00,  // RCCTRL0       RC Oscillator Configuration
                    0x59,  // FSTEST        Frequency Synthesizer Calibration Control,
                    0x7F,  // PTEST         Production Test
                    0x3F,  // AGCTEST       AGC Test
                    0x81,  // TEST2         Various Test Settings
                    0x3F,  // TEST1         Various Test Settings
                    0x0B   // TEST0         Various Test Settings
               };

static uint8_t cc1100_MSK_500_kb[CFG_REGISTER] = {
                    0x07,  // IOCFG2        GDO2 Output Pin Configuration
                    0x2E,  // IOCFG1        GDO1 Output Pin Configuration
                    0x80,  // IOCFG0        GDO0 Output Pin Configuration
                    0x07,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
                    0x57,  // SYNC1         Sync Word, High Byte
                    0x43,  // SYNC0         Sync Word, Low Byte
                    0x3E,  // PKTLEN        Packet Length
                    0x0E,  // PKTCTRL1      Packet Automation Control
                    0x45,  // PKTCTRL0      Packet Automation Control
                    0xFF,  // ADDR          Device Address
                    0x00,  // CHANNR        Channel Number
                    0x0C,  // FSCTRL1       Frequency Synthesizer Control
                    0x00,  // FSCTRL0       Frequency Synthesizer Control
                    0x21,  // FREQ2         Frequency Control Word, High Byte
                    0x65,  // FREQ1         Frequency Control Word, Middle Byte
                    0x6A,  // FREQ0         Frequency Control Word, Low Byte
                    0x0E,  // MDMCFG4       Modem Configuration
                    0x3B,  // MDMCFG3       Modem Configuration
                    0x73,  // MDMCFG2       Modem Configuration
                    0xA0,  // MDMCFG1       Modem Configuration
                    0xF8,  // MDMCFG0       Modem Configuration
                    0x00,  // DEVIATN       Modem Deviation Setting
                    0x07,  // MCSM2         Main Radio Control State Machine Configuration
                    0x0C,  // MCSM1         Main Radio Control State Machine Configuration
                    0x18,  // MCSM0         Main Radio Control State Machine Configuration
                    0x1D,  // FOCCFG        Frequency Offset Compensation Configuration
                    0x1C,  // BSCFG         Bit Synchronization Configuration
                    0xC7,  // AGCCTRL2      AGC Control
                    0x40,  // AGCCTRL1      AGC Control
                    0xB2,  // AGCCTRL0      AGC Control
                    0x02,  // WOREVT1       High Byte Event0 Timeout
                    0x26,  // WOREVT0       Low Byte Event0 Timeout
                    0x09,  // WORCTRL       Wake On Radio Control
                    0xB6,  // FREND1        Front End RX Configuration
                    0x17,  // FREND0        Front End TX Configuration
                    0xEA,  // FSCAL3        Frequency Synthesizer Calibration
                    0x0A,  // FSCAL2        Frequency Synthesizer Calibration
                    0x00,  // FSCAL1        Frequency Synthesizer Calibration
                    0x19,  // FSCAL0        Frequency Synthesizer Calibration
                    0x41,  // RCCTRL1       RC Oscillator Configuration
                    0x00,  // RCCTRL0       RC Oscillator Configuration
                    0x59,  // FSTEST        Frequency Synthesizer Calibration Control,
                    0x7F,  // PTEST         Production Test
                    0x3F,  // AGCTEST       AGC Test
                    0x81,  // TEST2         Various Test Settings
                    0x3F,  // TEST1         Various Test Settings
                    0x0B   // TEST0         Various Test Settings
               };

static uint8_t cc1100_OOK_4_8_kb[CFG_REGISTER] = {
                    0x06,  // IOCFG2        GDO2 Output Pin Configuration
                    0x2E,  // IOCFG1        GDO1 Output Pin Configuration
                    0x06,  // IOCFG0        GDO0 Output Pin Configuration
                    0x47,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
                    0x57,  // SYNC1         Sync Word, High Byte
                    0x43,  // SYNC0         Sync Word, Low Byte
                    0xFF,  // PKTLEN        Packet Length
                    0x04,  // PKTCTRL1      Packet Automation Control
                    0x05,  // PKTCTRL0      Packet Automation Control
                    0x00,  // ADDR          Device Address
                    0x00,  // CHANNR        Channel Number
                    0x06,  // FSCTRL1       Frequency Synthesizer Control
                    0x00,  // FSCTRL0       Frequency Synthesizer Control
                    0x21,  // FREQ2         Frequency Control Word, High Byte
                    0x65,  // FREQ1         Frequency Control Word, Middle Byte
                    0x6A,  // FREQ0         Frequency Control Word, Low Byte
                    0x87,  // MDMCFG4       Modem Configuration
                    0x83,  // MDMCFG3       Modem Configuration
                    0x3B,  // MDMCFG2       Modem Configuration
                    0x22,  // MDMCFG1       Modem Configuration
                    0xF8,  // MDMCFG0       Modem Configuration
                    0x15,  // DEVIATN       Modem Deviation Setting
                    0x07,  // MCSM2         Main Radio Control State Machine Configuration
                    0x30,  // MCSM1         Main Radio Control State Machine Configuration
                    0x18,  // MCSM0         Main Radio Control State Machine Configuration
                    0x14,  // FOCCFG        Frequency Offset Compensation Configuration
                    0x6C,  // BSCFG         Bit Synchronization Configuration
                    0x07,  // AGCCTRL2      AGC Control
                    0x00,  // AGCCTRL1      AGC Control
                    0x92,  // AGCCTRL0      AGC Control
                    0x87,  // WOREVT1       High Byte Event0 Timeout
                    0x6B,  // WOREVT0       Low Byte Event0 Timeout
                    0xFB,  // WORCTRL       Wake On Radio Control
                    0x56,  // FREND1        Front End RX Configuration
                    0x17,  // FREND0        Front End TX Configuration
                    0xE9,  // FSCAL3        Frequency Synthesizer Calibration
                    0x2A,  // FSCAL2        Frequency Synthesizer Calibration
                    0x00,  // FSCAL1        Frequency Synthesizer Calibration
                    0x1F,  // FSCAL0        Frequency Synthesizer Calibration
                    0x41,  // RCCTRL1       RC Oscillator Configuration
                    0x00,  // RCCTRL0       RC Oscillator Configuration
                    0x59,  // FSTEST        Frequency Synthesizer Calibration Control
                    0x7F,  // PTEST         Production Test
                    0x3F,  // AGCTEST       AGC Test
                    0x81,  // TEST2         Various Test Settings
                    0x35,  // TEST1         Various Test Settings
                    0x09,  // TEST0         Various Test Settings
               };

               //Patable index: -30  -20- -15  -10   0    5    7    10 dBm
static uint8_t patable_power_315[8] = {0x17,0x1D,0x26,0x69,0x51,0x86,0xCC,0xC3};
static uint8_t patable_power_433[8] = {0x6C,0x1C,0x06,0x3A,0x51,0x85,0xC8,0xC0};
static uint8_t patable_power_868[8] = {0x03,0x17,0x1D,0x26,0x50,0x86,0xCD,0xC0};
static uint8_t patable_power_915[8] = {0x0B,0x1B,0x6D,0x67,0x50,0x85,0xC9,0xC1};
//static uint8_t patable_power_2430[8] = {0x44,0x84,0x46,0x55,0xC6,0x6E,0x9A,0xFE};

//----------------------------------[END]---------------------------------------

//-------------------------[CC1100 reset function]------------------------------
void CC1100::reset(void)                  // reset defined in cc1100 datasheet
{
    digitalWrite(SS_PIN, LOW);
    delayMicroseconds(10);
    digitalWrite(SS_PIN, HIGH);
    delayMicroseconds(40);

    spi_write_strobe(SRES);
    delay(1);
}
//-----------------------------[END]--------------------------------------------

//------------------------[set Power Down]--------------------------------------
void CC1100::powerdown(void)
{
    sidle();
    spi_write_strobe(SPWD);               // CC1100 Power Down
}
//-----------------------------[end]--------------------------------------------

//---------------------------[WakeUp]-------------------------------------------
void CC1100::wakeup(void)
{
    digitalWrite(SS_PIN, LOW);
    delayMicroseconds(10);
    digitalWrite(SS_PIN, HIGH);
    delayMicroseconds(10);
    receive();                            // go to RX Mode
}
//-----------------------------[end]--------------------------------------------

//---------------------[CC1100 set debug level]---------------------------------
uint8_t CC1100::set_debug_level(uint8_t set_debug_level = 1)  //default ON
{
    debug_level = set_debug_level;        //set debug level of CC1101 outputs

    return debug_level;
}
//-----------------------------[end]--------------------------------------------

//---------------------[CC1100 get debug level]---------------------------------
uint8_t CC1100::get_debug_level(void)
{
    return debug_level;
}
//-----------------------------[end]--------------------------------------------

//----------------------[CC1100 init functions]---------------------------------
uint8_t CC1100::begin(volatile uint8_t &My_addr)
{
    uint8_t partnum, version;
    extern int cc1100_freq_select, cc1100_mode_select, cc1100_channel_select;

    pinMode(GDO0, INPUT);                 //setup AVR GPIO ports
    pinMode(GDO2, INPUT);

    set_debug_level(set_debug_level());   //set debug level of CC1101 outputs

    if(debug_level > 0){
          printf("Init CC1100...\r\n");
    }

    spi_begin();                          //inits SPI Interface
    reset();                              //CC1100 init reset

    spi_write_strobe(SFTX);delayMicroseconds(100);//flush the TX_fifo content
    spi_write_strobe(SFRX);delayMicroseconds(100);//flush the RX_fifo content

    partnum = spi_read_register(PARTNUM); //reads CC1100 partnumber
    version = spi_read_register(VERSION); //reads CC1100 version number

    //checks if valid Chip ID is found. Usualy 0x03 or 0x14. if not -> abort
    if(version == 0x00 || version == 0xFF){
        if(debug_level > 0){
            printf("no CC11xx found!\r\n");
        }
        return FALSE;
    }

    if(debug_level > 0){
          printf("Partnumber: 0x%02X\r\n", partnum);
          printf("Version   : 0x%02X\r\n", version);
    }




    //set modulation mode
    set_mode(cc1100_mode_select);

    //set ISM band
    set_ISM(cc1100_freq_select);

    //set channel
    set_channel(cc1100_channel_select);

    //set output power amplifier
    set_output_power_level(0);            //set PA to 0dBm as default

    //set my receiver address
    set_myaddr(My_addr);                  //My_Addr from EEPROM to global variable

    if(debug_level > 0){
          printf("...done!\r\n");
    }

    receive();                                  //set CC1100 in receive mode

    return TRUE;
}
//-------------------------------[end]------------------------------------------

//-----------------[finish's the CC1100 operation]------------------------------
void CC1100::end(void)
{
    powerdown();                          //power down CC1100
}
//-------------------------------[end]------------------------------------------

//-----------------------[show all CC1100 registers]----------------------------
void CC1100::show_register_settings(void)
{
    if(debug_level > 0){
        uint8_t config_reg_verify[CFG_REGISTER],Patable_verify[CFG_REGISTER];

        spi_read_burst(READ_BURST,config_reg_verify,CFG_REGISTER);  //reads all 47 config register from cc1100
        spi_read_burst(PATABLE_BURST,Patable_verify,8);             //reads output power settings from cc1100

        //show_main_settings();
        printf("Config Register:\r\n");

        for(uint8_t i = 0 ; i < CFG_REGISTER; i++)  //showes rx_buffer for debug
        {
            printf("0x%02X ", config_reg_verify[i]);
            if(i==9 || i==19 || i==29 || i==39) //just for beautiful output style
            {
                printf("\r\n");
            }
        }
        printf("\r\n");
        printf("PaTable:\r\n");

        for(uint8_t i = 0 ; i < 8; i++)         //showes rx_buffer for debug
        {
            printf("0x%02X ", Patable_verify[i]);
        }
        printf("\r\n");
    }
}
//-------------------------------[end]------------------------------------------

//--------------------------[show settings]-------------------------------------
void CC1100::show_main_settings(void)
{
     extern volatile uint8_t My_addr;
     extern int cc1100_mode_select, cc1100_freq_select, cc1100_channel_select;

     printf("Mode: %d\r\n", cc1100_mode_select);
     printf("Frequency: %d\r\n", cc1100_freq_select);
     printf("Channel: %d\r\n", cc1100_channel_select);
     printf("My_Addr: %d\r\n", My_addr);
}
//-------------------------------[end]------------------------------------------

//----------------------------[idle mode]---------------------------------------
uint8_t CC1100::sidle(void)
{
    uint8_t marcstate;

    spi_write_strobe(SIDLE);              //sets to idle first. must be in

    marcstate = 0xFF;                     //set unknown/dummy state value

    while(marcstate != 0x01)              //0x01 = sidle
    {
        marcstate = (spi_read_register(MARCSTATE) & 0x1F); //read out state of cc1100 to be sure in RX
        //printf("marcstate_rx: 0x%02X\r", marcstate);
    }
    //Serial.println();
    delayMicroseconds(100);
    return TRUE;
}
//-------------------------------[end]------------------------------------------

//---------------------------[transmit mode]------------------------------------
uint8_t CC1100::transmit(void)
{
    uint8_t marcstate;

    sidle();                              //sets to idle first.
    spi_write_strobe(STX);                //sends the data over air

    marcstate = 0xFF;                     //set unknown/dummy state value

    while(marcstate != 0x01)              //0x01 = ILDE after sending data
    {
        marcstate = (spi_read_register(MARCSTATE) & 0x1F); //read out state of cc1100 to be sure in IDLE and TX is finished
        //printf("marcstate_tx: 0x%02X ",marcstate);
    }
    //printf("\r\n");
    delayMicroseconds(100);
    return TRUE;
}
///-------------------------------[end]------------------------------------------

//---------------------------[receive mode]-------------------------------------
uint8_t CC1100::receive(void)
{
    uint8_t marcstate;

    sidle();                              //sets to idle first.
    spi_write_strobe(SRX);                //writes receive strobe (receive mode)

    marcstate = 0xFF;                     //set unknown/dummy state value

    while(marcstate != 0x0D)              //0x0D = RX
    {
        marcstate = (spi_read_register(MARCSTATE) & 0x1F); //read out state of cc1100 to be sure in RX
        //printf("marcstate_rx: 0x%02X\r", marcstate);
    }
    //printf("\r\n");
    delayMicroseconds(100);
    return TRUE;
}
//-------------------------------[end]------------------------------------------

//------------[enables WOR Mode  EVENT0 ~1890ms; rx_timeout ~235ms]--------------------
void CC1100::wor_enable()
{
/*
    EVENT1 = WORCTRL[6:4] -> Datasheet page 88
    EVENT0 = (750/Xtal)*(WOREVT1<<8+WOREVT0)*2^(5*WOR_RES) = (750/26Meg)*65407*2^(5*0) = 1.89s

                        (WOR_RES=0;RX_TIME=0)               -> Datasheet page 80
i.E RX_TIMEOUT = EVENT0*       (3.6038)      *26/26Meg = 235.8ms
                        (WOR_RES=0;RX_TIME=1)               -> Datasheet page 80
i.E.RX_TIMEOUT = EVENT0*       (1.8029)      *26/26Meg = 117.9ms
*/
    sidle();

    spi_write_register(MCSM0, 0x18);    //FS Autocalibration
    spi_write_register(MCSM2, 0x01);    //MCSM2.RX_TIME = 1b

    // configure EVENT0 time
    spi_write_register(WOREVT1, 0xFF);  //High byte Event0 timeout
    spi_write_register(WOREVT0, 0x7F);  //Low byte Event0 timeout

    // configure EVENT1 time
    spi_write_register(WORCTRL, 0x78);  //WOR_RES=0b; tEVENT1=0111b=48d -> 48*(750/26MHz)= 1.385ms

    spi_write_strobe(SFRX);             //flush RX buffer
    spi_write_strobe(SWORRST);          //resets the WOR timer to the programmed Event 1
    spi_write_strobe(SWOR);             //put the radio in WOR mode when CSn is released

    delayMicroseconds(100);
}
//-------------------------------[end]------------------------------------------

//------------------------[disable WOR Mode]-------------------------------------
void CC1100::wor_disable()
{
    sidle();                            //exit WOR Mode
    spi_write_register(MCSM2, 0x07);    //stay in RX. No RX timeout
}
//-------------------------------[end]------------------------------------------

//------------------------[resets WOR Timer]------------------------------------
void CC1100::wor_reset()
{
    sidle();                            //go to IDLE
    spi_write_register(MCSM2, 0x01);    //MCSM2.RX_TIME = 1b
    spi_write_strobe(SFRX);             //flush RX buffer
    spi_write_strobe(SWORRST);          //resets the WOR timer to the programmed Event 1
    spi_write_strobe(SWOR);             //put the radio in WOR mode when CSn is released

    delayMicroseconds(100);
}
//-------------------------------[end]------------------------------------------

//-------------------------[tx_payload_burst]-----------------------------------
uint8_t CC1100::tx_payload_burst(uint8_t my_addr, uint8_t rx_addr,
                              uint8_t *txbuffer, uint8_t length)
{
    txbuffer[0] = length-1;
    txbuffer[1] = rx_addr;
    txbuffer[2] = my_addr;

    spi_write_burst(TXFIFO_BURST,txbuffer,length); //writes TX_Buffer +1 because of pktlen must be also transfered

    if(debug_level > 0){
        printf("TX_FIFO: ");
        for(uint8_t i = 0 ; i < length; i++)       //TX_fifo debug out
        {
             printf("0x%02X ", txbuffer[i]);
        }
        printf("\r\n");
  }
  return TRUE;
}
//-------------------------------[end]------------------------------------------

//------------------[rx_payload_burst - package received]-----------------------
uint8_t CC1100::rx_payload_burst(uint8_t rxbuffer[], uint8_t &pktlen)
{
    uint8_t bytes_in_RXFIFO = 0;
    uint8_t res = 0;

    bytes_in_RXFIFO = spi_read_register(RXBYTES);              //reads the number of bytes in RXFIFO

    if((bytes_in_RXFIFO & 0x7F) && !(bytes_in_RXFIFO & 0x80))  //if bytes in buffer and no RX Overflow
    {
        spi_read_burst(RXFIFO_BURST, rxbuffer, bytes_in_RXFIFO);
        pktlen = rxbuffer[0];
        res = TRUE;
    }
    else
    {
        if(debug_level > 0){
            printf("no bytes in RX buffer or RX Overflow!: ");printf("0x%02X \r\n", bytes_in_RXFIFO);
        }
        sidle();                                                  //set to IDLE
        spi_write_strobe(SFRX);delayMicroseconds(100);            //flush RX Buffer
        receive();                                                //set to receive mode
        res = FALSE;
    }

    return res;
}
//-------------------------------[end]------------------------------------------

//---------------------------[sent packet]--------------------------------------
uint8_t CC1100::sent_packet(uint8_t my_addr, uint8_t rx_addr, uint8_t *txbuffer,
                            uint8_t pktlen,  uint8_t tx_retries)
{
    uint8_t pktlen_ack, rssi, lqi;                              //default package len for ACK
    uint8_t rxbuffer[FIFOBUFFER];
    uint8_t tx_retries_count = 0;
    uint8_t from_sender;
    uint16_t ackWaitCounter = 0;

    if(pktlen > (FIFOBUFFER - 1))
    {
        printf("ERROR: package size overflow\r\n");
        return FALSE;
    }

    do                                                          //sent package out with retries
    {
        tx_payload_burst(my_addr, rx_addr, txbuffer, pktlen);   //loads the data in cc1100 buffer
        transmit();                                             //sents data over air
        receive();                                              //receive mode

        if(rx_addr == BROADCAST_ADDRESS){                       //no wait acknowledge if sent to broadcast address or tx_retries = 0
            return TRUE;                                        //successful sent to BROADCAST_ADDRESS
        }

        while (ackWaitCounter < ACK_TIMEOUT )                   //wait for an acknowledge
        {
            if (packet_available() == TRUE)                     //if RF package received check package acknowge
            {
                from_sender = rx_addr;                          //the original message sender address
                rx_fifo_erase(rxbuffer);                        //erase RX software buffer
                rx_payload_burst(rxbuffer, pktlen_ack);         //reads package in buffer
                check_acknowledge(rxbuffer, pktlen_ack, from_sender, my_addr); //check if received message is an acknowledge from client
                return TRUE;                                    //package successfully sent
            }
            else{
                ackWaitCounter++;                               //increment ACK wait counter
                delay(1);                                       //delay to give receiver time
            }
        }

        ackWaitCounter = 0;                                     //resets the ACK_Timeout
        tx_retries_count++;                                     //increase tx retry counter

        if(debug_level > 0){                                    //debug output messages
            printf(" #:");
            printf("0x%02X \r\n", tx_retries_count);
        }
    }while(tx_retries_count <= tx_retries);                     //while count of retries is reaches

    return FALSE;                                               //sent failed. too many retries
}
//-------------------------------[end]------------------------------------------

//--------------------------[sent ACKNOWLEDGE]------------------------------------
void CC1100::sent_acknowledge(uint8_t my_addr, uint8_t tx_addr)
{
    uint8_t pktlen = 0x06;                                      //complete Pktlen for ACK packet
    uint8_t tx_buffer[0x06];                                    //tx buffer array init

    tx_buffer[3] = 'A'; tx_buffer[4] = 'c'; tx_buffer[5] = 'k'; //fill buffer with ACK Payload

    tx_payload_burst(my_addr, tx_addr, tx_buffer, pktlen);      //load payload to CC1100
    transmit();                                                 //sent package over the air
    receive();                                                  //set CC1100 in receive mode

    if(debug_level > 0){                                        //debut output
        printf("Ack_sent!\r\n");
    }
}
//-------------------------------[end]------------------------------------------
//----------------------[check if Packet is received]---------------------------
uint8_t CC1100::packet_available()
{
    if(digitalRead(GDO2) == TRUE)                           //if RF package received
    {
        if(spi_read_register(IOCFG2) == 0x06)               //if sync word detect mode is used
        {
            while(digitalRead(GDO2) == TRUE){               //wait till sync word is fully received
                printf("!\r\n");
            }                                                  //for sync word receive
        }

        if(debug_level > 0){
             //printf("Pkt->:\r\n");
        }

        return TRUE;
    }
    return FALSE;
}
//-------------------------------[end]------------------------------------------

//------------------[check Payload for ACK or Data]-----------------------------
uint8_t CC1100::get_payload(uint8_t rxbuffer[], uint8_t &pktlen, uint8_t &my_addr,
                            uint8_t &sender, int8_t &rssi_dbm, uint8_t &lqi)
{
    uint8_t crc;

    rx_fifo_erase(rxbuffer);                               //delete rx_fifo bufffer

    if(rx_payload_burst(rxbuffer, pktlen) == FALSE)        //read package in buffer
    {
        rx_fifo_erase(rxbuffer);                           //delete rx_fifo bufffer
        return FALSE;                                    //exit
    }
    else
    {
        my_addr = rxbuffer[1];                             //set receiver address to my_addr
        sender = rxbuffer[2];

        if(check_acknowledge(rxbuffer, pktlen, sender, my_addr) == TRUE) //acknowlage received?
        {
            rx_fifo_erase(rxbuffer);                       //delete rx_fifo bufffer
            return FALSE;                                //Ack received -> finished
        }
        else                                               //real data, and sent acknowladge
        {
            rssi_dbm = rssi_convert(rxbuffer[pktlen + 1]); //converts receiver strength to dBm
            lqi = lqi_convert(rxbuffer[pktlen + 2]);       //get rf quialtiy indicator
            crc = check_crc(lqi);                          //get packet CRC

            if(debug_level > 0){                           //debug output messages
                if(rxbuffer[1] == BROADCAST_ADDRESS)       //if my receiver address is BROADCAST_ADDRESS
                {
                    printf("BROADCAST message\r\n");
                }

                printf("RX_FIFO:");
                for(uint8_t i = 0 ; i < pktlen + 1; i++)   //showes rx_buffer for debug
                {
                    printf("0x%02X ", rxbuffer[i]);
                }
                printf("| 0x%02X 0x%02X |", rxbuffer[pktlen+1], rxbuffer[pktlen+2]);
                printf("\r\n");

                printf("RSSI:%d ", rssi_dbm);
                printf("LQI:");printf("0x%02X ", lqi);
                printf("CRC:");printf("0x%02X ", crc);
                printf("\r\n");
            }

            my_addr = rxbuffer[1];                         //set receiver address to my_addr
            sender = rxbuffer[2];                          //set from_sender address

            if(my_addr != BROADCAST_ADDRESS)               //send only ack if no BROADCAST_ADDRESS
            {
                sent_acknowledge(my_addr, sender);           //sending acknowlage to sender!
            }

            return TRUE;
        }
        return FALSE;
    }
}
//-------------------------------[end]------------------------------------------

//-------------------------[check ACKNOWLEDGE]------------------------------------
uint8_t CC1100::check_acknowledge(uint8_t *rxbuffer, uint8_t pktlen, uint8_t sender, uint8_t my_addr)
{
    int8_t rssi_dbm;
    uint8_t crc, lqi;

    if((pktlen == 0x05 && \
        rxbuffer[1] == my_addr || rxbuffer[1] == BROADCAST_ADDRESS) && \
        rxbuffer[2] == sender && \
        rxbuffer[3] == 'A' && rxbuffer[4] == 'c' && rxbuffer[5] == 'k')   //acknowledge received!
        {
            if(rxbuffer[1] == BROADCAST_ADDRESS){                           //if receiver address BROADCAST_ADDRESS skip acknowledge
                if(debug_level > 0){
                    printf("BROADCAST ACK\r\n");
                }
                return FALSE;
            }
            rssi_dbm = rssi_convert(rxbuffer[pktlen + 1]);
            lqi = lqi_convert(rxbuffer[pktlen + 2]);
            crc = check_crc(lqi);

            if(debug_level > 0){
                printf("ACK! ");
                printf("RSSI:%i ",rssi_dbm);
                printf("LQI:0x%02X ",lqi);
                printf("CRC:0x%02X\r\n",crc);
            }
            return TRUE;
        }
    return FALSE;
}
//-------------------------------[end]------------------------------------------

//------------[check if Packet is received within defined time in ms]-----------
uint8_t CC1100::wait_for_packet(uint16_t milliseconds)
{
    for(uint16_t i = 0; i < milliseconds; i++)
        {
            delay(1);                 //delay till system has data available
            if (packet_available())
            {
                return TRUE;
            }
        }
    return FALSE;
}
//-------------------------------[end]------------------------------------------

//--------------------------[tx_fifo_erase]-------------------------------------
void CC1100::tx_fifo_erase(uint8_t *txbuffer)
{
    memset(txbuffer, 0, sizeof(FIFOBUFFER));  //erased the TX_fifo array content to "0"
}
//-------------------------------[end]------------------------------------------

//--------------------------[rx_fifo_erase]-------------------------------------
void CC1100::rx_fifo_erase(uint8_t *rxbuffer)
{
    memset(rxbuffer, 0, sizeof(FIFOBUFFER)); //erased the RX_fifo array content to "0"
}
//-------------------------------[end]------------------------------------------

//------------------------[set CC1100 address]----------------------------------
void CC1100::set_myaddr(uint8_t addr)
{
    spi_write_register(ADDR,addr);          //stores MyAddr in the CC1100
}
//-------------------------------[end]------------------------------------------

//---------------------------[set channel]--------------------------------------
void CC1100::set_channel(uint8_t channel)
{
    spi_write_register(CHANNR,channel);   //stores the new channel # in the CC1100

    return;
}
//-------------------------------[end]------------------------------------------

//-[set modulation mode 1 = GFSK_1_2_kb; 2 = GFSK_38_4_kb; 3 = GFSK_100_kb; 4 = MSK_250_kb; 5 = MSK_500_kb; 6 = OOK_4_8_kb]-
void CC1100::set_mode(uint8_t mode)
{

    switch (mode)
    {
        case 0x01:
                    spi_write_burst(WRITE_BURST,cc1100_GFSK_1_2_kb,CFG_REGISTER);
                    break;
        case 0x02:
                    spi_write_burst(WRITE_BURST,cc1100_GFSK_38_4_kb,CFG_REGISTER);
                    break;
        case 0x03:
                    spi_write_burst(WRITE_BURST,cc1100_GFSK_100_kb,CFG_REGISTER);
                    break;
        case 0x04:
                    spi_write_burst(WRITE_BURST,cc1100_MSK_250_kb,CFG_REGISTER);
                    break;
        case 0x05:
                    spi_write_burst(WRITE_BURST,cc1100_MSK_500_kb,CFG_REGISTER);
                    break;
        case 0x06:
                    spi_write_burst(WRITE_BURST,cc1100_OOK_4_8_kb,CFG_REGISTER);
                    break;
        default:
                    spi_write_burst(WRITE_BURST,cc1100_GFSK_100_kb,CFG_REGISTER);
                    break;
    }
    return;
}
//------------------------------------------end]-----------------------------------

//---------[set ISM Band 1=315MHz; 2=433MHz; 3=868MHz; 4=915MHz]----------------
void CC1100::set_ISM(uint8_t ism_freq)
{
    uint8_t freq2, freq1, freq0;

    switch (ism_freq)                                                       //loads the RF freq which is defined in cc1100_freq_select
    {
        case 0x01:                                                          //315MHz
                    freq2=0x0C;
                    freq1=0x1D;
                    freq0=0x89;
                    spi_write_burst(PATABLE_BURST,patable_power_315,8);
                    break;
        case 0x02:                                                          //433.92MHz
                    freq2=0x10;
                    freq1=0xB0;
                    freq0=0x71;
                    spi_write_burst(PATABLE_BURST,patable_power_433,8);
                    break;
        case 0x03:                                                          //868.3MHz
                    freq2=0x21;
                    freq1=0x65;
                    freq0=0x6A;
                    spi_write_burst(PATABLE_BURST,patable_power_868,8);
                    break;
        case 0x04:                                                          //915MHz
                    freq2=0x23;
                    freq1=0x31;
                    freq0=0x3B;
                    spi_write_burst(PATABLE_BURST,patable_power_915,8);
                    break;
        /*
        case 0x05:                                                          //2430MHz
                    freq2=0x5D;
                    freq1=0x76;
                    freq0=0x27;
                    spi_write_burst(PATABLE_BURST,patable_power_2430,8);
                    break;
        */
        default:                                                             //default is 868.3MHz
                    freq2=0x21;
                    freq1=0x65;
                    freq0=0x6A;
                    spi_write_burst(PATABLE_BURST,patable_power_868,8);    //sets up output power ramp register
                    break;
    }

    spi_write_register(FREQ2,freq2);                                         //stores the new freq setting for defined ISM band
    spi_write_register(FREQ1,freq1);
    spi_write_register(FREQ0,freq0);

     return;
}
//-------------------------------[end]------------------------------------------

//--------------------------[set frequency]-------------------------------------
void CC1100::set_patable(uint8_t *patable_arr)
{
    spi_write_burst(PATABLE_BURST,patable_arr,8);   //writes output power settings to cc1100    "104us"
}
//-------------------------------[end]------------------------------------------

//-------------------------[set output power]-----------------------------------
void CC1100::set_output_power_level(int8_t dBm)
{
    uint8_t pa = 0xC0;

    if      (dBm <= -30) pa = 0x00;
    else if (dBm <= -20) pa = 0x01;
    else if (dBm <= -15) pa = 0x02;
    else if (dBm <= -10) pa = 0x03;
    else if (dBm <= 0)   pa = 0x04;
    else if (dBm <= 5)   pa = 0x05;
    else if (dBm <= 7)   pa = 0x06;
    else if (dBm <= 10)  pa = 0x07;

    spi_write_register(FREND0,pa);
}
//-------------------------------[end]------------------------------------------

//-------[set Modulation type 2-FSK=0; GFSK=1; ASK/OOK=3; 4-FSK=4; MSK=7]------
void CC1100::set_modulation_type(uint8_t cfg)
{
    uint8_t data;
    data = spi_read_register(MDMCFG2);
    data = (data & 0x8F) | (((cfg) << 4) & 0x70);
    spi_write_register(MDMCFG2, data);
    //printf("MDMCFG2: 0x%02X\n", data);
}
//-------------------------------[end]-----------------------------------------

//------------------------[set preamble Len]-----------------------------------
void CC1100::set_preamble_len(uint8_t cfg)
{
    uint8_t data;
    data = spi_read_register(MDMCFG1);
    data = (data & 0x8F) | (((cfg) << 4) & 0x70);
    spi_write_register(MDMCFG1, data);
    //printf("MDMCFG2: 0x%02X\n", data);
}
//-------------------------------[end]-----------------------------------------

//-------------------[set modem datarate and deviant]--------------------------
void CC1100::set_datarate(uint8_t mdmcfg4, uint8_t mdmcfg3, uint8_t deviant)
{
    spi_write_register(MDMCFG4, mdmcfg4);
    spi_write_register(MDMCFG3, mdmcfg3);
    spi_write_register(DEVIATN, deviant);
}
//-------------------------------[end]-----------------------------------------

//----------------------[set sync mode no sync=0;]-----------------------------
void CC1100::set_sync_mode(uint8_t cfg) // 0=no sync word; 1,2 = 16bit sync word, 3= 32bit sync word
{
    uint8_t data;
    data = spi_read_register(MDMCFG2);
    data = (data & 0xF8) | (cfg & 0x07);
    spi_write_register(MDMCFG2, data);
    //printf("MDMCFG2: 0x%02X\n", data);
}
//-------------------------------[end]-----------------------------------------

//---------------[set FEC ON=TRUE; OFF=FALSE]----------------------------------
void CC1100::set_fec(uint8_t cfg)
{
    uint8_t data;
    data = spi_read_register(MDMCFG1);
    data = (data & 0x7F) | (((cfg) << 7) & 0x80);
    spi_write_register(MDMCFG1, data);
    printf("MDMCFG1: 0x%02X\n", data);
}
//-------------------------------[end]------------------------------------------

//---------------[set data_whitening ON=TRUE; OFF=FALSE]------------------------
void CC1100::set_data_whitening(uint8_t cfg)
{
    uint8_t data;
    data = spi_read_register(PKTCTRL0);
    data = (data & 0xBF) | (((cfg) << 6) & 0x40);
    spi_write_register(PKTCTRL0, data);
    //printf("PKTCTRL0: 0x%02X\n", data);
}
//-------------------------------[end]-----------------------------------------

//------------[set manchester encoding ON=TRUE; OFF=FALSE]---------------------
void CC1100::set_manchester_encoding(uint8_t cfg)
{
    uint8_t data;
    data = spi_read_register(MDMCFG2);
    data = (data & 0xF7) | (((cfg) << 3) & 0x08);
    spi_write_register(MDMCFG2, data);
    //printf("MDMCFG2: 0x%02X\n", data);
}
//-------------------------------[end]------------------------------------------

//--------------------------[rssi_convert]--------------------------------------
int8_t CC1100::rssi_convert(uint8_t Rssi_hex)
{
    int8_t rssi_dbm;
    int16_t Rssi_dec;

    Rssi_dec = Rssi_hex;        //convert unsigned to signed

    if(Rssi_dec >= 128){
        rssi_dbm=((Rssi_dec-256)/2)-RSSI_OFFSET_868MHZ;
    }
    else{
        if(Rssi_dec<128){
            rssi_dbm=((Rssi_dec)/2)-RSSI_OFFSET_868MHZ;
        }
    }
    return rssi_dbm;
}
//-------------------------------[end]------------------------------------------

//----------------------------[lqi convert]-------------------------------------
uint8_t CC1100::lqi_convert(uint8_t lqi)
{
    return (lqi & 0x7F);
}
//-------------------------------[end]------------------------------------------

//----------------------------[check crc]---------------------------------------
uint8_t CC1100::check_crc(uint8_t lqi)
{
    return (lqi & 0x80);
}
//-------------------------------[end]------------------------------------------

/*
//----------------------------[get temp]----------------------------------------
uint8_t CC1100::get_temp(uint8_t *ptemp_Arr)
{
    const uint8_t num_samples = 8;
    uint16_t adc_result = 0;
    uint32_t temperature = 0;

    sidle();                              //sets CC1100 into IDLE
    spi_write_register(PTEST,0xBF);       //enable temp sensor
    delay(50);                            //wait a bit

    for(uint8_t i=0;i<num_samples;i++)    //sampling analog temperature value
    {
        adc_result += analogRead(GDO0);
        delay(1);
    }
    adc_result = adc_result / num_samples;
    //Serial.println(adc_result);

    temperature = (adc_result * CC1100_TEMP_ADC_MV) / CC1100_TEMP_CELS_CO;

    ptemp_Arr[0] = temperature / 10;      //cut last digit
    ptemp_Arr[1] = temperature % 10;      //isolate last digit

    if(debug_level > 0){
        Serial.print(F("Temp:"));Serial.print(ptemp_Arr[0]);Serial.print(F("."));Serial.println(ptemp_Arr[1]);
    }

    spi_write_register(PTEST,0x7F);       //writes 0x7F back to PTest (app. note)

    receive();
    return (*ptemp_Arr);
}
*/
//-------------------------------[end]------------------------------------------

//|==================== SPI Initialisation for CC1100 =========================|
void CC1100::spi_begin(void)
{
     int x = 0;
     //printf ("init SPI bus... ");
     if ((x = wiringPiSPISetup (0, 8000000)) < 0)  //4MHz SPI speed
     {
          if(debug_level > 0){
          printf ("ERROR: wiringPiSPISetup failed!\r\n");
          }
     }
     else{
          //printf ("wiringSPI is up\r\n");
          }
}
//------------------[write register]--------------------------------
void CC1100::spi_write_register(uint8_t spi_instr, uint8_t value)
{
     uint8_t tbuf[2] = {0};
     tbuf[0] = spi_instr | WRITE_SINGLE_BYTE;
     tbuf[1] = value;
     uint8_t len = 2;
     wiringPiSPIDataRW (0, tbuf, len) ;

     return;
}
//|============================ Ein Register lesen ============================|
uint8_t CC1100::spi_read_register(uint8_t spi_instr)
{
     uint8_t value;
     uint8_t rbuf[2] = {0};
     rbuf[0] = spi_instr | READ_SINGLE_BYTE;
     uint8_t len = 2;
     wiringPiSPIDataRW (0, rbuf, len) ;
     value = rbuf[1];
     //printf("SPI_arr_0: 0x%02X\n", rbuf[0]);
     //printf("SPI_arr_1: 0x%02X\n", rbuf[1]);
     return value;
}
//|========================= ein Kommando schreiben ========================|
void CC1100::spi_write_strobe(uint8_t spi_instr)
{
     uint8_t tbuf[1] = {0};
     tbuf[0] = spi_instr;
     //printf("SPI_data: 0x%02X\n", tbuf[0]);
     wiringPiSPIDataRW (0, tbuf, 1) ;
 }
//|======= Mehrere hintereinanderliegende Register auf einmal lesen =======|
void CC1100::spi_read_burst(uint8_t spi_instr, uint8_t *pArr, uint8_t len)
{
     uint8_t rbuf[len + 1];
     rbuf[0] = spi_instr | READ_BURST;
     wiringPiSPIDataRW (0, rbuf, len + 1) ;
     for (uint8_t i=0; i<len ;i++ )
     {
          pArr[i] = rbuf[i+1];
          //printf("SPI_arr_read: 0x%02X\n", pArr[i]);
     }
}
//|======= Mehrere hintereinanderliegende Register auf einmal schreiben =======|
void CC1100::spi_write_burst(uint8_t spi_instr, uint8_t *pArr, uint8_t len)
{
     uint8_t tbuf[len + 1];
     tbuf[0] = spi_instr | WRITE_BURST;
     for (uint8_t i=0; i<len ;i++ )
     {
          tbuf[i+1] = pArr[i];
          //printf("SPI_arr_write: 0x%02X\n", tbuf[i+1]);
     }
     wiringPiSPIDataRW (0, tbuf, len + 1) ;
}
//|================================= END =======================================|
