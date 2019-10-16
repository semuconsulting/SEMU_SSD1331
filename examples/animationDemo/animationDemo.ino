/***************************************************
  SEMU_SSD1331 library animationDemo example

  Illustration of how to stop-frame animate bitmap images.
  Horse images courtesy of Eadweard Muybridge, father of stop-frame animation.
	
	(there is also a set of spiral images you can try)

  The image header files used in this demo were created using
  Vladimir Riuson's lcd-image-convertor utility:
  https://github.com/riuson/lcd-image-converter
  Suitable presets and templates for use with this utility can be found here:
  https://github.com/semuconsulting/SEMU_SSD1331/tree/master/lcd_image_convert_template

  NB: requires a minimum 128MB program (flash) memory to handle all the included image files.
  Suitable MCUs include Teensy 3.2 or later, Arduino Due, Zero or M0.

 ****************************************************/
//#define DEBUG

#include <SEMU_SSD1331.h>
#include "_images/horse1.h"
#include "_images/horse2.h"
#include "_images/horse3.h"
#include "_images/horse4.h"
#include "_images/horse5.h"
#include "_images/horse6.h"
#include "_images/horse7.h"
#include "_images/horse8.h"

#define sclk 13   // marked SCL or CK on OLED board
#define mosi 11   // marked SDA or SI on OLED board
#define cs   10   // marked CS or OC on OLED board
#define rst  9    // marked RES or R on OLED board
#define dc   8    // marked DC or sometimes (confusingly) RS on OLED board

#define FRAMES 8  // number of frames in animation

const tImage animation[FRAMES] = {
  horse1, horse2, horse3, horse4, horse5, horse6, horse7, horse8
};

uint16_t frame, fps;
unsigned long t;

SEMU_SSD1331 display = SEMU_SSD1331(&SPI, cs, dc, rst);

void setup(void) {

  display.begin();
  t = millis();
  frame = 0;
  fps = 24; // set target frames per second
  // individual full frame takes about 17ms to draw,
  // so maximum practical frame rate is about 50 fps

}

void loop() {

  // do other stuff here

  doAnimation();

}

void doAnimation() {

  if ((millis() - t) > 1000 / fps ) {
    display.drawImage(&animation[frame]);
    if (frame++ > FRAMES - 1) {
      frame = 0;
    }
    t = millis();
  }

}
