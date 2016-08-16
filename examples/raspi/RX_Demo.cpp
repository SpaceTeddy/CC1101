#include <cc1100.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <termios.h>
#include <sys/select.h>
#include <iostream>     // std::cin, std::cout, std::hex

#include <getopt.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define PACKAGE    "CC1100 SW"
#define VERSION_SW    "0.0.1"

struct termios orig_termios;

//--------------------------[Global CC1100 variables]--------------------------
uint8_t Tx_fifo[FIFOBUFFER], Rx_fifo[FIFOBUFFER];
uint8_t My_addr, Tx_addr, Rx_addr, Pktlen, pktlen, Lqi, Rssi;
uint8_t rx_addr,sender,lqi;
 int8_t rssi_dbm;
volatile uint8_t cc1101_packet_available;


int cc1100_freq_select, cc1100_mode_select, cc1100_channel_select;
uint8_t cc1100_debug = 0;								//set CC1100 lib in no-debug output mode
volatile uint8_t quite_mode=0;
//-------------------------- [End] --------------------------

void print_help(int exval) {
	printf("%s,%s by CW\r\n", PACKAGE, VERSION_SW);
	printf("%s [-h] [-V] [-q] [-a My_Addr] [-r Rx_Addr] [-c channel] [-f frequency]\r\n", PACKAGE);
	printf("          [-m modulation]\n\r\n\r");
	printf("  -h              			print this help and exit\r\n");
	printf("  -V              			print version and exit\r\n\r\n");

	printf("  -v              			set verbose flag\r\n");
	printf("  -q              			set quite mode flag\r\n");
	printf("  -a my address [1-255] 		set my address\r\n\r\n");
	//printf("  -r rx address [1-255] 	  	set my address\r\n\r\n");
	printf("  -c channel 	[1-255] 		set transmit channel\r\n");
	printf("  -f frequency  [315,434,868,915]  	set ISM band\r\n\r\n");
	printf("  -m modulation [100,250,500] 		set modulation\r\n\r\n");

	exit(exval);
}

//------------- keyboard setting -------------------
void reset_terminal_mode()
{
    tcsetattr(0,TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
    struct termios new_termios;

    /* take two copies */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler and set new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    new_termios.c_oflag |= OPOST;
    tcsetattr(0, TCSANOW, &new_termios);
    setvbuf(stdout, (char *)NULL, _IONBF, 0);		//disables printf() buffer
    //setvbuf(stdout, (char *)NULL, _IOLBF, 0);		//enable printf() buffer
}

//|============================ Main ============================|
int main(int argc, char *argv[]) {
	//------------- command line option parser -------------------
	int opt;
	// no arguments given
	if(argc == 1) {
		fprintf(stderr, "This program needs arguments....\n\r\n\r");
		print_help(1);
	}

	while((opt = getopt(argc, argv, "hVvqa:c:f:m:")) != -1) {
		switch(opt) {
		case 'h':
			print_help(0);
			break;
		case 'V':
			printf("%s %s\n\n", PACKAGE, VERSION_SW);
			exit(0);
			break;
		case 'v':
			printf("%s: Verbose option is set `%c'\n", PACKAGE, optopt);
			break;
		case 'q':
			printf("%s: quite mode is set `%c'\n", PACKAGE, optopt);
			quite_mode = 1;
			break;
		case 'a':
			My_addr = atoi (optarg);
			break;
		case 'c':
			cc1100_channel_select = atoi (optarg);
			break;
		case 'f':
			cc1100_freq_select = atoi (optarg);
			switch(cc1100_freq_select){
				case 315:
					cc1100_freq_select = 1;
					break;
				case 434:
					cc1100_freq_select = 2;
					break;
				case 868:
					cc1100_freq_select = 3;
					break;
				case 915:
					cc1100_freq_select = 4;
					break;
				}
			break;
			case 'm':
				cc1100_mode_select = atoi (optarg);

				switch(cc1100_mode_select){
				case 1:
					cc1100_mode_select = 1;
					break;
				case 38:
					cc1100_mode_select = 2;
					break;
				
				case 100:
					cc1100_mode_select = 3;
					break;
				case 250:
					cc1100_mode_select = 4;
					break;
				case 500:
					cc1100_mode_select = 5;
					break;
				}
				break;
				case ':':
					fprintf(stderr, "%s: Error - Option `%c' needs a value\n\n", PACKAGE, optopt);
					print_help(1);
					break;
				case '?':
					fprintf(stderr, "%s: Error - No such option: `%c'\n\n", PACKAGE, optopt);
					print_help(1);
		}
	}

	// print all remaining options
	for(; optind < argc; optind++)
		printf("argument: %s\n", argv[optind]);

	//------------- welcome message ------------------------
	printf("Raspberry CC1101 SPI Library test\n");

	//------------- hardware setup ------------------------
	if(quite_mode == 1){
		cc1100_debug = 0;								//set CC1100 lib in no-debug output mode
	}else if(quite_mode == 0){
		cc1100_debug = 1;
	}
	set_conio_terminal_mode();							//setup console input/output

	wiringPiSetup();									//setup wiringPi library

	cc1100.begin(My_addr);								//setup cc1000 RF IC
	cc1100.silde();
	//cc1100.set_mode(0x04);                   //set modulation mode
	//cc1100.set_ISM(0x03);                    //set frequency
	//cc1100.set_channel(0x01);                //set channel
	cc1100.set_output_power_level(10);        //set PA level
	//cc1100.set_myaddr(0x05);
	//cc1100.spi_write_register(IOCFG2, 0x06); //set module in sync mode detection mode
	cc1100.receive();

	cc1100.show_main_settings();             //shows setting debug messages to UART

	//------------------------- Main Loop ------------------------
	for (;;) {
		delay(1);                            //delay to reduce system load
		
		if (cc1100.packet_available())		 //checks if a packed is available
		{
		  cc1100.get_payload(Rx_fifo, pktlen, rx_addr, sender, rssi_dbm, lqi); //stores the payload data to Rx_fifo
			cc1101_packet_available = TRUE;							//ready for next packet
		}
	}
	return 0;
}
//-------------------- END OF MAIN --------------------------------
