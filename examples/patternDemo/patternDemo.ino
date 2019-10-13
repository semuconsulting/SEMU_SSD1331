
/***************************************************
  SEMU_SSD1331 library patternDemo example which can be used to benchmark
  against standard Adafruit_SSD1331 library.

  Test of various point, line and rectangle drawing routines
  using hardware optimisation.

 ****************************************************/
//#define DEBUG

#include <SEMU_SSD1331.h>
//#include <Adafruit_SSD1331.h>

#define sclk 13   // marked SCL or CK on OLED board
#define mosi 11   // marked SDA or SI on OLED board
#define cs   10   // marked CS or OC on OLED board
#define rst  9    // marked RES or R on OLED board
#define dc   8    // marked DC or sometimes (confusingly) RS on OLED board

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

#define PAUSE 1000

// set up SPI display - uses hardware SPI pins on Teensy (MOSI 11 SCLK 13)
SEMU_SSD1331 display = SEMU_SSD1331(&SPI, cs, dc, rst);
//Adafruit_SSD1331 display = Adafruit_SSD1331(&SPI, cs, dc, rst);

float f;
unsigned long totalDuration, testDuration;

void setup(void) {

#if defined(DEBUG)
  while (!Serial);    // wait for Serial port to open - may take a few seconds on Teensy and Micro boards
  Serial.begin(9600);
  Serial.println("Starting SSD1331 test routines..");
#endif
  randomSeed(analogRead(0));
  display.begin(0x72);

}

void loop() {

  // use setOrientation() to set portrait rather than landscape mode
  // display.setOrientation(SSD1331_ROTATE_090);
  
  /*
  // NB: Adafruit_GFX library setRotation() does not currently cater for hardware re-orientation
  // via setOrientation() and may have other issues. If you want to use existing width() or height()
  // functions, you'll need to compensate for portrait mode along the following lines:
   
  if (display.getOrientation() && SSD1331_PORTRAIT) {
    h = display.width();
    w = display.height();
  }
  else {
    w = display.width();
    h = display.height();
  }
  */

  totalDuration = millis();
#if defined(DEBUG)
  Serial.print("Test iteration started at: ");
  Serial.println(totalDuration, DEC);
#endif

  display.fillScreen(BLACK);
  testDuration = millis();
  patterns();
#if defined(DEBUG)
  Serial.print("Line patterns took: ");
  Serial.println(millis() - testDuration, DEC);
#endif
  delay(PAUSE);

  display.fillScreen(BLACK);
  testDuration = millis();
  zoomingSquares();
#if defined(DEBUG)
  Serial.print("Zooming rectangles took: ");
  Serial.println(millis() - testDuration, DEC);
#endif
  delay(PAUSE);

  display.fillScreen(BLACK);
  testDuration = millis();
  randomlines(500);
#if defined(DEBUG)
  Serial.print("Random lines took: ");
  Serial.println(millis() - testDuration, DEC);
#endif
  delay(PAUSE);

  display.fillScreen(BLACK);
  testDuration = millis();
  dial();
#if defined(DEBUG)
  Serial.print("Rotating dial took: ");
  Serial.println(millis() - testDuration, DEC);
#endif
  delay(PAUSE);

  display.fillScreen(BLACK);
  testDuration = millis();
  oscilloscope(100);
#if defined(DEBUG)
  Serial.print("Oscilloscope took: ");
  Serial.println(millis() - testDuration, DEC);
#endif
  delay(PAUSE);

  display.fillScreen(BLACK);
  testDuration = millis();
  rectangles(32, 100);
  rectangles(16, 100);
  rectangles(8, 100);
#if defined(DEBUG)
  Serial.print("Random rectangles took: ");
  Serial.println(millis() - testDuration, DEC);

  Serial.print("Total duration of tests: ");
  Serial.println(millis() - totalDuration, DEC);
#endif


}

/**************************************************************
  Draw lots of rectangles in random colours
*************************************************************/
void rectangles(uint8_t rsize, uint16_t iterations) {

  uint16_t i, x, y , filled;
  uint8_t w, h;
  h = display.TFTHEIGHT; // this allows for hardware orientiation via SETREMAP
  w = display.TFTWIDTH; // this allows for hardware orientiation via SETREMAP

  for (i = 0; i < iterations; i++) {
    for (x = 0; x < w; x += rsize) {
      for (y = 0; y < h; y += rsize) {
        filled = random(2);

        if (filled < 1) {
          display.drawRect (x, y, (x + rsize - 1) - x, (y + rsize - 1) - y, random(0xffff));
        }
        else {
          display.fillRect (x, y, (x + rsize - 1) - x, (y + rsize - 1) - y, random(0xffff));
        }
      }

    }
  }

}

/**************************************************************
  Draw lots of random lines in random colours
*************************************************************/
void randomlines(uint16_t iterations) {

  uint16_t i;
  uint8_t w, h;
  h = display.TFTHEIGHT; // this allows for hardware orientiation via SETREMAP
  w = display.TFTWIDTH; // this allows for hardware orientiation via SETREMAP

  for (i = 0; i < iterations; i++) {
    display.drawLine (random(), random(h), random(w), random(h), random(0xffff));
  }
}

/**************************************************************
  Draw some geometric line patterns
*************************************************************/
void patterns() {

  uint8_t x, y , n;
  uint16_t col;
  uint8_t w, h;

  h = display.TFTHEIGHT-1;
  w = display.TFTWIDTH-1;

  display.fillScreen(BLACK);

  col = random(0xffff);
  for (x = 0; x < w; x += 4) {
    display.drawLine (x, 0, w - x, h, col);
  }
  col = random(0xffff);
  for (x = 0; x < h; x += 4) {
    display.drawLine (0, x, w, h - x, col);
  }

  delay(PAUSE);
  display.fillScreen(BLACK);

  for (x = 0; x < h; x += 4) {
    display.drawLine (0, x, x * (w / h), h, random(0xffff));
    //delay(10);
  }
  for (x = 0; x < h; x += 4) {
    display.drawLine (x * (w / h), h, w, h - x, random(0xffff));
    //delay(10);
  }
  for (x = 0; x < h; x += 4) {
    display.drawLine (w, h - x, w - x * (w / h), 0, random(0xffff));
    //delay(10);
  }
  for (x = 0; x < h; x += 4) {
    display.drawLine (w - x * (w / h), 0, 0, x, random(0xffff));
    //delay(10);
  }
  delay(PAUSE);
  display.fillScreen(BLACK);

  for (n = 40; n > 2; n--) {
    col = random(0xffff);
    for (x = 0; x < w; x += n) {
      display.drawFastVLine (x, 0, h, col);
    }
    for (y = 0; y < h; y += n) {
      display.drawFastHLine (0, y, w, col);
    }
    delay(100);
    display.fillScreen(BLACK);
  }
}

/**************************************************************
  Draw a revolving dial
*************************************************************/
void dial() {

  uint16_t i;
  uint8_t w, h, x0, y0;
  h = display.TFTHEIGHT;
  w = display.TFTWIDTH;
  
  uint8_t radius = 28;
  float rx, ry;
  x0 = w / 2;
  y0 = h / 2;
  for ( i = 0; i < (360 * 5); i++) {

    rx = (float)radius * cos((float)(i - 90) * 71 / 4068);
    ry = (float)radius * sin((float)(i - 90) * 71 / 4068);

    display.drawLine (x0, y0, x0 + rx, y0 + ry, WHITE);
    delayMicroseconds(400);
    display.drawLine (x0, y0, x0 + rx, y0 + ry, BLACK);

    if (i < 360) {
      display.drawPixel(x0 + rx, y0 + ry, YELLOW);
    }
    else if (i < 720) {
      display.drawPixel(x0 + rx, y0 + ry, GREEN);
    }
    else if (i < 1080) {
      display.drawPixel(x0 + rx, y0 + ry, RED);
    }
    else {
      display.drawPixel(x0 + rx, y0 + ry, BLACK);
    }
  }

}

/**************************************************************
  Simulate an oscilloscope display with two waveforms
*************************************************************/
void oscilloscope(uint16_t iterations) {

  uint16_t x, y;
  float s1, s2;
  uint8_t xT;
  xT = display.TFTWIDTH;

  for (y = 0; y < iterations; y += 5) {
    for (x = 0; x < xT - 1; x++) {
      s1 = sin((x + y) / 5.0) * 10.0 + 16.0;
      s2 = sin((x + 10 + y) / 4.0) * 10.0 + 42.0;
      display.drawPixel(x, (uint16_t)s1, RED);
      display.drawPixel(x, (uint16_t)s2, GREEN);
      delay(1);
    }
    for (x = 0; x < xT - 1; x++) {
      s1 = sin((x + y) / 5.0) * 10.0 + 16.0;
      s2 = sin((x + 10 + y) / 4.0) * 10.0 + 42.0;
      display.drawPixel(x, (uint16_t)s1, BLACK);
      display.drawPixel(x, (uint16_t)s2, BLACK);
      delay(1);
    }
  }

}

/**************************************************************
  Draw some zooming squares
*************************************************************/
void zoomingSquares() {

  uint16_t i, n;
  uint16_t col;
  int16_t x0, y0, x1,  y1;
  uint8_t w, h;
  h = display.TFTHEIGHT;
  w = display.TFTWIDTH;

  for (n = 0; n < 10; n++) {

    col = random(0xffff);
    for (i = 15; i > 0; i --) {

      x0 = (int16_t)3 * i;
      y0 = (int16_t)2 * i;
      x1 = w - x0;
      y1 = h - y0;
      display.drawRect(x0, y0, x1 - x0, y1 - y0, col);
      delay(30);
      display.drawRect(x0, y0, x1 - x0, y1 - y0, BLACK);
      delay(30);
    }
  }

  for (n = 0; n < 10; n++) {

    col = random(0xffff);
    for (i = 15; i > 0; i --) {

      x0 = (int16_t)3 * i;
      y0 = (int16_t)2 * i;
      x1 = w - x0;
      y1 = h - y0;
      display.fillRect(x0, y0, x1 - x0, y1 - y0, col);
      delay(30);
    }
  }

}
