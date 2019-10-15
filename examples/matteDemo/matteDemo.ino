/***************************************************
  SEMU_SSD1331 library matteDemo example

  Illustrates how to use the drawImage() and drawMaskedImage methods to 
  display transparent images and matte ('green screen') fx.
  
  The image header files used in this demo were created using
  Vladimir Riuson's lcd-image-convertor utility:
  https://github.com/riuson/lcd-image-converter
  Suitable presets and templates for use with this utility can be found here:
  https://github.com/semuconsulting/SEMU_SSD1331/tree/master/lcd_image_convert_template

 ****************************************************/
//#define DEBUG

#include <SEMU_SSD1331.h>
#include "astronaut.h"
#include "astronautmatte.h"
#include "background.h"

#define sclk 13   // marked SCL or CK on OLED board
#define mosi 11   // marked SDA or SI on OLED board
#define cs   10   // marked CS or OC on OLED board
#define rst  9    // marked RES or R on OLED board
#define dc   8    // marked DC or sometimes (confusingly) RS on OLED board

#define PAUSE 2000

SEMU_SSD1331 display = SEMU_SSD1331(&SPI, cs, dc, rst);

void setup(void) {

  display.begin();

}

void loop() {

  display.clearWindow();

  // display the foreground image without transparency
  // image transparency is defined in the tImage struct as a boolean.
  // (see the foot of the astronaut.h file)
  display.drawImage(&astronaut);
  delay(PAUSE);
  
  // display background image
  display.drawImage(&background);
  delay(PAUSE);

  // now overlay foreground image but this time with the
  // transparency flag in this image set to 'true'
  // (see the foot of the astronautmatte.h file)
  display.drawImage(&astronautmatte);
  delay(PAUSE);

  display.clearWindow();
  delay(PAUSE/2);
    
  // the drawMaskedImage() method does this as a single operation
  display.drawMaskedImage(0,0, &background, &astronautmatte);
  delay(PAUSE);

}
