# SEMU\_SSD1331

Arduino library for SSD1331-based 96x64 RGB OLED boards

## Description

This library provides a range of hardware optimised graphics functions for the popular .95" 96x64 SSD1331 driver-based 16-bit 
colour SPI OLED boards which are widely available from [Adafruit](https://www.adafruit.com/product/684) and elsewhere.  

Based on the original [Adafruit SSD1331 library](https://github.com/adafruit/Adafruit-SSD1331-OLED-Driver-Library-for-Arduino) with additional hardware-optimised functions. 
It inherits the Adafruit_SPITFT class and is intended to represent a more fully-featured replacement for the Adafruit_SSD1331_OLED library.

Originally motivated by a desire to: 
1. Implement the full range of SSD1331 hardware configuration and graphics commands
2. Add a flexible, hardware-optimised drawImage function from flash memory maps.
3. Add functions to facilitate multi-screen animation.

The image header files used in the examples were created using [Vladimir Riuson's excellent lcd-image-convertor utility](https://github.com/riuson/lcd-image-converter). 
The tImage (16-bit RGB color) and bwImage (8-bit monochrome) templates in the lcd-image-convert folder can be used to create the appropriate PROGMEM structures from bitmaps.

## Current Status

The following hardware graphics functions are implemented, all of which are significantly faster that the equivalent software routines:
* drawImage (from flash (PROGMEM) memory map. Utilises hardware pointer auto-increment to improve speed)
* drawMaskedImage (draw 'masked' image with specified mask (matte) and colour)
* clearWindow
* copyWindow (copies a portion of the display to another area)

The following hardware configuration functions are implemented:
* setOrientation (8 permutations available - normal, 90, 180 and 270 rotation, each of which can also be mirrored)
* setGrayScale (by passing decimal gamma value)
* resetGrayScale (linear gray scale i.e. gamma = 1.0)
* dimWindow
* display on/off/rotate/invert ('negative')
* display scrolling (horizontal and/or vertical, all or selected rows)

## Wiring Connections

<!-- START WIRING TABLE -->

SPI pin markings vary between boards and can be confusing. Boards with a built-in SD card reader tend to have 10 pins, 3 of which (usually marked SO, 
SC and CD) relate specifically to the SD card reader. Boards which have no SD card reader tend to have 7 pins.

![oled_noSDreader](/OLED_type1.jpg) ![oled_withSDreader](/OLED_type2.jpg)

Pin on OLED board       | Pin on Arduino | Purpose
----------------------- | -------------- | ------------------------------------------------------ 
GND (G)                 | GND            | Ground
VCC (\+)                | +5V            | Power **NB** some boards may run off 3.3V - check datasheet
SCL (CK)                | Pin 13         | SPI SCLK Clock (hardware) *1*
SDA (SI)                | Pin 11         | SPI MOSI Master Out, Slave In (hardware) *1*
SO                      | Pin 12         | SPI MISO Master In, Slave Out for SD Card reader, if present (hardware) *1*
RES (R)                 | Digital pin 10 | SPI Reset 
DC (sometimes RS)       | Digital pin 9  | SPI SDIO Serial Data
CS (OC) on first board  | Digital pin 8  | SPI Chip Select pin for first OLED
SC                      | Digital pin 4  | SPI Chip Select pin for SD Card reader, if present
CD                      | N/C            | Used to detect if SD card is inserted, if present

* *1* Check microcontroller datasheet for details of which hardware SPI pins to use.

**If you're using two OLED displays (e.g. to simulate a pair of eyes), the pins on the second board should be configured as above WITH THE EXCEPTION OF:**

Pin on 2nd OLED board   | Pin on Arduino | Purpose
----------------------- | -------------- | ------------------------------------------------------ 
RES (R) on 2nd board    | Digital pin 10 | SPI Reset for second OLED; uses same physical pin as first board but MUST BE assigned to a fictional pin (e.g. RST2 = 99) in the sketch
CS (OC) on 2nd board    | Digital pin 7  | SPI Chip Select for second OLED

<!-- END WIRING TABLE -->

<!-- START COMPATIBILITY TABLE -->

## Compatibility

**NB: Use of drawImage function is subject to flash (program) memory limits on all devices**

MCU                | Tested Works | Doesn't Work | Not Tested  | Notes
------------------ | :----------: | :----------: | :---------: | -----
Arduino UNO        |      X       |              |             | 
Arduino Micro      |      X       |              |             | 
Arduino Zero M0    |      X       |              |             | 
Arduino DUE        |      X       |              |             | 
Adafruit Metro M4  |              |       	     |     X       |
Teensy 3.2 @ 72MHz |      X       |              |             | 
Teensy 3.6 @ 180MHz |      X       |              |             |
Teensy 4.0 @ 600MHz |             |              |     X        |


<!-- END COMPATIBILITY TABLE -->
