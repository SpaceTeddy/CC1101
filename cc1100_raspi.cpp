/*-------------------------------------------------------------------------------
'                     CC1100 Raspberry Pi Library
'                     ---------------------------
'
'  
'
'
' 
'
'-------------------------------------------------------------------------------*/

#include <cc1100.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>


//CC1100 cc1100;

extern uint8_t cc1100_debug;
//-------------------[global default settings 868 Mhz]---------------------------------

static uint8_t cc1100_GFSK_1_2_kb[CFG_REGISTER] = {0x07,0x2E,0x80,0x07,0x57,0x43,0x3E,0x0D,0x45,0xFF,
										0x00,0x08,0x00,0x21,0x65,0x6A,0xF5,0x83,0x13,0xA0,
										0xF8,0x15,0x07,0x0C,0x19,0x16,0x6C,0x03,0x40,0x91,
										0x02,0x26,0x09,0x56,0x17,0xA9,0x0A,0x00,0x11,0x41,
										0x00,0x59,0x7F,0x3F,0x81,0x3F,0x0B};

static uint8_t cc1100_GFSK_38_4_kb[CFG_REGISTER] = {0x07,0x2E,0x80,0x07,0x57,0x43,0x3E,0x0E,0x45,0xFF,
										0x00,0x06,0x00,0x21,0x65,0x6A,0xCA,0x83,0x13,0xA0,
										0xF8,0x34,0x07,0x0C,0x19,0x16,0x6C,0x43,0x40,0x91,
										0x02,0x26,0x09,0x56,0x17,0xA9,0x0A,0x00,0x11,0x41,
										0x00,0x59,0x7F,0x3F,0x81,0x3F,0x0B};

static uint8_t cc1100_GFSK_100_kb[CFG_REGISTER] = {0x07,0x2E,0x80,0x07,0x57,0x43,0x3E,0x0E,0x45,0xFF,
									  0x00,0x08,0x00,0x21,0x65,0x6A,0x5B,0xF8,0x13,0xA0,
									  0xF8,0x47,0x07,0x0C,0x18,0x1D,0x1C,0xC7,0x00,0xB2,
									  0x02,0x26,0x09,0xB6,0x17,0xEA,0x0A,0x00,0x11,0x41,
									  0x00,0x59,0x7F,0x3F,0x81,0x3F,0x0B};

static uint8_t cc1100_MSK_250_kb[CFG_REGISTER] = {0x07,0x2E,0x80,0x07,0x57,0x43,0x3E,0x0E,0x45,0xFF,
									  0x00,0x0B,0x00,0x21,0x65,0x6A,0x2D,0x3B,0x73,0xA0,
									  0xF8,0x00,0x07,0x0C,0x18,0x1D,0x1C,0xC7,0x00,0xB2,
									  0x02,0x26,0x09,0xB6,0x17,0xEA,0x0A,0x00,0x11,0x41,
									  0x00,0x59,0x7F,0x3F,0x81,0x3F,0x0B};

static uint8_t cc1100_MSK_500_kb[CFG_REGISTER] = {0x07,0x2E,0x80,0x07,0x57,0x43,0x3E,0x0E,0x45,0xFF,
									  0x00,0x0C,0x00,0x21,0x65,0x6A,0x0E,0x3B,0x73,0xA0,
									  0xF8,0x00,0x07,0x0C,0x18,0x1D,0x1C,0xC7,0x40,0xB2,
									  0x02,0x26,0x09,0xB6,0x17,0xEA,0x0A,0x00,0x19,0x41,
									  0x00,0x59,0x7F,0x3F,0x81,0x3F,0x0B};

static uint8_t patable_power_315[8] = {0x17,0x1D,0x26,0x69,0x51,0x86,0xCC,0xC3};
static uint8_t patable_power_433[8] = {0x6C,0x1C,0x06,0x3A,0x51,0x85,0xC8,0xC0};
static uint8_t patable_power_868[8] = {0x03,0x17,0x1D,0x26,0x50,0x86,0xCD,0xC0};
static uint8_t patable_power_915[8] = {0x0B,0x1B,0x6D,0x67,0x50,0x85,0xC9,0xC1};
//static uint8_t patable_power_2430[8] = {0x44,0x84,0x46,0x55,0xC6,0x6E,0x9A,0xFE};

//-----------------------------[END]------------------------------------------
//-------------------------[CC1100 reset function]------------------------------
void CC1100::reset(void)			      // reset defined in cc1100 datasheet
{
  digitalWrite(SS_PIN, LOW);
  delayMicroseconds(10);
  digitalWrite(SS_PIN, HIGH);
  delayMicroseconds(40);

  spi_write_strobe(SRES);
  delay(1);
}
//-----------------------------[END]--------------------------------------------


//------------------[set Power Down]--------------------------------
void CC1100::powerdown(void)
{
	silde();
	spi_write_strobe(SPWD);											//set to Power Down stage
	return;
}
//----------------------[end]--------------------------------

//---------------------[WakeUp]--------------------------------
void CC1100::wakeup(void)
{
	digitalWrite(SS_PIN, LOW);     		// CS low
	delayMicroseconds(10);
	digitalWrite(SS_PIN, HIGH);      	// CS high
	delayMicroseconds(10);
	receive();					// go to RX Mode
	
	return;
}
//----------------------[end]--------------------------------

//-----------------[CC1100 init functions "8.9ms"]-----------------------
uint8_t CC1100::begin(volatile uint8_t &My_addr)
	{
	//uint8_t cc1100_freq_select, cc1100_mode_select, cc1100_channel_select;
	uint8_t partnum, version;

	extern int cc1100_freq_select, cc1100_mode_select, cc1100_channel_select;


	pinMode(GDO2, INPUT);
	//pinMode(RX_LED, OUTPUT);
	//pinMode(TX_LED, OUTPUT);
	
	if (cc1100_debug == 1) {
		printf("Init CC1100...\r\n");
	}
	
	spi_begin();
	reset();										           //CC1100 reset

	spi_write_strobe(SFTX);delayMicroseconds(100); //flush the TX_fifo content
	spi_write_strobe(SFRX);delayMicroseconds(100); //flush the RX_fifo content
	
	partnum = spi_read_register(PARTNUM);
	version = spi_read_register(VERSION);
	
	if(version == 0x00 || version == 0xFF)
		{
			if (cc1100_debug == 1) {
				printf("no CC11xx found!\r\n");
			}
			
			return FALSE;
		}
	
	if (cc1100_debug == 1) {
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
	set_output_power_level(0);					//set PA to 0dBm as default

	//set my receiver address
	set_myaddr(My_addr);						    //My_Addr from EEPROM to global variable


	//cc1100_show_register_settings();
	//show_main_settings();

	if (cc1100_debug == 1) {
		printf("...done!\r\n");
	}
	
	receive();									//set CC1100 in receive mode
	return TRUE;
	}	

//-----------------[finish's the CC1100 operation]------------------------------
void CC1100::end(void)
{
	powerdown();																	//power down CC1100
}
//-------------------------------[end]------------------------------------------

//----------------------[end]---------------------------------------
void CC1100::show_register_settings(void)
	{
		uint8_t config_reg_verify[CFG_REGISTER],Patable_verify[CFG_REGISTER];

		spi_read_burst(READ_BURST,config_reg_verify,CFG_REGISTER);	//reads all 47 config register from cc1100	"359.63us"
		spi_read_burst(PATABLE_BURST,Patable_verify,8);				//reads output power settings from cc1100	"104us"
	
		show_main_settings();

		printf("Config Register:\r\n");
	
		for(uint8_t i = 0 ; i < CFG_REGISTER; i++) 		           //showes rx_buffer for debug
			{
				printf("0x%02X ", config_reg_verify[i]);

				if(i==9 || i==19 || i==29 || i==39)			       //just for beautiful output style
					{	
					printf("\r\n");
					}
			}
			printf("\r\n");
			printf("PaTable:\r\n");
	
			for(uint8_t i = 0 ; i < 8; i++) 					   //showes rx_buffer for debug
				{
				printf("0x%02X ", Patable_verify[i]);
				}
			printf("\r\n");

	}
//-----------------[show settings]----------------------------------
void CC1100::show_main_settings(void)
{
	extern volatile uint8_t My_addr;
	extern int cc1100_mode_select, cc1100_freq_select, cc1100_channel_select;

	printf("Mode: %d\r\n", cc1100_mode_select);
	printf("Frequency: %d\r\n", cc1100_freq_select);
	printf("Channel: %d\r\n", cc1100_channel_select);
	printf("My_Addr: %d\r\n", My_addr);
}
//----------------------[end]---------------------------------------

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
		delayMicroseconds(100);                          //must be in for ever reason
	}
	//Serial.println();

	spi_write_strobe(SFTX);delayMicroseconds(100);		 //flush the Tx_fifo content -> a must for interrupt handling
}
//-------------------------------[end]------------------------------------------

//-----------------[sent packet]----------------------------------
uint8_t CC1100::sent_packet(uint8_t my_addr, uint8_t rx_addr, uint8_t *txbuffer,
                            uint8_t pktlen,  uint8_t tx_retries)
{
	uint8_t pktlen_ack;																			//default package len for ACK
	uint8_t rxbuffer[FIFOBUFFER];
	uint8_t tx_retries_count = 0;
	uint16_t ackWaitCounter = 0;

	while(tx_retries_count != tx_retries)										//sent package out with retries
	{
		tx_payload_burst(my_addr, rx_addr, txbuffer, pktlen);	//loads the data in cc1100 buffer
		transmit();												                    //sents data over air
		receive();												                    //receive mode

		if(rx_addr == BROADCAST_ADDRESS){											//no wait acknowge if sent to broadcast address
			return TRUE;																				//successful sent to BROADCAST_ADDRESS
		}

		while (ackWaitCounter != ACK_TIMEOUT ) 						  	//Wait for an acknowge
		{
			delay(1);																				//delay to give receiver time
			if (packet_available() == TRUE)					      			//if RF package received check package acknowge
			{
				uint8_t from_sender = rx_addr;                  	//the original message sender address
				rx_fifo_erase(rxbuffer);
				rx_payload_burst(rxbuffer, pktlen_ack);						//reads package in buffer
				check_acknolage(rxbuffer, pktlen_ack, from_sender);
				return TRUE;																			//package successfully sent
			}
			else{
				ackWaitCounter++;                   							//increment ACK wait counter
			}																										//if RF package received
		}
			ackWaitCounter = 0;																	//resets the ACK_Timeout
			tx_retries_count++;																	//increase tx retry counter

			#if CC1100_DEBUG == 1																//debug output messages
				printf(" #:");
				printf("0x%02X \r\n", tx_retries_count);
			#endif
	}

	return FALSE;																						//sent failed. too many retries
}
//----------------------[end]---------------------------------------

//------------------[receive mode]----------------------------------
void CC1100::receive(void)
{
	uint8_t marcstate;

	silde();			                             //sets to idle first.
	spi_write_strobe(SRX);									//writes receive strobe (receive mode)
	marcstate = 0xFF;									//set unknown/dummy state value
	
	while(marcstate != 0x0D)							//0x0D = RX 
	{
		marcstate = (spi_read_register(MARCSTATE) & 0x1F);		//read out state of
		//printf("Marcstate: 0x%02X\r", marcstate);
	}
}	
//----------------------[end]---------------------------------------

//----------------[tx_payload_burst]--------------------------------
void CC1100::tx_payload_burst(uint8_t my_addr,uint8_t rx_addr, uint8_t *txbuffer, uint8_t length)
{
	txbuffer[0] = length-1;
	txbuffer[1] = rx_addr;
	txbuffer[2] = my_addr;

	spi_write_burst(TXFIFO_BURST,txbuffer,length);		//writes TX_Buffer +1 because of pktlen must be also transfered
	
	if (cc1100_debug == 1) {
		printf("TX_FIFO: ");
		for(uint8_t i = 0 ; i < length; i++) 			//TX_fifo debug out
		{
			printf("0x%02X ", txbuffer[i]);
		}
		printf("\r\n");
	}
	return;
}
//----------------------[end]---------------------------------------

//------------[rx_payload_burst - package received]-----------------
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
		}
	silde();
	spi_write_strobe(SFRX);delayMicroseconds(100);
	receive();
	}
//----------------------[end]--------------------------------


//-----[check if Packet is received within defined time in ms]----
uint8_t CC1100::wait_for_packet(uint8_t milliseconds)
{
	for(uint8_t i = 0; i< milliseconds; i++)
		{
			delay(1);									//delay till system has data available
			if (packet_available())
			{
				return TRUE;
			}
			else if(i == milliseconds) 
			{
				if(cc1100_debug == 1){
					printf("no packet received!\r\n");
					return FALSE;
				}
			}
		}
	return TRUE;
}
//----------------------[end]--------------------------------


//-----------------[check ACKNOLAGE]-------------------------
uint8_t CC1100::check_acknolage(uint8_t *rxbuffer, uint8_t pktlen, uint8_t sender)
{
	extern volatile uint8_t My_addr;
  //uint8_t tx_addr = rxbuffer[2];

	if((rxbuffer[1] == My_addr || rxbuffer[1] == BROADCAST_ADDRESS) && rxbuffer[2] == sender && rxbuffer[3] == 'A' && rxbuffer[4] == 'c' && rxbuffer[5] == 'k') 		 //acknolage received!
		{
			if(rxbuffer[1] == BROADCAST_ADDRESS){							//if receiver address BROADCAST_ADDRESS skip acknolage
				#if CC1100_DEBUG == 1
					//Serial.println(F("BROADCAST ACK"));
				#endif
				return FALSE;
			}
			int8_t rssi_dbm = rssi_convert(rxbuffer[pktlen + 1]);
			uint8_t lqi = lqi_convert(rxbuffer[pktlen + 2]);
			uint8_t crc = check_crc(lqi);
			if(cc1100_debug == 1){
				//Serial.println();
				printf("ACK! ");
				printf("RSSI:");printf("0x%02X ", rssi_dbm);
				printf("LQI:");printf("0x%02X ", lqi);
				printf("CRC:");printf("0x%02X ", crc);
				printf("\r\n");
			}
			return TRUE;
		}
	return FALSE;
}
//----------------------[end]--------------------------------

//-----------------[sent ACKNOLAGE]--------------------------
void CC1100::sent_acknolage(uint8_t my_addr, uint8_t tx_addr)
{
	uint8_t pktlen = 0x06;																//complete Pktlen for ACK packet
	uint8_t tx_buffer[0x06];															//tx buffer array init

	tx_buffer[3] = 'A';	tx_buffer[4] = 'c';	tx_buffer[5] = 'k';	//fill buffer with ACK Payload

	tx_payload_burst(my_addr, tx_addr, tx_buffer, pktlen);//load payload to CC1100
	transmit();																						//sent package over the air
	receive();																						//set CC1100 in receive mode

	
	if (cc1100_debug == 1) {
		printf("Ack_sent!\r\n");
	}
}
//----------------------[end]--------------------------------

//----------------------[check if Packet is received]---------------------------
uint8_t CC1100::packet_available()
{
	if(digitalRead(GDO2) == TRUE)														//if RF package received
		{
			if(spi_read_register(IOCFG2) == 0x06)								//if sync word detect mode is used
			{
				while(digitalRead(GDO2) == TRUE); 								//for sync word receive
			}

			#if CC1100_DEBUG == 1
				//printf("Pkt->:\r\n");
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
	uint32_t time_stamp = millis();

	rx_fifo_erase(rxbuffer);															//delete rx_fifo bufffer
	rx_payload_burst(rxbuffer, pktlen);							     	//read package in buffer

	if(pktlen == 0x00)																		//packet len not plausible?
		{
			#if CC1100_DEBUG == 1
				//Serial.println(F("bad packet!"));
			#endif
			return FALSE;
		}

	sender = rxbuffer[2];

	if(check_acknolage(rxbuffer, pktlen, sender) == TRUE) //acknowlage received?
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
					printf("BROADCAST message\r\n");
				}
				printf("RX_FIFO:");
				for(uint8_t i = 0 ; i < pktlen + 3; i++) 		    //showes rx_buffer for debug
					{
					printf("0x%02X ", rxbuffer[i]);
					}printf("\r\n");

					printf("ACK! ");
					printf("RSSI:%d ", rssi_dbm);
					printf("LQI:");printf("0x%02X ", lqi);
					printf("CRC:");printf("0x%02X ", crc);
					printf("\r\n");
			#endif

			my_addr = rxbuffer[1];														//set receiver address to my_addr
			sender = rxbuffer[2];															//set from_sender address

			if(my_addr != BROADCAST_ADDRESS)									//send only ack if no BROADCAST_ADDRESS
			{
				sent_acknolage(my_addr, sender);								//sending acknolage to sender!
				printf("time:%d ", millis()-time_stamp);printf("\r\n");
			}
			return TRUE;
		}
}
//-------------------------------[end]------------------------------------------


//------------------[tx_fifo_erase]--------------------------------
void CC1100::tx_fifo_erase(uint8_t *txbuffer)
{
	memset(txbuffer, 0, sizeof(FIFOBUFFER));						//erased the TX_fifo array content to "0"
}
//----------------------[end]--------------------------------

//------------------[rx_fifo_erase]--------------------------------
void CC1100::rx_fifo_erase(uint8_t *rxbuffer)
{
 	memset(rxbuffer, 0, sizeof(FIFOBUFFER));						//erased the RX_fifo array content to "0"
}
//----------------------[end]--------------------------------


//------------------[set transmit mode]--------------------------------
void CC1100::set_mode(uint8_t mode)
{
	switch (mode) 																			//loads the RF mode which is defined in cc1100_mode_select
	{
		case 0x01:
					spi_write_burst(WRITE_BURST,cc1100_GFSK_1_2_kb,CFG_REGISTER);
					break;
		case 0x02:
					spi_write_burst(WRITE_BURST,cc1100_GFSK_38_4_kb,CFG_REGISTER);			//sets up settings for GFSK 38,4 kbit mode/speed
					break;
		case 0x03:
					spi_write_burst(WRITE_BURST,cc1100_GFSK_100_kb,CFG_REGISTER);			//sets up settings for GFSK 100 kbit mode/speed
					break;
		case 0x04:
					spi_write_burst(WRITE_BURST,cc1100_MSK_250_kb,CFG_REGISTER);			//sets up settings for GFSK 38,4 kbit mode/speed
					break;
		case 0x05:
					spi_write_burst(WRITE_BURST,cc1100_MSK_500_kb,CFG_REGISTER);			//sets up settings for GFSK 38,4 kbit mode/speed
					break;
		default:
					spi_write_burst(WRITE_BURST,cc1100_GFSK_100_kb,CFG_REGISTER);			//sets up settings for GFSK 100 kbit mode/speed
					break;
	}
	
	return;
}
//----------------------[end]--------------------------------

//------------------[set ISM Band]--------------------------------
void CC1100::set_ISM(uint8_t ism_freq)
{
	uint8_t freq2, freq1, freq0;
	
	switch (ism_freq) 														//loads the RF freq which is defined in cc1100_freq_select
	{
		case 0x01:															//315MHz
					freq2=0x0C;			
					freq1=0x1D;
					freq0=0x89;
					spi_write_burst(PATABLE_BURST,patable_power_315,8);
					break;
		case 0x02:															//433MHz
					freq2=0x10;
					freq1=0xA7;
					freq0=0x62;
					spi_write_burst(PATABLE_BURST,patable_power_433,8);
					break;
		case 0x03:															//868.3MHz
					freq2=0x21;
					freq1=0x65;
					freq0=0x6A;
					spi_write_burst(PATABLE_BURST,patable_power_868,8);
					break;
		case 0x04:															//915MHz
					freq2=0x23;
					freq1=0x31;
					freq0=0x3B;
					spi_write_burst(PATABLE_BURST,patable_power_915,8);
					break;
		/*
		case 0x05:															//2430MHz
					freq2=0x5D;
					freq1=0x76;
					freq0=0x27;
					spi_write_burst(PATABLE_BURST,patable_power_2430,8);
					break;
		*/
		default:															//default is 868.3MHz
					freq2=0x21;
					freq1=0x65;
					freq0=0x6A;
					spi_write_burst(PATABLE_BURST,patable_power_868,8);    //sets up output power ramp register
					break;
	}

	spi_write_register(FREQ2,freq2);
	spi_write_register(FREQ1,freq1);
	spi_write_register(FREQ0,freq0);

	return;
}
//----------------------[end]--------------------------------

//------------------------[set CC1100 address]----------------------------------
void CC1100::set_myaddr(uint8_t addr)
{
	spi_write_register(ADDR,addr);									//stores MyAddr in the CC1100
}
//-------------------------------[end]------------------------------------------

//------------------[set channel]--------------------------------
void CC1100::set_channel(uint8_t channel)
{
	spi_write_register(CHANNR,channel);

	return;
}
//----------------------[end]--------------------------------

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

//------------------[rssi_convert]--------------------------------
int8_t CC1100::rssi_convert(uint8_t Rssi_hex)
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
//--------------------[get temp - only for Arduino]------------------------------
/*
int16_t CC1100::cc1100_get_temp(void)
{
	int32_t adc_val = 0;
	
	uint8_t txbuf[2];


	spi_write(SIDLE);										//sets the cc1100 into IDLE

	txbuf[0] = PTEST_ADDR;
	txbuf[1] = 0xBF;
	spi_write_burst(WRITE_BURST, txbuf, 2);
	
	adc_val = analogRead(GDO0);								//read out temp_voltage, VBAT is example
	
	txbuf[0] = PTEST_ADDR;
	txbuf[1] = 0x7F;
	spi_write_burst(WRITE_BURST, txbuf, 2);
	
	cc1100_receive();
	
	return (((adc_val*CC1100_TEMP_ADC_MV)/CC1100_TEMP_CELS_CO));
}*/
//----------------------[end]--------------------------------

//|================ SPI Initialisation for CC1100 ===================|
void CC1100::spi_begin(void)
{
	int x = 0;
	//printf ("init SPI bus... ");
	if ((x = wiringPiSPISetup (0, 8000000)) < 0)
	{
		if(cc1100_debug == 1){
		printf ("ERROR: wiringPiSPISetup failed!\r\n");
		}
	}
	else{
		if(cc1100_debug == 1){
		//printf ("wiringSPI is up\r\n");
		}
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
