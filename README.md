CC1101
======

driver library for Ti CC1100 / CC1101. 
Contains Lib for Arduino and Raspberry Pi. 
Note: Raspi need wiringPi 


Hardware connection:
====================
check cc1101_arduino.h or cc1101_raspi.h for Pin description

CC1101 Vdd = 3.3V
CC1101 max. digital voltage level = 3.3V (not 5V tolerant)

CC1101<->Arduino

SI     -      MOSI (11)
SO     -      MISO (12)
CS     -      SS   (10)
SCLK   -      SCK  (13)
GDO2   -      free GPIO (3) 
GDO0   -      not used in this demo
GND    -      GND


CC1101<->Raspi

Vdd    -    3.3V (P1-01)
SI     -    MOSI (P1-19)
SO     -    MISO (P1-21)
CS     -    SS   (P1-24)
SCLK   -    SCK  (P1-23)
GDO2   -    free GPIO (P1-22) 
GDO0   -    not used in this demo
GND    -    P1-25

