EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:ML_Kicad
LIBS:open-project
LIBS:SparkFun
LIBS:w_connectors
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date "21 aug 2014"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L M04LOCK JP2
U 1 1 53CAE238
P 2250 3150
F 0 "JP2" H 2050 3480 50  0000 L BNN
F 1 "OUTPUT" H 2050 2850 50  0000 L BNN
F 2 "SparkFun-1X04_LOCK" H 2250 3300 50  0001 C CNN
F 3 "" H 2250 3150 60  0000 C CNN
	1    2250 3150
	-1   0    0    1   
$EndComp
$Comp
L BARREL_JACK CON1
U 1 1 53D3C759
P 2450 2350
F 0 "CON1" H 2450 2600 60  0000 C CNN
F 1 "BARREL_JACK" H 2450 2150 60  0000 C CNN
F 2 "" H 2450 2350 60  0000 C CNN
F 3 "" H 2450 2350 60  0000 C CNN
	1    2450 2350
	-1   0    0    1   
$EndComp
Text GLabel 2100 2450 0    30   Input ~ 0
VSTRIP
Wire Wire Line
	2100 2450 2150 2450
Text GLabel 2100 2250 0    30   Input ~ 0
GND
Wire Wire Line
	2100 2250 2150 2250
Wire Wire Line
	2150 2250 2150 2350
$Comp
L CP1 C1
U 1 1 53D3C960
P 1500 2450
F 0 "C1" H 1550 2550 50  0000 L CNN
F 1 "CP1" H 1550 2350 50  0000 L CNN
F 2 "~" H 1500 2450 60  0000 C CNN
F 3 "~" H 1500 2450 60  0000 C CNN
	1    1500 2450
	1    0    0    -1  
$EndComp
Text GLabel 1350 2250 0    30   Input ~ 0
VSTRIP
Text GLabel 1350 2650 0    30   Input ~ 0
GND
Wire Wire Line
	1350 2650 1500 2650
Wire Wire Line
	1350 2250 1500 2250
$Comp
L M04LOCK JP1
U 1 1 53F65D4E
P 1600 3150
F 0 "JP1" H 1400 3480 50  0000 L BNN
F 1 "INPUT" H 1400 2850 50  0000 L BNN
F 2 "SparkFun-1X04_LOCK" H 1600 3300 50  0001 C CNN
F 3 "" H 1600 3150 60  0000 C CNN
	1    1600 3150
	1    0    0    1   
$EndComp
Wire Wire Line
	1800 3350 2050 3350
Wire Wire Line
	1800 3250 2050 3250
Wire Wire Line
	1800 3150 2050 3150
NoConn ~ 1800 3050
Text GLabel 2050 3000 1    30   Input ~ 0
VSTRIP
Wire Wire Line
	2050 3000 2050 3050
Text GLabel 2050 3400 3    30   Input ~ 0
GND
Wire Wire Line
	2050 3350 2050 3400
$EndSCHEMATC
