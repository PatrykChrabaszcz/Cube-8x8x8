/*
 * menu.h
 *
 *  Created on: 2013-05-17
 *      Author: Tomasz
 */

#ifndef MENU_H_
#define MENU_H_

volatile unsigned char current_menu;
volatile unsigned char menu_event;

// Event Macros
#define E_IDDLE 0
#define E_UP 1
#define E_DOWN 2
#define E_OK 3
#define E_BACK 4

void change_menu();
void credits_display(void);
void snake_game(void);
void change_led(void);

#endif /* MENU_H_ */
