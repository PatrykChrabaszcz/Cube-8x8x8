/*
 * ir.h
 *
 *  Created on: Jun 7, 2013
 *  Author: Tomasz
 */

#ifndef IR_H_
#define IR_H_

#define IR_PORT PORTD
#define IR_PIN PD6
#define IR_DDR DDRD
#define IR_IN (1<<IR_PIN)

#define TIMER1_PRESCALER 8

#define ir_micro_s(num) ((num)*(F_CPU/1000000)/TIMER1_PRESCALER)

#define TOLERANCE 200
#define MIN_HALF_BIT 	ir_micro_s(889 - TOLERANCE)
#define MAX_HALF_BIT 	ir_micro_s(889 + TOLERANCE)
#define MAX_BIT ir_micro_s  ((889+889) + TOLERANCE)


// ------------------------------------------- Remote control Keys definition
#define teletext_red 55
#define teletext_green 54
#define teletext_yellow 50
#define teletext_blue 52
#define menu 36
#define button_ok 59
#define power_off 12
#define mute 13
#define vol_up 16
#define vol_down 17
#define prog_up 32
#define prog_down 33
#define cursor_up prog_up
#define cursor_down prog_down
#define cursor_left vol_down
#define cursor_right vol_up
#define OK button_ok
// ------------------------------------------- 

// Values for RC5
extern volatile uint8_t toggle_bit;		// Bit TOGGLE
extern volatile uint8_t address;		// Address
extern volatile uint8_t command;		// Command

// Flag 1 informs that key was received
extern volatile uint8_t Ir_key_press_flag;

void ir_init();

#endif /* IR_H_ */
