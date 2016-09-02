/*
 * usart.h
 *
 *  Created on: Jul 21, 2013
 *  Author: Patryk Chrabaszcz
 */

#ifndef USART_H_
#define USART_H_

#include "config.h"

// Set desired baud rate
#define BAUDRATE 115200


// Calculate UBRR value
#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)

volatile int usartPlanesCounter;

volatile int rowCounter;

void usartOff();
void usartOn();

void usartTransmit( unsigned char data );
unsigned char usartReceive();

int usartMode();

void initUsart();

#endif
