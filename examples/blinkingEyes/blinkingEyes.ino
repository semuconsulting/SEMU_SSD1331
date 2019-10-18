/*******************************************************************************************************************************

  SEMU_SSD1331 library blinkingEyes demo

  Illustrates use of SEMU_SSD1331 drawMaskedSegment() function, which displays a transparent mask
  image over a moveable 'segment' of a larger underlying background image. Very fast frame rates are
  supported (up to around 40 fps). In this demo, the masks represent eyelids and the underlying
  image represents an eyeball, thus simulating a pair of moving, blinking eyes.

  This is a more simplistic, 'cartoony' approach than the impressive 'Uncanny Eyes' project for
  SSD1351 OLEDs, but the drawMaskedSegment() function is readily adaptable to other applications.

  NB: This demo requires about 140kB of program storage space, so WON'T run on a low-end Arduino like a UNO.

  WeatherWax Cosplay / SEMU Consulting - 2018

  The image header files used in this demo were created using
  Vladimir Riuson's lcd-image-convertor utility:
  https://github.com/riuson/lcd-image-converter
  Suitable presets and templates for use with this utility can be found here:
  https://github.com/semuconsulting/SEMU_SSD1331/tree/master/lcd_image_convert_template

  NB: Multiple OLED boards should share the same SPI MISO, SCLK, RESET and DC lines but each board must have a separate SPI CS (OC) line.

  NB: SPI wiring to displays should be as short as possible to avoid glitches - ideally no more than 5cm.
******************************************************************************************************************************/

//#define DEBUG
#define DUALDISPLAYS // comment this out if you only have a single display

#include <SEMU_SSD1331.h>

#include "_images/eyeball.h"
#include "_images/blink1m.h"
#include "_images/blink2m.h"
#include "_images/blink3m.h"
#include "_images/blink4m.h"
#include "_images/blink5m.h"
#include "_images/blink6m.h"

#define sclk 13   // marked SCL or CK on OLED board
#define mosi 11   // marked SDA or SI or DIN on OLED board
#define cs   10   // marked CS or OC on OLED board
#define rst  9    // marked RES or R on OLED board
#define dc   8    // marked DC or D/C or occasionally (confusingly) RS on OLED board

#if defined(DUALDISPLAYS)
// reset pin on second OLED must have a fictional assignment (e.g. 99)
// even though it's connected to the same physical pin as the first OLED
#define cs2   6   // marked CS or OC on second OLED board
#define rst2  99  // marked RES or R on second OLED board
#endif

#define LEFT 0
#define RIGHT 1
#define AUTOMIN 300 // governs how often eyeball moves
#define AUTOMAX 1500
#define BLINKMIN 1000 // governs how often eyelid blinks
#define BLINKMAX 4000

const tImage mask[6] = {
  blink1m, blink2m, blink3m, blink4m, blink5m, blink6m
};
//const tImage maskR[4] = {
//  maskr1, maskr2, mask3, mask4
//};

SEMU_SSD1331 displays[] = {
  SEMU_SSD1331(cs, dc, rst),
#if defined(DUALDISPLAYS)
  SEMU_SSD1331(cs2, dc, rst2)
#endif
};

uint16_t lastx;
uint16_t lasty;
uint16_t min_delta = 5;
uint16_t max_delta = 1023;
uint32_t blinkPeriod = 3000;
uint8_t blinkMaskL = 0;
uint8_t blinkMaskR = 0;
uint8_t blinkIter = 0;
bool autoMode = true;
uint32_t autoPeriod;
bool inMotion = false;
uint16_t eyex = 512;
uint16_t eyey = 512;
uint16_t targetx;
uint16_t targety;

void setup(void) {

  displays[LEFT].begin();
#if defined(DUALDISPLAYS)
  displays[RIGHT].begin();
#endif

  displays[LEFT].setOrientation(SSD1331_ROTATE_NORMALFLIP); // set the left display to mirror the right
  displays[RIGHT].setOrientation(SSD1331_ROTATE_NORMAL);
  randomSeed(analogRead(0));

}

void loop() {

  blinkTimer(); // refresh blink timer
  autoMode = true;

  autoTimer(); // refresh auto timer

  setEyes (eyex, eyey);

}

//**************************************************************************
// Sets position of left and right eyes, with eyelid mask overlay governed
// by random blink interval
//
// For the purposes of this demo the x,y coordinates are generated automatically,
// but they could just as easily be provided via, say, analogRead() of a thumb
// joystick potentiometer or wireless gamepad controller.
//**************************************************************************
void setEyes(uint16_t x, uint16_t y) {

  if (x <= max_delta && y <= max_delta) {

    x = map(x, 0, 1023, displays[LEFT].TFTWIDTH - 1, 0);
    y = map(y, 0, 1023, displays[LEFT].TFTHEIGHT - 1, 0);

    // mirror sense of x coordinate in left display
    displays[LEFT].drawMaskedSegment(64-x, y, &eyeball, &mask[blinkMaskL]);
#if defined(DUALDISPLAYS)
    displays[RIGHT].drawMaskedSegment(x, y, &eyeball, &mask[blinkMaskR]);
#endif

  }

}

//**************************************************************************
// Refreshes blink interval timer and eyelid display mask
// Blink interval is random between 1 and 3 seconds
//**************************************************************************
void blinkTimer () {

  if (millis() > blinkPeriod) {

    switch (blinkIter) {
      case 0:
        blinkPeriod = millis() + random(BLINKMIN, BLINKMAX);
        blinkMaskL = 0;
        blinkMaskR = 0;
        break;
      case 1:
        blinkPeriod = millis() + 3;
        blinkMaskL = 1;
        blinkMaskR = 1;
        break;
      case 2:
        blinkPeriod = millis() + 3;
        blinkMaskL = 2;
        blinkMaskR = 2;
        break;
      case 3:
        blinkPeriod = millis() + 3;
        blinkMaskL = 3;
        blinkMaskR = 3;
        break;
      case 4:
        blinkPeriod = millis() + 3;
        blinkMaskL = 4;
        blinkMaskR = 4;
        break;
      case 5:
        blinkPeriod = millis() + 3;
        blinkMaskL = 5;
        blinkMaskR = 5;
        break;
      case 6:
        blinkPeriod = millis() + 3;
        blinkMaskL = 4;
        blinkMaskR = 4;
        break;
      case 7:
        blinkPeriod = millis() + 4;
        blinkMaskL = 3;
        blinkMaskR = 3;
        break;
      case 8:
        blinkPeriod = millis() + 6;
        blinkMaskL = 2;
        blinkMaskR = 2;
        break;
      case 9:
        blinkPeriod = millis() + 6;
        blinkMaskL = 1;
        blinkMaskR = 1;
        break;
    }

    blinkIter++;
    if (blinkIter > 9) {
      blinkIter = 0;
    }

  }

}

//**************************************************************************
// Generates random x,y target at periodic intervals and then increments head
// and eye positions towards that target.
//**************************************************************************
void autoTimer () {

  int16_t inc = 0;
  uint16_t s = 5;

  if (millis() > autoPeriod) {

    switch (inMotion) {
      case false:
        autoPeriod = millis() + random(AUTOMIN, AUTOMAX);
        inMotion = true;
        targetx = random(430, 823); // adjusts extent of left/right eyeball movement
        targety = random(350, 680); // adjusts extent of up/down eyeball movement
        break;
      case true:
        autoPeriod = millis() + 10;

        inc = targetx - eyex;
        eyex += inc / s;
        inc = targety - eyey;
        eyey += inc / s;

        if (abs(targetx - eyex) <= s && abs(targety - eyey) <= s) {
          inMotion = false;
        }
        break;
    }

  }

}
