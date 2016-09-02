/*
 * usart.c
 *
 *  Created on: Jun 7, 2013
 *      Author: Patryk Chrabaszcz
 */

#include "usart.h"

//UART Recive Interrupt
ISR(USART_RXC_vect)
{
	//Temp for storing recived data
	uint8_t temp;
	//Constantly updates Cube (row by row)
	temp=UDR;
	Cube[usartPlanesCounter][rowCounter]=temp;

	if(++rowCounter==8)
	{
		rowCounter=0;
		if(++usartPlanesCounter==8)
			usartPlanesCounter=0;
	}

}

//USART Initialization
void USART_Init( unsigned int baud )
{
	// Set baud rate
	UBRRH = (unsigned char)(baud>>8);
	UBRRL = (unsigned char)baud;

	// Enable receiver and transmitter
	UCSRB = (1<<RXEN)|(1<<TXEN);

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
	usartInit();
	usartTransmit(0b01010101);
	while(1)
	{
		if(lastKey==SELECT_KEY)
		{
			usartOff();
			return 0;
		}
	}
}


void _init_usart( )
{
	UBRRH=0x00;
	UBRRL = 6; 	// Set the baud rate
	UCSRB = 0x18; // Enable transmitter and reciever
	UCSRC = 0x86; // Set as 8 bit data, No parity bit, 1 stop bit.
}


