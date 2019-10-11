
/***************************************************
  SEMU_SSD1331 library patternTest example which can be used to benchmark
  against standard Adafruit_SSD1331 library.

  Test of various point, line and rectangle drawing routines
  using hardware optimisation.

  WIRING CONNECTIONS:

  NB: The pin markings tend to vary from board to board and SPI notation in general
  is often somewhat confusing, so double check the manufacturer's/supplier's
  pin specifications.

  OLED Pin		Purpose					Arduino Pin
  -------------------------------------------------------------------------
  G/GND			Ground						GND
  +/VCC			Supply voltage	  +5V (+3.3V on some boards - check specs)
  DC			  Data						  Digital pin 8
  R/RST			Reset						  Digital pin 9
  OC or CS	SPI Chip select for OLED	Digital pin 10 (CS)
  SI/SDA		SPI MOSI					Digital pin 11 (DOUT)
  CK/SCL		SPI Clock (SCLK)	Digital pin 13 (SCK)

  Following only applicable to boards with SD card readers - not used in this example.
  SO			  SPI MISO
  SC			  SPI Chip select for SD card reader (if fitted)
  CD			  SD card reader detect

 ****************************************************/
#define DEBUG

#include <SEMU_SSD1331.h>
//#include <Adafruit_SSD1331.h>

#define sclk 13     // SPI clock - marked 'CK' or 'SCL' on most SSD1331 OLED boards
#define mosi 11     // SPI master out, slave in - marked 'SI' or 'SDA' on most SSD1331 OLED boards
#define cs   10     // SPI chip select - marked 'OC' or 'CS' on most SSD1331 OLED boards. NB: The 'SC' pin is a separate chip select for the SD card, if fitted.
#define rst  9      // SPI reset - marked 'R' or 'RST' on most SSD1331 OLED boards
#define dc   8      // SPI data - marked 'DC' on most SSD1331 OLED boards

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

  for (i = 0; i < iterations; i++) {
    for (x = 0; x < 96; x += rsize) {
      for (y = 0; y < 64; y += rsize) {
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

  for (i = 0; i < iterations; i++) {
    display.drawLine (random(95), random(63), random(95), random(63), random(0xffff));
  }
}

/**************************************************************
  Draw some geometric line patterns
*************************************************************/
void patterns() {

  uint8_t x, y , n;
  uint16_t col;
  uint8_t xT = display.width() - 1;
  uint8_t yT = display.height() - 1;

  display.fillScreen(BLACK);

  col = random(0xffff);
  for (x = 0; x < xT; x += 4) {
    display.drawLine (x, 0, xT - x, yT, col);
  }
  col = random(0xffff);
  for (x = 0; x < yT; x += 4) {
    display.drawLine (0, x, xT, yT - x, col);
  }

  delay(PAUSE);
  display.fillScreen(BLACK);

  for (x = 0; x < yT; x += 4) {
    display.drawLine (0, x, x * 1.5, yT, random(0xffff));
    //delay(10);
  }
  for (x = 0; x < yT; x += 4) {
    display.drawLine (x * 1.5, yT, xT, yT - x, random(0xffff));
    //delay(10);
  }
  for (x = 0; x < yT; x += 4) {
    display.drawLine (xT, yT - x, xT - x * 1.5, 0, random(0xffff));
    //delay(10);
  }
  for (x = 0; x < yT; x += 4) {
    display.drawLine (xT - x * 1.5, 0, 0, x, random(0xffff));
    //delay(10);
  }
  delay(PAUSE);
  display.fillScreen(BLACK);

  for (n = 40; n > 2; n--) {
    col = random(0xffff);
    for (x = 0; x < xT; x += n) {
      display.drawFastVLine (x, 0, yT, col);
    }
    for (y = 0; y < yT; y += n) {
      display.drawFastHLine (0, y, xT, col);
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
  uint8_t radius = 28;
  uint8_t x0 = 48;
  uint8_t y0 = 32;
  float rx, ry;
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

  uint8_t xT = display.width();

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
  int16_t w = display.width() - 1;
  int16_t h = display.height() - 1;

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
