/***************************************************
  SEMU_SSD1331 library experimental test harness

  Test harness for whatever I happen to be messing with

  The image header files used in this test were created using
  Vladimir Riuson's lcd-image-convertor utility:
  https://github.com/riuson/lcd-image-converter
  Suitable presets and templates for use with this utility can be found here:
  https://github.com/semuconsulting/SEMU_SSD1331/tree/master/lcd_image_convert_template

 ****************************************************/
#define DEBUG

#include <SEMU_SSD1331.h>
#include "test64x64.h"
#include "test48x48.h"
#include "test48x61.h"
#include "test62x40.h"
#include "test68x45bw.h"
#include "invader.h"

#define sclk 13   // marked SCL or CK on OLED board
#define mosi 11   // marked SDA or SI on OLED board
#define cs   10   // marked CS or OC on OLED board
#define rst  9    // marked RES or R on OLED board
#define dc   8    // marked DC or sometimes (confusingly) RS on OLED board

#define joyx  A8
#define joyy  A9

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

#define PAUSE 500

SEMU_SSD1331 display = SEMU_SSD1331(&SPI, cs, dc, rst);

const tImage images[4] = {
  test64x64, test48x48, test62x40, test48x61
};

uint8_t modes[] = {SSD1331_ROTATE_NORMAL, SSD1331_ROTATE_NORMALFLIP,
                   SSD1331_ROTATE_090, SSD1331_ROTATE_090FLIP,
                   SSD1331_ROTATE_180, SSD1331_ROTATE_180FLIP,
                   SSD1331_ROTATE_270, SSD1331_ROTATE_270FLIP
                  };

String modesDesc[] = {"Normal", "Normal Flipped",
                      "Rotated 90", "Rotated 90 Flipped",
                      "Rotated 180", "Rotated 180 Flipped",
                      "Rotated 270", "Rotated 270 Flipped"
                     };

uint8_t i, f, n, x, y, ix, iy, xValue, yValue, ws, hs;
uint16_t xv, yv, lastx, lasty;

void setup(void) {

  Serial.begin(9600);
  pinMode(joyx, INPUT);
  pinMode(joyy, INPUT);
  display.begin();
  display.fillScreen(BLACK);
  display.setOrientation(SSD1331_ROTATE_NORMAL);
  lastx = 99;
  lasty = 99;
  ws = 10;
  hs = 10;
}

void loop() {

  if (readJoystick()) {
    moveSprite();
  }

  //orientationTest();

}

bool readJoystick() {
// reads analog joystick potentiometer and returns flag
// signifying whether readings have changed above a
// specified threshold (prevents display jitter)

  if (display.getOrientation() & SSD1331_PORTRAIT) {
    xv = analogRead(joyy);
    yv = analogRead(joyx);
  }
  else {
    xv = analogRead(joyx);
    yv = analogRead(joyy);
  }

  if ( (abs(xv - lastx) > 3) || (abs(yv - lasty) > 3) ) {
    lastx = xv;
    lasty = yv;
    return true;
  }
  else {
    return false;
  }

}


void moveSprite() {
// just messing around with a sprite and joystick

  xValue = map(xv, 0, 1023, 0, 95-ws);
  yValue = map(yv, 0, 1023, 64-hs, 0);

  display.fillScreen(BLACK);
  display.drawImage(xValue, yValue, &invader, false, 0xffff);

}


void orientationTest() {
// this is a test of the drawImage() function
// this tests that transparent and non-transparent bitmaps render properly
// in all possible orientation modes, particularly portrait

  for (i = 0; i < 8; i++ ) {

    display.setOrientation(modes[i]);
#if defined(DEBUG)
    Serial.print("Orientation: ");
    Serial.println(modesDesc[i]);
#endif

    paintImage(i, GREEN, 0, 0, 64, 64, &images[0], 0xffff);
    paintImage(i, BLUE, 0, 0, 48, 48, &images[1], 0xffff);
    paintImage(i, RED, 0, 0, 62, 40, &images[2], 0xf543);
    paintImage(i, BLUE, 0, 0, 48, 61, &images[3], 0xf543);
    //display.drawImage(5,5, &test68x45bw, false, 0x00);

  }
}

void paintImage(uint8_t i, uint16_t c, uint8_t x, uint8_t y, uint8_t w, uint8_t h,
                const tImage *image, uint16_t tcol) {

  // paint image at origin
  display.fillScreen(c);
  display.drawImage(x, y, image, false);
  delay(PAUSE);
  display.fillScreen(c);
  display.drawImage(x, y, image, true, tcol);
  delay(PAUSE);

  if (display.getOrientation() & SSD1331_PORTRAIT) {
    x = (display.height() - h) / 2;
    y = (display.width() - w) / 2;
  }
  else {
    x = (display.width() - w) / 2;
    y = (display.height() - h) / 2;
  }
  // paint image centred
  display.fillScreen(c);
  display.drawImage(x, y, image, false);
  delay(PAUSE);
  display.fillScreen(c);
  display.drawImage(x, y, image, true, tcol);
  delay(PAUSE);

  if (display.getOrientation() & SSD1331_PORTRAIT) {
    x = random(display.TFTWIDTH - h);
    y = random(display.TFTHEIGHT - w);
  }
  else {
    x = random(display.TFTWIDTH - w);
    y = random(display.TFTHEIGHT - h);
  }

  Serial.print("Random x in ");
  Serial.print(modesDesc[i]);
  Serial.print(" mode, image width ");
  Serial.print(w);
  Serial.print(": ");
  Serial.print(x);
  Serial.print(" : ");
  Serial.println(x + w);
  Serial.print("Random y in ");
  Serial.print(modesDesc[i]);
  Serial.print(" mode, image height ");
  Serial.print(h);
  Serial.print(": ");
  Serial.print(y);
  Serial.print(" : ");
  Serial.println(y + h);
  // paint image at random location within display bounds
  display.fillScreen(c);
  display.drawImage(x, y, image, false);
  delay(PAUSE);
  display.fillScreen(c);
  display.drawImage(x, y, image, true, tcol);
  delay(PAUSE);

}
