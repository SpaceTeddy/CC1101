/*-------------------------------------------------------------------------------
'                     CC1100 Raspberry Pi Library
'                     ---------------------------
'
'  
'  by
'
'  christian weithe
'
'-------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <cc1100.h>


CC1100 cc1100;

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
static uint8_t patable_power_868[8] = {0x0D,0x1C,0x34,0x67,0x60,0x85,0xCC,0xC3};
static uint8_t patable_power_915[8] = {0x0B,0x1B,0x6D,0x67,0x50,0x85,0xC9,0xC1};
//static uint8_t patable_power_2430[8] = {0x44,0x84,0x46,0x55,0xC6,0x6E,0x9A,0xFE};

//-----------------------------[END]------------------------------------------


//---------------[CC1100 reset functions "1052.25us"]-----------------------
void CC1100::cc1100_reset(void)			// reset defined in cc1100 datasheet
    {
    digitalWrite(SS_PIN, LOW);     		// CS low
    delayMicroseconds(10);
	digitalWrite(SS_PIN, HIGH);      	// CS high
	delayMicroseconds(40);
	
	spi_write(SRES);
	delay(1);
	}
//------------------------[END]-----------------------	

//-----------------[CC1100 init functions "8.9ms"]-----------------------
uint8_t CC1100::cc1100_init(void)
	{
	extern uint8_t My_addr;
	extern uint8_t cc1100_freq_select,cc1100_mode_select,cc1100_channel_select;
	
	uint8_t partnum, version;
	
	pinMode(GDO2, INPUT);
	//pinMode(RX_LED, OUTPUT);
	//pinMode(TX_LED, OUTPUT);
	
	if (cc1100_debug == 1) {
		printf("Init CC1100...\r\n");
	}
	
	spi_init();											//default. can also be placed in main.c
	cc1100_reset();										//resets the cc1100
	
	spi_write(SFTX);									//flush the TX_fifo content -> a must for interrupt handling
	spi_write(SFRX);									//flush the RX_fifo content -> a must for interrupt handling

	partnum = spi_read(PARTNUM_ADDR);
	version = spi_read(VERSION_ADDR);
	
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
	
	//set default parameter and transmit Mode. Must be always write first
	//cc1100_mode_select = 4;
	cc1100_set_mode(cc1100_mode_select);									//250kbits
	
	//set ISM band
	//cc1100_freq_select = 3;
	cc1100_set_ISM(cc1100_freq_select);									//868MHz
	
	//set channel
	//cc1100_channel_select = 1;
	cc1100_set_channel(cc1100_channel_select);								//writes channel from EEPROM to channel register
	
	//set my receiver address
	//My_addr = 3;
	cc1100_set_myaddr(My_addr);												//My_Addr

	//cc1100_show_register_settings();
	//cc1100_show_main_settings();

	if (cc1100_debug == 1) {
		printf("...done!\r\n");
	}
	
	cc1100_receive();								//set CC1100 in receive mode
	return TRUE;
	}	
//----------------------[end]---------------------------------------
void CC1100::cc1100_show_register_settings(void)
	{
		uint8_t config_reg_verify[CFG_REGISTER],Patable_verify[CFG_REGISTER];

		cc1100.spi_read_burst(READ_BURST,config_reg_verify,CFG_REGISTER);					//reads all 47 config register from cc1100	"359.63us"
		cc1100.spi_read_burst(PATABLE_READ_BURST,Patable_verify,8);				//reads output power settings from cc1100	"104us"
	
		//cc1100.cc1100_show_main_settings();

		printf("Config Register:\r\n");
	
		for(uint8_t i = 0 ; i < CFG_REGISTER; i++) 		//showes rx_buffer for debug
			{
				printf("0x%02X ", config_reg_verify[i]);

				if(i==9 || i==19 || i==29 || i==39)			//just for beautiful output style
					{	
					printf("\r\n");
					}
			}
			printf("\r\n");
			printf("PaTable:\r\n");
	
			for(uint8_t i = 0 ; i < 8; i++) 					//showes rx_buffer for debug
				{
				printf("0x%02X ", Patable_verify[i]);
				}
			printf("\r\n");

	}
//-----------------[show settings]----------------------------------
void CC1100::cc1100_show_main_settings(void)
{
	extern uint8_t My_addr;
	extern uint8_t cc1100_freq_select,cc1100_mode_select,cc1100_channel_select;

	printf("Mode: %d\r\n", cc1100_mode_select);
	printf("Frequency: %d\r\n", cc1100_freq_select);
	printf("Channel: %d\r\n", cc1100_channel_select);
	printf("My_Addr: %d\r\n", My_addr);
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
			delay(1);
			
			if (cc1100_check_packet_received() == TRUE)					//if RF package received
			{
				return TRUE;
			}															//if RF package received

			ackWaitCounter++;                   						// Decrement wait counter
		}
		ackWaitCounter=0;												// resets the ACK_Timeout
		tx_retries_count++;												// increase tx retry counter
	
		if (cc1100_debug == 1) {
			printf("TX retries: 0x%02X \r\n", tx_retries_count);
		}
	}
	return FALSE;
}
//----------------------[end]---------------------------------------

//-----------------[transmit mode]----------------------------------
void CC1100::cc1100_transmit(void)
	{
		uint8_t marcstate;

		//spi_write_burst(Patable_burst,Patable,8);		//writes output power settings to cc1100, because of powerdown loss setting	"104us"
	
		digitalWrite(TX_LED, HIGH);     				//TX_LED high
		spi_write(STX);									//sends the data over air
		marcstate = 0xFF;								//set unknown/dummy state value
		
		while(marcstate != 0x01)						//0x01 = IDLE 
		{
			marcstate = spi_read(MARCSTATE_ADDR);		//read out state of cc1100 to be sure in IDLE and TX is finished
			//printf("Marcstate: 0x%02X\r", marcstate);
		}

		/*if (cc1100.spi_read(IOCFG2_ADDR) == 0x06)		//is GDO2 sets for sync word assert
		{
			// Wait for the sync word to be transmitted
			while(digitalRead(GDO2) == FALSE); 			// Wait until GDO0 line goes high

			// Wait until the end of the packet transmission
			while(digitalRead(GDO2) == TRUE);			// Wait until GDO0 line goes low
		}*/

		spi_write(SFTX);								//flush the Tx_fifo content -> a must for interrupt handling
		digitalWrite(TX_LED, LOW);     					//TX_LED high
	}
//----------------------[end]---------------------------------------	

//------------------[receive mode]----------------------------------
void CC1100::cc1100_receive(void)
{
	uint8_t marcstate;

	spi_write(SIDLE);								//sets to idle first. must be in
	spi_write(SRX);									//writes receive strobe (receive mode)
	marcstate = 0xFF;									//set unknown/dummy state value
	
	while(marcstate != 0x0D)							//0x0D = RX 
	{
		marcstate = spi_read(MARCSTATE_ADDR);			//read out state of cc1100 to be sure in RX
		//printf("Marcstate: 0x%02X\r", marcstate);
	}
}	
//----------------------[end]---------------------------------------

//----------------[tx_payload_burst]--------------------------------
uint8_t CC1100::cc1100_tx_payload_burst(uint8_t my_addr,uint8_t rx_addr, uint8_t *txbuffer, uint8_t length)
{
	txbuffer[0] = length-1;
	txbuffer[1] = rx_addr;
	txbuffer[2] = my_addr;

	spi_write_burst(TX_FIFO_ADDR,txbuffer,length);		//writes TX_Buffer +1 because of pktlen must be also transfered
	
	if (cc1100_debug == 1) {
		printf("TX_FIFO: ");
		for(uint8_t i = 0 ; i < length; i++) 			//TX_fifo debug out
		{
			printf("0x%02X ", txbuffer[i]);
		}
		printf("\r\n");
	}
	return TRUE;
}
//----------------------[end]---------------------------------------

//------------[rx_payload_burst - package received]-----------------
uint8_t CC1100::cc1100_rx_payload_burst()
	{
	
	extern uint8_t Rx_fifo[];
	extern volatile uint8_t Rx_addr, Tx_addr, Pktlen, Lqi;
	extern volatile int8_t Rssi_dbm;
	
	Pktlen = spi_read(RX_FIFO_ADDR_SINGLE);						//received pktlen +1 for complete TX buffer
	Rx_fifo[0] = Pktlen;
	
	for(uint8_t i=1;i<Pktlen+3;i++)								//+3 because of i=1 
		{
			Rx_fifo[i] = spi_read(RX_FIFO_ADDR_SINGLE);
		}
	Rx_addr = Rx_fifo[1];										//my address
	Tx_addr = Rx_fifo[2];										//address from sender

	Rssi_dbm = cc1100_rssi_convert(Rx_fifo[Pktlen + 1]);		//RSSI value in dbm
	Lqi = Rx_fifo[Pktlen + 2];									//LQI  value in hex
	
	spi_write(SFRX);											//flush the RX_fifo content -> a must for interrupt handling
	
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
			delay(1);									//delay till system has data available
			if (cc1100_check_packet_received())
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
	
	if	(cc1100_check_acknolage() == TRUE) 					//acknolage received!
		{
			digitalWrite(RX_LED, LOW);     					//RX_LED LOW
			return FALSE;
		}
	else 													//real data, and sent acknoladge
		{
		
		if (cc1100_debug == 1) {
			printf("RX_FIFO: ");
			for(uint8_t i = 0 ; i < Pktlen + 3; i++) 		//showes rx_buffer for debug
			{
				printf("0x%02X ", Rx_fifo[i]);
			}
			printf("\r\n");
			printf("RSSI:%d ", Rssi_dbm);printf("LQI:0x%02X\r\n", Lqi);
		}
		
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
		if (cc1100_debug == 1) {
			printf("Ack_OK! RSSI:%d ", Rssi_dbm);	printf("Lqi:0x%02X\r\n", Lqi);
		}
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
	cc1100_receive();											//set CC1100 in receive mode
	
	if (cc1100_debug == 1) {
		printf("Ack_sent!\r\n");
	}
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
	spi_write(SIDLE);
	spi_write(SPWD);											//set to Power Down stage
	return TRUE;
}
//----------------------[end]--------------------------------

//---------------------[WakeUp]--------------------------------
uint8_t CC1100::cc1100_wakeup(void)
{
	digitalWrite(SS_PIN, LOW);     		// CS low
	delayMicroseconds(10);
	digitalWrite(SS_PIN, HIGH);      	// CS high
	delayMicroseconds(10);
	cc1100_receive();					// go to RX Mode
	
	return TRUE;
}
//----------------------[end]--------------------------------

//------------------[set transmit mode]--------------------------------
uint8_t CC1100::cc1100_set_mode(uint8_t mode)
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
	
	return TRUE;
}
//----------------------[end]--------------------------------

//------------------[set channel]--------------------------------
uint8_t CC1100::cc1100_set_myaddr(uint8_t addr)
{
	spi_write_register(ADDR_ADDR,addr);

	return TRUE;
}
//----------------------[end]--------------------------------

//------------------[set ISM Band]--------------------------------
uint8_t CC1100::cc1100_set_ISM(uint8_t ism_freq)
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
					spi_write_burst(PATABLE_BURST,patable_power_868,8);			//sets up output power ramp register
					break;
	}

	spi_write_register(FREQ2_ADDR,freq2);
	spi_write_register(FREQ1_ADDR,freq1);
	spi_write_register(FREQ0_ADDR,freq0);

	//cc1100_receive();														//goes to RX mode
	
	return TRUE;
}
//----------------------[end]--------------------------------

//------------------[set channel]--------------------------------
uint8_t CC1100::cc1100_set_channel(uint8_t channel)
{
	spi_write_register(CHANNR_ADDR,channel);

	return TRUE;
}
//----------------------[end]--------------------------------

//------------------[set channel]--------------------------------
uint8_t CC1100::cc1100_set_patable(uint8_t *patable_arr)
{
	spi_write_burst(PATABLE_BURST,patable_arr,8);								//writes output power settings to cc1100	"104us"
	
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
void CC1100::spi_init(void)
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
uint8_t CC1100::spi_write_register(uint8_t reg_addr, uint8_t value)
{
	uint8_t tbuf[2] = {0};
	tbuf[0] = reg_addr | WRITE_SINGLE_BYTE;
	tbuf[1] = value;
	uint8_t len = 2;
	wiringPiSPIDataRW (0, tbuf, len) ;

	return TRUE;
}
//|============================ Ein Register lesen ============================|
uint8_t CC1100::spi_read(uint8_t spi_instr)
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
void CC1100::spi_write(uint8_t spi_instr)
{
	uint8_t tbuf[1] = {0};
	tbuf[0] = spi_instr | WRITE_SINGLE_BYTE;
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
