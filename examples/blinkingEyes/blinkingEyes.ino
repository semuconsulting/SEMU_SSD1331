/*******************************************************************************************************************************

  SEMU_SSD1331 library blinkingEyes demo
  
  Illustrates use of SEMU_SSD1331 drawMaskedSegment() function, which displays a transparent mask 
  image over a moveable 'segment' of a larger underlying background image. Very fast frame rates are 
  supported (up to around 40 fps). In this demo, the masks represent eyelids and the underlying 
  image represents an eyeball, thus simulating a pair of moving, blinking eyes.

  This is a more simplistic, 'cartoony' approach than the impressive 'Uncanny Eyes' project for 
  SSD1351 OLEDs, but the drawMaskedSegment() function is readily adaptable to other applications.

  NB: This demo requires about 150kB of program storage space, so WON'T run on a low-end Arduino like a UNO.
  NB: Only works properly in landscape orientation at the moment.

  Originally done for an 'animatronix' dragon project in which the motion of a dragon's 'eyes' could
  be controlled via a wireless gamepad controller or thumb potentiometer, but also switched to an
  autonomous 'auto' demonstration mode. In this example all the manual remote control stuff has been stripped to 
  leave only the OLED 'auto' functionality. The original target was a Teensy 3.2 - MCUs with more program memory 
  available (e.g. Teensy 3.6 or 4.0) could accommodate more sophisticated animations.
  ( https://github.com/WeatherwaxCosplay/Robodragon if anyone's interested. )

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

#include "_images/deye.h"
#include "_images/maskl1.h"
#include "_images/maskl2.h"
#include "_images/maskr1.h"
#include "_images/maskr2.h"
#include "_images/mask3.h"
#include "_images/mask4.h"

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
#define BLINKMIN 500 // governs how often eyelid blinks
#define BLINKMAX 2500

const tImage maskL[4] = {
  maskl1, maskl2, mask3, mask4
};
const tImage maskR[4] = {
  maskr1, maskr2, mask3, mask4
};

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

    displays[LEFT].drawMaskedSegment(x, y, &deye, &maskL[blinkMaskL]);
#if defined(DUALDISPLAYS)
    displays[RIGHT].drawMaskedSegment(x, y, &deye, &maskR[blinkMaskR]);
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
        blinkPeriod = millis() + 25;
        blinkMaskL = 1;
        blinkMaskR = 1;
        break;
      case 2:
        blinkPeriod = millis() + 25;
        blinkMaskL = 2;
        blinkMaskR = 2;
        break;
      case 3:
        blinkPeriod = millis() + 25;
        blinkMaskL = 3;
        blinkMaskR = 3;
        break;
      case 4:
        blinkPeriod = millis() + 30;
        blinkMaskL = 2;
        blinkMaskR = 2;
        break;
      case 5:
        blinkPeriod = millis() + 30;
        blinkMaskL = 1;
        blinkMaskR = 1;
        break;
    }

    blinkIter++;
    if (blinkIter > 5) {
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
        targetx = random(200, 823);
        targety = random(200, 823);
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
