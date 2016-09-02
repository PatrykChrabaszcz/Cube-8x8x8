/*
 * usart.h
 *
 *  Created on: Jun 7, 2013
 *      Author: Patryk Chrabaszcz
 */

#include "config.h"


//Set desired baud rate
#define BAUDRATE 9600


//Calculate UBRR value
#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)

volatile int usartPlanesCounter;

volatile int rowCounter;

void usartInit();
void usartOff();
void usartOn();

void usartTransmit( unsigned char data );
unsigned char usartReceive();

int usartMode();
void _init_usart( );
void USART_Init( unsigned int baud );
