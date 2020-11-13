/*********************************************************************
  Spinning 3D wireframe cube demo

  Not sure where this example originated but I based it
  on an anonymous source by Googling 'Arduino 3D_engine'
*********************************************************************/
#include <SEMU_SSD1331.h>

#define sclk 13
#define mosi 11
#define cs   10
#define rst  9
#define dc   8

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF
#define ORANGE          0xfc40
#define PURPLE          0x815b
#define BROWN           0x9a85
#define INDIGO          0x480f
#define MAROON          0x7800
#define TURQUOISE       0x3ef9

float cube[8][3];
uint8_t point_conn[8][8] = {0};
float rad_X = PI / 4;
float rad_Y = PI / 3;
float rad_Z = PI / 8;
uint8_t DIM = 32;
uint8_t XOFF = 48;
uint8_t YOFF = 32;
uint16_t edge_col[12] = {
  RED,
  GREEN,
  BLUE,
  YELLOW,
  CYAN,
  MAGENTA,
  ORANGE,
  PURPLE,
  BROWN,
  INDIGO,
  MAROON,
  TURQUOISE
};


SEMU_SSD1331 display = SEMU_SSD1331(&SPI, cs, dc, rst);

void setup() {

  display.begin();
  display.clearWindow();

  createConn(point_conn);
}


void loop() {
  /* Define cube corner coordinates */
  /* Every loop cube needs to start from first shape, otherwise floating point */
  /* numbers will saturate or go zero */

  createCube(cube);

  display.clearWindow();

  /* 3D rotation in world coords */
  rotate_X(cube, 8, rad_X);
  rotate_Y(cube, 8, rad_X);
  rotate_Z(cube, 8, rad_Z);

  filldispbuffer(cube, point_conn, 8);

  rad_X += 0.04;
  rad_Y += 0.06;
  rad_Z += 0.08;

  delay(20);
}

void filldispbuffer(float obj[][3], uint8_t edges[][8], uint8_t num_point) {

  uint16_t x1, y1;
  uint16_t x2, y2;
  uint8_t c = 0;

  for (uint8_t i = 0; i < 8; i++)
    for (uint8_t j = i + 1; j < 8; j++)
      if ( edges[i][j] ) {

        //Simple 2D projection
        x1 = (uint16_t)(obj[i][1] * DIM) + XOFF;
        y1 = (uint16_t)(obj[i][2] * DIM) + YOFF;

        x2 = (uint16_t)(obj[j][1] * DIM) + XOFF;
        y2 = (uint16_t)(obj[j][2] * DIM) + YOFF;

        display.drawLine(x1, y1, x2, y2, edge_col[c]);
        c += 1;
      }
}

void rotate_X(float result[][3], uint8_t num_point, float rad) {
  float temp;
  for (uint8_t i = 0; i < num_point; i++) {
    temp = result[i][1] * (float)cos(rad) - result[i][2] * (float)sin(rad);
    result[i][2] = result[i][1] * (float)sin(rad) + result[i][2] * (float)cos(rad);
    result[i][1] = temp;
  }
}

void rotate_Y(float result[][3], uint8_t num_point, float rad) {
  float temp;
  for (uint8_t i = 0; i < num_point; i++) {
    temp = result[i][0] * (float)cos(rad) + result[i][2] * (float)sin(rad);
    result[i][2] = -result[i][0] * (float)sin(rad) + result[i][2] * (float)cos(rad);
    result[i][0] = temp;
  }
}

void rotate_Z(float result[][3], uint8_t num_point, float rad) {
  float temp;
  for (uint8_t i = 0; i < num_point; i++) {
    temp = result[i][0] * (float)cos(rad) - result[i][1] * (float)sin(rad);
    result[i][1] = -result[i][0] * (float)sin(rad) + result[i][1] * (float)cos(rad);
    result[i][0] = temp;
  }
}

/* manual definition for cube corner coordinates */
void createCube(float obj[][3]) {
  obj[0][0] = 0.5;
  obj[0][1] = 0.5;
  obj[0][2] = 0.5;

  obj[1][0] = 0.5;
  obj[1][1] = 0.5;
  obj[1][2] = -0.5;

  obj[2][0] = 0.5;
  obj[2][1] = -0.5;
  obj[2][2] = 0.5;

  obj[3][0] = 0.5;
  obj[3][1] = -0.5;
  obj[3][2] = -0.5;

  obj[4][0] = -0.5;
  obj[4][1] = 0.5;
  obj[4][2] = 0.5;

  obj[5][0] = -0.5;
  obj[5][1] = 0.5;
  obj[5][2] = -0.5;

  obj[6][0] = -0.5;
  obj[6][1] = -0.5;
  obj[6][2] = 0.5;

  obj[7][0] = -0.5;
  obj[7][1] = -0.5;
  obj[7][2] = -0.5;
};

/* manual definition for edge connectivity */
void createConn(uint8_t edges[][8]) {
  edges[0][1] = 1;
  edges[0][2] = 1;
  edges[0][4] = 1;

  edges[1][0] = 1;
  edges[1][5] = 1;
  edges[1][3] = 1;

  edges[2][0] = 1;
  edges[2][3] = 1;
  edges[2][6] = 1;

  edges[3][7] = 1;
  edges[3][2] = 1;
  edges[3][1] = 1;

  edges[4][0] = 1;
  edges[4][5] = 1;
  edges[4][6] = 1;

  edges[5][4] = 1;
  edges[5][1] = 1;
  edges[5][7] = 1;

  edges[6][2] = 1;
  edges[6][4] = 1;
  edges[6][7] = 1;

  edges[7][3] = 1;
  edges[7][5] = 1;
  edges[7][6] = 1;
}
