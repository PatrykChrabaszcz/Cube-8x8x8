/*
 * LCD.h
 *
 *  Created on: 2013-04-11
 *      Author: Tomasz
 */

#ifndef LCD_H_
#define LCD_H_

//-----------------------------------------------------------------------------------------------------------
// *** LCD header File ***
//
// - 4 bit data transfer
// - Own pins configuration
// - Own RW pin enable/disable configuration
// - Own LCD size configuration
//-----------------------------------------------------------------------------------------------------------
#ifndef LCDTEST_H_
#define LCDTEST_H_
//----------------------------------------------------------------------------------------
//
//		Configurations
//
//----------------------------------------------------------------------------------------
// LCD size (X - rows, Y - columns)
#define LCD_Y 4
#define LCD_X 16

// RW pin configuration [0 - disabled (connected to GND), 1 - enabled (connected to uC)]
#define USE_RW 1

// Pins configuration
#define LCD_D7_PORT PORTA
#define LCD_D7 		PA6
#define LCD_D7_DDR 	DDRA
#define LCD_D7_PIN 	PINA

#define LCD_D6_PORT PORTA
#define LCD_D6 		PA5
#define LCD_D6_DDR 	DDRA
#define LCD_D6_PIN 	PINA

#define LCD_D5_PORT PORTA
#define LCD_D5 		PA4
#define LCD_D5_DDR 	DDRA
#define LCD_D5_PIN 	PINA

#define LCD_D4_PORT PORTA
#define LCD_D4		PA3
#define LCD_D4_DDR 	DDRA
#define LCD_D4_PIN 	PINA

#define LCD_RS_PORT PORTA
#define LCD_RS 		PA0
#define LCD_RS_DDR 	DDRA
#define LCD_RS_PIN 	PINA

#define LCD_RW_PORT PORTA
#define LCD_RW 		PA1
#define LCD_RW_DDR 	DDRA
#define LCD_RW_PIN 	PINA

#define LCD_E_PORT  PORTA
#define LCD_E 		PA2
#define LCD_E_DDR 	DDRA
#define LCD_E_PIN 	PINA

//---------------------------------------------------------------


//----------------------------------------------------------------------------------------
//
//		Commands configuration
//
//		1 - enable  0 - disable
//----------------------------------------------------------------------------------------

#define USE_LCD_LOCATE	1			// enable setting cursor position

#define USE_LCD_CHAR 	1			// enable sending 1 char as a function argument

#define USE_LCD_STR_P 	1			// enable sending string from FLASH
#define USE_LCD_STR_E 	1			// enable sending string from EEPROM

#define USE_LCD_INT 	1			// enable displaying decimal number
#define USE_LCD_HEX 	0			// enable displaying hexadecimal number

#define USE_LCD_DEFCHAR		1		// enable sending 1 char from RAM
#define USE_LCD_DEFCHAR_P 	1		// enable sending 1 char from FLASH
#define USE_LCD_DEFCHAR_E 	1		// enable sending 1 char from EEPROM

#define USE_LCD_CURSOR_ON 		0	// enable on/off cursor
#define USE_LCD_CURSOR_BLINK 	0	// enable on/off blinking cursor
#define USE_LCD_CURSOR_HOME 	0	// enable setting cursor at home position

//----------------------------------------------------------------------------------------



// Layers' addresses (DDRAM)(depends on LCD size)
#if ( (LCD_Y == 4) && (LCD_X == 16) )
#define LCD_LINE1 0x00
#define LCD_LINE2 0x28
#define LCD_LINE3 0x14
#define LCD_LINE4 0x54
#endif
#if ( (LCD_Y == 4) && (LCD_X == 20) )
#define LCD_LINE1 0x00
#define LCD_LINE2 0x40
#define LCD_LINE3 0x14
#define LCD_LINE4 0x54
#endif

#if(LCD_Y == 2)
#define LCD_LINE1 0x00
#define LCD_LINE2 0x40
#define LCD_LINE3 0x10
#define LCD_LINE4 0x50
#endif



// Komendy steruj¹ce
#define LCDC_CLS					0x01
#define LCDC_HOME					0x02
#define LCDC_ENTRY					0x04
#define LCDC_ENTRYR					0x02
#define LCDC_ENTRYL					0
#define LCDC_MOVE					0x01
#define LCDC_ONOFF					0x08
#define LCDC_DISPLAYON				0x04
#define LCDC_CURSORON				0x02
#define LCDC_CURSOROFF				0
#define LCDC_BLINKON				0x01
#define LCDC_SHIFT					0x10
#define LCDC_SHIFTDISP				0x08
#define LCDC_SHIFTR					0x04
#define LCDC_SHIFTL					0
#define LCDC_FUNC					0x20
#define LCDC_FUNC8B					0x10
#define LCDC_FUNC4B					0
#define LCDC_FUNC2L					0x08
#define LCDC_FUNC1L					0
#define LCDC_FUNC5x10				0x04
#define LCDC_FUNC5x7				0
#define LCDC_SET_CGRAM				0x40
#define LCDC_SET_DDRAM				0x80


// functions
void lcd_init(void);
void lcd_cls(void);
void lcd_str(char * str);
void lcd_locate(uint8_t y, uint8_t x);
void lcd_char(char c);
void lcd_str_P(const char * str);
void lcd_str_E(char * str);
void lcd_int(int val);
void lcd_hex(int val);
void lcd_defchar(uint8_t nr, uint8_t *def_char);
void lcd_defchar_P(uint8_t nr, uint8_t *def_char);
void lcd_defchar_E(uint8_t nr, uint8_t *def_char);
void lcd_home(void);
void lcd_cursor_on(void);
void lcd_cursor_off(void);
void lcd_blink_on(void);
void lcd_blink_off(void);


#endif /* LCD_H_ */

#endif /* LCD_H_ */
