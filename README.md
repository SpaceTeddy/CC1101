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
