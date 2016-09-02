/*
 * lcd.h
 *
 *  Created on: Jun 7, 2013
 *  Author: Tomasz
 */

#ifndef LCD_H_
#define LCD_H_

//Size of lcd---------------------------------------------------------------
#define LCD_Y 4
#define LCD_X 20
//--------------------------------------------------------------------------

//Lines addresses (DDRAM)(depends on LCD size)------------------------------
#if ( (LCD_Y == 4) && (LCD_X == 16) )
#define LCD_LINE1 0x00
#define LCD_LINE2 0x40
#define LCD_LINE3 0x14
#define LCD_LINE4 0x54
#endif
#if ( (LCD_Y == 4) && (LCD_X == 20) )
#define LCD_LINE1 0x00
#define LCD_LINE2 0x40
#define LCD_LINE3 0x10
#define LCD_LINE4 0x50
#endif

#if(LCD_Y == 2)
#define LCD_LINE1 0x00
#define LCD_LINE2 0x40
#define LCD_LINE3 0x10
#define LCD_LINE4 0x50
#endif
//--------------------------------------------------------------------------

//HD44780 commands----------------------------------------------------------
#define LCD_CLEAR				0x01	// LCD clear, cursor on (0,0), Window switching mode clear, max 1.64ms

#define LCD_HOME				0x02	// Cursor on (0,0), max 1.64ms

#define LCD_ENTRY_MODE				0x04		
	#define LCD_EM_SHIFT_CURSOR		0		
	#define LCD_EM_SHIFT_DISPLAY		1			
	#define LCD_EM_DECREMENT		0		
	#define LCD_EM_INCREMENT		2		

#define LCD_DISPLAY_ONOFF			0x08	
	#define LCD_DISPLAY_OFF			0		
	#define LCD_DISPLAY_ON			4		
	#define LCD_CURSOR_OFF			0		
	#define LCD_CURSOR_ON			2		
	#define LCD_CURSOR_NOBLINK		0		
	#define LCD_CURSOR_BLINK		1		

#define LCD_DISPLAY_CURSOR_SHIFT		0x10
	#define LCD_SHIFT_CURSOR		0		
	#define LCD_SHIFT_DISPLAY		8		
	#define LCD_SHIFT_LEFT			0		
	#define LCD_SHIFT_RIGHT			4		

#define LCD_FUNCTION_SET			0x20	
	#define LCD_FONT5x7			0		
	#define LCD_FONT5x10			4		
	#define LCD_ONE_LINE			0		
	#define LCD_TWO_LINE			8		
	#define LCD_4_BIT			0		
	#define LCD_8_BIT			16		

#define LCD_CGRAM_SET				0x40	
#define LCD_DDRAM_SET				0x80	
//--------------------------------------------------------------------------

//Pins configuration (4 data line configuration)----------------------------
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
//--------------------------------------------------------------------------

//Macros to set '0' or '1'--------------------------------------------------
#define SET_D4 	LCD_D4_PORT |= (1<<LCD_D4)
#define CLR_D4 	LCD_D4_PORT &= ~(1<<LCD_D4)

#define SET_D5 	LCD_D5_PORT |= (1<<LCD_D5)
#define CLR_D5 	LCD_D5_PORT &= ~(1<<LCD_D5)

#define SET_D6 	LCD_D6_PORT |= (1<<LCD_D6)
#define CLR_D6 	LCD_D6_PORT &= ~(1<<LCD_D6)

#define SET_D7 	LCD_D7_PORT |= (1<<LCD_D7)
#define CLR_D7 	LCD_D7_PORT &= ~(1<<LCD_D7)

#define SET_RS 	LCD_RS_PORT |= (1<<LCD_RS)		// Set RS - commands
#define CLR_RS 	LCD_RS_PORT &= ~(1<<LCD_RS)		// Clear RS - data

#define SET_RW 	LCD_RW_PORT |= (1<<LCD_RW)		// Set RW - read data from LCD
#define CLR_RW 	LCD_RW_PORT &= ~(1<<LCD_RW)		// Clear RW - write data to LCD

#define SET_E 	LCD_E_PORT |= (1<<LCD_E)		// Set E - starts data read/write
#define CLR_E 	LCD_E_PORT &= ~(1<<LCD_E)		// Clear E  - starts data read/write
//--------------------------------------------------------------------------

//Functions-----------------------------------------------------------------
//static inline void lcdDataDirOut(void);
//static inline void lcdDataDirIn(void);
//static inline void lcdWriteHalf(uint8_t data);
//static inline uint8_t lcdReadHalf(void);
//static uint8_t lcdReadByte(void);
//static void lcdWriteByte(uint8_t data);
//static inline uint8_t lcdCheckBF(void);
//static inline void lcdWriteCommand(uint8_t cmd);
//static inline void lcdWriteData(uint8_t data);
void lcdInit(void);
void lcdCls(void);
void lcdGoTo(uint8_t y, uint8_t x);
void lcdStr(const char * str);
void lcdStrPgm(const char * str);
//---------------------------------------------------------------------------
#endif
