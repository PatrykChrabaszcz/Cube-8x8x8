/*
 * menu.c
 *
 *  Created on: Jun 7, 2013
 *  Author: Tomasz
 */

#include <avr/pgmspace.h>
#include <stdlib.h>
#include "menu.h"
#include "lcd.h"
#include "usart.h"
#include "snake.h"
//-----------------------------------------------------------------------
//Initialization
currentMenu = &main;
menuEvent = E_IDLE;
mSpeedGame = MEDIUM_SPEED;
//-----------------------------------------------------------------------
//text of menus
const prog_char mainText =		"MENU";
const prog_char menu1Text =		"Snake Game";
const prog_char menu11Text =		"Play Game";
const prog_char menu12Text =		"Level";
const prog_char menu121Text =		"Easy";
const prog_char menu122Text =		"Medium";
const prog_char menu123Text =		"Hard";
const prog_char menu124Text =		"Insane";
const prog_char menu13Text =		"Highscores";
const prog_char menu2Text =		"Visualization";
const prog_char menu21Text = 		"Enable USART";
const prog_char menu3Text = 		"Credits";
const prog_char Empty = 		"                    ";
const prog_char Line = 			"=====================";
const prog_char Arrow = 		"->";
//-----------------------------------------------------------------------
//generate the links in menu
main = { mainText, { &menu1, &menu2, &menu3 }, 3, 0, NULL, NULL };
	menu1 = { menu1Text, { &menu11, &menu12, &menu13 }, 3, 0, &main, NULL };
		menu11 = { menu11Text, { NULL }, 0, 0, &menu1, menuPlayGame };
		menu12 = { menu12Text, { &menu121, &menu122, &menu123, &menu124 }, 4, 0, &menu1, NULL };
			menu121 = { menu121Text, { NULL }, 0, 0, &menu12, menuSetEasy };
			menu122 = { menu122Text, { NULL }, 0, 0, &menu12, menuSetMedium };
			menu123 = { menu123Text, { NULL }, 0, 0, &menu12, menuSetHard };
			menu124 = { menu124Text, { NULL }, 0, 0, &menu12, menuSetInsane };
		menu13 = { menu13Text, { NULL }, 0, 0, &menu1, menuHighscores };
	menu2 = { menu2Text, { &menu21 }, 1, 0, &main, NULL };
		menu21 = { menu21Text, { NULL }, 0, 0, &menu2, menuEnableUsart };
	menu3 = { menu3Text, { NULL }, 0, 0, &main, menuCredits };
//-----------------------------------------------------------------------
void menuPlayGame(void)
{
	usartOff();
	snakeGame( mSpeedGame );
	menuBackTo( &main );
	changeMenu();
}
void menuSetEasy(void)  { mSpeedGame = EASY_SPEED; menuEvent = E_BACK; changeMenu(); }
void menuSetMedium(void) { mSpeedGame = MEDIUM_SPEED; menuEvent = E_BACK; changeMenu(); }
void menuSetHard(void) { mSpeedGame = HARD_SPEED; menuEvent = E_BACK; changeMenu(); }
void menuSetInsane(void) { mSpeedGame = INSANE_SPEED; menuEvent = E_BACK; changeMenu(); }
void menuHighscores(void)
{
	strGoTo(0,0);
	lcdStr("=====HIGHSCORES=====");
	strGoTo(1,1);
	lcdStr("1. "); lcdStr(bestPlayer1);
	strGoTo(2,1);
	lcdStr("2. "); lcdStr(bestPlayer2);
	strGoTo(3,1);
	lcdStr("3. "); lcdStr(bestPlayer3);
}
void menuEnableUsart(void) { usartOn(); menuEvent = E_BACK; changeMenu(); }
void menuCredits(void)
{

}
//-----------------------------------------------------------------------
void changeMenu()
{
    switch (menuEvent)
    {
        case E_UP:      menuUp(currentMenu); break;
        case E_DOWN:    menuDown(currentMenu); break;
        case E_ENTER:   menuEnter(currentMenu); break;
        case E_BACK:    menuBack(currentMenu); break;
    }
    drawMenu();
    if(currentMenu->callback)
        currentMenu->callback();
    menuEvent = E_IDLE;
}
//-----------------------------------------------------------------------
void drawMenu()
{
	for (uint8_t i = 0; i < LCD_Y; i++)
		lcdBuff[i] = Empty;
	char buff[2];

	lcdBuff[0] = Line;
	strPut(lcdBuff[0], currentMenu->text, (uint8_t)(LCD_X-strlen(currentMenu->text))/2);

	// Write first line of submenu to lcd buffer
	if(currentMenu->currentSubmenu > 0)
	{
		strPut(lcdBuff[1], itoa(currentMenu->currentSubmenu-1, buff, 10), 3);
		strPut(lcdBuff[1], ". ", 4+(uint8_t)(currentMenu->submenuSize/10));
		strPut(lcdBuff[1], currentMenu->submenu[currentMenu->currentSubmenu-1], 6+(uint8_t)(currentMenu->submenuSize/10));
	}
	// Write second line of submenu to lcd buffer (current submenu)
	strPut(lcdBuff[2], itoa(currentMenu->currentSubmenu, buff, 10), 3);
	strPut(lcdBuff[2], ". ", 4+(uint8_t)(currentMenu->submenuSize/10));
	strPut(lcdBuff[2], currentMenu->submenu[currentMenu->currentSubmenu], 6+(uint8_t)(currentMenu->submenuSize/10));
	// Write third line of submenu to lcd buffer
	if(currentMenu->currentSubmenu < currentMenu->submenuSize)
	{
		strPut(lcdBuff[3], itoa(currentMenu->currentSubmenu+1, buff, 10), 3);
		strPut(lcdBuff[3], ". ", 4+(uint8_t)(currentMenu->submenuSize/10));
		strPut(lcdBuff[3], currentMenu->submenu[currentMenu->currentSubmenu+1], 6+(uint8_t)(currentMenu->submenuSize/10));
	}
	// Write arrow on current submenu to lcd buffer
	strPut(lcdBuff[2], Arrow, 0);

	// Draw lcd buffer on lcd
	for (uint8_t i = 0; i < LCD_Y; i++)
	{
		lcdGoTo(i,0);
		lcdStrPgm(lcdBuff[i]);
	}
}
//-----------------------------------------------------------------------
char * strPut(char * strTo, const char * strFrom, uint8_t pos)
{
	uint8_t strToLen = strlen(strTo);
	if (pos < strToLen)
	{
		for (uint8_t i = pos; (i < strToLen) && (strFrom[i-pos] != '\0'); i++)
			strTo[i] = strFrom[i-pos];
	}
	return strTo;
}
//-----------------------------------------------------------------------
