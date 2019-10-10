/***************************************************
  This is an adaption of the Adafruit SSD1331 library incorporating additional native
  features of the SSD1331 OLED. Original copyright text follows...
  
  This is a library for the 0.96" 16-bit Color OLED with SSD1331 driver chip
  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/684
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#ifndef _SEMU_SSD1331_H_
#define _SEMU_SSD1331_H_

#include "Arduino.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>


// Select one of these defines to set the pixel color order
#define SSD1331_COLORORDER_RGB
// #define SSD1331_COLORORDER_BGR

#if defined SSD1331_COLORORDER_RGB && defined SSD1331_COLORORDER_BGR
  #error "RGB and BGR can not both be defined for SSD1331_COLORODER."
#endif

// Timing Delays
#define SSD1331_DELAYS_HWFILL		    (1000)
#define SSD1331_DELAYS_HWLINE       (400)

// SSD1331 Commands
#define SSD1331_CMD_DRAWLINE 		    0x21
#define SSD1331_CMD_DRAWRECT 		    0x22
#define SSD1331_CMD_FILL 			      0x26
#define SSD1331_CMD_SETCOLUMN 		  0x15
#define SSD1331_CMD_SETROW    		  0x75
#define SSD1331_CMD_CONTRASTA 		  0x81
#define SSD1331_CMD_CONTRASTB 		  0x82
#define SSD1331_CMD_CONTRASTC		    0x83
#define SSD1331_CMD_MASTERCURRENT 	0x87
#define SSD1331_CMD_SETREMAP 		    0xA0
#define SSD1331_CMD_STARTLINE 		  0xA1
#define SSD1331_CMD_DISPLAYOFFSET 	0xA2
#define SSD1331_CMD_NORMALDISPLAY 	0xA4
#define SSD1331_CMD_DISPLAYALLON  	0xA5
#define SSD1331_CMD_DISPLAYALLOFF 	0xA6
#define SSD1331_CMD_INVERTDISPLAY 	0xA7
#define SSD1331_CMD_SETMULTIPLEX  	0xA8
#define SSD1331_CMD_SETMASTER 		  0xAD
#define SSD1331_CMD_DISPLAYOFF 		  0xAE
#define SSD1331_CMD_DISPLAYON     	0xAF
#define SSD1331_CMD_POWERMODE 		  0xB0
#define SSD1331_CMD_PRECHARGE 		  0xB1
#define SSD1331_CMD_CLOCKDIV 		    0xB3
#define SSD1331_CMD_PRECHARGEA 		  0x8A
#define SSD1331_CMD_PRECHARGEB 		  0x8B
#define SSD1331_CMD_PRECHARGEC 		  0x8C
#define SSD1331_CMD_PRECHARGELEVEL 	0xBB
#define SSD1331_CMD_VCOMH 			    0xBE 

// enhanced constants & commands added by SEMU
#define SSD1331_CMD_COPY            0x23
#define SSD1331_CMD_DIM             0x24
#define SSD1331_CMD_CLEAR           0x25
#define SSD1331_FILL_OFF            0x00
#define SSD1331_FILL_ON             0x01
#define SSD1331_CMD_SETGRAYSCALE	  0xB8
#define SSD1331_CMD_RESETGRAYSCALE	0xB9

#define SSD1331_CMD_SETSCROLL       0x27
#define SSD1331_CMD_SCROLLOFF       0x2E
#define SSD1331_CMD_SCROLLON        0x2F

/*
 Standard and Modified Display Remap modes

 NB: Most hobbyist SSD1331-based OLEDs are supplied on breakout boards which mount the display upside 
 down in order to facilitate access to the pins, so the 'normal' initialisation mode is actually rotated
 by 180 degrees relative to the OLED.

 SETREMAP command uses an 8-bit mask:
 A(0)   - Address increment (0 = landscape mode, 1 = portrait mode)
 A(1)   - RAM column address mapping (0 = left to right, 1 = right to left i.e. mirrored on vertical axis)
 A(2)   - Colour order (0 = RGB, 1 = BGR)
 A(3)   - Left-right swapping (0 = disabled, 1 = enabled)
 A(4)   - COM Scan mode (0 = COM(0) to COM(n-1), 1 = COM(n-1) to COM(0) i.e. mirrored on horizontal axis)
 A(5)   - COM Odd Even Split (0 = disabled, 1 = enabled)
 A(6/7) - Colour depth (0 = 256, 1 = 65k format 1, 2 = 65k format 2)
 */
 
#define SSD1331_PORTRAIT            0x01    // 0b00000001
#define SSD1331_FLIP_X              0x02    // 0b00000010
#define SSD1331_BGR                 0x04		// 0b00000100
#define SSD1331_FLIP_LR             0x08    // 0b00001000
#define SSD1331_FLIP_Y              0x10		// 0b00010000
#define SSD1331_SPLIT_OE            0x20		// 0b00100000
#define SSD1331_COLOR_65K1          0x40		// 0b01000000
#define SSD1331_COLOR_65K2          0x80		// 0b10000000

// SSD1331 Hardware orientation modes
#define SSD1331_ROTATE_NORMAL       0x72 		// 0b01110010
#define SSD1331_ROTATE_NORMALFLIP  	0x70 		// 0b01110000
#define SSD1331_ROTATE_090       	  0x71 		// 0b01110001
#define SSD1331_ROTATE_090FLIP   	  0x61 		// 0b01100001
#define SSD1331_ROTATE_180		 	    0x60 		// 0b01100000
#define SSD1331_ROTATE_180FLIP	 	  0x62 		// 0b01100010
#define SSD1331_ROTATE_270		 	    0x63 		// 0b01100011
#define SSD1331_ROTATE_270FLIP	 	  0x73 		// 0b01110011

// SSD1331 Hardware scroll modes
#define SSD1331_SCROLL_OFF		 	    0x00		// scrolling inactive
#define SSD1331_SCROLL_ON		 	      0x01		// scrolling active
#define SSD1331_SCROLL_X	 	        0x02		// scrolling in horizontal direction
#define SSD1331_SCROLL_Y	 	        0x04    // scrolling in vertical direction

// Optimised function to read image header file from program memory
template<typename T> T PROGMEM_read(const T * sce) {
	static T temp;
	memcpy_P(&temp, sce, sizeof(T));
	return temp;
}

// full colour image
typedef struct PROGMEM {
	const uint16_t *data;
	uint16_t width;
	uint16_t height;
	uint16_t pixels;	// total number of pixels in image
	uint8_t depth; 		// bits per pixel
	bool istrans;       // if transparent
	uint16_t tcolor;    // transparency color
} tImage;

// monochrome image
typedef struct PROGMEM {
	const uint8_t *data;
	uint16_t width;
	uint16_t height;
	uint16_t pixels;	// total number of pixels in image
	uint8_t depth; 		// bits per pixel
	bool istrans;       // if transparent
	uint8_t tcolor;     // transparency color
} bwImage;

/// Class to manage hardware interface with SSD1331 chipset
class SEMU_SSD1331 : public Adafruit_SPITFT {
 public:
  SEMU_SSD1331(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst);
  SEMU_SSD1331(int8_t cs, int8_t dc, int8_t rst);
  // 3-4 args using hardware SPI (must specify peripheral) (reset optional)
  SEMU_SSD1331(SPIClass *spi, int8_t cs, int8_t dc, int8_t rst = -1);

  // commands
  void begin(uint32_t begin=8000000);

  void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

  void enableDisplay(boolean enable);
  void setDisplayMode(uint8_t mode);
  void setOrientation(uint8_t orientation);
  void setGrayScale(float gamma);
  void clearWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
	void clearWindow();
	void dimWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
	void setScroll(uint8_t x_speed, uint8_t y_speed, uint8_t y0, uint8_t rows,
		uint8_t interval);
  void startScroll(void);
  void stopScroll(void);
  
  void drawImage(uint16_t x0, uint16_t y0, const tImage *img);
  void drawImage(uint16_t x0, uint16_t y0, const bwImage *img);
  void drawImage(const tImage *img);
  void drawImage(const bwImage *img);
  void drawMaskedImage(uint16_t x0, uint16_t y0, const tImage *img, const tImage *mask);
  void drawMaskedSegment(uint16_t x0, uint16_t y0, const tImage *img, const tImage *mask);
	
	/*****************************************************************************
	Adafruit_GFX line and rectangle drawing potential overrides
	*****************************************************************************/
/*
  void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color);
  void drawRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, 
    uint16_t border_color, uint16_t fill_color, bool filled);
  void drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
  void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
	void drawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint16_t color);
  void drawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint16_t color);
  void fillScreen(uint16_t color);
*/  
  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void fillScreen(uint16_t color);
  void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
  void drawRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, 
    uint16_t border_color, uint16_t fill_color, bool filled);
  void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

	// getter functions
	uint8_t getOrientation(void);
	
  static const int16_t TFTWIDTH = 96;    ///< The width of the display
  static const int16_t TFTHEIGHT = 64;   ///< The height of the display
 
  private:
 
   volatile uint16_t _backcolor = 0x0000;
   volatile uint8_t _scrollmode = SSD1331_SCROLL_OFF;
   volatile uint8_t _orientation = SSD1331_ROTATE_NORMAL;
   volatile uint8_t _mode = SSD1331_CMD_NORMALDISPLAY;

};

#endif