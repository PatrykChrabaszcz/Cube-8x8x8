/*
 * usart.c
 *
 *  Created on: Jul 21, 2013
 *  Author: Patryk Chrabaszcz
 */

#include "usart.h"

// UART Recive Interrupt

ISR(USART_RXC_vect)
{
	//Constantly updates Cube (row by row)
	uint8_t temp = UDR;
	Cube[usartPlanesCounter][rowCounter]=temp;

	if(++rowCounter==8)
	{
		rowCounter=0;
		if(++usartPlanesCounter==8)
			usartPlanesCounter=0;
	}
}

// USART Initialization

void initUsart()
{
	// Set baud rate
	UBRRH = (UBRRVAL>>8);
	UBRRL = UBRRVAL;
	// Enable receiver and transmitter
	UCSRB = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
	// Set frame format: 8data, 2stop bit
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
}

void usartOff()
{
	UCSRB=0x00;
}
void usartOn()
{
	UCSRB=(1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
}

//USART Transmit 8bit data
void usartTransmit( unsigned char data )
{
	// Wait for empty transmit buffer
	while ( !( UCSRA & (1<<UDRE)) )
		;
	// Put data into buffer, sends the data
	UDR = data;
}
unsigned char usartReceive( void )
{
	// Wait for data to be received
	while ( !(UCSRA & (1<<RXC)) )
		;
	// Get and return received data from buffer
	return UDR;
}

int usartMode()
{
	usartTransmit(0b01010101);
	while(1)
	{
		if(lastKey==SELECT_KEY)
		{
			usartOff();
			return 0;
		}
	}
	return 0;
}
