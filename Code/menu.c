/*
 * menu.c
 *
 *  Created on: 2013-05-17
 *      Author: Tomasz
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "menu.h"
#include "lcd.h"


const char T00[] PROGMEM  = {"**      MENU      **"};
const char T01[] PROGMEM  = {"   1. SNAKE GAME"};
const char T01E[] PROGMEM  = {"-> 1. SNAKE GAME"};
const char T02[] PROGMEM  = {"   2. VISUALISATIONS"};
const char T03[] PROGMEM  = {"   3. CREDITS"};

const char T10[] PROGMEM  = {"**   SANKE GAME   **"};
const char T11[] PROGMEM  = {"   1. PLAY GAME"};
const char T12[] PROGMEM  = {"   2. SETTINGS"};
const char T13[] PROGMEM  = {"   3. HIGHSCORES"};

const char T20[] PROGMEM  = {"** VISUALISATIONS **"};
const char T21[] PROGMEM  = {"   1. V1"};
const char T22[] PROGMEM  = {"   2. V2"};
const char T23[] PROGMEM  = {"   3. V3"};

const char T30[] PROGMEM  = {"**     CREDITS    **"};
const char T31[] PROGMEM  = {"MALENDA M."};
const char T32[] PROGMEM  = {"CHRABASZCZ P."};
const char T33[] PROGMEM  = {"LAWRYNOWICZ R."};
const char T34[] PROGMEM  = {"GASIOR T."};
const char T3X[] PROGMEM  = {"################"};

const char T120[] PROGMEM  = {"**   SETTINGS    **"};
const char T121[] PROGMEM  = {"   1.  EASY        "};
const char T122[] PROGMEM  = {"   2.  MEDIUM      "};
const char T123[] PROGMEM  = {"   3.  HARD        "};
const char T124[] PROGMEM  = {"   4.  INSANE      "};

const char T130[] PROGMEM  = {"**   HIGHSCORES   **"};
const char T131[] PROGMEM  = {"   1.  0           "};
const char T132[] PROGMEM  = {"   2.  0           "};
const char T133[] PROGMEM  = {"   3.  0           "};

const char empty[] PROGMEM  = {"                    "};
const char arrow[] PROGMEM  = {"->"};
const char kratki[] PROGMEM = {"##"};
const char puste_kratki[] PROGMEM = {"  "};

volatile unsigned char current_menu = 0;
volatile unsigned char menu_event = E_IDDLE;


void change_led(void)
{
	PORTD ^= (1<<PD7);
}
void snake_game(void)
{
	lcd_cls();
	lcd_locate(1,5);
	lcd_str_P("SNAKE GAME ...");
}
void credits_display(void)
{
	lcd_cls();
	for(uint8_t i=1; i<18; i++)
	{
		_delay_ms(300);
		if (i==1)
		{
			lcd_locate(4-i,3);
			lcd_str_P(T31);
		}
		else if(i>1 && i<5)
		{
			lcd_locate(5-i,3);
			lcd_str_P(empty);
			lcd_locate(4-i,3);
			lcd_str_P(T31);
		}
		else if (i==5)
		{
			lcd_locate(5-i,3);
			lcd_str_P(empty);
			lcd_locate(8-i,3);
			lcd_str_P(T32);
		}
		else if(i>5 && i<9)
		{
			lcd_locate(9-i,3);
			lcd_str_P(empty);
			lcd_locate(8-i,3);
			lcd_str_P(T32);
		}
		else if (i==9)
		{
			lcd_locate(9-i,3);
			lcd_str_P(empty);
			lcd_locate(12-i,3);
			lcd_str_P(T33);
		}
		else if(i>9 && i<13)
		{
			lcd_locate(13-i,3);
			lcd_str_P(empty);
			lcd_locate(12-i,3);
			lcd_str_P(T33);
		}
		else if (i==13)
		{
			lcd_locate(13-i,3);
			lcd_str_P(empty);
			lcd_locate(16-i,3);
			lcd_str_P(T34);
		}
		else if(i>13 && i<17)
		{
			lcd_locate(17-i,3);
			lcd_str_P(empty);
			lcd_locate(16-i,3);
			lcd_str_P(T34);
		}
	}
	lcd_cls();
	menu_event = E_BACK;
}
typedef struct {
	char state[5];
	void (*callback)(void);
	const char *first_line;
	const char *second_line;
	const char *third_line;
	const char *forth_line;
} menu_item;

const menu_item MENU [] ={
	// Main Menu
	{{0,0,1,3,0}, 0, T00, empty, T01E, T02},
	{{1,0,2,6,1}, 0, T00, T01, T02, T03},
	{{2,1,2,9,2}, 0, T00, T02, T03, empty},
	// Snake Game Menu
	{{3,3,4,10,0}, 0, T10, empty, T11, T12},
	{{4,3,5,11,0}, 0, T10, T11, T12, T13},
	{{5,4,5,15,0}, 0, T10, T12, T13, empty},
	// Visualization Menu
	{{6,6,7,6,1}, 0/*V1 funkcja*/, T20, empty, T21, T22},
	{{7,6,8,7,1}, 0/*V2 funkcja*/, T20, T21, T22, T23},
	{{8,7,8,8,1}, 0/*V3 funkcja*/, T20, T22, T23, empty},
	// Credits Menu
	{{9,0,0,0,2}, credits_display, 0,0,0,0},
	// Play Game - Snake Game Menu
	{{10,10,10,10,3}, snake_game, 0,0,0,0},
	// Settings - Snake Game Menu
	{{11,11,12,11,4}, 0, T120, empty, T121, T122},
	{{12,11,13,12,4}, 0, T120, T121, T122, T123},
	{{13,12,14,13,4}, 0, T120, T122, T123, T124},
	{{14,13,14,14,4}, 0, T120, T123, T124, empty},
	// HighScores - Snake Game Menu
	{{15,15,16,15,5}, 0, T130, empty, T131, T132},
	{{16,15,17,16,5}, 0, T130, T131, T132, T133},
	{{17,16,17,17,5}, 0, T130, T132, T133, empty},

};

void change_menu(void)
{
	current_menu = MENU[current_menu].state[menu_event];
	lcd_cls();
	lcd_locate(0,0);
	lcd_str_P((const char*)MENU[current_menu].first_line);
	lcd_locate(1,0);
	lcd_str_P((const char*)MENU[current_menu].second_line);
	lcd_locate(2,0);
	lcd_str_P((const char*)MENU[current_menu].third_line);
	lcd_locate(3,0);
	lcd_str_P((const char*)MENU[current_menu].forth_line);
	lcd_locate(2,0);
	lcd_str_P(arrow);

	if (MENU[current_menu].callback)
	{
		MENU[current_menu].callback();
	}
	menu_event = E_IDDLE;
}






