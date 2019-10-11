/***************************************************
  SEMU_SSD1331 library orientationDemo example

  Demo of various hardware orientation functions;
  - normal
  - rotated through 90, 180 and 270 degrees
  - vertical and horizontal mirroring

  The image header files used in this demo were created using
  Vladimir Riuson's lcd-image-convertor utility:
  https://github.com/riuson/lcd-image-converter
  Suitable presets and templates for use with this utility can be found here:
  https://github.com/semuconsulting/SEMU_SSD1331/tree/master/lcd_image_convert_template

 ****************************************************/
//#define DEBUG 0

#include <SEMU_SSD1331.h>

#define sclk 13   // marked SCL or CK on OLED board
#define mosi 11   // marked SDA or SI on OLED board
#define cs   10   // marked CS or OC on OLED board
#define rst  9    // marked RES or R on OLED board
#define dc   8    // marked DC or sometimes (confusingly) RS on OLED board

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
