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
  if (x1 > TFTWIDTH-1) x1 = TFTWIDTH-1;
  if (y1 > TFTHEIGHT-1) y1 = TFTHEIGHT-1;

  uint8_t x2 = (x+w-1);
  uint8_t y2 = (y+h-1);
  if (x2 > TFTWIDTH-1) x2 = TFTWIDTH-1;
  if (y2 > TFTHEIGHT-1) y2 = TFTHEIGHT-1;

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
  
  writeCommand(SSD1331_CMD_SETCOLUMN); // Column addr set
  writeCommand(x1);
  writeCommand(x2);
  writeCommand(SSD1331_CMD_SETROW); // Row addr set
  writeCommand(y1);
  writeCommand(y2);
  
  startWrite(); // why if already surrounded by SPI txn???
  
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
void SEMU_SSD1331::setOrientation(uint8_t orientation) {

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
void SEMU_SSD1331::setDisplayMode(uint8_t mode) {

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
void SEMU_SSD1331::clearWindow(uint8_t x0, uint8_t y0, uint8_t x1,
	uint8_t y1) {

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
    @brief  Implements hardware COPY command
    @param    x0     first x coordinate
	@param    y0     first y coordinate
	@param    x1     second x coordinate
	@param    y1     second y coordinate
	@param    x2     destination top x coordinate
	@param    y2     destination top y coordinate
	
*/
/**************************************************************************/
void SEMU_SSD1331::copyWindow(uint8_t x0, uint8_t y0, uint8_t x1,
	uint8_t y1, uint8_t x2, uint8_t y2) {
  
	startWrite();
	writeCommand(SSD1331_CMD_COPY);
	writeCommand(x0);
	writeCommand(y0);
	writeCommand(x1);
	writeCommand(y1);
	writeCommand(x2);
	writeCommand(y2);
	endWrite();
	delayMicroseconds(SSD1331_DELAYS_HWFILL);

}

/**************************************************************************/
/*!
    @brief  Implements non-overlapping 'MOVE' via hardware COPY and CLEAR commands
    @param    x0     first x coordinate
	@param    y0     first y coordinate
	@param    x1     second x coordinate
	@param    y1     second y coordinate
	@param    x2     destination top x coordinate
	@param    y2     destination top y coordinate
	
*/
/**************************************************************************/
void SEMU_SSD1331::moveWindow(uint8_t x0, uint8_t y0, uint8_t x1,
	uint8_t y1, uint8_t x2, uint8_t y2) {

	// Crude hardware implementation which only supports
	// non-overlapping moves
	
	startWrite();
	writeCommand(SSD1331_CMD_COPY);
	writeCommand(x0);
	writeCommand(y0);
	writeCommand(x1);
	writeCommand(y1);
	writeCommand(x2);
	writeCommand(y2);
	endWrite();
	delayMicroseconds(SSD1331_DELAYS_HWFILL);
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
    @brief  Implements hardware DIM command
    @param    x0     top coordinate
	@param    y0     leftmost coordinate
	@param    x1     bottom coordinate
	@param    y1     rightmost coordinate
	
*/
/**************************************************************************/
void SEMU_SSD1331::dimWindow(uint8_t x0, uint8_t y0, uint8_t x1,
	uint8_t y1) {
	
	startWrite();
	writeCommand(SSD1331_CMD_DIM);
	writeCommand(x0);
	writeCommand(y0);
	writeCommand(x1);
	writeCommand(y1);
	endWrite();
	delayMicroseconds(SSD1331_DELAYS_HWFILL);

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
	if (x_speed > TFTWIDTH - 1) {
		x_speed = TFTWIDTH - 1;
	}
	if (y_speed > TFTHEIGHT - 1) {
		y_speed = TFTHEIGHT - 1;
	}
	if (y0 > TFTHEIGHT) {
		y0 = TFTHEIGHT;
	}
	if (rows + y0 > TFTHEIGHT) {
		rows = TFTHEIGHT;
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

/**************************************************************************/
/*!
    @brief  Paints a full colour image from flash memory (PROGMEM)
    @param    x0    x (horizontal) starting display coordinate
    @param    y0    y (vertical) starting display coordinate
    @param    *img  pointer to PROGMEM image bitmap
*/
/**************************************************************************/
void SEMU_SSD1331::drawImage(uint8_t x0, uint8_t y0, const tImage *img) {

	uint16_t x, y, px, color;
	bool skipping = false;
	const uint16_t * imageData = PROGMEM_read(&img->data);// copy pixels from flash (program) memory into SRAM (data) memory
	uint16_t iWidth = pgm_read_word(&img->width);      // copy image width
	uint16_t iHeight = pgm_read_word(&img->height);    // copy image height
	uint16_t iSize = pgm_read_word(&img->pixels);      // copy number of pixels in image
	//uint8_t  iDepth = pgm_read_byte(&img->depth);    // copy number of bits per pixel (i.e. colour depth)
	bool     isTrans = pgm_read_byte(&img->istrans);   // whether image is transparent or not
	uint16_t iTcolor = pgm_read_word(&img->tcolor);    // color to be rendered as transparent, if above flag is set

	px = 0;
	  
  // if this is a non-transparent, full screen image, we can paint the entire display
  // starting at pixel 0 using the hardware's cursor auto-increment
  // takes around 24ms per image
  
	if (x0 == 0 && y0 == 0 && iWidth == _width && iHeight == _height && !isTrans) {

    startWrite();
		for (px = 0; px < iSize; px++) {
			color = pgm_read_word(&imageData[px]);
			SPI_WRITE16(color);  //  pushColor now deprecated
		}
		endWrite();
		
	}
	
	// otherwise we have to reposition the cursor programmatically, which is about
	// 10-15% slower
	else {

    startWrite();
		for (y = y0; y < y0 + iHeight; y++) {

			setCursor(x0, y);
			for (x = x0; x < x0 + iWidth; x++) {
				color = pgm_read_word(&imageData[px]);
				// if this pixel transparent colour, skip it
				if ((isTrans) && (color == iTcolor)) {
					skipping = true;
					//pushColor(0xffff);    // this worked pre-Adafruit_SPI but there's now a bug
					//SPI_WRITE16(0xffff);  // in the handling of setCursor I still need to fix
				}
				// otherwise, draw the pixel
				else {
					if (skipping) {
						skipping = false;
						setCursor(x, y);
					}
					//pushColor(color);
					SPI_WRITE16(color); // pushColor now deprecated
				}
				px++;
			}

		}
		endWrite();

  }

}

/**************************************************************************/
/*!
    @brief  Paints a monochrome image from flash memory (PROGMEM)
    @param    x0    x (horizontal) starting display coordinate
    @param    y0    y (vertical) starting display coordinate
    @param    *img  pointer to PROGMEM image bitmap
*/
/**************************************************************************/
void SEMU_SSD1331::drawImage(uint8_t x0, uint8_t y0, const bwImage *img) {

	uint16_t x, y; 
	uint8_t px, color;
	bool skipping = false;
	const uint8_t * imageData = PROGMEM_read(&img->data);// copy pixels from flash (program) memory into SRAM (data) memory
	uint16_t iWidth = pgm_read_word(&img->width);        // copy image width
	uint16_t iHeight = pgm_read_word(&img->height);      // copy image height
	uint16_t iSize = pgm_read_word(&img->pixels);        // copy number of pixels in image
	//uint8_t  iDepth = pgm_read_byte(&img->depth);      // copy number of bits per pixel (i.e. colour depth)
	bool     isTrans = pgm_read_byte(&img->istrans);     // whether image is transparent or not
	uint8_t iTcolor = pgm_read_word(&img->tcolor);       // color to be rendered as transparent, if above flag is set

	px = 0;
		
  // if this is a non-transparent, full screen image, we can paint the entire display
  // starting at pixel 0 using the hardware's cursor auto-increment
	if (x0 == 0 && y0 == 0 && iWidth == _width && iHeight == _height && !isTrans) {
	
	  startWrite();
		for (px = 0; px < iSize; px++) {
			color = pgm_read_byte(&imageData[px]);
			SPI_WRITE16(color); //  pushColor now deprecated
		}
		endWrite();
		
	}
	// otherwise we have to reposition the cursor programmatically, which is about
	// 10-15% slower
	else {
	
	  startWrite();
		for (y = y0; y < y0 + iHeight; y++) {
			setCursor(x0, y);
			for (x = x0; x < x0 + iWidth; x++) {
				color = pgm_read_byte(&imageData[px]);
				// if this pixel transparent colour, skip it
				if ((isTrans) && (color == iTcolor)) {
					skipping = true;
					//pushColor(0xffff);    // this worked pre-Adafruit_SPI but there's now a bug
					//SPI_WRITE16(0xffff);  // in the handling of setCursor I still need to fix
				}
				// otherwise, draw the pixel
				else {
					if (skipping) {
						skipping = false;
						setCursor(x, y);
					}
					//pushColor(color);
					SPI_WRITE16(color); //  pushColor now deprecated
				}
				px++;
			}
		}
		endWrite();

  }

}

/**************************************************************************/
/*!
    @brief  Paints a full colour image from flash memory (PROGMEM) at default coordinates
    @param    *img  pointer to PROGMEM image bitmap
*/
/**************************************************************************/
void SEMU_SSD1331::drawImage(const tImage *img) {

	drawImage(0,0,img);

}

/**************************************************************************/
/*!
    @brief  Paints a monochrome image from flash memory (PROGMEM) at default coordinates
    @param    *img  pointer to PROGMEM image bitmap
*/
/**************************************************************************/
void SEMU_SSD1331::drawImage(const bwImage *img) {

	drawImage(0,0,img);

}

/**************************************************************************/
/*!
    @brief  Paints a masked full colour image from flash memory (PROGMEM) with a 
    specified 16-bit colour mask image
    @param    x0    x (horizontal) starting display coordinate
    @param    y0    y (vertical) starting display coordinate
    @param    *img  pointer to PROGMEM image bitmap
	  @param    *mask pointer to PROGMEM mask bitmap
*/
/**************************************************************************/
void SEMU_SSD1331::drawMaskedImage(uint8_t x0, uint8_t y0,
	const tImage *img, const tImage *mask) {

	uint16_t x, y;
	uint16_t iColour, mColour;
	uint16_t px;
	const uint16_t * imageData = PROGMEM_read(&img->data);  // copy image pixels from flash (program) memory into SRAM (data) memory
	const uint16_t * maskData = PROGMEM_read(&mask->data);  // copy mask pixels from flash (program) memory into SRAM (data) memory
	uint16_t iWidth = pgm_read_word(&img->width);           // copy image width
	uint16_t iHeight = pgm_read_word(&img->height);         // copy image height
	//uint16_t iSize = pgm_read_word(&img->pixels);           // copy number of pixels in image
	//uint8_t iDepth = pgm_read_byte(&img->depth);            // copy number of bits per pixel (i.e. colour depth)
	uint16_t iTcolor = pgm_read_word(&mask->tcolor);        // color to be rendered as transparent, if above flag is set

	//goTo(x0, y0);                          		// initialise cursor to top left
	px = 0;

  startWrite();
	for (y = y0; y < y0 + iHeight; y++) { 
		setCursor(x0, y);
		for (x = x0; x < x0 + iWidth; x++) {
			iColour = pgm_read_word(&imageData[px]);   // read in the image data
			mColour = pgm_read_word(&maskData[px]);		// read in the mask data
			if (mColour == iTcolor) {   // if the mask pixel is transparent, draw the image pixel	  
				//pushColor(iColour);
				SPI_WRITE16(iColour); // pushColor now deprecated
			}
			else {						// otherwise, draw the mask pixel
				//pushColor(mColour);
				SPI_WRITE16(mColour); // pushColor now deprecated
			}
			px++;
		}

	}
	endWrite();

}

/**************************************************************************/
/*!
    @brief  Displays a masked, display-sized segment of a larger 16-bit colour image 
    bitmap beginning at the specified x0,y0 image coordinates.
	  The image bitmap must be at least as large as the display.
	  The mask bitmap must be the same size as the display.
    @param    x0    x (horizontal) starting display coordinate
    @param    y0    y (vertical) starting display coordinate
    @param    *img  pointer to PROGMEM image bitmap
	  @param    *mask pointer to PROGMEM mask bitmap
*/
/**************************************************************************/
void SEMU_SSD1331::drawMaskedSegment(uint8_t x0, uint8_t y0,
	const tImage *img, const tImage *mask) {

	uint16_t x, y;
	uint16_t iColour, mColour;
	uint16_t pxi, pxm;
	const uint16_t * imageData = PROGMEM_read(&img->data);  // copy image pixels from flash (program) memory into SRAM (data) memory
	const uint16_t * maskData = PROGMEM_read(&mask->data);  // copy mask pixels from flash (program) memory into SRAM (data) memory
	uint16_t iWidth = pgm_read_word(&img->width);           // copy image width
	uint16_t mWidth = pgm_read_word(&mask->width);          // copy mask width
	uint16_t iHeight = pgm_read_word(&img->height);         // copy image height
	//uint16_t iSize = pgm_read_word(&img->pixels);         // copy number of pixels in image
	//uint8_t iDepth = pgm_read_byte(&img->depth);          // copy number of bits per pixel (i.e. colour depth)
	bool     isTrans = pgm_read_byte(&mask->istrans);        // whether image is transparent or not
	uint16_t iTcolor = pgm_read_word(&mask->tcolor);         // color to be rendered as transparent, if above flag is set

	startWrite();
	for (y = 0; y < TFTHEIGHT; y++) {
		
		pxi = (iWidth * (y + y0 - 1)) + x0 - 1; 			// set image pixel to leftmost pixel of segment
		pxm = mWidth * y; 									// set mask pixel to leftmost pixel of segment
		
		setCursor(0, y);
		
		for (x = 0; x < TFTWIDTH; x++) {
			iColour = pgm_read_word(&imageData[pxi]);   	// read in the image data
			mColour = pgm_read_word(&maskData[pxm]);		// read in the mask data
			if (mColour == iTcolor) {   					// if the mask pixel is transparent, draw the mask pixel	  
				//pushColor(mColour);
				SPI_WRITE16(mColour); // pushColor now deprecated
			}
			else {											// otherwise, draw the image pixel
				//pushColor(iColour);
				SPI_WRITE16(iColour); // pushColor now deprecated
			}
			pxi++;
			pxm++;	
		}

	}
	endWrite();
	
}

/**************************************************************************/
/*!
    @brief  Draws a line using hardware DRAWLINE command
      ADAFRUIT_GFX OVERRIDE
    @param   x0    x (horizontal) starting line coordinate
    @param   y0    y (vertical) starting line coordinate
    @param   x1    x (horizontal) starting line coordinate
	  @param   y1    y (vertical) ending line coordinate
	  @param   color color of line
*/
/**************************************************************************/

void SEMU_SSD1331::drawLine(int16_t x0, int16_t y0, int16_t x1,
	int16_t y1, uint16_t color) {
	
	startWrite();
	writeCommand(SSD1331_CMD_DRAWLINE);
	writeCommand(x0);
	writeCommand(y0);
	writeCommand(x1);
	writeCommand(y1);
	//delayMicroseconds (SSD1331_DELAYS_HWLINE);
	writeCommand((uint8_t)((color >> 11) << 1));
	writeCommand((uint8_t)((color >> 5) & 0x3F));
	writeCommand((uint8_t)((color << 1) & 0x3F));
	endWrite();
	delayMicroseconds(SSD1331_DELAYS_HWLINE);
	
}

/**************************************************************************/
/*!
    @brief  Draws a filled rectangle using hardware DRAWRECT and FILL commands
    @param   x0    x (horizontal) starting rectangle coordinate
    @param   y0    y (vertical) starting rectangle coordinate
    @param   x1    x (horizontal) ending rectangle coordinate
    @param   y1    y (vertical) ending rectangle coordinate
	@param   border_color border color of rectangle
	@param   fill_color fill color of rectangle	  
	@param   filled whether filled or not  
*/
/**************************************************************************/

void SEMU_SSD1331::drawRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, 
  uint16_t border_color, uint16_t fill_color, bool filled) {
	
	startWrite();
	writeCommand(SSD1331_CMD_FILL);
	writeCommand(filled);
	endWrite();
	//delayMicroseconds(SSD1331_DELAYS_HWFILL);
	startWrite();
	writeCommand(SSD1331_CMD_DRAWRECT);
	writeCommand(x0);
	writeCommand(y0);
	writeCommand(x1);
	writeCommand(y1);
	//delayMicroseconds(SSD1331_DELAYS_HWFILL);
	writeCommand((uint8_t)((border_color >> 11) << 1));
	writeCommand((uint8_t)((border_color >> 5) & 0x3F));
	writeCommand((uint8_t)((border_color << 1) & 0x3F));
	//delayMicroseconds(SSD1331_DELAYS_HWFILL);
	writeCommand((uint8_t)((fill_color >> 11) << 1));
	writeCommand((uint8_t)((fill_color >> 5) & 0x3F));
	writeCommand((uint8_t)((fill_color << 1) & 0x3F));
	endWrite();
	delayMicroseconds(SSD1331_DELAYS_HWFILL);
	
}

/**************************************************************************/
/*!
    @brief  Draws a rectangular box using hardware DRAWRECT and FILL commands
          ADAFRUIT_GFX OVERRIDE
    @param   x0    x (horizontal) starting rectangle coordinate
    @param   y0    y (vertical) starting rectangle coordinate
    @param   w     width of rectangle
	  @param   h     height of rectangle
	  @param   color color of rectangle border
*/
/**************************************************************************/

void SEMU_SSD1331::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, 
  uint16_t color) {

  drawRect(x, y, x + w-1, y + h-1, color, color, false);
	
}

/**************************************************************************/
/*!
    @brief  Draws a filled rectangle using hardware DRAWRECT and FILL commands
    @param   x0    x (horizontal) starting rectangle coordinate
    @param   y0    y (vertical) starting rectangle coordinate
    @param   w     width of rectangle
	  @param   h     height of rectangle
	  @param   color color of rectangle
*/
/**************************************************************************/

void SEMU_SSD1331::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, 
  uint16_t color) {

  drawRect(x, y, x + w-1, y + h-1, color, color, true);
	
}

/**************************************************************************/
/*!
    @brief  Draws a vertical line using hardware DRAWLINE command
    @param   x0    x (horizontal) starting line coordinate
    @param   y0    y (vertical) starting line coordinate
	  @param   h     height of line
	  @param   color color of line
*/
/**************************************************************************/

void SEMU_SSD1331::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {

	drawLine(x, y, x, y+h-1, color);
	
}

/**************************************************************************/
/*!
    @brief  Draws a horizontal line using hardware DRAWLINE command
    @param   x0    x (horizontal) starting line coordinate
    @param   y0    y (vertical) starting line coordinate
    @param   w     width of line
	  @param   color color of line

/**************************************************************************/

void SEMU_SSD1331::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {

	drawLine(x, y, x+w-1, y, color);

}

/**************************************************************************/
/*!
    @brief  Fills screen with specified color using hardware FILL command
	@param   color fill color
*/
/**************************************************************************/

void SEMU_SSD1331::fillScreen(uint16_t color) {

  drawRect(0, 0, TFTWIDTH - 1, TFTHEIGHT -1, color, color, true);
	
}

/**************************************************************************/
/*!
    @brief  Returns display hardware orientation
*/
/**************************************************************************/
uint8_t SEMU_SSD1331::getOrientation(void){
	return _orientation;
}

/**************************************************************************/
/*!
    @brief  Check that coordinates are within physical display boundaries
	@param  x0 first x coordinate
	@param  y0 first 7 coordinate
	@param  x1 second x coordinate (optional, defaults to 0)
	@param  y1 second y coordinate (optional, defaults to 0)
	@param  x2 third x coordinate (optional, defaults to 0)
	@param  y2 third y coordinate (optional, defaults to 0)
	
*/
/**************************************************************************/
bool SEMU_SSD1331::inBounds(int16_t x0, int16_t y0, int16_t x1,
	int16_t y1, int16_t x2, int16_t y2) {

	return ((y0 >= _height) || (y1 >= _height) || (y2 >= _height)
		|| (x0 >= _width) || (x1 >= _width) || (x2 >= _width));

}

/**************************************************************************/
/*!
    @brief  Converts individual RGB values into single R5G6B5 pixel
	@param  r red value
	@param  g green value
	@param  r blue value
*/
/**************************************************************************/
uint16_t rgb_to_pixel(uint8_t r, uint8_t g, uint8_t b) {
	uint16_t pix = (r << 11) + (g << 5) + b;
	return pix;
}

/**************************************************************************/
/*!
    @brief  Converts Color structure into single R5G6B5 pixel
	@param  col Color structure
*/
/**************************************************************************/
uint16_t rgb_to_pixel(Color col) {
	uint16_t pix = (col.r << 11) + (col.g << 5) + col.b;
	return pix;
}

/**************************************************************************/
/*!
    @brief  Converts single R5G6B5 pixel into Color structure
	@param  pixel R5G6B5 pixel

*/
/**************************************************************************/
Color pixel_to_rgb(uint16_t pixel) {
	Color col;
	col.r = (pixel & 0b1111100000000000) >> 11;
	col.g = (pixel & 0b0000011111100000) >> 5;
	col.b = pixel & 0b000000000011111;
	return col;
}



