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
#include "frank.h"
#include "testcard.h"

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

void setup(void) {

  display.begin();

}

void loop() {

  showtestcard();
  delay(PAUSE);

  showfrank();
  delay(PAUSE);

}

void showtestcard() {

  // draw full screen image
  display.drawImage(&testcard); 
  delay(PAUSE);

  // copy section of image to another location
  display.copyWindow(30,15,62,45,0,0);
  delay(PAUSE);
  display.copyWindow(30,15,62,45,0,33);
  delay(PAUSE);
  display.copyWindow(30,15,62,45,63,0);
  delay(PAUSE);
  display.copyWindow(30,15,62,45,63,33);
  delay(PAUSE);

  // 'move' section of image to another location
  // doesn't currently support overlapping moves
  display.drawImage(&testcard); 
  delay(PAUSE);
  // copy section of image to another location
  display.moveWindow(30,15,62,45,0,0);
  delay(PAUSE);
  display.moveWindow(0,0,32,30,0,33);
  delay(PAUSE);
  display.moveWindow(0,32,32,30,62,0);
  delay(PAUSE);
  display.moveWindow(62,0,32,30,62,33);
  delay(PAUSE);
  
  // flip display left-right - still the same image,
  // but the display itself is flipped
  display.setOrientation(SSD1331_ROTATE_NORMALFLIP); 
  display.drawImage(&testcard);
  delay(PAUSE);

  // flip display upside down
  display.setOrientation(SSD1331_ROTATE_180);
  display.drawImage(&testcard);
  delay(PAUSE);
  
  // back to normal
  display.setOrientation(SSD1331_ROTATE_NORMAL);
  display.drawImage(&testcard);

}

void showfrank() {

  float i;

  // display full screen image
  display.drawImage(&frank);
  delay(PAUSE);

  // invert the display i.e. display a 'negative' image
  display.setDisplayMode(SSD1331_CMD_INVERTDISPLAY);
  delay(PAUSE);
  
  // back to normal
  display.setDisplayMode(SSD1331_CMD_NORMALDISPLAY);
  delay(PAUSE);

  // cycle the gray scale up and down
  for (i = 1.0; i < 10; i += 0.4) {
    display.setGrayScale(i);
    delay(100);
  }
  for (i = 10; i > 0.2; i -= 0.5) {
    display.setGrayScale(i);
    delay(50);
  }
  for (i = 0.2; i < 1.0; i += 0.1) {
    display.setGrayScale(i);
    delay(200);
  }
  
  // back to normal
  display.setGrayScale(1.0);

}
