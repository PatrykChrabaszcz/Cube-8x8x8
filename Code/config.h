/*
 * config.h
 *
 *  Created on: Jul 21, 2013
 *  Author: Patryk Chrabaszcz
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define TRUE 1
#define FALSE 0

// Keys codes
#define NONE_KEY 0x00
#define UP_KEY 0x01
#define DOWN_KEY 0x02
#define LEFT_KEY 0x03
#define RIGHT_KEY 0x04
#define UPT_KEY 0x05
#define DOWNT_KEY 0x06
#define START_KEY 0x07
#define SELECT_KEY 0x08

// SPI macros SPI: PB2 PB4 PB5 PB7
#define SEND_DATA(data) { SPDR = data; while (!(SPSR & (1<<SPIF))); }
#define SET_DATA() { PORTB &= ~(1<<PB2); PORTB |= (1<<PB2); }
#define REGISTERS_ENABLE() PORTB |= (1<<PB4);
#define REGISTERS_DISABLE() PORTB &= ~(1<<PB4);

// Keys checking macros returns  1 if key is ON
#define UP_KEY_F !(PIND & (1<<PD4))
#define DOWN_KEY_F !(PIND & (1<<PD3))
#define LEFT_KEY_F !(PIND & (1<<PD5))
#define RIGHT_KEY_F !(PINC & (1<<PC2))
#define UPT_KEY_F !(PIND & (1<<PD7))
#define DOWNT_KEY_F !(PINC & (1<<PC0))
#define START_KEY_F !(PIND & (1<<PD2))
#define SELECT_KEY_F !(PINC & (1<<PC1))

// Cube off macro
#define CUBE_OFF for (int i=0;i<8;i++) for(int j=0;j<8;j++) Cube[i][j]=0x00;

// Variables
uint8_t lastKey;		// Last pressed key
volatile uint8_t planesCounter; // Used in interrupt
uint8_t Cube[8][8];		// Table in which saved are actual Led settings
				// Cube[z][y] - coordinates
				// 0x00000001 - x coordinate
				// Example: Cube[0][0]=0x01 sets [0][0][0] ON
				// Example: Cube[2][1]=0b00010010 sets [1][1][2] and [4][1][2] ON
				// x,y,z are from <0,7>

//Base functions
void setLedOn(uint8_t , uint8_t, uint8_t );
void setLedOff(uint8_t , uint8_t, uint8_t );
void init();
uint8_t getKey();				//Reads key and returns data
void delayMs(uint8_t i);

#endif
