/*------------------------------------------------------------------------------
'                     CC1100 Arduino Library
'                     ----------------------
'
'
'  by
'
'  
'  module contains helper helper code from other people. Thx for that
'-----------------------------------------------------------------------------*/
#include <cc1100.h>

//CC1100 cc1100;

//-------------------[global EEPROM default settings 868 Mhz]-------------------
static uint8_t cc1100_GFSK_1_2_kb[] EEMEM = {
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
					0x19,  // MCSM0         Main Radio Control State Machine Configuration
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

static uint8_t cc1100_GFSK_38_4_kb[] EEMEM = {
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
				  	0x19,  // MCSM0         Main Radio Control State Machine Configuration
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

static uint8_t cc1100_GFSK_100_kb[] EEMEM = {
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

static uint8_t cc1100_MSK_250_kb[] EEMEM = {
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

static uint8_t cc1100_MSK_500_kb[] EEMEM = {
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

static uint8_t cc1100_OOK_4_8_kb[] EEMEM = {
					0x06,  // IOCFG2        GDO2 Output Pin Configuration
				  	0x2E,  // IOCFG1        GDO1 Output Pin Configuration
					0x06,  // IOCFG0        GDO0 Output Pin Configuration
					0x47,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
					0xD3,  // SYNC1         Sync Word, High Byte
					0x91,  // SYNC0         Sync Word, Low Byte
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
static uint8_t patable_power_315[] EEMEM = {0x17,0x1D,0x26,0x69,0x51,0x86,0xCC,0xC3};
static uint8_t patable_power_433[] EEMEM = {0x6C,0x1C,0x06,0x3A,0x51,0x85,0xC8,0xC0};
static uint8_t patable_power_868[] EEMEM = {0x03,0x17,0x1D,0x26,0x50,0x86,0xCD,0xC0};
static uint8_t patable_power_915[] EEMEM = {0x0B,0x1B,0x6D,0x67,0x50,0x85,0xC9,0xC1};
//static uint8_t patable_power_2430[] EEMEM = {0x44,0x84,0x46,0x55,0xC6,0x6E,0x9A,0xFE};

//-----------------------------[END]--------------------------------------------

//-------------------------[CC1100 reset function]------------------------------
void CC1100::reset(void)			      // reset defined in cc1100 datasheet
{
  digitalWrite(SS_PIN, LOW);
	_delay_us(10);
	digitalWrite(SS_PIN, HIGH);
	_delay_us(40);

	spi_write_strobe(SRES);
	_delay_ms(1);
}
//-----------------------------[END]--------------------------------------------

//------------------------[set Power Down]--------------------------------------
void CC1100::powerdown(void)
{
	silde();
	spi_write_strobe(SPWD);           // CC!!00 Power Down
}
//-----------------------------[end]--------------------------------------------

//---------------------------[WakeUp]-------------------------------------------
void CC1100::wakeup(void)
{
	digitalWrite(SS_PIN, LOW);
	_delay_us(10);
	digitalWrite(SS_PIN, HIGH);
	_delay_us(10);
	receive();// go to RX Mode
}
//-----------------------------[end]--------------------------------------------

//-------------------[CC1100 init functions "8.9ms"]----------------------------
uint8_t CC1100::begin(volatile uint8_t &My_addr)
{
	uint8_t cc1100_freq_select, cc1100_mode_select, cc1100_channel_select;
	uint8_t partnum, version;

	pinMode(GDO0, INPUT);						       //setup SPI ports
	pinMode(GDO2, INPUT);

	#ifdef CC1100_DEBUG
		Serial.println(F("Init CC1100..."));
	#endif

	spi_begin();
	reset();								//CC1100 reset

	spi_write_strobe(SFTX);_delay_us(100); //flush the TX_fifo content
	spi_write_strobe(SFRX);_delay_us(100); //flush the RX_fifo content

	partnum = spi_read_register(PARTNUM);
	version = spi_read_register(VERSION);

	//checks if valid Chip ID is found. Usualy 0x03 or 0x14. if not -> abort
	if(version == 0x00 || version == 0xFF)
		{
			#if CC1100_DEBUG == 1
				Serial.print(F("no CC11xx found!"));
				Serial.println();
				end();												  //CC1100 Powerdown and disable SPI bus
			#endif

			return FALSE;
		}

	#if CC1100_DEBUG == 1
		Serial.print(F("Partnumber:"));
		uart_puthex_byte(partnum);
		Serial.println();

		Serial.print(F("Version:"));
		uart_puthex_byte(version);
		Serial.println();
	#endif

	//reads setting parameters from eeprom
	My_addr = eeprom_read_byte((const uint8_t*)EEPROM_ADDRESS_CC1100_MY_ADDR);
	cc1100_freq_select = eeprom_read_byte((const uint8_t*)EEPROM_ADDRESS_CC1100_FREQUENCY);
	cc1100_mode_select = eeprom_read_byte((const uint8_t*)EEPROM_ADDRESS_CC1100_MODE);
	cc1100_channel_select = eeprom_read_byte((const uint8_t*)EEPROM_ADDRESS_CC1100_CHANNEL);

	//if no valid settings are available, CC100 Powerdown and SPI disable
	if(My_addr == 0xFF ||
		cc1100_freq_select == 0xFF ||
		cc1100_mode_select == 0xFF ||
		cc1100_channel_select == 0xFF)
		{
			#if CC1100_DEBUG == 1
				Serial.print(F("no EEPROM settings..."));
				Serial.println();
				end();												//CC1100 Powerdown and disable SPI bus
			#endif

			return FALSE;
		}

	//set modulation mode
	set_mode(cc1100_mode_select);

	//set ISM band
	set_ISM(cc1100_freq_select);

	//set channel
	set_channel(cc1100_channel_select);

	//set output power amplifier
	set_output_power_level(0);					//set PA to 0dBm as default

	//set my receiver address
	set_myaddr(My_addr);						    //My_Addr from EEPROM to global variable

	#if CC1100_DEBUG == 1
		Serial.println(F("...done"));
	#endif

	//set CC1100 in receive mode
	receive();

	return TRUE;
}
//-------------------------------[end]------------------------------------------

//-----------------[finish's the CC1100 operation]------------------------------
void CC1100::end(void)
{
	powerdown();																	//power down CC1100
	spi_end();																		//disable SPI Interface
}
//-------------------------------[end]------------------------------------------

//-----------------------[show all CC1100 registers]----------------------------
void CC1100::show_register_settings(void)
{
	#if CC1100_DEBUG == 1
		uint8_t config_reg_verify[CFG_REGISTER],Patable_verify[CFG_REGISTER];

		spi_read_burst(READ_BURST,config_reg_verify,CFG_REGISTER);	//reads all 47 config register
		spi_read_burst(PATABLE_BURST,Patable_verify,8);				//reads output power settings

		//show_main_settings();
		Serial.println(F("Cfg_reg:"));

		for(uint8_t i = 0 ; i < CFG_REGISTER; i++) 		//showes rx_buffer for debug
			{
				uart_puthex_byte(config_reg_verify[i]);Serial.print(F(" "));
				if(i==9 || i==19 || i==29 || i==39)				//just for beautiful output style
					{
						Serial.println();
					}
			}
			Serial.println();
			Serial.println(F("PaTable:"));

			for(uint8_t i = 0 ; i < 8; i++) 						//showes rx_buffer for debug
				{
					uart_puthex_byte(Patable_verify[i]);Serial.print(F(" "));
				}
		Serial.println();
	#endif
}
//-------------------------------[end]------------------------------------------

//--------------------------[show settings]-------------------------------------
void CC1100::show_main_settings(void)
{
	#if CC1100_DEBUG == 1
		Serial.print(F("Mode:"));
		Serial.print(eeprom_read_byte((const uint8_t*)EEPROM_ADDRESS_CC1100_MODE));
		Serial.println();

		Serial.print(F("Frequency:"));
		Serial.print(eeprom_read_byte((const uint8_t*)EEPROM_ADDRESS_CC1100_FREQUENCY));
		Serial.println();

		Serial.print(F("Channel:"));
		Serial.print(eeprom_read_byte((const uint8_t*)EEPROM_ADDRESS_CC1100_CHANNEL));
		Serial.println();

		Serial.print(F("My_Addr:"));
		Serial.print(eeprom_read_byte((const uint8_t*)EEPROM_ADDRESS_CC1100_MY_ADDR));
		Serial.println();
	#endif
}
//-------------------------------[end]------------------------------------------

//----------------------------[idle mode]---------------------------------------
void CC1100::silde(void)
{
	uint8_t marcstate;

	spi_write_strobe(SIDLE);								  //sets to idle first. must be in
	marcstate = 0xFF;								        	//set unknown/dummy state value

	while(marcstate != 0x01)						    	//0x01 = SILDE
	{
		marcstate = (spi_read_register(MARCSTATE) & 0x1F);		//read out state of cc1100 to be sure in RX
		//uart_puthex_byte(marcstate);
	}
	//Serial.println();
}
//-------------------------------[end]------------------------------------------

//---------------------------[transmit mode]------------------------------------
void CC1100::transmit(void)
{
	uint8_t marcstate;

	spi_write_strobe(STX);								     //sends the data over air
	marcstate = 0xFF;								           //set unknown/dummy state value

	while(marcstate != 0x01)						       //0x01 = ILDE after sending data
	{
		marcstate = (spi_read_register(MARCSTATE) & 0x1F);		 //read out state of cc1100 to be sure in IDLE and TX is finished
		//uart_puthex_byte(marcstate);
		_delay_us(100);                          //must be in for ever reason
	}
	//Serial.println();

	spi_write_strobe(SFTX);_delay_us(100);		 //flush the Tx_fifo content -> a must for interrupt handling
}
//-------------------------------[end]------------------------------------------

//---------------------------[receive mode]-------------------------------------
void CC1100::receive(void)
{
	uint8_t marcstate;

	silde();			                             //sets to idle first.
	spi_write_strobe(SRX);                     //writes receive strobe (receive mode)
	marcstate = 0xFF;	                         //set unknown/dummy state value

	while(marcstate != 0x0D)                   //0x0D = RX
	{
		marcstate = (spi_read_register(MARCSTATE) & 0x1F);		//read out state of cc1100 to be sure in RX
		//uart_puthex_byte(marcstate);
	}
	//Serial.println();
}
//-------------------------------[end]------------------------------------------

//-------------------------[tx_payload_burst]-----------------------------------
void CC1100::tx_payload_burst(uint8_t my_addr, uint8_t rx_addr,
	                               uint8_t *txbuffer, uint8_t length)
{
	txbuffer[0] = length-1;
	txbuffer[1] = rx_addr;
	txbuffer[2] = my_addr;

	spi_write_burst(TXFIFO_BURST,txbuffer,length);	//writes TX_Buffer +1 because of pktlen must be also transfered

	#if CC1100_DEBUG == 1
		Serial.print(F("TX_FIFO:"));
		for(uint8_t i = 0 ; i < length; i++) 			   //TX_fifo debug out
			{
			uart_puthex_byte(txbuffer[i]);
			}
		Serial.println();
	#endif
}
//-------------------------------[end]------------------------------------------

//------------------[rx_payload_burst - package received]-----------------------
void CC1100::rx_payload_burst(uint8_t rxbuffer[], uint8_t &pktlen)
{
	uint8_t bytes_in_RXFIFO = spi_read_register(RXBYTES); //reads the number of bytes in RXFIFO

	if (bytes_in_RXFIFO & 0x7F && !(bytes_in_RXFIFO & 0x80))
		{
			pktlen = spi_read_register(RXFIFO_SINGLE_BYTE);		//received pktlen +1 for complete TX buffer
			rxbuffer[0] = pktlen;
			for(uint8_t i = 1;i < pktlen + 3;i++)			        //+3 because of i=1 and RSSI and LQI
				{
					rxbuffer[i] = spi_read_register(RXFIFO_SINGLE_BYTE);
				}
		}else
		{
				//Serial.print(F("bad RX buffer!"));
		}
	silde();
	spi_write_strobe(SFRX);_delay_us(100);
	receive();
}
//-------------------------------[end]------------------------------------------

//---------------------------[sent packet]--------------------------------------
uint8_t CC1100::sent_packet(uint8_t my_addr, uint8_t rx_addr, uint8_t *txbuffer,
                            uint8_t pktlen,  uint8_t tx_retries)
{
	uint8_t pktlen_ack;																			//default package len for ACK
	uint8_t rxbuffer[FIFOBUFFER];
	uint8_t tx_retries_count = 0;
	uint16_t ackWaitCounter = 0;

	do										                                  //sent package out with retries
	{
		tx_payload_burst(my_addr, rx_addr, txbuffer, pktlen);	//loads the data in cc1100 buffer
		transmit();												                    //sents data over air
		receive();												                    //receive mode

		if(rx_addr == BROADCAST_ADDRESS || tx_retries == 0){  //no wait acknowge if sent to broadcast address or tx_retries = 0
			return TRUE;																				//successful sent to BROADCAST_ADDRESS
		}

		while (ackWaitCounter < ACK_TIMEOUT ) 						  	//Wait for an acknowge
		{
			if (packet_available() == TRUE)					      			//if RF package received check package acknowge
			{
				uint8_t from_sender = rx_addr;                  	//the original message sender address
				rx_fifo_erase(rxbuffer);
				rx_payload_burst(rxbuffer, pktlen_ack);						//reads package in buffer
				check_acknolage(rxbuffer, pktlen_ack, from_sender, my_addr);
				return TRUE;																			//package successfully sent
			}
			else{
				ackWaitCounter++;                   							//increment ACK wait counter
				_delay_ms(1);																			//delay to give receiver time
			}
		}

		ackWaitCounter = 0;																	 //resets the ACK_Timeout
		tx_retries_count++;																	 //increase tx retry counter

		#if CC1100_DEBUG == 1																 //debug output messages
			Serial.print(F(" #:"));
			uart_puthex_byte(tx_retries_count-1);
			Serial.println();
		#endif
	}while(tx_retries_count <= tx_retries);                //while count of retries is reaches

	return FALSE;																					 //sent failed. too many retries
}
//-------------------------------[end]------------------------------------------

//--------------------------[sent ACKNOLAGE]------------------------------------
void CC1100::sent_acknolage(uint8_t my_addr, uint8_t tx_addr)
{
	uint8_t pktlen = 0x06;																//complete Pktlen for ACK packet
	uint8_t tx_buffer[0x06];															//tx buffer array init

	tx_buffer[3] = 'A';	tx_buffer[4] = 'c';	tx_buffer[5] = 'k';	//fill buffer with ACK Payload

	tx_payload_burst(my_addr, tx_addr, tx_buffer, pktlen);//load payload to CC1100
	transmit();																						//sent package over the air
	receive();																						//set CC1100 in receive mode

	#if CC1100_DEBUG == 1																	//debut output
		//Serial.println();
		Serial.println(F("Ack_sent!"));
	#endif
}
//-------------------------------[end]------------------------------------------
//----------------------[check if Packet is received]---------------------------
uint8_t CC1100::packet_available()
{
	if(digitalRead(GDO2) == TRUE)														//if RF package received
		{
			//Serial.println("GDO2!");
			if(spi_read_register(IOCFG2) == 0x06)								//if sync word detect mode is used
			{
				while(digitalRead(GDO2) == TRUE){                 //for sync word receive
					//Serial.println(F("!"));
				};
			}

			#if CC1100_DEBUG == 1
				//Serial.println("Pkt->:");
			#endif

			return TRUE;
		}
	return FALSE;
}
//-------------------------------[end]------------------------------------------

//------------------[check Payload for ACK or Data]-----------------------------
uint8_t CC1100::get_payload(uint8_t rxbuffer[], uint8_t &pktlen, uint8_t &my_addr,
	                           uint8_t &sender, int8_t &rssi_dbm, uint8_t &lqi)
{
	rx_fifo_erase(rxbuffer);															//delete rx_fifo bufffer
	rx_payload_burst(rxbuffer, pktlen);							     	//read package in buffer

	if(pktlen == 0x00)																		//packet len not plausible?
		{
			#if CC1100_DEBUG == 1
				Serial.println(F("bad packet!"));
			#endif
			return FALSE;
		}

  my_addr = rxbuffer[1];														//set receiver address to my_addr
	sender = rxbuffer[2];

	if(check_acknolage(rxbuffer, pktlen, sender, my_addr) == TRUE) //acknowlage received?
		{
			return FALSE;																		 	//Ack received -> finished
		}
	else 													                       	//real data, and sent acknowladge
		{
			rssi_dbm = rssi_convert(rxbuffer[pktlen + 1]);		//converts receiver strength to dBm
			lqi = lqi_convert(rxbuffer[pktlen + 2]);
			uint8_t crc = check_crc(lqi);											//get rf quialtiy indicator

			#if CC1100_DEBUG == 1															//debug output messages
				if(rxbuffer[1] == BROADCAST_ADDRESS)						//if my receiver address is BROADCAST_ADDRESS
				{
					Serial.println(F("BROADCAST message"));
				}
				Serial.print(F("RX_FIFO:"));
				for(uint8_t i = 0 ; i < pktlen + 3; i++) 		    //showes rx_buffer for debug
					{
						uart_puthex_byte(rxbuffer[i]);
					}
				Serial.println();

				Serial.print(F("RSSI:"));uart_puti(rssi_dbm);Serial.print(F(" "));
				Serial.print(F("LQI:"));uart_puthex_byte(lqi);Serial.print(F(" "));
				Serial.print(F("CRC:"));uart_puthex_byte(crc);
				Serial.println();
			#endif

			my_addr = rxbuffer[1];														//set receiver address to my_addr
			sender = rxbuffer[2];															//set from_sender address

			if(my_addr != BROADCAST_ADDRESS)									//send only ack if no BROADCAST_ADDRESS
			{
				sent_acknolage(my_addr, sender);								//sending acknolage to sender!
			}
			return TRUE;
		}
}
//-------------------------------[end]------------------------------------------

//-------------------------[check ACKNOLAGE]------------------------------------
uint8_t CC1100::check_acknolage(uint8_t *rxbuffer, uint8_t pktlen, uint8_t sender, uint8_t my_addr)
{
	//extern volatile uint8_t My_addr;
  //uint8_t tx_addr = rxbuffer[2];

	if((rxbuffer[1] == my_addr || rxbuffer[1] == BROADCAST_ADDRESS) && rxbuffer[2] == sender && rxbuffer[3] == 'A' && rxbuffer[4] == 'c' && rxbuffer[5] == 'k') 		 //acknolage received!
		{
			if(rxbuffer[1] == BROADCAST_ADDRESS){							//if receiver address BROADCAST_ADDRESS skip acknolage
				#if CC1100_DEBUG == 1
					Serial.println(F("BROADCAST ACK"));
				#endif
				return FALSE;
			}
			int8_t rssi_dbm = rssi_convert(rxbuffer[pktlen + 1]);
			uint8_t lqi = lqi_convert(rxbuffer[pktlen + 2]);
			uint8_t crc = check_crc(lqi);
			#if CC1100_DEBUG == 1
				//Serial.println();
				Serial.print(F("ACK! "));
				Serial.print(F("RSSI:"));uart_puti(rssi_dbm);Serial.print(F(" "));
				Serial.print(F("LQI:"));uart_puthex_byte(lqi);Serial.print(F(" "));
				Serial.print(F("CRC:"));uart_puthex_byte(crc);
				Serial.println();
			#endif
			return TRUE;
		}
	return FALSE;
}
//-------------------------------[end]------------------------------------------

//------------[check if Packet is received within defined time in ms]-----------
uint8_t CC1100::wait_for_packet(uint8_t milliseconds)
{
	for(uint8_t i = 0; i< milliseconds; i++)
		{
			_delay_ms(1);																			//delay till system has data available
			if (packet_available())
			{
				return TRUE;
			}
			else if(i == milliseconds)
			{
				Serial.println(F("no packet received!"));
				return FALSE;
			}
		}
}
//-------------------------------[end]------------------------------------------

//--------------------------[tx_fifo_erase]-------------------------------------
void CC1100::tx_fifo_erase(uint8_t *txbuffer)
{
	memset(txbuffer, 0, sizeof(FIFOBUFFER));				  //erased the TX_fifo array content to "0"
}
//-------------------------------[end]------------------------------------------

//--------------------------[rx_fifo_erase]-------------------------------------
void CC1100::rx_fifo_erase(uint8_t *rxbuffer)
{
 	memset(rxbuffer, 0, sizeof(FIFOBUFFER));				  //erased the RX_fifo array content to "0"
}
//-------------------------------[end]------------------------------------------

//------------------------[set CC1100 address]----------------------------------
void CC1100::set_myaddr(uint8_t addr)
{
	spi_write_register(ADDR,addr);									//stores MyAddr in the CC1100
	eeprom_write_byte((uint8_t*)EEPROM_ADDRESS_CC1100_MY_ADDR,addr);				//writes byte to eeprom
}
//-------------------------------[end]------------------------------------------

//---------------------------[set channel]--------------------------------------
void CC1100::set_channel(uint8_t channel)
{
	spi_write_register(CHANNR,channel);					//stores the new channel # in the CC1100
	eeprom_write_byte((uint8_t*)EEPROM_ADDRESS_CC1100_CHANNEL,channel);		//writes byte to eeprom
}
//-------------------------------[end]------------------------------------------

//-----------------------[set modulation mode]----------------------------------
void CC1100::set_mode(uint8_t mode)
{
	uint8_t Cfg_reg[CFG_REGISTER];

	switch (mode)
	{
		case 0x01:
					eeprom_read_block(Cfg_reg,cc1100_GFSK_1_2_kb,CFG_REGISTER);			  //sets up settings for GFSK 1,2 kbit mode/speed
					break;
		case 0x02:
					eeprom_read_block(Cfg_reg,cc1100_GFSK_38_4_kb,CFG_REGISTER);			//sets up settings for GFSK 38,4 kbit mode/speed
					break;
		case 0x03:
					eeprom_read_block(Cfg_reg,cc1100_GFSK_100_kb,CFG_REGISTER);				//sets up settings for GFSK 100 kbit mode/speed
					break;
		case 0x04:
					eeprom_read_block(Cfg_reg,cc1100_MSK_250_kb,CFG_REGISTER);				//sets up settings for GFSK 38,4 kbit mode/speed
					break;
		case 0x05:
					eeprom_read_block(Cfg_reg,cc1100_MSK_500_kb,CFG_REGISTER);				//sets up settings for GFSK 38,4 kbit mode/speed
					break;
		case 0x06:
					eeprom_read_block(Cfg_reg,cc1100_OOK_4_8_kb,CFG_REGISTER);				//sets up settings for GFSK 38,4 kbit mode/speed
					break;
		default:
					eeprom_read_block(Cfg_reg,cc1100_GFSK_38_4_kb,CFG_REGISTER);			//sets up settings for GFSK 38,4 kbit mode/speed
					mode = 0x02;
					break;
	}

	spi_write_burst(WRITE_BURST,Cfg_reg,CFG_REGISTER);											  //writes all 47 config register
	eeprom_write_byte((uint8_t*)EEPROM_ADDRESS_CC1100_MODE,mode);							//sets the cc1100 frequency
}
//-------------------------------[end]------------------------------------------

//---------------------------[set ISM Band]-------------------------------------
void CC1100::set_ISM(uint8_t ism_freq)
{
	uint8_t freq2, freq1, freq0;
  uint8_t Patable[8];

	switch (ism_freq) 												//loads the RF freq which is defined in cc1100_freq_select
	{
		case 0x01:															//315MHz
					freq2=0x0C;
					freq1=0x1D;
					freq0=0x89;
					eeprom_read_block(Patable,patable_power_315,8);
					break;
		case 0x02:															//433.92MHz
					freq2=0x10;
					freq1=0xB0;
					freq0=0x71;
					eeprom_read_block(Patable,patable_power_433,8);
					break;
		case 0x03:															//868.3MHz
					freq2=0x21;
					freq1=0x65;
					freq0=0x6A;
					eeprom_read_block(Patable,patable_power_868,8);
					break;
		case 0x04:															//915MHz
					freq2=0x23;
					freq1=0x31;
					freq0=0x3B;
					eeprom_read_block(Patable,patable_power_915,8);
					break;
		/*
		case 0x05:															//2430MHz
					freq2=0x5D;
					freq1=0x76;
					freq0=0x27;
					eeprom_read_block(Patable,patable_power_2430,8);
					break;
		*/
		default:															 //default is 868.3MHz
					freq2=0x21;
					freq1=0x65;
					freq0=0x6A;
					eeprom_read_block(Patable,patable_power_868,8);		//sets up output power ramp register
					ism_freq = 0x03;
					break;
	}

	spi_write_register(FREQ2,freq2);				//stores the new freq setting for defined ISM band
	spi_write_register(FREQ1,freq1);
	spi_write_register(FREQ0,freq0);

	spi_write_burst(PATABLE_BURST,Patable,8);								//writes output power settings to cc1100

	eeprom_write_byte((uint8_t*)EEPROM_ADDRESS_CC1100_FREQUENCY,ism_freq);	//selects the cc1100 frequency
}
//-------------------------------[end]------------------------------------------

/*
void CC1100::set_freq(uint32_t freq)
{
    uint32_t num;

    num = freq * (0x10000 / 1000000.0) / 26; //26MHz oscillator
    FREQ2 = num >> 16;
    FREQ1 = (num>>8) & 0xff;
    FREQ0 = num & 0xff;
}*/

//---------------------------[set PATABLE]--------------------------------------
void CC1100::set_patable(uint8_t *patable_arr)
{
	spi_write_burst(PATABLE_BURST,patable_arr,8);								//writes output power settings to cc1100	"104us"
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
//---------------[set data_whitening ON=TRUE; OFF=FALSE]-------------------------
void CC1100::set_data_whitening(uint8_t cfg)
{
	uint8_t data;
  data = spi_read_register(PKTCTRL0);
  data = (data & 0xBF) | (((cfg) << 6) & 0x40);
  spi_write_register(PKTCTRL0, data);
}
//-------------------------------[end]------------------------------------------
//------------[set manchester encoding ON=TRUE; OFF=FALSE]----------------------
void CC1100::set_manchaster_encoding(uint8_t cfg)
{
	uint8_t data;
  data = spi_read_register(MDMCFG2);
  data = (data & 0xF7) | (((cfg) << 3) & 0x08);
  spi_write_register(PKTCTRL0, data);
}
//-------------------------------[end]------------------------------------------


//--------------------------[rssi_convert]--------------------------------------
int8_t CC1100::rssi_convert(uint8_t Rssi_hex)
{
	int8_t rssi_dbm;
	int16_t Rssi_dec;

	Rssi_dec = Rssi_hex;		//convert unsigned to signed

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

//------------------[lqi convert]--------------------------------
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

//----------------------------[get temp]----------------------------------------
uint8_t CC1100::get_temp(uint8_t *ptemp_Arr)
{
	const uint8_t num_samples = 8;
  uint16_t adc_result = 0;
	uint32_t temperature = 0;

	silde();									            //sets CC1100 into IDLE
	spi_write_register(PTEST,0xBF);				//enable temp sensor
	_delay_ms(50);												//wait a bit

	for(uint8_t i=0;i<num_samples;i++)    //sampling analog temperature value
	{
    adc_result += analogRead(GDO0);
		_delay_ms(1);
  }
  adc_result = adc_result / num_samples;
	//Serial.println(adc_result);

	temperature = (adc_result * CC1100_TEMP_ADC_MV) / CC1100_TEMP_CELS_CO;

	ptemp_Arr[0] = temperature / 10;      //cut last digit
	ptemp_Arr[1] = temperature % 10;      //isolate last digit

	#if CC1100_DEBUG == 1
		Serial.print(F("Temp:"));Serial.print(ptemp_Arr[0]);Serial.print(F("."));Serial.println(ptemp_Arr[1]);
	#endif

	spi_write_register(PTEST,0x7F);				//writes 0x7F back to PTest (app. note)

	receive();
	return (*ptemp_Arr);
}
//-------------------------------[end]------------------------------------------

//|==================== SPI Initialisation for CC1100 =========================|
void CC1100::spi_begin(void)
{
	pinMode(SCK_PIN, OUTPUT);
	pinMode(MOSI_PIN, OUTPUT);
	pinMode(MISO_PIN, INPUT);
	pinMode(SS_PIN, OUTPUT);

	//|--- Aktivieren des SPI Master Interfaces, fosc = fclk / x --------|
/*
	Spi prescaler:
	SPI2X SPR1 SPR0
	  0     0     0    fosc/4
	  0     0     1    fosc/16
	  0     1     0    fosc/64
	  0     1     1    fosc/128
	  1     0     0    fosc/2
	  1     0     1    fosc/8
	  1     1     0    fosc/32
	  1     1     1    fosc/64
*/
   SPCR = ((1<<SPE)|               		// SPI Enable
           (0<<SPIE)|              		// SPI Interupt Enable
           (0<<DORD)|              		// Data Order (0:MSB first / 1:LSB first)
           (1<<MSTR)|              		// Master/Slave select
           (0<<SPR1)|(0<<SPR0)|   		// SPI Clock Rate
           (0<<CPOL)|             		// Clock Polarity (0:SCK low / 1:SCK hi when idle)
           (0<<CPHA));             		// Clock Phase (0:leading / 1:trailing edge sampling)

   SPSR =  (1<<SPI2X);             		// Double Clock Rate
}
//-------------------------------[end]------------------------------------------

//|==================== SPI Initialisation for CC1100 =========================|
void CC1100::spi_end(void)
{
	pinMode(SCK_PIN, INPUT);
	pinMode(MOSI_PIN, INPUT);
	pinMode(MISO_PIN, INPUT);
	pinMode(SS_PIN, INPUT);

   SPCR = ((0<<SPE) |               	// SPI Enable
           (0<<SPIE)|              		// SPI Interupt Enable
           (0<<DORD)|              		// Data Order (0:MSB first / 1:LSB first)
           (1<<MSTR)|              		// Master/Slave select
           (0<<SPR1)|(0<<SPR0)|   		// SPI Clock Rate ( 0 0 = osc/4; 0 1 = osc/16; 1 0 = osc/64; 1 1= 0sc/128)
           (0<<CPOL)|             		// Clock Polarity (0:SCK low / 1:SCK hi when idle)
           (0<<CPHA));             		// Clock Phase (0:leading / 1:trailing edge sampling)

   SPSR =  (0<<SPI2X);             		// Double Clock Rate
}
//-------------------------------[end]------------------------------------------
//|============================= SPI Transmission =============================|
uint8_t CC1100::spi_putc(uint8_t data)
{
	SPDR = data;
  while( !( SPSR & (1<<SPIF) ) ); 	// Warten bis Byte gesendet wurde

	return SPDR;
}
//|==================== schreibe strobe command  ==============================|
void CC1100::spi_write_strobe(uint8_t spi_instr)
{
	digitalWrite(SS_PIN, LOW);          // CS low
	while(digitalRead(MOSI_PIN) == 0);  // Wait until MOSI_PIN becomes LOW
	spi_putc(spi_instr);
	digitalWrite(SS_PIN, HIGH);         // CS high
}
//|======================= Status byte lesen ==================================|
/*
uint8_t CC1100::spi_read_status(uint8_t spi_instr)
{
	digitalWrite(SS_PIN, LOW);         // CS low
	while(digitalRead(MOSI_PIN) == 0); // Wait until MOSI_PIN becomes LOW
	spi_putc(spi_instr | Read_burst);
	spi_instr = spi_putc(0xff);
	digitalWrite(SS_PIN, HIGH);        // CS high

	return spi_instr;
}
*/
//|========================== Ein Register lesen ==============================|
uint8_t CC1100::spi_read_register(uint8_t spi_instr)
{
	digitalWrite(SS_PIN, LOW);         // CS low
	while(digitalRead(MOSI_PIN) == 0); // Wait until MOSI_PIN becomes LOW
	spi_putc(spi_instr | READ_SINGLE_BYTE);
	spi_instr = spi_putc(0xff);
	digitalWrite(SS_PIN, HIGH);        // CS high

	return spi_instr;
}
//|========== Mehrere hintereinanderliegende Register auf einmal lesen ========|
void CC1100::spi_read_burst(uint8_t spi_instr, uint8_t *pArr, uint8_t length)
{
	digitalWrite(SS_PIN, LOW);           // CS low
  while(digitalRead(MOSI_PIN) == 0);   //Wait until MOSI_PIN becomes LOW
	spi_putc(spi_instr | READ_BURST);

	for(uint8_t i=0; i<length ;i++)
		{
			pArr[i] = spi_putc(0xFF);
		}

	digitalWrite(SS_PIN, HIGH);
}
//|======================= Ein Register schreiben =============================|
void CC1100::spi_write_register(uint8_t spi_instr, uint8_t value)
{
	digitalWrite(SS_PIN, LOW);         // CS low
 	while(digitalRead(MOSI_PIN) == 0); //Wait until MOSI_PIN becomes LOW
	spi_putc(spi_instr | WRITE_SINGLE_BYTE);
  spi_putc(value);
  digitalWrite(SS_PIN, HIGH);
}
//|======= Mehrere hintereinanderliegende Register auf einmal schreiben =======|
void CC1100::spi_write_burst(uint8_t spi_instr, uint8_t *pArr, uint8_t length)
{
	digitalWrite(SS_PIN, LOW);         // CS low
 	while(digitalRead(MOSI_PIN) == 0); //Wait until MOSI_PIN becomes LOW
	spi_putc(spi_instr | WRITE_BURST);

	for(uint8_t i=0; i<length ;i++)
  	{
			spi_putc(pArr[i]);
  	}

  digitalWrite(SS_PIN, HIGH);
}
//|=================================== END ====================================|


//|=========================== UART helper ====================================|

/*************************************************************************
Function: uart_puthex_nibble()
Purpose:  transmit lower nibble as ASCII-hex to UART
Input:    byte value
Returns:  none
This functions has been added by Martin Thomas <eversmith@heizung-thomas.de>
Don't blame P. Fleury if it doesn't work ;-)
**************************************************************************/
void CC1100::uart_puthex_nibble(const unsigned char b)
{
  unsigned char  c = b & 0x0f;
  if (c>9) c += 'A'-10;
  else c += '0';
  Serial.write(c);								//uart_putc replaced to Serial.write
} /* uart_puthex_nibble */

/*************************************************************************
Function: uart_puthex_byte()
Purpose:  transmit upper and lower nibble as ASCII-hex to UART
Input:    byte value
Returns:  none
This functions has been added by Martin Thomas <eversmith@heizung-thomas.de>
Don't blame P. Fleury if it doesn't work ;-)
**************************************************************************/
void CC1100::uart_puthex_byte(const unsigned char b)
{
  uart_puthex_nibble(b>>4);
  uart_puthex_nibble(b);
} /* uart_puthex_byte */

/*************************************************************************
Function: uart_puti()
Purpose:  transmit integer as ASCII to UART
Input:    integer value
Returns:  none
This functions has been added by Martin Thomas <eversmith@heizung-thomas.de>
Don't blame P. Fleury if it doesn't work ;-)
**************************************************************************/
void CC1100::uart_puti(const int val)
{
  char buffer[sizeof(int)*8+1];
  Serial.print(itoa(val, buffer, 10));			//uart_puts replaced to Serial.print

}/* uart_puti */
//|================================= END ======================================|
