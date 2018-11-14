EESchema Schematic File Version 4
LIBS:ArchMage-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Microchip_ATmega:ATmega1284P-AU U1
U 1 1 5BB328B5
P 5700 3700
F 0 "U1" H 5700 1614 50  0000 C CNN
F 1 "ATmega1284P-AU" H 5600 2150 50  0000 C CNN
F 2 "Housings_QFP:TQFP-44_10x10mm_Pitch0.8mm" H 5700 3700 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-8272-8-bit-AVR-microcontroller-ATmega164A_PA-324A_PA-644A_PA-1284_P_datasheet.pdf" H 5700 3700 50  0001 C CNN
	1    5700 3700
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x04_Counter_Clockwise J2
U 1 1 5BB3296C
P 8150 3500
F 0 "J2" H 8200 3817 50  0000 C CNN
F 1 "DIP8" H 8200 3726 50  0000 C CNN
F 2 "Housings_DIP:DIP-8_W7.62mm" H 8150 3500 50  0001 C CNN
F 3 "~" H 8150 3500 50  0001 C CNN
	1    8150 3500
	1    0    0    -1  
$EndComp
Text GLabel 7950 3400 0    50   Input ~ 0
RESET
Text GLabel 7950 3700 0    50   Input ~ 0
GND
Text GLabel 8450 3400 2    50   Input ~ 0
Vcc
Text GLabel 7950 3600 0    50   Input ~ 0
AD2
Text GLabel 7950 3500 0    50   Input ~ 0
PB3
Text GLabel 5100 2000 0    50   Input ~ 0
RESET
Text GLabel 5750 1500 1    50   Input ~ 0
Vcc
Wire Wire Line
	5750 1500 5750 1700
Wire Wire Line
	5750 1700 5700 1700
Wire Wire Line
	5750 1700 5800 1700
Connection ~ 5750 1700
Text GLabel 5700 5700 3    50   Input ~ 0
GND
Text GLabel 6300 3600 2    50   Input ~ 0
SCK
Text GLabel 6300 3500 2    50   Input ~ 0
MISO
Text GLabel 6300 3400 2    50   Input ~ 0
MOSI
Text GLabel 6300 3200 2    50   Input ~ 0
PB3
Text GLabel 6300 2200 2    50   Input ~ 0
AD2
Text GLabel 6300 2900 2    50   Input ~ 0
PB0
Text GLabel 6300 3000 2    50   Input ~ 0
PB1
Text GLabel 6300 3100 2    50   Input ~ 0
PB2
Text GLabel 9450 3150 0    50   Input ~ 0
SCK
Text GLabel 9450 3050 0    50   Input ~ 0
MISO
Text GLabel 9950 3150 2    50   Input ~ 0
MOSI
Text GLabel 9950 3050 2    50   Input ~ 0
Vcc
Text GLabel 9950 3250 2    50   Input ~ 0
GND
Text GLabel 9450 3250 0    50   Input ~ 0
RESET
Text GLabel 8800 4000 2    50   Input ~ 0
PB0
Text GLabel 8800 3800 2    50   Input ~ 0
PB1
Text GLabel 8800 3600 2    50   Input ~ 0
PB2
Wire Wire Line
	8800 3600 8750 3600
Wire Wire Line
	8750 3600 8750 3500
Wire Wire Line
	8750 3500 8450 3500
Wire Wire Line
	8700 3600 8450 3600
Wire Wire Line
	8800 3800 8700 3800
Wire Wire Line
	8600 3700 8450 3700
Wire Wire Line
	8800 4000 8600 4000
Wire Wire Line
	8700 3600 8700 3800
Wire Wire Line
	8600 3700 8600 4000
$Comp
L Connector_Generic:Conn_02x03_Odd_Even J1
U 1 1 5BE02E03
P 9650 3150
F 0 "J1" H 9700 3467 50  0000 C CNN
F 1 "ISP" H 9700 3376 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x03_Pitch2.54mm" H 9650 3150 50  0001 C CNN
F 3 "~" H 9650 3150 50  0001 C CNN
	1    9650 3150
	1    0    0    -1  
$EndComp
$EndSCHEMATC
