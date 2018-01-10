CC1101
======

driver library for Ti CC1100 / CC1101.<br />
Contains Lib for Arduino and Raspberry Pi.<br />
Note: Raspi need wiringPi<br />

Donation
========

If you are happy with the library and you want to
spend me a beer, please feel free to use the following link. ;)

https://www.paypal.me/bringmichzumschotter


Hardware connection
===================

check cc1101_arduino.h and/or cc1101_raspi.h for Pin description

CC1101 Vdd = 3.3V
CC1101 max. digital voltage level = 3.3V (not 5V tolerant)

```
CC1101<->Arduino

Vdd    -    3.3V
SI     -    MOSI (11)
SO     -    MISO (12)
CS     -    SS   (10)
SCLK   -    SCK  (13)
GDO2   -    GPIO ( 3)
GDO0   -    not used in this demo
GND    -    GND


CC1101<->Raspi

Vdd    -    3.3V (P1-01)
SI     -    MOSI (P1-19)
SO     -    MISO (P1-21)
CS     -    SS   (P1-24)
SCLK   -    SCK  (P1-23)
GDO2   -    GPIO (P1-22)
GDO0   -    not used in this demo
GND    -    P1-25
```

General description of RF packet
================================

```
-> pkt_len [1byte] | rx_addr [1byte] | tx_addr [1byte] | payload data [1..60bytes]
```

pkt_len = count of bytes which shall transfered over air (rx_addr + tx_addr + payload data)<br />
rx_addr = address of device, which shall receive the message (0x00 = broadcast to all devices)<br />
tx_addr = transmitter or my address. the receiver should know who has sent a message.<br />
payload = 1 to 60 bytes payload data.<br />

TX Bytes example:<br />
-> 0x06 0x03 0x01 0x00 0x01 0x02 0x03<br />

Basic configuration
===================

use **uint8_t CC1100::begin(volatile uint8_t &My_addr)** always as first configuration step. For Arduino devices, this function returns the device address, which was already stored in the Arduino EEPROM.

Device address
--------------
you should set a unique device address for the transmitter and a unique device address for the receiver. 
This can be done with **void CC1100::set_myaddr(uint8_t addr)**.

i.E. -> TX = 0x01 ; RX = 0x03


Modulation modes
----------------
the following modulation modes can be set by **void CC1100::set_mode(uint8_t mode)**. Transmitter and receiver must have the same Mode setting.

```
1 = GFSK_1_2_kb
2 = GFSK_38_4_kb
3 = GFSK_100_kb
4 = MSK_250_kb
5 = MSK_500_kb
6 = OOK_4_8_kb
```

ISM frequency band
------------------
you can set a frequency operation band by **void CC1100::set_ISM(uint8_t ism_freq)** to make it compatible with your hardware.

```
1 = 315
2 = 433
3 = 868
4 = 915
```

Arduino specific
================

CC1101 RF settings must be stored in the Arduino EEPROM to have maximum flexibility with different mode settings and reduced memory usage.
Follow the following steps, how to store the compiled EEPROM file (*.eep) to your Arduino EEPROM. From my experience, you have to repeat this step only, if you have changed the Arduino Version, because the gcc compiler defines the location of the eeprom settings.

- compile the tx_demo or rx_demo example sketch
- remember the path of your compiled output data (Arduino *.hex file and *.eep file)
- use the python eeprom_create.py to generate the eeprom array for the eeprom_write.ino
  This is needed because the compiler can choose the EEPROM position by its own.
- usage: ``` ./eeprom_create.py <input *.eep file> ```
- you get an output file with like *.array
- open that file and copy the array content into the eeprom_write.ino sketch at the correct position
- compile the eeprom_write.ino sketch
- upload into to your connected arduino hardware
- open the Arduino Serial console, set the baudrate to 38400 and restart your arduino hardware
- type the character ```w``` to the input field and press the sent button
- wait till eeprom is written
- sent ```r``` to verify that eeprom is written.
- if your EEPROM data is written correct, you can compile and upload the RX_Demo or TX_Demo sketch to that hardware


Rapsberry Pi
============

How to compile Raspi Demo files
-------------------------------

be sure first, that you have already wireingPi installed on your Raspberry Pi hardware. 

copy RX_Demo.cpp, TX_Demo.cpp, cc1100_raspi.cpp, cc1100_raspi.h in the same directory and compile: <br />

RX_Demo.cpp<br />
```
sudo g++ -lwiringPi RX_Demo.cpp cc1100_raspi.cpp -o RX_Demo
sudo chmod 755 RX_Demo
```

TX_Demo.cpp<br />
```
sudo g++ -lwiringPi TX_Demo.cpp cc1100_raspi.cpp -o TX_Demo
sudo chmod 755 TX_Demo
```

Command Line parameters
-----------------------

TX_Demo:<br />
```
CC1100 SW [-h] [-V] [-a My_Addr] [-r RxDemo_Addr] [-i Msg_Interval] [-t tx_retries] [-c channel] [-f frequency]
          [-m modulation]

  -h              			print this help and exit
  -V              			print version and exit
  -v              			set verbose flag
  -a my address [1-255] 		set my address
  -r rx address [1-255] 	  	set RxDemo receiver address
  -i interval ms[1-6000] 	  	sets message interval timing
  -t tx_retries [0-255] 	  	sets message send retries
  -c channel    [1-255] 		set transmit channel
  -f frequency  [315,434,868,915]  	set ISM band
  -m modulation [100,250,500]           set modulation
  ```
  
  Example,<br />
  ```
  sudo ./TX_Demo -v -a1 -r3 -i1000 -t5 -c1 -f434 -m100
  ```
  
  RX_Demo:<br />
  ```
  CC1100 SW [-h] [-V] [-v] [-a My_Addr] [-c channel] [-f frequency] [-m modulation]
  -h              			print this help and exit
  -V              			print version and exit
  -v              			set verbose flag
  -a my address [1-255] 		set my address
  -c channel    [1-255] 		set transmit channel
  -f frequency  [315,434,868,915]  	set ISM band
  -m modulation [100,250,500]           set modulation
  ```
  
  Example,<br />
  ```
  sudo ./RX_Demo -v -a3 -c1 -f434 -m100
  ```
