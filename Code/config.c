/*
 * config.c
 *
 *  Created on: Jul 21, 2013
 *  Author: Patryk Chrabaszcz
 */

#include "config.h"

// Pins, SPI, Timer initialization
void init()
{
	// Ports
	// MOSI, SCK as outputs (for SPI)
	DDRB |= (1<<PB5) | (1<<PB7);

	// Pins associated with 74hc595
	DDRB |= (1<<PB2) | (1<<PB4);
	REGISTERS_ENABLE();

	// Pad Pins as inputs
	DDRD=0x00;
	PORTD=0xff;
	DDRC=0x00;
	PORTC=0xff;

	// Peripherials
	// SPI settings
	SPCR |= (1<<SPE) | (1<<MSTR) | (1<<DORD); 	// SPI enable, SPI Master, MSB transmit first
	SPCR &= ~((1<<SPR0) | (1<<SPR1)); 		// SPI clock /4 prescaler
	SPSR |= (1<<SPI2X); 				// Double speed for SPI

	// Timer 0 settings
	TCCR0 |= (1<<CS01); 	// Normal mode, internal clock, /8 prescaler
	TIMSK |= (1<<TOIE0); 	// Timer overflow interrupt enable

	sei(); // Global interrupts enable
}

// Leds base functions
void setLedOn(uint8_t x, uint8_t y, uint8_t z)
{
	Cube[z][y] |= (1<<x);
}

void setLedOff(uint8_t x, uint8_t y, uint8_t z)
{
	Cube[z][y] &= ~(1<<x);
}

// Returns value of pressed key (Keys codes in config.h)
uint8_t getKey()
{
	if(START_KEY_F)
		return START_KEY;

	if(SELECT_KEY_F)
		return SELECT_KEY;

	if(LEFT_KEY_F)
			return LEFT_KEY;

	if(RIGHT_KEY_F)
		return RIGHT_KEY;

	if(UP_KEY_F)
		return UP_KEY;

	if(DOWN_KEY_F)
		return DOWN_KEY;

	if(UPT_KEY_F)
		return UPT_KEY;

	if(DOWNT_KEY_F)
		return DOWNT_KEY;

	return NONE_KEY;
}

// Timer 0 Interrupt
ISR(TIMER0_OVF_vect)
{
	// Reset Cube
	REGISTERS_DISABLE();
	REGISTERS_ENABLE();
	SET_DATA();

	// Sending plane[planes_counter]
	for(uint8_t i = 0; i < 8; i++)
	{
		SEND_DATA(Cube[planesCounter][7-i]);
	}

	// Setting on plane[planes_couner]
	SEND_DATA(1<<planesCounter);
	SET_DATA();

	if(++planesCounter == 8)
	{
		planesCounter=0;
	}

}

// Others
void delayMs(uint8_t i)
{
	for(int j = 0; j < i; j++)
		_delay_ms(1);
}
