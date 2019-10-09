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

#include "SEMU_SSD1331.h"
#include "pins_arduino.h"
#include "wiring_private.h"

/***********************************/


/*!
  @brief   SPI displays set an address window rectangle for blitting pixels
  @param  x  Top left corner x coordinate
  @param  y  Top left corner x coordinate
  @param  w  Width of window
  @param  h  Height of window
*/
void SEMU_SSD1331::setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {

  uint8_t x1 = x;
  uint8_t y1 = y;
  if (x1 > 95) x1 = 95;
  if (y1 > 63) y1 = 63;

  uint8_t x2 = (x+w-1);
  uint8_t y2 = (y+h-1);
  if (x2 > 95) x2 = 95;
  if (y2 > 63) y2 = 63;

  if (x1 > x2) {
    uint8_t t = x2;
    x2 = x1;
    x1 = t;
  }
  if (y1 > y2) {
    uint8_t t = y2;
    y2 = y1;
    y1 = t;
  }

  sendCommand(0x15); // Column addr set
  sendCommand(x1);
  sendCommand(x2);

  sendCommand(0x75); // Column addr set
  sendCommand(y1);
  sendCommand(y2);

  startWrite();
}


/**************************************************************************/
/*!
    @brief   Initialize SSD1331 chip
    Connects to the SSD1331 over SPI and sends initialization procedure commands
    @param    freq  Desired SPI clock frequency
*/
/**************************************************************************/
void SEMU_SSD1331::begin(uint32_t freq) {
    initSPI(freq);

    // Initialization Sequence
    sendCommand(SSD1331_CMD_DISPLAYOFF);  	// 0xAE
    sendCommand(SSD1331_CMD_SETREMAP); 	// 0xA0
#if defined SSD1331_COLORORDER_RGB
    sendCommand(0x72);				// RGB Color
#else
    sendCommand(0x76);				// BGR Color
#endif
    sendCommand(SSD1331_CMD_STARTLINE); 	// 0xA1
    sendCommand(0x0);
    sendCommand(SSD1331_CMD_DISPLAYOFFSET); 	// 0xA2
    sendCommand(0x0);
    sendCommand(SSD1331_CMD_NORMALDISPLAY);  	// 0xA4
    sendCommand(SSD1331_CMD_SETMULTIPLEX); 	// 0xA8
    sendCommand(0x3F);  			// 0x3F 1/64 duty
    sendCommand(SSD1331_CMD_SETMASTER);  	// 0xAD
    sendCommand(0x8E);
    sendCommand(SSD1331_CMD_POWERMODE);  	// 0xB0
    sendCommand(0x0B);
    sendCommand(SSD1331_CMD_PRECHARGE);  	// 0xB1
    sendCommand(0x31);
    sendCommand(SSD1331_CMD_CLOCKDIV);  	// 0xB3
    sendCommand(0xF0);  // 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)
    sendCommand(SSD1331_CMD_PRECHARGEA);  	// 0x8A
    sendCommand(0x64);
    sendCommand(SSD1331_CMD_PRECHARGEB);  	// 0x8B
    sendCommand(0x78);
    sendCommand(SSD1331_CMD_PRECHARGEC);  	// 0x8C
    sendCommand(0x64);
    sendCommand(SSD1331_CMD_PRECHARGELEVEL);  	// 0xBB
    sendCommand(0x3A);
    sendCommand(SSD1331_CMD_VCOMH);  		// 0xBE
    sendCommand(0x3E);
    sendCommand(SSD1331_CMD_MASTERCURRENT);  	// 0x87
    sendCommand(0x06);
    sendCommand(SSD1331_CMD_CONTRASTA);  	// 0x81
    sendCommand(0x91);
    sendCommand(SSD1331_CMD_CONTRASTB);  	// 0x82
    sendCommand(0x50);
    sendCommand(SSD1331_CMD_CONTRASTC);  	// 0x83
    sendCommand(0x7D);
    sendCommand(SSD1331_CMD_DISPLAYON);	//--turn on oled panel
    _width  = TFTWIDTH;
    _height = TFTHEIGHT;
}




/**************************************************************************/
/*!
    @brief  Instantiate Adafruit SSD1331 driver with software SPI
    @param    cs    Chip select pin #
    @param    dc    Data/Command pin #
    @param    mosi  SPI MOSI pin #
    @param    sclk  SPI Clock pin #
    @param    rst   Reset pin # (optional, pass -1 if unused)
*/
/**************************************************************************/
SEMU_SSD1331::SEMU_SSD1331(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst) :  Adafruit_SPITFT(TFTWIDTH, TFTHEIGHT, cs, dc, mosi, sclk, rst, -1) {
}

/**************************************************************************/
/*!
    @brief  Instantiate Adafruit SSD1331 driver with hardware SPI
    @param    cs    Chip select pin #
    @param    dc    Data/Command pin #
    @param    rst   Reset pin # (optional, pass -1 if unused)
*/
/**************************************************************************/
SEMU_SSD1331::SEMU_SSD1331(int8_t cs, int8_t dc, int8_t rst) : Adafruit_SPITFT(TFTWIDTH, TFTHEIGHT, cs, dc, rst) {
}

/**************************************************************************/
/*!
    @brief  Instantiate Adafruit SSD1331 driver with hardware SPI
    @param    spi   Pointer to an existing SPIClass instance (e.g. &SPI, the
                    microcontroller's primary SPI bus).
    @param    cs    Chip select pin #
    @param    dc    Data/Command pin #
    @param    rst   Reset pin # (optional, pass -1 if unused)
*/
/**************************************************************************/
SEMU_SSD1331::SEMU_SSD1331(SPIClass *spi, int8_t cs, int8_t dc, int8_t rst) :
#if defined(ESP8266)
  Adafruit_SPITFT(TFTWIDTH, TFTWIDTH, cs, dc, rst) {
#else
  Adafruit_SPITFT(TFTWIDTH, TFTWIDTH, spi, cs, dc, rst) {
#endif
}

/**************************************************************************/
/*!
    @brief  Change whether display is on or off
    @param	 enable True if you want the display ON, false OFF
*/
/**************************************************************************/
void SEMU_SSD1331::enableDisplay(boolean enable) {
    sendCommand(enable ? SSD1331_CMD_DISPLAYON : SSD1331_CMD_DISPLAYOFF);
}

/**************************************************************************/
/*!
    @brief  Change hardware orientation of display
    @param	orientation Set to required orientation mode
*/
/**************************************************************************/
void SEMU_SSD1331::setOrientation(int8_t orientation) {

switch (orientation) {
	
	// landscape modes
    case SSD1331_ROTATE_NORMAL:
	  case SSD1331_ROTATE_NORMALFLIP:
		setRotation(0);
		_width = TFTWIDTH;
		_height = TFTHEIGHT;
		break;
	  case SSD1331_ROTATE_180:       
    case SSD1331_ROTATE_180FLIP: 
		setRotation(2);
		_width = TFTWIDTH;
		_height = TFTHEIGHT;
		break;
	// portrait modes
    case SSD1331_ROTATE_090:       
    case SSD1331_ROTATE_090FLIP:
		setRotation(1);
		_width = TFTHEIGHT;
		_height = TFTWIDTH;
		break;		
    case SSD1331_ROTATE_270:      
    case SSD1331_ROTATE_270FLIP:
		setRotation(3);
		_width = TFTHEIGHT;
		_height = TFTWIDTH;
		break;
	  default:
		return;
		
	}
	
	_orientation = orientation;
	startWrite();
	writeCommand(SSD1331_CMD_SETREMAP);
	writeCommand(orientation);
	endWrite();
	delayMicroseconds(SSD1331_DELAYS_HWFILL);
	
}

/**************************************************************************/
/*!
    @brief  Sets display mode from range of supported modes
	  @param  mode  integer representing mode
	
*/
/**************************************************************************/
void SEMU_SSD1331::setDisplayMode(int8_t mode) {

	switch (mode) {
	
	// only handle supported display modes
	case SSD1331_CMD_NORMALDISPLAY:
	case SSD1331_CMD_DISPLAYALLON:
	case SSD1331_CMD_DISPLAYALLOFF:
	case SSD1331_CMD_INVERTDISPLAY:
	case SSD1331_CMD_DISPLAYOFF:
	case SSD1331_CMD_DISPLAYON:
		sendCommand(mode);
		delayMicroseconds(SSD1331_DELAYS_HWFILL);
		break;
	}
}

/**************************************************************************/
/*!
    @brief  Implements hardware SETGRAYSCALE command
	SSD1331 implements 32 levels of grayscale, each of which can be amended over the
    range decimal 0 - 125. This uses a gamma function to make adjustment more intuitive.
    gamma = 1.0 - linear curve (default) 
    gamma > 1.0 - darkening exponential curve
    gamma < 1.0 - brightening exponential curve
	@param  gamma  gamma greyscale value
	
*/
/**************************************************************************/
void SEMU_SSD1331::setGrayScale(float gamma) {

	uint8_t i, pw;

	if (gamma == 1.0) { // linear curve
		startWrite();
		writeCommand(SSD1331_CMD_RESETGRAYSCALE);
		endWrite();
	}
	else { // exponential curve - +ve is darker, -ve is lighter
		startWrite();
		writeCommand(SSD1331_CMD_SETGRAYSCALE);
		for (i = 0; i < 32; i++) {
			pw = 125.0 * pow((i * 2.0 + 1.0) / 63.0, gamma);
			writeCommand(pw);
		}
		endWrite();
	}
	
}

/**************************************************************************/
/*!
    @brief  Implements hardware CLEAR command
    @param    x0     from top coordinate
	  @param    y0     from leftmost coordinate
	  @param    x1     from bottom coordinate
	  @param    y1     from rightmost coordinate
	
*/
/**************************************************************************/
void SEMU_SSD1331::clearWindow(int16_t x0, int16_t y0, int16_t x1,
	int16_t y1) {

	startWrite();
	writeCommand(SSD1331_CMD_CLEAR);
	writeCommand(x0);
	writeCommand(y0);
	writeCommand(x1);
	writeCommand(y1);
	endWrite();
	delayMicroseconds(SSD1331_DELAYS_HWFILL);

}

/**************************************************************************/
/*!
    @brief  Implements hardware CLEAR command for entire display
	
*/
/**************************************************************************/
void SEMU_SSD1331::clearWindow() {

	clearWindow(0, 0, TFTWIDTH - 1, TFTHEIGHT - 1);

}

/**************************************************************************/
/*!
    @brief  Implements hardware SETSCROLL command
    @param    x_speed     horizontal scroll speed
	  @param    y_speed     vertical scroll speed
	  @param    y0          starting row
	  @param    rows        number of rows to scroll
	  @param    interval    scrolling offset
	
*/
/**************************************************************************/
void SEMU_SSD1331::setScroll(uint8_t x_speed, uint8_t y_speed,
	uint8_t y0, uint8_t rows, uint8_t interval) {

	// check scroll not active 
	if (_scrollmode & SSD1331_SCROLL_ON) return;
	
	_scrollmode = 0;
	
	// check bounds 
	if (x_speed > _width - 1) {
		x_speed = _width - 1;
	}
	if (y_speed > _height - 1) {
		y_speed = _height - 1;
	}
	if (y0 > _height) {
		y0 = _height;
	}
	if (rows + y0 > _height) {
		rows = _height;
	}
	if (interval > 3) {
		interval = 3;
	}

	// set new scroll parameters
	startWrite();
	writeCommand(SSD1331_CMD_SETSCROLL);
	//delayMicroseconds(SSD1331_DELAYS_HWFILL);
	writeCommand(x_speed); // horizontal offset per step i.e. horizontal scroll speed
	writeCommand(y0);			// start row for scroll
	writeCommand(rows);			// number of scrolling rows
	writeCommand(y_speed);// vertical offset per step i.e. vertical scroll speed
	writeCommand(interval); // time interval between scroll steps 0 = 6 frames per scroll step, 1 = 10 frames, 2 = 100 frames, 3 = 200 frames
	endWrite();
	delayMicroseconds(SSD1331_DELAYS_HWFILL);
	
	if (x_speed > 0) {_scrollmode |= SSD1331_SCROLL_X;}
	if (y_speed > 0) {_scrollmode |= SSD1331_SCROLL_Y;}

}

/**************************************************************************/
/*!
    @brief  Implements hardware SCROLL ON commmand
	
*/
/**************************************************************************/
void SEMU_SSD1331::startScroll() {

	// start scrolling if setup and not already active
	if (_scrollmode ^ SSD1331_SCROLL_ON) {

		sendCommand(SSD1331_CMD_SCROLLON);
		delayMicroseconds(SSD1331_DELAYS_HWFILL);
		_scrollmode |= SSD1331_SCROLL_ON;
	}

}

/**************************************************************************/
/*!
    @brief  Implements hardware SCROLL OFF commmand
	
*/
/**************************************************************************/
void SEMU_SSD1331::stopScroll() {

	// stop scrolling if already active
	if (_scrollmode & SSD1331_SCROLL_ON) {
		
		sendCommand(SSD1331_CMD_SCROLLOFF);
		delayMicroseconds(SSD1331_DELAYS_HWFILL);
		_scrollmode = SSD1331_SCROLL_OFF;
	}

}
