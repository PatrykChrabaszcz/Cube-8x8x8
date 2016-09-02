#include "snake.h"
#include "config.h"
//#include "usart.h"
//#include <avr/io.h>
//#include <util/delay.h>
//#include <avr/interrupt.h>
//#include <avr/pgmspace.h>


int main ()
{
	init();
	lcd_init();
	ir_init();
	lcd_cls();
	change_menu();

	DDRD |= (1<<PD7);
	PORTD |= (1<<PD7);

	init();

	while(1)
	{
		// Part to play the game
		// Function has internal while loop
		snakeGame(150);


		// Part to test USART 
		// usartTransmit(usartReceive());
		//_delay_ms(100);
		
		if(Ir_key_press_flag)
		{
			if (!address && prog_up==command)
			{
				_delay_ms(300);
				menu_event = E_UP;
			}
			if( !address && prog_down==command)
			{
				_delay_ms(300);
				menu_event = E_DOWN;
			}
			if (!address && cursor_left==command)
			{
				_delay_ms(300);
				menu_event = E_BACK;
			}
			if (!address && OK==command)
			{
				_delay_ms(300);
				menu_event = E_OK;
			}

			Ir_key_press_flag=0;
			command=0xff;
			address=0xff;
		}
	}
}





