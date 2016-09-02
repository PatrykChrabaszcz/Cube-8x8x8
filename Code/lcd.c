/*
 * lcd.c
 *
 *  Created on: Jun 7, 2013
 *  Author: Tomasz
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "lcd.h"

// Function to set pins D4 - D7 as output
static inline void lcdDataDirOut(void)
{
	LCD_D4_DDR |= _BV(LCD_D4);
	LCD_D5_DDR |= _BV(LCD_D5);
	LCD_D6_DDR |= _BV(LCD_D6);
	LCD_D7_DDR |= _BV(LCD_D7);
}

// Function to set pins D4 - D7 as input
static inline void lcdDataDirIn(void)
{
	LCD_D4_DDR &= ~_BV(LCD_D4);
	LCD_D5_DDR &= ~_BV(LCD_D5);
	LCD_D6_DDR &= ~_BV(LCD_D6);
	LCD_D7_DDR &= ~_BV(LCD_D7);
}

// function to set D4 - D7 lower half of the byte
static inline void lcdWriteHalf(uint8_t data)
{
	if (data & _BV(0)) SET_D4; else CLR_D4;
	if (data & _BV(1)) SET_D5; else CLR_D5;
	if (data & _BV(2)) SET_D6; else CLR_D6;
	if (data & _BV(3)) SET_D7; else CLR_D7;
}

// Function to read the D4 - D7 state
static inline uint8_t lcdReadHalf(void)
{
	uint8_t result = 0;
	if(LCD_D4_PIN & _BV(LCD_D4)) result |= _BV(0);
	if(LCD_D5_PIN & _BV(LCD_D5)) result |= _BV(1);
	if(LCD_D6_PIN & _BV(LCD_D6)) result |= _BV(2);
	if(LCD_D7_PIN & _BV(LCD_D7)) result |= _BV(3);
	return result;
}

// Function to read a byte from LCD
static uint8_t lcdReadByte(void)
{
	uint8_t result = 0;
	lcdDataDirIn();

	SET_RW;
	SET_E;
	result = (lcdReadHalf() << 4);		// Read Part
	CLR_E;

	SET_E;
	result |= lcdReadHalf();		// Read other part
	CLR_E;

	return result;
}

// Busy flag read
static inline uint8_t lcdCheckBF(void)
{
	CLR_RS;
	return lcdReadByte();
}

// Function writing a byte to LCD
static void lcdWriteByte(uint8_t data)
{
	lcdDataDirOut();

	CLR_RW;

	SET_E;
	lcdWriteHalf(data >> 4);			// Writing part
	CLR_E;

	SET_E;
	lcdWriteHalf(data);				// Write other part
	CLR_E;

	while( (lcdCheckBF() & _BV(7)) ) {}		// Check Busy flag
}

// Send a command to LCD
static inline void lcdWriteCommand(uint8_t cmd)
{
	CLR_RS;
	lcdWriteByte(cmd);
}

//Send data to LCD
static inline void lcdWriteData(uint8_t data)
{
	SET_RS;
	lcdWriteByte(data);
}

// Initialize LCD
void lcdInit(void)
{
	// Pin configuration
	lcdDataDirOut();
	LCD_E_DDR  |= _BV(LCD_E);
	LCD_RS_DDR |= _BV(LCD_RS);
	LCD_RW_DDR |= _BV(LCD_RW);

	SET_E;
	SET_RS;
	SET_RW;

	_delay_ms(15); // Time required for voltage stabilization 

	CLR_E;
	CLR_RS;
	CLR_RW;

	SET_E;
	lcdWriteHalf(0x03);	// 8bit mode
	CLR_E;
	_delay_ms(4.1);

	SET_E;
	lcdWriteHalf(0x03);	// Still 8bit mode
	CLR_E;
	_delay_us(100);

	SET_E;
	lcdWriteHalf(0x03);	// Still 8bit mode
	CLR_E;
	_delay_us(100);

	SET_E;
	lcdWriteHalf(0x02);// 4bit mode
	CLR_E;
	_delay_us(100);

	lcdWriteCommand( LCD_FUNCTION_SET | LCD_4_BIT | LCD_TWO_LINE | LCD_FONT5x7);
	lcdWriteCommand( LCD_DISPLAY_ONOFF | LCD_CURSOR_OFF );
	lcdWriteCommand( LCD_DISPLAY_ONOFF | LCD_DISPLAY_ON );
	lcdWriteCommand( LCD_ENTRY_MODE | LCD_EM_INCREMENT );

	lcdCls(); // Clear LCD
}

// Screen cleaning Function
void lcdCls(void)
{
	lcdWriteCommand( LCD_CLEAR );
	_delay_ms(4.9);
}

// Function to set cursor on the screen 
void lcdGoTo(uint8_t y, uint8_t x)
{
	switch(y)
	{
		case 0: y = LCD_LINE1; break;
		case 1: y = LCD_LINE2; break;
		case 2: y = LCD_LINE3; break;
		case 3: y = LCD_LINE4; break;
	}
	lcdWriteCommand( (0x80 + y + x) );
}

// Function to write a text on LCD (RAM)
void lcdStr(const char * str)
{
	char c;
	while ( (c=*(str++)) )
		lcdWriteData( ( c >= 0x80 && c <= 0x87 ) ? (c & 0x07) : c);
}

// Function to write a text on LCD (FLASH)
void lcdStrPgm(const char * str)
{
	char c;
	while ( (c=pgm_read_byte(str++)) )
		lcdWriteData( ( c >= 0x80 && c <= 0x87 ) ? (c & 0x07) : c);
}
