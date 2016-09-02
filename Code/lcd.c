/*
 * LCD.c
 *
 *  Created on: 2013-04-11
 *      Author: Tomasz
 */
//----------------------------------------------------------------------------------------
// *** LCD Implementation File ***
//
// - 4 bit data transfer
// - Own pins configuration
// - Own RW pin enable/disable configuration
// - Own LCD size configuration
//----------------------------------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <util/delay.h>

#include "lcd.h"

#define SET_RS 	LCD_RS_PORT |= (1<<LCD_RS)		// SET RS
#define CLR_RS 	LCD_RS_PORT &= ~(1<<LCD_RS)		// CLEAR RS

#define SET_RW 	LCD_RW_PORT |= (1<<LCD_RW)		// SET RW - read data from LCD
#define CLR_RW 	LCD_RW_PORT &= ~(1<<LCD_RW)		// CLEAR RW - write data to LCD

#define SET_E 	LCD_E_PORT |= (1<<LCD_E)		// SET E
#define CLR_E 	LCD_E_PORT &= ~(1<<LCD_E)		// CLEAR E

uint8_t check_BF(void);	//check busy flag function


//********************* FUNCTIONS *********************

//----------------------------------------------------------------------------------------
//
//								 Set D4...D7 pins as output
//
//----------------------------------------------------------------------------------------
static inline void data_dir_out(void)
{
	LCD_D7_DDR	|= (1<<LCD_D7);
	LCD_D6_DDR	|= (1<<LCD_D6);
	LCD_D5_DDR	|= (1<<LCD_D5);
	LCD_D4_DDR	|= (1<<LCD_D4);
}

//----------------------------------------------------------------------------------------
//
//								 Set D4...D7 pins as input
//
//----------------------------------------------------------------------------------------
static inline void data_dir_in(void)
{
	LCD_D7_DDR	&= ~(1<<LCD_D7);
	LCD_D6_DDR	&= ~(1<<LCD_D6);
	LCD_D5_DDR	&= ~(1<<LCD_D5);
	LCD_D4_DDR	&= ~(1<<LCD_D4);
}

//----------------------------------------------------------------------------------------
//
//							 Send half byte to LCD via D4...D7 pins
//
//----------------------------------------------------------------------------------------
static inline void lcd_sendHalf(uint8_t data)
{
	if (data&(1<<0)) LCD_D4_PORT |= (1<<LCD_D4); else LCD_D4_PORT &= ~(1<<LCD_D4);
	if (data&(1<<1)) LCD_D5_PORT |= (1<<LCD_D5); else LCD_D5_PORT &= ~(1<<LCD_D5);
	if (data&(1<<2)) LCD_D6_PORT |= (1<<LCD_D6); else LCD_D6_PORT &= ~(1<<LCD_D6);
	if (data&(1<<3)) LCD_D7_PORT |= (1<<LCD_D7); else LCD_D7_PORT &= ~(1<<LCD_D7);
}

#if USE_RW == 1
//----------------------------------------------------------------------------------------
//
//						 Recive half byte from LCD via D4...D7 pins
//
//----------------------------------------------------------------------------------------
static inline uint8_t lcd_readHalf(void)
{
	uint8_t result=0;

	if(LCD_D4_PIN &(1<<LCD_D4)) result |= (1<<0);
	if(LCD_D5_PIN &(1<<LCD_D5)) result |= (1<<1);
	if(LCD_D6_PIN &(1<<LCD_D6)) result |= (1<<2);
	if(LCD_D7_PIN &(1<<LCD_D7)) result |= (1<<3);

	return result;
}
#endif

//----------------------------------------------------------------------------------------
//
//						 Write byte to LCD via D4...D7 pins
//
//----------------------------------------------------------------------------------------
void _lcd_write_byte(unsigned char _data)
{
	data_dir_out();

#if USE_RW == 1
	CLR_RW;
#endif

	SET_E;
	lcd_sendHalf(_data >> 4);			// send elder byte's half 0b(0101)0101
	CLR_E;

	SET_E;
	lcd_sendHalf(_data);				// send second byte's half 0b0101(0101)
	CLR_E;

#if USE_RW == 1
	while( (check_BF() & (1<<7)) );
#else
	_delay_us(120);
#endif

}

#if USE_RW == 1
//----------------------------------------------------------------------------------------
//
//							 Read byte from LCD via D4...D7 pins
//
//----------------------------------------------------------------------------------------
uint8_t _lcd_read_byte(void)
{
	uint8_t result=0;
	data_dir_in();

	SET_RW;

	SET_E;
	result = (lcd_readHalf() << 4);		// read elder byte half 0b(0101)0101
	CLR_E;

	SET_E;
	result |= lcd_readHalf();			// read second byte half 0b0101(0101)
	CLR_E;

	return result;
}
#endif


#if USE_RW == 1
//----------------------------------------------------------------------------------------
//
//								 Check Busy Flag
//
//----------------------------------------------------------------------------------------
uint8_t check_BF(void)
{
	CLR_RS;
	return _lcd_read_byte();
}
#endif


//----------------------------------------------------------------------------------------
//
//								 Write commend to LCD
//
//----------------------------------------------------------------------------------------
void lcd_write_cmd(uint8_t cmd)
{
	CLR_RS;
	_lcd_write_byte(cmd);
}

//----------------------------------------------------------------------------------------
//
//								 Write data to LCD
//
//----------------------------------------------------------------------------------------
void lcd_write_data(uint8_t data)
{
	SET_RS;
	_lcd_write_byte(data);
}

#if USE_LCD_CHAR == 1
//----------------------------------------------------------------------------------------
//
//		 					Send 1 char as a function argument
//
//----------------------------------------------------------------------------------------
void lcd_char(char c)
{
	lcd_write_data( ( c>=0x80 && c<=0x87 ) ? (c & 0x07) : c);
}
#endif

//----------------------------------------------------------------------------------------
//
//		 					Send string from RAM
//
//----------------------------------------------------------------------------------------
void lcd_str(char * str)
{
	register char znak;
	while ( (znak=*(str++)) )
		lcd_write_data( ( znak>=0x80 && znak<=0x87 ) ? (znak & 0x07) : znak);
}

#if USE_LCD_STR_P == 1
//----------------------------------------------------------------------------------------
//
//							Send string from FLASH
//
//----------------------------------------------------------------------------------------
void lcd_str_P(const char * str)
{
	register char znak;
	while ( (znak=pgm_read_byte(str++)) )
		lcd_write_data( ( (znak>=0x80) && (znak<=0x87) ) ? (znak & 0x07) : znak);
}
#endif


#if USE_LCD_STR_E == 1
//----------------------------------------------------------------------------------------
//
//		 					Send string from EEPROM
//
//----------------------------------------------------------------------------------------
void lcd_str_E(char * str)
{
	register char znak;
	while(1)
	{
		znak=eeprom_read_byte( (uint8_t *)(str++) );
		if(!znak || znak==0xFF) break;
		else lcd_write_data( ( (znak>=0x80) && (znak<=0x87) ) ? (znak & 0x07) : znak);
	}
}
#endif


#if USE_LCD_INT == 1
//----------------------------------------------------------------------------------------
//
//		 Display decimal number
//
//----------------------------------------------------------------------------------------
void lcd_int(int val)
{
	char bufor[17];
	lcd_str( itoa(val, bufor, 10) );
}
#endif

#if USE_LCD_HEX == 1
//----------------------------------------------------------------------------------------
//
//		 Display hexadecimal number
//
//----------------------------------------------------------------------------------------
void lcd_hex(int val)
{
	char bufor[17];
	lcd_str( itoa(val, bufor, 16) );
}
#endif

#if USE_LCD_DEFCHAR == 1
//----------------------------------------------------------------------------------------
//
//		Define char
//
//		nr: 		- CGRAM memory code (0x80 to 0x87)
//		*def_char:	- 7 bytes array pointer
//
//----------------------------------------------------------------------------------------
void lcd_defchar(uint8_t nr, uint8_t *def_char)
{
	register uint8_t i,c;
	lcd_write_cmd( 64+((nr&0x07)*8) );
	for(i=0;i<8;i++)
	{
		c = *(def_char++);
		lcd_write_data(c);
	}
}
#endif

#if USE_LCD_DEFCHAR_P == 1
//----------------------------------------------------------------------------------------
//
//		Define char in FLASH memory
//
//		nr: 		- CGRAM memory code (0x80 to 0x87)
//		*def_char:	- 7 bytes array pointer
//
//----------------------------------------------------------------------------------------
void lcd_defchar_P(uint8_t nr, uint8_t *def_char)
{
	register uint8_t i,c;
	lcd_write_cmd( 64+((nr&0x07)*8) );
	for(i=0;i<8;i++)
	{
		c = pgm_read_byte(def_char++);
		lcd_write_data(c);
	}
}
#endif

#if USE_LCD_DEFCHAR_E == 1
//----------------------------------------------------------------------------------------
//
//		Define char in EEPROM memory
//
//		nr: 		- CGRAM memory code (0x80 to 0x87)
//		*def_char:	- 7 bytes array pointer
//
//----------------------------------------------------------------------------------------
void lcd_defchar_E(uint8_t nr, uint8_t *def_char)
{
	register uint8_t i,c;

	lcd_write_cmd( 64+((nr&0x07)*8) );
	for(i=0;i<8;i++)
	{
		c = eeprom_read_byte(def_char++);
		lcd_write_data(c);
	}
}
#endif


#if USE_LCD_LOCATE == 1
//----------------------------------------------------------------------------------------
//
//		Cursor position (Y - rows 0...3, X - coulmns 0...20
//
// 		Y = od 0 do 3
// 		X = od 0 do n
//
//----------------------------------------------------------------------------------------
void lcd_locate(uint8_t y, uint8_t x)
{
	switch(y)
	{
		case 0: y = LCD_LINE1; break;

#if (LCD_Y>1)
	    case 1: y = LCD_LINE2; break;
#endif
#if (LCD_Y>2)
    	case 2: y = LCD_LINE3; break;
#endif
#if (LCD_Y>3)
    	case 3: y = LCD_LINE4; break;
#endif
	}

	lcd_write_cmd( (0x80 + y + x) );
}
#endif


//----------------------------------------------------------------------------------------
//
//		Display clear
//
//----------------------------------------------------------------------------------------
void lcd_cls(void)
{
	lcd_write_cmd( LCDC_CLS );

	#if USE_RW == 0
		_delay_ms(4.9);
	#endif
}


#if USE_LCD_CURSOR_HOME == 1
//----------------------------------------------------------------------------------------
//
//		Locate cursor at Y-0, X-0
//
//----------------------------------------------------------------------------------------
void lcd_home(void)
{
	lcd_write_cmd( LCDC_CLS|LCDC_HOME );

	#if USE_RW == 0
		_delay_ms(4.9);
	#endif
}
#endif

#if USE_LCD_CURSOR_ON == 1
//----------------------------------------------------------------------------------------
//
//		Cursor enabled
//
//----------------------------------------------------------------------------------------
void lcd_cursor_on(void)
{
	lcd_write_cmd( LCDC_ONOFF|LCDC_DISPLAYON|LCDC_CURSORON);
}

//----------------------------------------------------------------------------------------
//
//		Curosr disabled
//
//----------------------------------------------------------------------------------------
void lcd_cursor_off(void)
{
	lcd_write_cmd( LCDC_ONOFF|LCDC_DISPLAYON);
}
#endif


#if USE_LCD_CURSOR_BLINK == 1
//----------------------------------------------------------------------------------------
//
//		Blink cursor enabled
//
//----------------------------------------------------------------------------------------
void lcd_blink_on(void)
{
	lcd_write_cmd( LCDC_ONOFF|LCDC_DISPLAYON|LCDC_CURSORON|LCDC_BLINKON);
}

//----------------------------------------------------------------------------------------
//
//		Blink cursor disabled
//
//----------------------------------------------------------------------------------------
void lcd_blink_off(void)
{
	lcd_write_cmd( LCDC_ONOFF|LCDC_DISPLAYON);
}
#endif




//----------------------------------------------------------------------------------------
//
//		 					 LCD initialization
//
//----------------------------------------------------------------------------------------
void lcd_init(void)
{
	data_dir_out();
	LCD_RS_DDR |= (1<<LCD_RS);
	LCD_E_DDR |= (1<<LCD_E);
	#if USE_RW == 1
		LCD_RW_DDR |= (1<<LCD_RW);
	#endif

	LCD_RS_PORT |= (1<<LCD_RS);
	LCD_E_PORT |= (1<<LCD_E);
	#if USE_RW == 1
		LCD_RW_PORT |= (1<<LCD_RW);
	#endif

	_delay_ms(15);
	LCD_E_PORT &= ~(1<<LCD_E);
	LCD_RS_PORT &= ~(1<<LCD_RS);
	#if USE_RW == 1
	LCD_RW_PORT &= ~(1<<LCD_RW);
	#endif

	// wired commends below, sth with general initialization 4bit data transfer
	// just skip them, those are okey
	SET_E;
	lcd_sendHalf(0x03);	// 8bit mode
	CLR_E;
	_delay_ms(4.1);

	SET_E;
	lcd_sendHalf(0x03);	// still 8bit mode
	CLR_E;
	_delay_us(100);

	SET_E;
	lcd_sendHalf(0x03);	// hold on, it's still 8bit mode
	CLR_E;
	_delay_us(100);

	SET_E;
	lcd_sendHalf(0x02);// 4bit mode
	CLR_E;
	_delay_us(100);

	// check_BF enabled
	// 4bit mode
	lcd_write_cmd( LCDC_FUNC|LCDC_FUNC4B|LCDC_FUNC2L|LCDC_FUNC5x7 );
	// cursor off
	lcd_write_cmd( LCDC_ONOFF|LCDC_CURSOROFF );
	// LCD on
	lcd_write_cmd( LCDC_ONOFF|LCDC_DISPLAYON );

	lcd_write_cmd( LCDC_ENTRY|LCDC_ENTRYR );

	// display clear
	lcd_cls();
}
