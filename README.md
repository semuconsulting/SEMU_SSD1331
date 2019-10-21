# SEMU\_SSD1331

Arduino library for SSD1331-based 96x64 RGB OLED boards

## Description

This library provides a range of hardware optimised graphics functions for the popular .95" 96x64 SSD1331 driver-based 16-bit 
colour SPI OLED boards which are widely available from [Adafruit](https://www.adafruit.com/product/684) and elsewhere.  

Based on the original [Adafruit SSD1331 library](https://github.com/adafruit/Adafruit-SSD1331-OLED-Driver-Library-for-Arduino) with additional hardware-optimised functions. 
It inherits the Adafruit_SPITFT class and is intended to represent a more fully-featured replacement for the Adafruit_SSD1331_OLED library.

Originally motivated by a desire to: 
1. Implement the full range of SSD1331 hardware configuration and graphics commands.
2. Add a flexible, hardware-optimised drawImage function for bitmaps and masks.
3. Add functions to facilitate bitmap animation and 'green screen' effects.

The image header files used in the examples were created using [Vladimir Riuson's excellent lcd-image-convertor utility](https://github.com/riuson/lcd-image-converter). 
The tImage (16-bit RGB color) and bwImage (8-bit monochrome) templates in the lcd-image-convert folder can be used to create the appropriate PROGMEM structures from bitmaps.

## Current Status

The following structured bitmap display functions are implemented using various hardware and software optimisations:
* drawImage (from flash (PROGMEM) memory). Utilises hardware pointer auto-increment to improve speed - roughly 6-8 times faster than the generic software methods.
Supports 16-bit (R5G6B5) color, 8-bit (R3G3B2) color and 8-bit grayscale bitmaps. Includes support for transparent images with specified transparency colour.  See animation example.
* drawMaskedImage (draw 'masked' image with specified mask overlay (matte) and colour). See matte example.

The following functions must be used within an SPI transaction (i.e. preceded by startWrite() and followed by endWrite();
* setWindow - sets graphics RAM addressable window (similar to existing setAddrWindow() but without the embedded startwrite()).
* goTo - sets graphics RAM address pointer ('cursor').

The following hardware-optimised functions override Adafruit_GFX methods and are typically 7-8 times faster than the generic software methods:
* drawLine (draw line between specified coordinates).
* drawRect (draw rectangular box of specified start point, width and height).
* fillRect (draw filled rectangle of specified start point, width and height).
* drawFastVLine (draw vertical line of specified height).
* drawFastHLine (draw horizontal line of specified width).
* fillScreen (fill screen with specified color).

The following SSD1331 hardware configuration functions are implemented:
* setOrientation (8 permutations available - normal, 90, 180 and 270 rotation, each of which can also be mirrored). The 90 and 270 rotations represent portrait mode.
**NB** drawing routines using standard GFX functions (including 'setRotation()') may not take into account hardware re-orientation, particularly portrait mode. 
You may, for example, need to transpose 'width' and 'height' variables to achieve the desired result - see patternDemo example.
* getOrientation (returns current hardware orientation - can check for portrait mode using the construct 'if (getOrientation() & SSD1331_PORTRAIT)'.
* setGrayScale (by passing decimal gamma value).
* resetGrayScale (linear gray scale i.e. gamma = 1.0).
* clearWindow (more efficient than using fillScreen(BLACK)).
* copyWindow (copy section of display to another location).
* moveWindow ('move' section of display to another location - currently non-overlapping moves only).
* dimWindow (dim section of display according to current grayscale setting)
* display on/off/rotate/invert ('negative').
* display scrolling (horizontal and/or vertical, all or selected rows).

## To Do

* Add more overloaded drawImage() functions to cater for raw bitmaps (i.e. not contained within a tImage or bwImage struct) in PROGMEM and RAM, providing hardware-optimised alternatives to the GFX drawBitmap() functions.
* Implement improved orientation-specific boundary checking. For now, display will normally just wrap-around if coordinates exceed boundaries based on current SETREMAP mode.
* Improve moveWindow() to allow overlapping moves.
* Various specialised bitmap image handling functions.

## Wiring Connections

<!-- START WIRING TABLE -->

SPI pin markings vary between boards and can be confusing. Boards with a built-in SD card reader tend to have 10 pins, 3 of which (usually marked SO, 
SC and CD) relate specifically to the SD card reader. Boards which have no SD card reader tend to have 7 pins.

![oled_noSDreader](/OLED_type1.jpg) ![oled_withSDreader](/OLED_type2.jpg)

Pin on OLED board       | Pin on Arduino | Purpose
----------------------- | -------------- | ------------------------------------------------------ 
GND or G                | GND            | Ground
VCC or \+               | +5V            | Power **NB** some boards may run off 3.3V - check datasheet
SCL or CLK or CK        | Pin 13         | SPI SCLK Clock (hardware) *1*
SDA or SI or DIN        | Pin 11         | SPI MOSI Master Out, Slave In (hardware) *1*
SO                      | Pin 12         | SPI MISO Master In, Slave Out for SD Card reader, if present (hardware) *1*
RES or R                | Digital pin 9  | SPI Reset 
DC or D/C               | Digital pin 8  | SPI SDIO Serial Data (**NB** I have seen a board with this pin marked as 'RS' - be warned)
CS or OC                | Digital pin 10 | SPI Chip Select pin for first OLED
SC                      | Digital pin 7  | SPI Chip Select pin for SD Card reader, if present
CD                      | N/C            | Used to detect if SD card is inserted, if present
 
* *1* Check microcontroller datasheet for details of which hardware SPI pins to use.

**If you're using two OLED displays (e.g. to simulate a pair of eyes), the pins on the second board should be configured as above WITH THE EXCEPTION OF:**

Pin on 2nd OLED board   | Pin on Arduino | Purpose
----------------------- | -------------- | ------------------------------------------------------ 
RES (R) on 2nd board    | Digital pin 9  | SPI Reset for second OLED; uses same physical pin as first board but MUST BE assigned to a fictional pin (e.g. RST2 = 99) in the sketch
CS (OC) on 2nd board    | Digital pin 6  | SPI Chip Select for second OLED

<!-- END WIRING TABLE -->

<!-- START COMPATIBILITY TABLE -->

## Compatibility

**NB: Use of drawImage function is subject to flash (program) memory limits on all devices**

MCU                 | Tested Works | Doesn't Work | Not Tested  | Notes
------------------- | :----------: | :----------: | :---------: | --------------------
Arduino UNO         |      X       |              |             | Very limited memory, sketches with >1 bitmap won't load
Arduino Micro       |      X       |              |             | Very limited memory, sketches with >1 bitmap won't load
Arduino Zero M0     |      X       |              |             | 
Arduino DUE         |      X       |              |             | 
Adafruit Metro M4   |              |       	      |     X       |
Teensy 3.2 @ 72MHz  |      X       |              |             | 
Teensy 3.6 @ 180MHz |      X       |              |             |
Teensy 4.0 @ 600MHz |      X       |              |             | Use latest Teensyduino >=1.48

<!-- END COMPATIBILITY TABLE -->
