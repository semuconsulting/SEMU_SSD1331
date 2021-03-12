/***************************************************
  Clock face demo

  Not sure where this example originated but I got it
  from S.U.M.O.T.O.Y - Max MC Costa
***************************************************/

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

#define PAUSE 1000

#if defined ESP32 // use software SPI constructor for ESP32
SEMU_SSD1331 display = SEMU_SSD1331(cs, dc, mosi, sclk, rst);
#else
SEMU_SSD1331 display = SEMU_SSD1331(&SPI, cs, dc, rst);
#endif

uint16_t ccenterx, ccentery; //center x,y of the clock
const uint16_t cradius = 32;//radius of the clock
const float scosConst = 0.0174532925;
float sx = 0, sy = 1, mx = 1, my = 0, hx = -1, hy = 0;
float sdeg = 0, mdeg = 0, hdeg = 0;
uint16_t osx, osy, omx, omy, ohx, ohy;
uint16_t x0 = 0, x1 = 0, yy0 = 0, yy1 = 0;
uint32_t targetTime = 0;// for next 1 second timeout
uint8_t hh, mm, ss; //containers for current time

void drawClockFace() {
  display.fillCircle(ccenterx, ccentery, cradius - 1, BLUE);
  display.fillCircle(ccenterx, ccentery, cradius - 4, BLACK);
  // Draw 12 lines
  for (int i = 0; i < 360; i += 30) {
    sx = cos((i - 90) * scosConst);
    sy = sin((i - 90) * scosConst);
    x0 = sx * (cradius - 4) + ccenterx;
    yy0 = sy * (cradius - 4) + ccentery;
    x1 = sx * (cradius - 11) + ccenterx;
    yy1 = sy * (cradius - 11) + ccentery;
    display.drawLine(x0, yy0, x1, yy1, BLUE);
  }
}

static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9') v = *p - '0';
  return 10 * v + *++p - '0';
}

void setup(void) {
  display.begin();
  display.clearWindow();

  display.setTextColor(WHITE, BLACK);
  ccenterx = display.width() / 2;
  ccentery = display.height() / 2;
  osx = ccenterx;
  osy = ccentery;
  omx = ccenterx;
  omy = ccentery;
  ohx = ccenterx;
  ohy = ccentery;
  drawClockFace();// Draw clock face
  //get current time from compiler
  hh = conv2d(__TIME__);
  mm = conv2d(__TIME__ + 3);
  ss = conv2d(__TIME__ + 6);
  targetTime = millis() + 1000;
}

void drawClockHands(uint8_t h, uint8_t m, uint8_t s) {
  // Pre-compute hand degrees, x & y coords for a fast screen update
  sdeg = s * 6;                  // 0-59 -> 0-354
  mdeg = m * 6 + sdeg * 0.01666667;  // 0-59 -> 0-360 - includes seconds
  hdeg = h * 30 + mdeg * 0.0833333;  // 0-11 -> 0-360 - includes minutes and seconds
  hx = cos((hdeg - 90) * scosConst);
  hy = sin((hdeg - 90) * scosConst);
  mx = cos((mdeg - 90) * scosConst);
  my = sin((mdeg - 90) * scosConst);
  sx = cos((sdeg - 90) * scosConst);
  sy = sin((sdeg - 90) * scosConst);

  // Erase just old hand positions
  display.drawLine(ohx, ohy, ccenterx + 1, ccentery + 1, BLACK);
  display.drawLine(omx, omy, ccenterx + 1, ccentery + 1, BLACK);
  display.drawLine(osx, osy, ccenterx + 1, ccentery + 1, BLACK);
  // Draw new hand positions
  display.drawLine(hx * (cradius - 28) + ccenterx + 1, hy * (cradius - 28) + ccentery + 1, ccenterx + 1, ccentery + 1, WHITE);
  display.drawLine(mx * (cradius - 17) + ccenterx + 1, my * (cradius - 17) + ccentery + 1, ccenterx + 1, ccentery + 1, WHITE);
  display.drawLine(sx * (cradius - 14) + ccenterx + 1, sy * (cradius - 14) + ccentery + 1, ccenterx + 1, ccentery + 1, RED);
  display.fillCircle(ccenterx + 1, ccentery + 1, 3, RED);

  // Update old x&y coords
  osx = sx * (cradius - 14) + ccenterx + 1;
  osy = sy * (cradius - 14) + ccentery + 1;
  omx = mx * (cradius - 17) + ccenterx + 1;
  omy = my * (cradius - 17) + ccentery + 1;
  ohx = hx * (cradius - 28) + ccenterx + 1;
  ohy = hy * (cradius - 28) + ccentery + 1;
}


void loop() {
  if (targetTime < millis()) {
    targetTime = millis() + 1000;
    ss++;
    if (ss == 60) {
      ss = 0;
      mm++;
      if (mm > 59) {
        mm = 0;
        hh++;
        if (hh > 23) hh = 0;
      }
    }
    drawClockHands(hh, mm, ss);

  }
}
