/*
 * Based on Open Hardware Algerie OpenHDZ examples:
 * https://github.com/OpenHDZ/Arduino-experimentation
*/
#include <SEMU_SSD1331.h>

//#define ESP32  // uncomment to configure for ESP32 Devkit

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
#define COORDS

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

#if defined ESP32 // use software SPI constructor for ESP32
SEMU_SSD1331 display = SEMU_SSD1331(cs, dc, mosi, sclk, rst);
#else
SEMU_SSD1331 display = SEMU_SSD1331(&SPI, cs, dc, rst);
#endif

float rx = 0;
float ry = 0;
float rz = 0;
int8_t rate = 2;
int cSize = 25; // absolute cube dimensions
int zOff = 100; // offset near or far
int xOff = 00; // offset left or right
int yOff = 0; // offset up or down
int view_plane = 65; // how close observer is (higher = closer)

float cube3d[8][3] = {
  {xOff - cSize, yOff + cSize, zOff - cSize},
  {xOff + cSize, yOff + cSize, zOff - cSize},
  {xOff - cSize, yOff - cSize, zOff - cSize},
  {xOff + cSize, yOff - cSize, zOff - cSize},
  {xOff - cSize, yOff + cSize, zOff + cSize},
  {xOff + cSize, yOff + cSize, zOff + cSize},
  {xOff - cSize, yOff - cSize, zOff + cSize},
  {xOff + cSize, yOff - cSize, zOff + cSize}
};
unsigned char cube2d[8][2];

void setup() {
  display.begin();
  display.clearWindow();
}

void loop() {

  for (int16_t i = 0; i < 360; i ++) {
    xrotate(PI / 180);
    printcube();
  }

  for (int16_t i = 0; i < 360; i ++) {
    yrotate(PI / 180);
    printcube();
  }

  for (int16_t i = 0; i < 360; i ++) {
    zrotate(PI / 180);
    printcube();
  }

  for (int16_t i = 0; i < 360; i ++) {
    xrotate(PI / 180);
    yrotate(PI / 180);
    zrotate(PI / 180);
    printcube();
  }

}
void printcube() {
  //calculate 2d points
  for (byte i = 0; i < 8; i++) {
    cube2d[i][0] = (unsigned char)((cube3d[i][0] * view_plane / cube3d[i][2]) + (96 / 2));
    cube2d[i][1] = (unsigned char)((cube3d[i][1] * view_plane / cube3d[i][2]) + (64 / 2));
  }
  display.clearWindow();
  draw_cube();
#ifdef COORDS
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.print("X: ");
  display.println(int(rx * 180 / PI) % 360);
  display.print("Y: ");
  display.println(int(ry * 180 / PI) % 360);
  display.print("Z: ");
  display.println(int(rz * 180 / PI) % 360);
#else
  delay(rate);
#endif
}

void zrotate(float q) {
  rz += q;
  float tx, ty, temp;
  for (byte i = 0; i < 8; i++) {
    tx = cube3d[i][0] - xOff;
    ty = cube3d[i][1] - yOff;
    temp = tx * cos(q) - ty * sin(q);
    ty = tx * sin(q) + ty * cos(q);
    tx = temp;
    cube3d[i][0] = tx + xOff;
    cube3d[i][1] = ty + yOff;
  }
}

void yrotate(float q) {
  ry += q;
  float tx, tz, temp;
  for (byte i = 0; i < 8; i++) {
    tx = cube3d[i][0] - xOff;
    tz = cube3d[i][2] - zOff;
    temp = tz * cos(q) - tx * sin(q);
    tx = tz * sin(q) + tx * cos(q);
    tz = temp;
    cube3d[i][0] = tx + xOff;
    cube3d[i][2] = tz + zOff;
  }
}

void xrotate(float q) {
  rx += q;
  float ty, tz, temp;
  for (byte i = 0; i < 8; i++) {
    ty = cube3d[i][1] - yOff;
    tz = cube3d[i][2] - zOff;
    temp = ty * cos(q) - tz * sin(q);
    tz = ty * sin(q) + tz * cos(q);
    ty = temp;
    cube3d[i][1] = ty + yOff;
    cube3d[i][2] = tz + zOff;
  }
}

void draw_cube() {
  display.drawLine(cube2d[0][0], cube2d[0][1], cube2d[1][0], cube2d[1][1], RED);
  display.drawLine(cube2d[0][0], cube2d[0][1], cube2d[2][0], cube2d[2][1], RED);
  display.drawLine(cube2d[0][0], cube2d[0][1], cube2d[4][0], cube2d[4][1], GREEN);
  display.drawLine(cube2d[1][0], cube2d[1][1], cube2d[5][0], cube2d[5][1], GREEN);
  display.drawLine(cube2d[1][0], cube2d[1][1], cube2d[3][0], cube2d[3][1], RED);
  display.drawLine(cube2d[2][0], cube2d[2][1], cube2d[6][0], cube2d[6][1], GREEN);
  display.drawLine(cube2d[2][0], cube2d[2][1], cube2d[3][0], cube2d[3][1], RED);
  display.drawLine(cube2d[4][0], cube2d[4][1], cube2d[6][0], cube2d[6][1], BLUE);
  display.drawLine(cube2d[4][0], cube2d[4][1], cube2d[5][0], cube2d[5][1], BLUE);
  display.drawLine(cube2d[7][0], cube2d[7][1], cube2d[6][0], cube2d[6][1], BLUE);
  display.drawLine(cube2d[7][0], cube2d[7][1], cube2d[3][0], cube2d[3][1], GREEN);
  display.drawLine(cube2d[7][0], cube2d[7][1], cube2d[5][0], cube2d[5][1], BLUE);
}
