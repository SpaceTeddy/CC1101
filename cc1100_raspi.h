#ifndef cc1100_H
#define cc1100_H

#include <stdint.h>

/*-------------------------------[debug output 1/0]----------------------------*/
#define CC1100_DEBUG 			1

/*----------------------------------[standard]---------------------------------*/
#define TRUE  					1
#define FALSE 					0


//*************************************** pins **************************************************//
#define SCK_PIN   				14
#define MISO_PIN  				13
#define MOSI_PIN  				12
#define SS_PIN    				10
#define GDO2	  				6
//#define GDO0	  				5										no need for Raspberrry becasue of missing ADC
#define RX_LED	  				6
#define TX_LED	  				7

/*-------------------------------[CC1100 - misc]---------------------------------*/
#define CFG_REGISTER  			0x2F									//47 registers
#define FIFOBUFFER			0x42
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
//#define CC1100_TEMP_ADC_MV		3225									//3.3V/1023 . mV pro digit
//#define CC1100_TEMP_CELS_CO		2470									//Temperature coefficient 2.47mV per Grad Celsius

/*---------------------------[CC1100 - R/W offsets]------------------------------*/
#define WRITE_SINGLE_BYTE  		0x00
#define WRITE_BURST  			0x40
#define READ_SINGLE_BYTE  		0x80
#define READ_BURST  			0xC0
#define PATABLE_BURST  			0x7E                                //power control read/write
#define PATABLE_READ_BURST		0xFE                                //power control read/write

/*------------------------------[END R/W offsets]--------------------------------*/

/*---------------------------[CC1100 - FIFO commands]----------------------------*/
#define TX_FIFO_ADDR  			0x7F                                    //write burst only
#define TX_FIFO_ADDR_SINGLE 	0x3F                            		//write single only

#define RX_FIFO_ADDR  			0xFF                                    //read burst only
#define RX_FIFO_ADDR_SINGLE 	0xBF                            		//read single only
/*----------------------------[END FIFO commands]------------------------------*/

/*-------------------------[CC1100 - config register]----------------------------*/
#define IOCFG2_ADDR  			0x00                                    // GDO2 output pin configuration
#define IOCFG1_ADDR  			0x01                                    // GDO1 output pin configuration
#define IOCFG0_ADDR  			0x02                                    // GDO0 output pin configuration
#define FIFOTHR_ADDR  			0x03                                    // RX FIFO and TX FIFO thresholds
#define SYNC1_ADDR  			0x04                                    // Sync word, high byte
#define SYNC0_ADDR  			0x05                                    // Sync word, low byte
#define PKTLEN_ADDR  			0x06                                    // Packet length
#define PKTCTRL1_ADDR  			0x07                                  	// Packet automation control
#define PKTCTRL0_ADDR  			0x08                                  	// Packet automation control
#define ADDR_ADDR  				0x09                                    // Device address
#define CHANNR_ADDR  			0x0A                                    // Channel number
#define FSCTRL1_ADDR  			0x0B                                   	// Frequency synthesizer control
#define FSCTRL0_ADDR  			0x0C                                   	// Frequency synthesizer control
#define FREQ2_ADDR  			0x0D                                    // Frequency control word, high byte
#define FREQ1_ADDR  			0x0E                                    // Frequency control word, middle byte
#define FREQ0_ADDR  			0x0F                                    // Frequency control word, low byte

#define MDMCFG4_ADDR  			0x10                                   	// Modem configuration
#define MDMCFG3_ADDR  			0x11                                   	// Modem configuration
#define MDMCFG2_ADDR  			0x12                                   	// Modem configuration
#define MDMCFG1_ADDR  			0x13                                   	// Modem configuration
#define MDMCFG0_ADDR  			0x14                                   	// Modem configuration
#define DEVIATN_ADDR  			0x15                                   	// Modem deviation setting
#define MCSM2_ADDR  			0x16                                    // Main Radio Cntrl State Machine config
#define MCSM1_ADDR  			0x17                                    // Main Radio Cntrl State Machine config
#define MCSM0_ADDR  			0x18                                    // Main Radio Cntrl State Machine config
#define FOCCFG_ADDR  			0x19	                                // Frequency Offset Compensation config
#define BSCFG_ADDR  			0x1A                                    // Bit Synchronization configuration
#define AGCTRL2_ADDR 			0x1B                                    // AGC control
#define AGCTRL1_ADDR 			0x1C                                    // AGC control
#define AGCTRL0_ADDR 			0x1D                                    // AGC control
#define WOREVT1_ADDR 			0x1E                                   	// High byte Event 0 timeout
#define WOREVT0_ADDR 			0x1F                                   	// Low byte Event 0 timeout

#define WORCTRL_ADDR 			0x20                                   	// Wake On Radio control
#define FREND1_ADDR 			0x21                                    // Front end RX configuration
#define FREND0_ADDR 			0x22                                    // Front end TX configuration
#define FSCAL3_ADDR 			0x23                                    // Frequency synthesizer calibration
#define FSCAL2_ADDR 			0x24                                    // Frequency synthesizer calibration
#define FSCAL1_ADDR 			0x25                                    // Frequency synthesizer calibration
#define FSCAL0_ADDR 			0x26                                    // Frequency synthesizer calibration
#define RCCTRL1_ADDR 			0x27                                   	// RC oscillator configuration
#define RCCTRL0_ADDR 			0x28                                   	// RC oscillator configuration
#define FSTEST_ADDR 			0x29                                   	// Frequency synthesizer cal control
#define PTEST_ADDR 				0x2A                                    // Production test
#define AGCTEST_ADDR 			0x2B                                   	// AGC test
#define TEST2_ADDR 				0x2C                                    // Various test settings
#define TEST1_ADDR 				0x2D                                    // Various test settings
#define TEST0_ADDR 				0x2E                                    // Various test settings

#define PATABLE_ADDR  			0x3E                                    // Pa Table Adress

/*----------------------------[END config register]------------------------------*/

/*---------------------------[CC1100-command strobes]----------------------------*/
#define SRES  					0x30                                    // Reset chip
#define SFSTXON  				0x31                                    // Enable/calibrate freq synthesizer
#define SXOFF  					0x32                                    // Turn off crystal oscillator.
#define SCAL 					0x33                                    // Calibrate freq synthesizer & disable
#define SRX  					0x34                                    // Enable RX.
#define STX  					0x35                                    // Enable TX.
#define SIDLE  					0x36                                    // Exit RX / TX
#define SAFC  					0x37                                    // AFC adjustment of freq synthesizer
#define SWOR  					0x38                                    // Start automatic RX polling sequence
#define SPWD  					0x39                                    // Enter pwr down mode when CSn goes hi
#define SFRX  					0x3A                                    // Flush the RX FIFO buffer.
#define SFTX  					0x3B                                    // Flush the TX FIFO buffer.
#define SWORRST  				0x3C                                    // Reset real time clock.
#define SNOP  					0x3D                                    // No operation.
/*----------------------------[END command strobes]------------------------------*/

/*-------------------------[CC1100 - status register]----------------------------*/
#define PARTNUM_ADDR 			0xF0									// Part number
#define VERSION_ADDR 			0xF1									// Current version number
#define FREQEST_ADDR 			0xF2									// Frequency offset estimate
#define LQI_ADDR 				0xF3									// Demodulator estimate for link quality
#define RSSI_ADDR 				0xF4									// Received signal strength indication
#define MARCSTATE_ADDR 			0xF5									// Control state machine state
#define WORTIME1_ADDR 			0xF6									// High byte of WOR timer
#define WORTIME0_ADDR 			0xF7									// Low byte of WOR timer
#define PKTSTATUS_ADDR 			0xF8									// Current GDOx status and packet status
#define VCO_VC_DAC_ADDR 		0xF9									// Current setting from PLL cal module
#define TXBYTES_ADDR 			0xFA									// Underflow and # of bytes in TXFIFO
#define RXBYTES_ADDR 			0xFB									// Overflow and # of bytes in RXFIFO
//----------------------------[END status register]-------------------------------

class CC1100
{
	private:
		void spi_init(void);
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

		uint8_t spi_write_register(uint8_t reg_addr, uint8_t value);
		uint8_t spi_read(uint8_t spi_instr);
		void spi_write(uint8_t spi_instr);
		void spi_read_burst(uint8_t spi_instr, uint8_t *pArr, uint8_t len);
		void spi_write_burst(uint8_t spi_instr, uint8_t *pArr, uint8_t len);

};

extern CC1100 cc1100;



//=========================[CC1100 special functions]=============================


#endif // CC1100_H
