/***************************************************
  SEMU_SSD1331 library experimental testing


  The image header files used in this demo were created using
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

SEMU_SSD1331 display = SEMU_SSD1331(&SPI, cs, dc, rst);

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

uint8_t i, n, x, y, ix, iy;

void setup(void) {

  Serial.begin(9600);
  display.begin();

}

void loop() {

  display.fillScreen(BLUE);
  orientationTest();

}

// test of how drawImage functions in different orientations
void orientationTest() {

  for (i = 0; i < 8; i++ ) {

    display.setOrientation(modes[i]);
#if defined(DEBUG)
    Serial.print("Orientation: ");
    Serial.println(modesDesc[i]);
#endif
    display.fillScreen(BLUE);
    ix = 64;
    iy = 64;
    if (display.getOrientation() & SSD1331_PORTRAIT) {
      x = (display.height() - iy) / 2;
      y = (display.width() - ix) / 2;
    }
    else {
      x = (display.width() - ix) / 2;
      y = (display.height() - iy) / 2;
    }
    display.drawImage(x, y, &test64x64);
    delay(PAUSE);
    display.fillScreen(GREEN);
    ix = 48;
    iy = 48;
    if (display.getOrientation() & SSD1331_PORTRAIT) {
      x = (display.height() - iy) / 2;
      y = (display.width() - ix) / 2;
    }
    else {
      x = (display.width() - ix) / 2;
      y = (display.height() - iy) / 2;
    }
    display.drawImage(x, y, &test48x48);
    delay(PAUSE);
    display.fillScreen(RED);
    ix = 48;
    iy = 61;
    if (display.getOrientation() & SSD1331_PORTRAIT) {
      x = (display.height() - iy) / 2;
      y = (display.width() - ix) / 2;
    }
    else {
      x = (display.width() - ix) / 2;
      y = (display.height() - iy) / 2;
    }
    display.drawImage(x, y, &test48x61);
    delay(PAUSE);

  }

}
