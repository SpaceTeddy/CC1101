CC1101
======

driver library for Ti CC1100 / CC1101.<br />
Contains Lib for Arduino and Raspberry Pi.<br />
Note: Raspi need wiringPi<br />



Hardware connection:
====================
check cc1101_arduino.h or cc1101_raspi.h for Pin description

CC1101 Vdd = 3.3V
CC1101 max. digital voltage level = 3.3V (not 5V tolerant)

CC1101<->Arduino

SI     -    MOSI (11)<br />
SO     -    MISO (12)<br />
CS     -    SS   (10)<br />
SCLK   -    SCK  (13)<br />
GDO2   -    free GPIO (3) <br />
GDO0   -    not used in this demo<br />
GND    -    GND<br />


CC1101<->Raspi

Vdd    -    3.3V (P1-01)<br />
SI     -    MOSI (P1-19)<br />
SO     -    MISO (P1-21)<br />
CS     -    SS   (P1-24)<br />
SCLK   -    SCK  (P1-23)<br />
GDO2   -    free GPIO (P1-22) <br />
GDO0   -    not used in this demo<br />
GND    -    P1-25<br />

How to compile Raspi Demos:
===========================
copy RX_Demo.cpp, TX_Demo.cpp, cc1100_raspi.cpp, cc1100_raspi.h in the same directory and compile <br />

RX_Demo.cpp<br />
sudo g++ -lwiringPi RX_Demo.cpp cc1100_raspi.cpp -o RX_Demo<br />
sudo chmod 755 RX_Demo<br />
<br />
TX_Demo.cpp<br />
sudo g++ -lwiringPi TX_Demo.cpp cc1100_raspi.cpp -o TX_Demo<br />
sudo chmod 755 TX_Demo<br />
<br />
Command Line parameters:<br />
========================<br />
CC1100 SW [-h] [-V] [-q] [-a My_Addr] [-r Rx_Addr] [-c channel] [-f frequency]<br />
          [-m modulation]<br />
<br />
  -h              			print this help and exit<br />
  -V              			print version and exit<br />
  -v              			set verbose flag<br />
  -q              			set quite mode flag<br />
  -a my address [1-255] 		set my address<br />
  -c channel 	[1-255] 		set transmit channel<br />
  -f frequency  [315,434,868,915]  	set ISM band<br />
  -m modulation [100,250,500] 		set modulation<br />
  
  Example,<br />
  sudo ./TX_Demo -a1 -c1 -f434 -m100<br />
  sudo ./RX_Demo -a3 -c1 -f434 -m100<br />
  
  Note: in TX_demo program, the receiver address for RX_demo is hardcoded to ADDR = 0x03.<br /> 
 
