/***************************************************
  SEMU_SSD1331 library orientationTest example

  Test of various hardware orientation functions;
  - normal
  - rotated through 90, 180 and 270 degrees
  - vertical and horizontal mirroring

  WIRING CONNECTIONS:

  NB: The pin markings tend to vary from board to board and SPI notation in general
  is often somewhat confusing, so double check the manufacturer's/supplier's
  pin specifications.

  OLED Pin		Purpose						Arduino Pin
  -------------------------------------------------------------------------
  G/GND			Ground						GND
  +/VCC			Supply voltage				+5V (+3.3V on some boards - check specs)
  DC			Data						Digital pin 8
  R/RST			Reset						Digital pin 9
  OC or CS		SPI Chip select for OLED	Digital pin 10 (CS)
  SI/SDA		SPI MOSI					Digital pin 11 (DOUT)
  CK/SCL		SPI Clock (SCLK)			Digital pin 13 (SCK)

  Following only applicable to boards with SD card readers - not used in this example.
  SO			SPI MISO
  SC			SPI Chip select for SD card reader (if fitted)
  CD			SD card reader detect

 ****************************************************/
//#define DEBUG 0

#include <SEMU_SSD1331.h>

#define sclk 13   // marked SCL on board
#define mosi 11   // marked SDA on board
#define cs   10    // marked CS on board
#define rst  9   // marked RES on board
#define dc   8    // marked DC on board

#include "landscape.h"
#include "portrait.h"

SEMU_SSD1331 display = SEMU_SSD1331(&SPI, cs, dc, rst);

uint8_t modes[] = {SSD1331_ROTATE_NORMAL, SSD1331_ROTATE_NORMALFLIP,
                   SSD1331_ROTATE_180, SSD1331_ROTATE_180FLIP,
                   SSD1331_ROTATE_090, SSD1331_ROTATE_090FLIP,
                   SSD1331_ROTATE_270, SSD1331_ROTATE_270FLIP
                  };

void setup(void) {

#ifdef DEBUG
  while (!Serial);    // wait for Serial port to open - may take a few seconds on Teensy and Micro boards
  Serial.begin(9600);
  Serial.println("Starting SSD1331 test routines..");
#endif

  display.begin();

}

void loop() {
  display.clearWindow();
  testOrientation();
}

void testOrientation() {

  for (uint8_t i = 0; i < 8; i++) {

    display.setOrientation(modes[i]);
    if (modes[i] & SSD1331_PORTRAIT) {
      display.drawImage(&portrait);
    }
    else {
      display.drawImage(&landscape);
    }
    delay(1000);

  }

}
