/***************************************************
  SEMU_SSD1331 library animationDemo example

  Illustration of how to animate bitmap images.
  
  The image header files used in this demo were created using
  Vladimir Riuson's lcd-image-convertor utility:
  https://github.com/riuson/lcd-image-converter
  Suitable presets and templates for use with this utility can be found here:
  https://github.com/semuconsulting/SEMU_SSD1331/tree/master/lcd_image_convert_template

  *************************************************************************************
  NB: requires a minimum 128MB program (flash) memory to handle all the included image files.
  Suitable MCUs include Teensy 3.2 or later, Arduino Due, Zero or M0 (ensure your IDE is
  updated with the latest libraries for these boards).
  *************************************************************************************

 ****************************************************/
//#define DEBUG

#include <SEMU_SSD1331.h>
#include "horse1.h"
#include "horse2.h"
#include "horse3.h"
#include "horse4.h"
#include "horse5.h"
#include "horse6.h"
#include "horse7.h"
#include "horse8.h"

#define sclk 13   // marked SCL or CK on OLED board
#define mosi 11   // marked SDA or SI on OLED board
#define cs   10   // marked CS or OC on OLED board
#define rst  9    // marked RES or R on OLED board
#define dc   8    // marked DC or sometimes (confusingly) RS on OLED board

#define PAUSE 1000

const tImage animation[8] = {
  horse1, horse2, horse3, horse4, horse5, horse6, horse7, horse8
};

SEMU_SSD1331 display = SEMU_SSD1331(&SPI, cs, dc, rst);

uint8_t modes[] = {SSD1331_ROTATE_NORMAL, SSD1331_ROTATE_NORMALFLIP,
                   SSD1331_ROTATE_180, SSD1331_ROTATE_180FLIP,
                   SSD1331_ROTATE_090, SSD1331_ROTATE_090FLIP,
                   SSD1331_ROTATE_270, SSD1331_ROTATE_270FLIP
                  };

void setup(void) {

  display.begin();

}

void loop() {

  // show animation at (approximately) 20 frames per second
  showAnimation(20);

}

void showAnimation(uint8_t fps) {

  uint8_t i, n;

  display.clearWindow();

  for (n = 0; n < 10; n++) {
    for (i = 0; i < 8; i++) {

      display.drawImage(0, 0, &animation[i]);
      delay(1000/fps);

    }
  }

}
