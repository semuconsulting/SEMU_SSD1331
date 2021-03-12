/***************************************************
  Ballistic Gauge demo

  This example show how to create a round gauge that react like the real one with (almost) correct ballistic
  Adapted from original by S.U.M.O.T.O.Y - Max MC Costa
  If you modify or get better result please let him know
 ****************************************************/

//#define DEBUG

//#define ESP32  // uncomment to configure for ESP32 Devkit

#include <SEMU_SSD1331.h>

// You can use any (4 or) 5 pins
#if defined ESP32 // these are the usual hardware SPI pins for EPS32
#define sclk 18   // marked SCL or CK on OLED board
#define mosi 23   // marked SDA or SI on OLED board
#define cs   5    // marked CS or OC on OLED board
#define rst  16   // marked RES or R on OLED board
#define dc   17   // marked DC or sometimes (confusingly) RS on OLED board
#else             // these are the usual hardware SPI pins for Arduino
#define sclk 13   // marked SCL or CK on OLED board
#define mosi 11   // marked SDA or SI on OLED board
#define cs   10   // marked CS or OC on OLED board
#define rst  9    // marked RES or R on OLED board
#define dc   8    // marked DC or sometimes (confusingly) RS on OLED board
#endif

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

#define _GAUGEDIM 31

#if defined ESP32 // use software SPI constructor for ESP32
SEMU_SSD1331 display = SEMU_SSD1331(cs, dc, mosi, sclk, rst);
#else
SEMU_SSD1331 display = SEMU_SSD1331(&SPI, cs, dc, rst);
#endif

volatile int16_t curVal1 = 0;
volatile int16_t oldVal1 = 0;

void setup() {
  Serial.begin(9600);
  display.begin();
  display.clearWindow();
  drawGauge(_GAUGEDIM, _GAUGEDIM, _GAUGEDIM);
}

void loop(void) {
  curVal1 = random(1, 254);
  if (oldVal1 != curVal1) {
    drawNeedle(curVal1, _GAUGEDIM, _GAUGEDIM, _GAUGEDIM, GREEN, BLACK);
    oldVal1 = curVal1;
  }
}

void drawGauge(uint8_t x, uint8_t y, uint8_t r) {
  display.drawCircle(x, y, r, WHITE); //draw instrument container
  faceHelper(x, y, r, 150, 390, 1.3); //draw major ticks
  if (r > 15) faceHelper(x, y, r, 165, 375, 1.1); //draw minor ticks

}

void faceHelper(uint8_t x, uint8_t y, uint8_t r, int from, int to, float dev) {
  float dsec, fromSecX, fromSecY, toSecX, toSecY;
  int i;
  for (i = from; i <= to; i += 30) {
    dsec = i * (PI / 180);
    fromSecX = cos(dsec) * (r / dev);
    fromSecY = sin(dsec) * (r / dev);
    toSecX = cos(dsec) * r;
    toSecY = sin(dsec) * r;
    display.drawLine(1 + x + fromSecX, 1 + y + fromSecY, 1 + x + toSecX, 1 + y + toSecY, WHITE);
  }
}

void drawNeedle(int16_t val, uint8_t x, uint8_t y, uint8_t r, uint16_t color, uint16_t bcolor) {
  uint8_t i;
  if (curVal1 > oldVal1) {
    for (i = oldVal1; i <= curVal1; i++) {
      drawPointerHelper(i - 1, _GAUGEDIM, _GAUGEDIM, _GAUGEDIM, bcolor);
      drawPointerHelper(i, _GAUGEDIM, _GAUGEDIM, _GAUGEDIM, color);
      if ((curVal1 - oldVal1) < (128)) delay(1);//ballistic
    }
  }
  else {
    for (i = oldVal1; i >= curVal1; i--) {
      drawPointerHelper(i + 1, _GAUGEDIM, _GAUGEDIM, _GAUGEDIM, bcolor);
      drawPointerHelper(i, _GAUGEDIM, _GAUGEDIM, _GAUGEDIM, color);
      //ballistic
      if ((oldVal1 - curVal1) >= 128) {
        delay(1);
      } else {
        delay(3);
      }
    }
  }
}

void drawPointerHelper(int16_t val, uint8_t x, uint8_t y, uint8_t r, uint16_t color) {
  float dsec, toSecX, toSecY;
  int16_t minValue = 0;
  int16_t maxValue = 255;
  int fromDegree = 150;//start
  int toDegree = 240;//end
  if (val > maxValue) val = maxValue;
  if (val < minValue) val = minValue;
  dsec = (((float)(uint16_t)(val - minValue) / (float)(uint16_t)(maxValue - minValue) * toDegree) + fromDegree) * (PI / 180);
  toSecX = cos(dsec) * (r / 1.35);
  toSecY = sin(dsec) * (r / 1.35);
  display.drawLine(x, y, 1 + x + toSecX, 1 + y + toSecY, color);
  display.fillCircle(x, y, 2, color);
}
