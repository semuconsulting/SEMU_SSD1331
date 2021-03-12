/***************************************************
  SEMU_SSD1331 library imageDemo example

  Demo of various image handling functions.

  The image header files used in this demo were created using
  Vladimir Riuson's lcd-image-convertor utility:
  https://github.com/riuson/lcd-image-converter
  Suitable presets and templates for use with this utility can be found here:
  https://github.com/semuconsulting/SEMU_SSD1331/tree/master/lcd_image_convert_template

 ****************************************************/
//#define DEBUG

//#define ESP32  // uncomment to configure for ESP32 Devkit

#include <SEMU_SSD1331.h>
//#include "testcard_c565.h"    // 16-bit R5G6B5 format
//#include "testcard_c332.h"    // 8-bit R3G3B2 format
//#include "testcard_gs8.h"     // 8-bit grayscale format
//#include "testcard_bw.h"      // 1-bit monochrome
//#include "rgb_c565.h"         // 16-bit R5G6B5 format
//#include "rgb_c332.h"         // 8-bit R3G3B2 format
//#include "rgb_g8.h"           // 8-bit grayscale format
//#include "rgb_bw.h"           // 8-bit grayscale format
#include "google32_c565.h"

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

#define PAUSE 1000

#if defined ESP32 // use software SPI constructor for ESP32
SEMU_SSD1331 display = SEMU_SSD1331(cs, dc, mosi, sclk, rst);
#else
SEMU_SSD1331 display = SEMU_SSD1331(&SPI, cs, dc, rst);
#endif

unsigned long t0, t1, t2;
uint8_t c, r;

void setup(void) {

  Serial.begin(9600);
  display.begin();
  display.clearWindow();
  Serial.println("Start of benchmark for SEMU drawImage() vs Adafruit drawRGBBitmap():");
  Serial.println("(running on an 8-bit Arduino Nano)");
  Serial.println("");

}

void loop() {

  display.clearWindow();
  t0 = millis();
  for (c = 0; c < 96; c += 32) {
    for (r = 0; r < 64; r += 32) {
      display.drawImage(c, r, &bitmap32);
    }
  }
  t1 = millis();
  Serial.print("drawImage for 6 x 1024 pixel R5G6B5 images took : ");
  Serial.println(t1 -= t0);
  delay(PAUSE);
  display.clearWindow();
  t0 = millis();
  for (c = 0; c < 96; c += 32) {
    for (r = 0; r < 64; r += 32) {
      display.drawRGBBitmap(c, r, image_data_bitmap32, 32, 32);
    }
  }
  t2 = millis();
  Serial.print("drawRGBBitmap for 6 x 1024 pixel R5G6B5 images took : ");
  Serial.println(t2 -= t0);
  delay(PAUSE);
  Serial.print("drawImage() is ");
  Serial.print((float)t2 / (float)t1);
  Serial.println(" times faster than drawRGBBitmap().");

}
