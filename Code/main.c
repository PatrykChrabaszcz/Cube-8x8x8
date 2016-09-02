/*
 * main.c
 *
 *  Created on: Jul 21, 2013
 *  Author: Patryk Chrabaszcz
 */
#include "snake.h"
#include "config.h"
#include "usart.h"
#include "lcd.h"
#include "menu.h"


int main ()
{
	init();
	lcdInit();
	ir_init();
	initUsart();
	usartOff();
	lastKey = NONE_KEY;
	while(1)
	{
		for (uint8_t i = 0; i < 10; i++)
		{
			if (lastKey != getKey()) lastKey = getKey();
			_delay_ms(10);
		}
		switch (lastKey)
		{
			case NONE_KEY: menu_event = E_IDDLE; break;
			case UP_KEY: menu_event = E_UP; break;
			case DOWN_KEY: menu_event = E_DOWN; break;
			case SELECT_KEY: menu_event = E_OK; break;
			case START_KEY: menu_event = E_BACK; break;
		}
		change_menu();
	}
	return 0;
}
