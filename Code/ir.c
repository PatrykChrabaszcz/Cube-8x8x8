/*
 * ir.c
 *
 *  Created on: Jun 7, 2013
 *  Author: Tomasz
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "ir.h"

volatile uint8_t address;		// Address RC5
volatile uint8_t command;		// Command RC5
volatile uint8_t toggle_bit;		// Bit TOGGLE

volatile uint8_t Ir_key_press_flag;

volatile uint8_t rc5cnt;

void ir_init()
{
	IR_DDR &= ~IR_IN;			// Set as Input
	IR_PORT |= IR_IN;			// Pullup to VCC
	#if TIMER1_PRESCALER == 1
		TCCR1B |= (1<<CS10);         	// Timer1 / 1
	#endif
	#if TIMER1_PRESCALER == 8
		TCCR1B |= (1<<CS11);         	// Timer1 / 8
	#endif
	#if TIMER1_PRESCALER == 64
	  	TCCR1B |= (1<<CS11)|(1<<CS10); 	// Timer1 / 64
	#endif
	#if TIMER1_PRESCALER == 256
		TCCR1B |= (1<<CS12);         	// Timer1 / 256
	#endif

	TCCR1B &= ~(1<<ICES1);      		// Falling edge on ICP
	rc5cnt = 0;				

	TIMSK |= (1<<TICIE1);       	// ICP interrupt
	Ir_key_press_flag = 0;		// Remote control flag reset
}

// ICP1 Handler
ISR(TIMER1_CAPT_vect)
{
	#define FRAME_RESTART 0
	#define FRAME_OK 1
	#define FRAME_END 2
	#define FRAME_ERROR 3

	static uint16_t LastCapture;
	uint16_t PulseWidth;
	static uint8_t IrPulseCount;
	static uint16_t IrData;
	static uint8_t frame_status;

	PulseWidth = ICR1 - LastCapture;
	LastCapture = ICR1;

	TCCR1B ^= (1<<ICES1);	// Change firing edge 

	if (PulseWidth > MAX_BIT)
	{
		rc5cnt = 0;
	}

	if (rc5cnt > 0)
	{
		frame_status = FRAME_OK;
	}

	if (rc5cnt == 0)
	{
		IrData = 0;
		IrPulseCount = 0;
		TCCR1B |= (1<<ICES1);
		rc5cnt++;
		frame_status = FRAME_END;
	}

	if (frame_status == FRAME_OK)
	{
		if(PulseWidth<MIN_HALF_BIT)
		{
			frame_status = FRAME_RESTART;
		}
		if( PulseWidth > MAX_BIT )
		{
			frame_status = FRAME_RESTART;
		}

		if (frame_status == FRAME_OK)
		{
			if (PulseWidth > MAX_HALF_BIT)
			{
				rc5cnt++;
			}

			if (rc5cnt > 1)
			{
				if ( (rc5cnt % 2) == 0 )
				{
					IrData = IrData << 1;
					if((TCCR1B & (1<<ICES1)))
					{
						IrData |= 0x0001;
					}
					IrPulseCount++;

					if (IrPulseCount > 12)
					{
						if (Ir_key_press_flag == 0)
						{
							command = IrData & 0b0000000000111111;
							address = (IrData & 0b0000011111000000) >> 6;
							toggle_bit = (IrData & 0b0000100000000000) >> 11;
						}

						frame_status = FRAME_RESTART;
						Ir_key_press_flag = 1;
					}
				}
			}
			rc5cnt++;
		}
	}

	if (frame_status == FRAME_RESTART)
	{
		rc5cnt = 0;
		TCCR1B &= ~(1<<ICES1);
	}
}
