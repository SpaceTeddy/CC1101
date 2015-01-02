/*-------------------------------------------------------------------------------
'                     CC1100 Arduino Library
'                     ----------------------
'
'  
'  by
'
'  christian weithe
'  module contains helper helper code from other people. Thx for that
'-------------------------------------------------------------------------------*/
#include <cc1100.h>

CC1100 cc1100;

//-------------------[global EEPROM default settings 868 Mhz]---------------------------------

static uint8_t cc1100_GFSK_1_2_kb[] EEMEM =  {0x07,0x2E,0x80,0x07,0x57,0x43,0x3E,0x0D,0x45,0xFF,
										0x00,0x08,0x00,0x21,0x65,0x6A,0xF5,0x83,0x13,0xA0,
										0xF8,0x15,0x07,0x0C,0x19,0x16,0x6C,0x03,0x40,0x91,
										0x02,0x26,0x09,0x56,0x17,0xA9,0x0A,0x00,0x11,0x41,
										0x00,0x59,0x7F,0x3F,0x81,0x3F,0x0B};

static uint8_t cc1100_GFSK_38_4_kb[] EEMEM = {0x07,0x2E,0x80,0x07,0x57,0x43,0x3E,0x0E,0x45,0xFF,
										0x00,0x06,0x00,0x21,0x65,0x6A,0xCA,0x83,0x13,0xA0,
										0xF8,0x34,0x07,0x0C,0x19,0x16,0x6C,0x43,0x40,0x91,
										0x02,0x26,0x09,0x56,0x17,0xA9,0x0A,0x00,0x11,0x41,
										0x00,0x59,0x7F,0x3F,0x81,0x3F,0x0B};

static uint8_t cc1100_GFSK_100_kb[] EEMEM = {0x07,0x2E,0x80,0x07,0x57,0x43,0x3E,0x0E,0x45,0xFF,
									  0x00,0x08,0x00,0x21,0x65,0x6A,0x5B,0xF8,0x13,0xA0,
									  0xF8,0x47,0x07,0x0C,0x18,0x1D,0x1C,0xC7,0x00,0xB2,
									  0x02,0x26,0x09,0xB6,0x17,0xEA,0x0A,0x00,0x11,0x41,
									  0x00,0x59,0x7F,0x3F,0x81,0x3F,0x0B};

static uint8_t cc1100_MSK_250_kb[] EEMEM = {0x07,0x2E,0x80,0x07,0x57,0x43,0x3E,0x0E,0x45,0xFF,
									  0x00,0x0B,0x00,0x21,0x65,0x6A,0x2D,0x3B,0x73,0xA0,
									  0xF8,0x00,0x07,0x0C,0x18,0x1D,0x1C,0xC7,0x00,0xB2,
									  0x02,0x26,0x09,0xB6,0x17,0xEA,0x0A,0x00,0x11,0x41,
									  0x00,0x59,0x7F,0x3F,0x81,0x3F,0x0B};

static uint8_t cc1100_MSK_500_kb[] EEMEM = {0x07,0x2E,0x80,0x07,0x57,0x43,0x3E,0x0E,0x45,0xFF,
									  0x00,0x0C,0x00,0x21,0x65,0x6A,0x0E,0x3B,0x73,0xA0,
									  0xF8,0x00,0x07,0x0C,0x18,0x1D,0x1C,0xC7,0x40,0xB2,
									  0x02,0x26,0x09,0xB6,0x17,0xEA,0x0A,0x00,0x19,0x41,
									  0x00,0x59,0x7F,0x3F,0x81,0x3F,0x0B};

static uint8_t patable_power_315[] EEMEM = {0x17,0x1D,0x26,0x69,0x51,0x86,0xCC,0xC3};
static uint8_t patable_power_433[] EEMEM = {0x6C,0x1C,0x06,0x3A,0x51,0x85,0xC8,0xC0};
static uint8_t patable_power_868[] EEMEM = {0x0D,0x1C,0x34,0x67,0x60,0x85,0xCC,0xC3};
static uint8_t patable_power_915[] EEMEM = {0x0B,0x1B,0x6D,0x67,0x50,0x85,0xC9,0xC1};
//static uint8_t patable_power_2430[] EEMEM = {0x44,0x84,0x46,0x55,0xC6,0x6E,0x9A,0xFE};

//-----------------------------[END]------------------------------------------


//---------------[CC1100 reset functions "1052.25us"]-----------------------
void CC1100::cc1100_reset(void)			// reset defined in cc1100 datasheet
    {
    digitalWrite(SS_PIN, LOW);     		// CS low
	_delay_us(10);
	digitalWrite(SS_PIN, HIGH);      	// CS high
	_delay_us(40);
	
	spi_write(Sres,1);
	_delay_ms(1);
	}
//------------------------[END]-----------------------	

//-----------------[CC1100 init functions "8.9ms"]-----------------------
uint8_t CC1100::cc1100_init(void)
	{
	extern uint8_t My_addr;
	uint8_t cc1100_freq_select,cc1100_mode_select,cc1100_channel_select;
	
	uint8_t partnum, version;
	
	pinMode(GDO0, INPUT);								//setup ports 
	pinMode(GDO2, INPUT);
	pinMode(RX_LED, OUTPUT);
	pinMode(TX_LED, OUTPUT);
	
	#if CC1100_DEBUG == 1
		Serial.println(F("Init CC1100..."));
	#endif
	
	spi_init();											//default. can also be placed in main.c
	cc1100_reset();										//resets the cc1100
	
	spi_write(Sftx, 1);									//flush the TX_fifo content -> a must for interrupt handling
	spi_write(Sfrx, 1);									//flush the RX_fifo content -> a must for interrupt handling

	partnum = spi_read(Partnum_addr);
	version = spi_read(Version_addr);
	
	if(version == 0x00 || version == 0xFF)
		{
			#if CC1100_DEBUG == 1
				Serial.print(F("no CC11xx found!"));
				Serial.println();	
			#endif
			
			return FALSE;
		}
	
	#if CC1100_DEBUG == 1
		Serial.print(F("CC1100 Partnumber:"));
		uart_puthex_byte(partnum);
		Serial.println();	
		
		Serial.print(F("Version:"));
		uart_puthex_byte(version);
		Serial.println();
	#endif
	
	//reads setting parameters from eeprom
	My_addr = eeprom_read_byte((const uint8_t*)EEPROM_ADDRESS_CC1100_MY_ADDR);				//reads out saved My_addr byte
	cc1100_freq_select = eeprom_read_byte((const uint8_t*)EEPROM_ADDRESS_CC1100_FREQUENCY);	//reads out saved byte for 312, 433,868 or 915 MHz
	cc1100_mode_select = eeprom_read_byte((const uint8_t*)EEPROM_ADDRESS_CC1100_MODE);		//reads out saved byte for modulation mode
	cc1100_channel_select = eeprom_read_byte((const uint8_t*)EEPROM_ADDRESS_CC1100_CHANNEL);//reads out saved byte for modulation mode
	
	//set default parameter and transmit Mode. Must be always write first
	cc1100_set_mode(cc1100_mode_select);									//250kbits
	
	//set ISM band
	cc1100_set_ISM(cc1100_freq_select);										//868MHz
	
	//set channel
	cc1100_set_channel(cc1100_channel_select);								//writes channel from EEPROM to channel register
	
	//set my receiver address
	cc1100_set_myaddr(My_addr);												//My_Addr
	
	cc1100_show_main_settings();
	
	#if CC1100_DEBUG == 1
	Serial.println(F("...done"));
	#endif
	
	cc1100_receive();								//set CC1100 in receive mode
	return TRUE;
	}	
//----------------------[end]---------------------------------------
void CC1100::cc1100_show_register_settings(void)
	{
		uint8_t config_reg_verify[CFG_REGISTER],Patable_verify[CFG_REGISTER];

		cc1100.spi_read_burst(Read_burst,config_reg_verify,CFG_REGISTER,1);					//reads all 47 config register from cc1100	"359.63us"
		cc1100.spi_read_burst(Patable_read_burst,Patable_verify,8,1);				//reads output power settings from cc1100	"104us"
	
		cc1100.cc1100_show_main_settings();
		Serial.println(F("Cfg_reg:"));
	
		for(uint8_t i = 0 ; i < CFG_REGISTER; i++) 		//showes rx_buffer for debug
			{
				uart_puthex_byte(config_reg_verify[i]);Serial.print(" ");
				if(i==9 || i==19 || i==29 || i==39)			//just for beautiful output style
					{	
						Serial.println();
					}
			}
			Serial.println();	
			Serial.println(F("PaTable:"));
	
			for(uint8_t i = 0 ; i < 8; i++) 					//showes rx_buffer for debug
				{
					uart_puthex_byte(Patable_verify[i]);Serial.print(" ");
				}
		Serial.println();	
	}
//-----------------[show settings]----------------------------------
void CC1100::cc1100_show_main_settings(void)
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
//----------------------[end]---------------------------------------

//-----------------[sent packet]----------------------------------
uint8_t CC1100::cc1100_sent_packet(uint8_t my_addr, uint8_t rx_addr, uint8_t *txbuffer, uint8_t pktlen)
{
	uint8_t tx_retries_count = 0;
	uint16_t ackWaitCounter = 0;
	
	while(tx_retries_count != TX_RETRIES_MAX)
	{
		cc1100_tx_payload_burst(my_addr, rx_addr, txbuffer, pktlen);	//loads the data in cc1100 buffer
		cc1100_transmit();												//sents data over air
		cc1100_receive();												//receive mode
		
		while (ackWaitCounter != ACK_TIMEOUT ) 							//Wait for an acknowge
		{
			_delay_ms(1);
			
			if (cc1100_check_packet_received() == TRUE)					//if RF package received
			{
				return TRUE;
			}															//if RF package received

			ackWaitCounter++;                   						// Decrement wait counter
		}
		ackWaitCounter = 0;												// resets the ACK_Timeout
		tx_retries_count++;												// increase tx retry counter
	
		#if CC1100_DEBUG == 1
			uart_puthex_byte(tx_retries_count);Serial.println();
		#endif
	}
	return FALSE;
}
//----------------------[end]---------------------------------------

//-----------------[transmit mode]----------------------------------
void CC1100::cc1100_transmit(void)
	{
		uint8_t marcstate;
		//extern uint8_t Patable[8];
	
		//EIMSK &= ~(1<<INT1);							//disable INT1 because of TX sync word asserting
	
		//spi_write_burst(Patable_burst,Patable,8,1);		//writes output power settings to cc1100, because of powerdown loss setting	"104us"
	
		digitalWrite(TX_LED, HIGH);     				//TX_LED high
		spi_write(Stx,1);								//sends the data over air
		marcstate = 0xFF;								//set unknown/dummy state value
		
		while(marcstate != 0x01)						//0x01 = IDLE 
		{
			marcstate = spi_read(Marcstate_addr);		//read out state of cc1100 to be sure in IDLE and TX is finished
			//uart_puthex_byte(marcstate);
		}
		Serial.println();
		spi_write(Sftx, 1);								//flush the Tx_fifo content -> a must for interrupt handling
		digitalWrite(TX_LED, LOW);     					//TX_LED high
		//EIMSK |= (1<<INT1);							//enables INT1
	}
//----------------------[end]---------------------------------------	

//------------------[receive mode]----------------------------------
void CC1100::cc1100_receive(void)
{
	uint8_t marcstate;

	spi_write(Sidle, 1);								//sets to idle first. must be in
	spi_write(Srx,1);									//writes receive strobe (receive mode)
	marcstate = 0xFF;									//set unknown/dummy state value
	
	while(marcstate != 0x0D)							//0x0D = RX 
	{
		marcstate = spi_read(Marcstate_addr);			//read out state of cc1100 to be sure in RX
		//uart_puthex_byte(marcstate);
	}
	//Serial.println();
}	
//----------------------[end]---------------------------------------

//----------------[tx_payload_burst]--------------------------------
uint8_t CC1100::cc1100_tx_payload_burst(uint8_t my_addr,uint8_t rx_addr, uint8_t *txbuffer, uint8_t length)
{
	txbuffer[0] = length-1;
	txbuffer[1] = rx_addr;
	txbuffer[2] = my_addr;

	spi_write_burst(Tx_fifo_addr,txbuffer,length,1);	//writes TX_Buffer +1 because of pktlen must be also transfered
	
	#if CC1100_DEBUG == 1
		Serial.print(F("TX_FIFO:"));
		for(uint8_t i = 0 ; i < length; i++) 			//TX_fifo debug out
			{
			uart_puthex_byte(txbuffer[i]);
			}
		//Serial.println();
	#endif
	return TRUE;
}
//----------------------[end]---------------------------------------

//------------[rx_payload_burst - package received]-----------------
uint8_t CC1100::cc1100_rx_payload_burst()
	{
	
	extern uint8_t Rx_fifo[];
	extern volatile uint8_t Rx_addr, Tx_addr, Pktlen, Lqi;
	extern int8_t Rssi_dbm;
	
	Pktlen = spi_read(Rx_fifo_addr_single);						//received pktlen +1 for complete TX buffer
	Rx_fifo[0] = Pktlen;
	
	for(uint8_t i=1;i<Pktlen+3;i++)								//+3 because of i=1 
		{
			Rx_fifo[i] = spi_read(Rx_fifo_addr_single);
		}
	
	Rx_addr = Rx_fifo[1];										//my address
	Tx_addr = Rx_fifo[2];										//address from sender
	
	Rssi_dbm = cc1100_rssi_convert(Rx_fifo[Pktlen + 1]);		//RSSI value in dbm
	Lqi = Rx_fifo[Pktlen + 2];									//LQI  value in hex
	
	spi_write(Sfrx, 1);											//flush the RX_fifo content -> a must for interrupt handling
	
	return TRUE;
	}
//----------------------[end]--------------------------------

//-----------------[check if Packet is received]-------------------------
uint8_t CC1100::cc1100_check_packet_received(void)
{
	extern uint8_t Rx_fifo[]; 
	extern volatile uint8_t Rx_addr, My_addr;
	
	if (digitalRead(GDO2) == TRUE)
		{
			cc1100_rx_fifo_erase(Rx_fifo);
			cc1100_rx_payload_burst();							//reads package in buffer
	
			if ((Rx_addr == My_addr) || (Rx_addr == BROADCAST_ADDRESS))
			{
				cc1100_check_payload();							//checks the incomming data
				cc1100_receive();								//set CC1100 in receive mode
				return TRUE;
			}
		}
		return FALSE;
}
//----------------------[end]--------------------------------

//-----[check if Packet is received within defined time in ms]----
uint8_t CC1100::cc1100_wait_for_packet(uint8_t milliseconds)
{
	for(uint8_t i = 0; i< milliseconds; i++)
		{
			_delay_ms(1);									//delay till system has data available
			if (cc1100_check_packet_received())
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
//----------------------[end]--------------------------------
//-----------------[packed check in main loop]-------------------------
/*
void rx_package_received_check(void)
{
	place code here and add rx_package_received_check() in main loop
}
*/
//----------------------[end]--------------------------------
//-----------------[check Payload for ACK or Data]-------------------------
uint8_t CC1100::cc1100_check_payload(void)
{
	extern uint8_t Rx_fifo[];
	extern volatile uint8_t Pktlen, Lqi;
	extern volatile int8_t Rssi_dbm;
	
	digitalWrite(RX_LED, HIGH);     						// RX_LED high
	
	if	(cc1100_check_acknolage() == TRUE) 					//acknowlage received!
		{
			digitalWrite(RX_LED, LOW);     					//RX_LED LOW
			return FALSE;
		}
	else 													//real data, and sent acknowladge
		{
		
		#if CC1100_DEBUG == 1
			Serial.print(F("RX_FIFO:"));
			for(uint8_t i = 0 ; i < Pktlen + 3; i++) 		//showes rx_buffer for debug
				{
					uart_puthex_byte(Rx_fifo[i]);
				}
			Serial.println();	
			Serial.print(F("Rssi:"));uart_puti(Rssi_dbm);Serial.print(F(" "));Serial.print(F("Lqi:"));uart_puthex_byte(Lqi);Serial.println();
		#endif
		
			digitalWrite(RX_LED, LOW);     						//RX_LED LOW
			cc1100_sent_acknolage();							//sending acknolage to sender!
			return TRUE;
		}
}
//----------------------[end]--------------------------------

//-----------------[check ACKNOLAGE]-------------------------
uint8_t CC1100::cc1100_check_acknolage(void)
{
	extern volatile uint8_t Rx_fifo[], Tx_addr, My_addr, Lqi;
	extern volatile int8_t Rssi_dbm;
	
	if	(Rx_fifo[1] == My_addr && Rx_fifo[2] == Tx_addr && Rx_fifo[3] == 'A' && Rx_fifo[4] == 'c' && Rx_fifo[5] == 'k') //acknolage received!
		{
		#if CC1100_DEBUG == 1
			Serial.print(F("Ack_OK! RSSI:"));uart_puti(Rssi_dbm);Serial.print(F(" "));Serial.print(F("Lqi:"));uart_puthex_byte(Lqi);
			Serial.println();
		#endif
		return TRUE;
		}
	return FALSE;
}
//----------------------[end]--------------------------------

//-----------------[sent ACKNOLAGE]--------------------------
void CC1100::cc1100_sent_acknolage(void)
{
	uint8_t pktlen = 0x06;										//complete Pktlen for ACK packet
	extern uint8_t Tx_fifo[];
	extern volatile uint8_t My_addr, Tx_addr;
	
	Tx_fifo[3] = 'A';	Tx_fifo[4] = 'c';	Tx_fifo[5] = 'k';
	
	cc1100_tx_payload_burst(My_addr, Tx_addr, Tx_fifo, pktlen);
	cc1100_transmit();
	cc1100_receive();						//set CC1100 in receive mode
	
	#if CC1100_DEBUG == 1
		Serial.print(F("Ack_sent!"));
		Serial.println();
	#endif
}
//----------------------[end]--------------------------------

//------------------[tx_fifo_erase]--------------------------------
void CC1100::cc1100_tx_fifo_erase(uint8_t *txbuffer)
{
	memset(txbuffer, 0, sizeof(FIFOBUFFER));						//erased the TX_fifo array content to "0"
}
//----------------------[end]--------------------------------

//------------------[rx_fifo_erase]--------------------------------
void CC1100::cc1100_rx_fifo_erase(uint8_t *rxbuffer)
{
 	memset(rxbuffer, 0, sizeof(FIFOBUFFER));						//erased the RX_fifo array content to "0"
}
//----------------------[end]--------------------------------


//------------------[set Power Down]--------------------------------
uint8_t CC1100::cc1100_powerdown(void)
{
	spi_write(Sidle, 1);
	spi_write(Spwd, 1);											//set to Power Down stage
	return TRUE;
}
//----------------------[end]--------------------------------

//---------------------[WakeUp]--------------------------------
uint8_t CC1100::cc1100_wakeup(void)
{
	digitalWrite(SS_PIN, LOW);     		// CS low
	_delay_us(10);
	digitalWrite(SS_PIN, HIGH);      	// CS high
	_delay_us(10);
	cc1100_receive();					// go to RX Mode
	
	return TRUE;
}
//----------------------[end]--------------------------------

//------------------[set transmit mode]--------------------------------
uint8_t CC1100::cc1100_set_mode(uint8_t mode)
{
	uint8_t Cfg_reg[CFG_REGISTER];
	
	switch (mode) 																			//loads the RF mode which is defined in cc1100_mode_select
	{
		case 0x01:
					eeprom_read_block(Cfg_reg,cc1100_GFSK_1_2_kb,CFG_REGISTER);			  	//sets up settings for GFSK 1,2 kbit mode/speed				
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
		default:
					eeprom_read_block(Cfg_reg,cc1100_GFSK_38_4_kb,CFG_REGISTER);			//sets up settings for GFSK 38,4 kbit mode/speed
					break;
	}
	
	spi_write_burst(Write_burst,Cfg_reg,CFG_REGISTER,1);					//writes all 47 config register to cc1100 	"530us"
	
	eeprom_write_byte((uint8_t*)EEPROM_ADDRESS_CC1100_MODE,mode);			//sets the cc1100 frequency
	
	return TRUE;
}
//----------------------[end]--------------------------------

//------------------[set channel]--------------------------------
uint8_t CC1100::cc1100_set_myaddr(uint8_t addr)
{
	cc1100.spi_write(Addr_addr,0);cc1100.spi_write(addr,1);					//stores MyAddr in the CC1100
	eeprom_write_byte((uint8_t*)EEPROM_ADDRESS_CC1100_MY_ADDR,addr);		//writes byte to eeprom
	
	return TRUE;
}
//----------------------[end]--------------------------------

//------------------[set ISM Band]--------------------------------
uint8_t CC1100::cc1100_set_ISM(uint8_t ism_freq)
{
	uint8_t freq2, freq1, freq0;
	
	extern uint8_t Patable[8];
	
	switch (ism_freq) 														//loads the RF freq which is defined in cc1100_freq_select
	{
		case 0x01:															//315MHz
					freq2=0x0C;			
					freq1=0x1D;
					freq0=0x89;
					eeprom_read_block(Patable,patable_power_315,8);	
					break;
		case 0x02:															//433MHz
					freq2=0x10;
					freq1=0xA7;
					freq0=0x62;
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
		default:															//default is 868.3MHz
					freq2=0x21;
					freq1=0x65;
					freq0=0x6A;
					eeprom_read_block(Patable,patable_power_868,8);			//sets up output power ramp register
					break;
	}
	
	spi_write(Freq2_addr,0);cc1100.spi_write(freq2,1);						//stores the new freq setting for defined ISM band
	spi_write(Freq1_addr,0);cc1100.spi_write(freq1,1);	
	spi_write(Freq0_addr,0);cc1100.spi_write(freq0,1);	
	
	spi_write_burst(Patable_burst,Patable,8,1);								//writes output power settings to cc1100	"104us"
	
	//cc1100_receive();														//goes to RX mode
	
	eeprom_write_byte((uint8_t*)EEPROM_ADDRESS_CC1100_FREQUENCY,ism_freq);	//selects the cc1100 frequency
	
	return TRUE;
}
//----------------------[end]--------------------------------

//------------------[set channel]--------------------------------
uint8_t CC1100::cc1100_set_channel(uint8_t channel)
{
	cc1100.spi_write(Channr_addr,0);cc1100.spi_write(channel,1);			//stores the new channel # in the CC1100
	eeprom_write_byte((uint8_t*)EEPROM_ADDRESS_CC1100_CHANNEL,channel);		//writes byte to eeprom
	
	return TRUE;
}
//----------------------[end]--------------------------------

//------------------[set channel]--------------------------------
uint8_t CC1100::cc1100_set_patable(uint8_t *patable_arr)
{
	spi_write_burst(Patable_burst,patable_arr,8,1);								//writes output power settings to cc1100	"104us"
	
	return TRUE;
}
//----------------------[end]--------------------------------
//------------------[rssi_convert]--------------------------------
int8_t CC1100::cc1100_rssi_convert(uint8_t Rssi_hex)
{
	int8_t rssi_dbm;
	int16_t Rssi_dec;
	
	Rssi_dec = Rssi_hex;										//convert unsigned to signed
	
	if(Rssi_dec >= 128)
		{
		rssi_dbm=((Rssi_dec-256)/2)-RSSI_OFFSET_868MHZ;			//rssi_offset via datasheet
		}
	else
		{
		if(Rssi_dec<128)
			{
			rssi_dbm=((Rssi_dec)/2)-RSSI_OFFSET_868MHZ;
			}
		}
	return rssi_dbm;
}
//----------------------[end]--------------------------------

//--------------------[get temp]------------------------------
int16_t CC1100::cc1100_get_temp(void)
{
	int32_t adc_val = 0;
	
	spi_write(Sidle, 1);									//sets the cc1100 into IDLE
	spi_write(Ptest_addr,0);spi_write(0xBF,1);				//to enable temp sensor
	
	adc_val = analogRead(GDO0);								//read out temp_voltage, VBAT is example
	
	spi_write(Ptest_addr,0);spi_write(0x7F,1);				//writes 0x7F back to PTest (app. note)
	
	cc1100_receive();
	
	return (((adc_val*CC1100_TEMP_ADC_MV)/CC1100_TEMP_CELS_CO));
}
//----------------------[end]--------------------------------

//|================ SPI Initialisation for CC1100 ===================|
void CC1100::spi_init(void)
{
//|----------- Aktivieren der Pins f�r das SPI Interface ------------|
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
   
   SPSR =  (0<<SPI2X);             		// Double Clock Rate
}

//|============================= SPI Transmission =============================|
uint8_t CC1100::spi_putc(uint8_t data)
{
	SPDR = data;
    while( !( SPSR & (1<<SPIF) ) ); 	// Warten bis Byte gesendet wurde
	
	return SPDR;
}

//|============================ Ein Register lesen ============================|
uint8_t CC1100::spi_read(uint8_t spi_instr)
{
	digitalWrite(SS_PIN, LOW);    							// CS low
	while(digitalRead(MOSI_PIN) == 0);						// Wait until MOSI_PIN becomes LOW
	spi_putc(spi_instr | Read_single_byte);
	spi_instr = spi_putc(0xff); 							// mu� tranferieren um daten zu bekommen
	digitalWrite(SS_PIN, HIGH);     						// CS high

	return spi_instr;
}
//|======= Mehrere hintereinanderliegende Register auf einmal lesen =======|
void CC1100::spi_read_burst(uint8_t spi_instr, uint8_t *pArr, uint8_t length, uint8_t cs)
{
	digitalWrite(SS_PIN, LOW);    							// CS low
  	while(digitalRead(MOSI_PIN) == 0);						//Wait until MOSI_PIN becomes LOW
	spi_putc(spi_instr | Read_burst);
   
	for (uint8_t i=0; i<length ;i++ )
		{
			pArr[i] = spi_putc(0xFF);   					//mu� tranferieren um daten zu bekommen; PArr[i] = pointer auf array in main
		}
   
	if (cs==1)
		{
			digitalWrite(SS_PIN, HIGH);      				// CS high
		}
}
//|========================= In ein Register schreiben ========================|
void CC1100::spi_write(uint8_t spi_instr, uint8_t cs )
{
 	digitalWrite(SS_PIN, LOW);    							// CS low
 	while(digitalRead(MOSI_PIN) == 0);						//Wait until MOSI_PIN becomes LOW
	spi_putc(spi_instr | Write_single_byte);
   
	if (cs==1)
	{
		digitalWrite(SS_PIN, HIGH);      					// CS high
	}
}
//|======= Mehrere hintereinanderliegende Register auf einmal schreiben =======|
void CC1100::spi_write_burst(uint8_t spi_instr, uint8_t *pArr, uint8_t length, uint8_t cs)
{
	digitalWrite(SS_PIN, LOW);    							// CS low
 	while(digitalRead(MOSI_PIN) == 0);						//Wait until MOSI_PIN becomes LOW
	spi_putc(spi_instr | Write_burst);
   
	for (uint8_t i=0; i<length ;i++ )
		spi_putc(pArr[i]);
	if (cs==1)
	{
		digitalWrite(SS_PIN, HIGH);     					// CS high
	}
}
//|================================= END =======================================|


//|============================ UART helper ====================================|

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
//|================================= END =======================================|
