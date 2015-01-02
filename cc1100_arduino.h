#ifndef cc1100_H
#define cc1100_H

#include <Arduino.h>
#include <avr/eeprom.h>
#include <util/delay.h>


/*-------------------------------[debug output 1/0]----------------------------*/
#define CC1100_DEBUG 1

/*----------------------------------[standard]---------------------------------*/
#define TRUE  1
#define FALSE 0

//|=============================[EEPROM addresses]=============================
#define EEPROM_ADDRESS_CC1100_FREQUENCY	0x150
#define EEPROM_ADDRESS_CC1100_MODE		0x151
#define EEPROM_ADDRESS_CC1100_MY_ADDR	0x152
#define EEPROM_ADDRESS_CC1100_CHANNEL	0x153

//*************************************** pins **************************************************//
#define SCK_PIN   13
#define MISO_PIN  12
#define MOSI_PIN  11
#define SS_PIN    10
#define GDO2	  9
#define GDO0	  A0
#define RX_LED	  6
#define TX_LED	  7

/*-------------------------------[CC1100 - misc]---------------------------------*/
#define CFG_REGISTER  			0x2F									//47 registers
#define FIFOBUFFER				0x42
#define RSSI_OFFSET_868MHZ		0x4E									//dec = 74
#define TX_RETRIES_MAX			0x05									//tx_retries_max
#define ACK_TIMEOUT				25										//ACK timeout in ms
#define CC1100_COMPARE_REGISTER 0x00									//register compare 0=no compare 1=compare
#define BROADCAST_ADDRESS 		0x00									//broadcast address
#define CC1100_FREQ_315MHZ		0x01	
#define CC1100_FREQ_434MHZ		0x02
#define CC1100_FREQ_868MHZ		0x03
#define CC1100_FREQ_915MHZ		0x04
//#define CC1100_FREQ_2430MHZ		0x05
#define CC1100_TEMP_ADC_MV		3225									//3.3V/1023 . mV pro digit
#define CC1100_TEMP_CELS_CO		2470									//Temperature coefficient 2.47mV per Grad Celsius
/*---------------------------[CC1100 - R/W offsets]------------------------------*/
#define Write_single_byte  		0x00
#define Write_burst  			0x40
#define Read_single_byte  		0x80
#define Read_burst  			0xC0
#define Patable_burst  			0x7E                                    //power control read/write
#define Patable_read_burst		0xFE                                    //power control read/write

/*------------------------------[END R/W offsets]--------------------------------*/

/*---------------------------[CC1100 - FIFO commands]----------------------------*/
#define Tx_fifo_addr  		0x7F                                    //write burst only
#define Tx_fifo_addr_single 0x3F                            		//write single only

#define Rx_fifo_addr  		0xFF                                    //read burst only
#define Rx_fifo_addr_single 0xBF                            		//read single only
/*----------------------------[END FIFO commands]------------------------------*/

/*-------------------------[CC1100 - config register]----------------------------*/
#define Patable_addr  		0x3E                                    // Pa Table Adress

#define Iocfg2_addr  		0x00                                    // GDO2 output pin configuration
#define Iocfg1_addr  		0x01                                    // GDO1 output pin configuration
#define Iocfg0_addr  		0x02                                    // GDO0 output pin configuration
#define Fifothr_addr  		0x03                                    // RX FIFO and TX FIFO thresholds
#define Sync1_addr  		0x04                                    // Sync word, high byte
#define Sync0_addr  		0x05                                    // Sync word, low byte
#define Pktlen_addr  		0x06                                    // Packet length
#define Pktctrl1_addr  		0x07                                  	// Packet automation control
#define Pktctrl0_addr  		0x08                                  	// Packet automation control
#define Addr_addr  			0x09                                    // Device address
#define Channr_addr  		0x0A                                    // Channel number
#define Fsctrl1_addr  		0x0B                                   	// Frequency synthesizer control
#define Fsctrl0_addr  		0x0C                                   	// Frequency synthesizer control
#define Freq2_addr  		0x0D                                    // Frequency control word, high byte
#define Freq1_addr  		0x0E                                    // Frequency control word, middle byte
#define Freq0_addr  		0x0F                                    // Frequency control word, low byte

#define Mdmcfg4_addr  		0x10                                   	// Modem configuration
#define Mdmcfg3_addr  		0x11                                   	// Modem configuration
#define Mdmcfg2_addr  		0x12                                   	// Modem configuration
#define Mdmcfg1_addr  		0x13                                   	// Modem configuration
#define Mdmcfg0_addr  		0x14                                   	// Modem configuration
#define Deviatn_addr  		0x15                                   	// Modem deviation setting
#define Mcsm2_addr  		0x16                                    // Main Radio Cntrl State Machine config
#define Mcsm1_addr  		0x17                                    // Main Radio Cntrl State Machine config
#define Mcsm0_addr  		0x18                                    // Main Radio Cntrl State Machine config
#define Foccfg_addr  		0x19	                                // Frequency Offset Compensation config
#define Bscfg_addr  		0x1A                                    // Bit Synchronization configuration
#define Agctrl2_addr 		0x1B                                    // AGC control
#define Agctrl1_addr 		0x1C                                    // AGC control
#define Agctrl0_addr 		0x1D                                    // AGC control
#define Worevt1_addr 		0x1E                                   	// High byte Event 0 timeout
#define Worevt0_addr 		0x1F                                   	// Low byte Event 0 timeout

#define Worctrl_addr 		0x20                                   	// Wake On Radio control
#define Frend1_addr 		0x21                                    // Front end RX configuration
#define Frend0_addr 		0x22                                    // Front end TX configuration
#define Fscal3_addr 		0x23                                    // Frequency synthesizer calibration
#define Fscal2_addr 		0x24                                    // Frequency synthesizer calibration
#define Fscal1_addr 		0x25                                    // Frequency synthesizer calibration
#define Fscal0_addr 		0x26                                    // Frequency synthesizer calibration
#define Rcctrl1_addr 		0x27                                   	// RC oscillator configuration
#define Rcctrl0_addr 		0x28                                   	// RC oscillator configuration
#define Fstest_addr 		0x29                                   	// Frequency synthesizer cal control
#define Ptest_addr 			0x2A                                    // Production test
#define Agctest_addr 		0x2B                                   	// AGC test
#define Test2_addr 			0x2C                                    // Various test settings
#define Test1_addr 			0x2D                                    // Various test settings
#define Test0_addr 			0x2E                                    // Various test settings
/*----------------------------[END config register]------------------------------*/

/*---------------------------[CC1100-command strobes]----------------------------*/
#define Sres  				0x30                                    // Reset chip
#define Sfstxon  			0x31                                    // Enable/calibrate freq synthesizer
#define Sxoff  				0x32                                    // Turn off crystal oscillator.
#define Scal 				0x33                                    // Calibrate freq synthesizer & disable
#define Srx  				0x34                                    // Enable RX.
#define Stx  				0x35                                    // Enable TX.
#define Sidle  				0x36                                    // Exit RX / TX
#define Safc  				0x37                                    // AFC adjustment of freq synthesizer
#define Swor  				0x38                                    // Start automatic RX polling sequence
#define Spwd  				0x39                                    // Enter pwr down mode when CSn goes hi
#define Sfrx  				0x3A                                    // Flush the RX FIFO buffer.
#define Sftx  				0x3B                                    // Flush the TX FIFO buffer.
#define Sworrst  			0x3C                                    // Reset real time clock.
#define Snop  				0x3D                                    // No operation.
/*----------------------------[END command strobes]------------------------------*/

/*-------------------------[CC1100 - status register]----------------------------*/
#define Partnum_addr 		0xF0									// Part number
#define Version_addr 		0xF1									// Current version number
#define Freqest_addr 		0xF2									// Frequency offset estimate
#define Lqi_addr 			0xF3									// Demodulator estimate for link quality
#define Rssi_addr 			0xF4									// Received signal strength indication
#define Marcstate_addr 		0xF5									// Control state machine state
#define Wortime1_addr 		0xF6									// High byte of WOR timer
#define Wortime0_addr 		0xF7									// Low byte of WOR timer
#define Pktstatus_addr 		0xF8									// Current GDOx status and packet status
#define Vco_vc_dac_addr 	0xF9									// Current setting from PLL cal module
#define Txbytes_addr 		0xFA									// Underflow and # of bytes in TXFIFO
#define Rxbytes_addr 		0xFB									// Overflow and # of bytes in RXFIFO
//----------------------------[END status register]-------------------------------


class CC1100
{
	private:
		void spi_init(void);
		uint8_t spi_putc(uint8_t data);
		void cc1100_reset(void);
		void cc1100_transmit(void);
		
		
	public:
	
		int16_t bin2bcd(int8_t a);
		
		uint8_t cc1100_init(void);
		void cc1100_show_register_settings(void);
		void cc1100_show_main_settings(void);
		void cc1100_receive(void);
		uint8_t cc1100_powerdown(void);
		uint8_t cc1100_wakeup(void);
		uint8_t cc1100_rx_payload_single(uint8_t position);
		uint8_t cc1100_check_payload(void);
		uint8_t cc1100_check_packet_received(void);
		uint8_t cc1100_wait_for_packet(uint8_t milliseconds);
		uint8_t cc1100_check_acknolage(void);
		uint8_t cc1100_sent_packet(uint8_t my_addr, uint8_t rx_addr, uint8_t *txbuffer, uint8_t pktlen);
		void cc1100_write_patable(void);
		void cc1100_read_config_register(void);
		int8_t cc1100_rssi_convert(uint8_t Rssi);
		int16_t cc1100_get_temp(void);
		uint8_t cc1100_tx_payload_burst(uint8_t my_addr, uint8_t rx_addr, uint8_t *txbuffer, uint8_t length);
		uint8_t cc1100_rx_payload_burst(void);
		void cc1100_sent_acknolage(void);
		void cc1100_rx_fifo_erase(uint8_t *rxbuffer);
		void cc1100_tx_fifo_erase(uint8_t *txbuffer);
		uint8_t cc1100_set_myaddr(uint8_t addr);
		uint8_t cc1100_set_channel(uint8_t channel);
		uint8_t cc1100_set_ISM(uint8_t ism_freq);
		uint8_t cc1100_set_mode(uint8_t mode);
		uint8_t cc1100_set_patable(uint8_t *patable_arr);
		uint8_t spi_read(uint8_t spi_instr);
		void spi_read_burst(uint8_t spi_instr, uint8_t *pArr, uint8_t length, uint8_t cs);
		void spi_write(uint8_t spi_instr, uint8_t cs);
		void spi_write_burst(uint8_t spi_instr, uint8_t *pArr, uint8_t length, uint8_t cs);
				
		void uart_puthex_nibble(const unsigned char b);
		void uart_puthex_byte(const unsigned char  b);
		void uart_puti(const int val);
};

extern CC1100 cc1100;


//=========================[CC1100 special functions]=============================


#endif // CC1100_H
