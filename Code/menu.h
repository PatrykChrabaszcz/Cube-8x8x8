/*
 * menu.h
 *
 *  Created on: Jul 21, 2013
 *  Author: Tomasz
 */
#ifndef MENU_H_
#define MENU_H_

#include "lcd.h"
//-----------------------------------------------------------------------
// Menu event types
#define E_UP    0x00
#define E_DOWN  0x01
#define E_ENTER 0x02
#define E_BACK  0x03
#define E_IDLE  0x04
//-----------------------------------------------------------------------
// Structure of menu element
typedef struct
{
    const char * text;
    MENU * submenu[];
    uint8_t submenuSize;
    uint8_t currentSubmenu;
    MENU * parent;
    void (* callback)(void);
} MENU;
//-----------------------------------------------------------------------
// Some global variables
MENU * currentMenu; 		// Pointer to current active menu
volatile uint8_t menuEvent; 	// Current menu event
char * lcdBuff[LCD_Y]; 		// Lcd text buffer
uint16_t mSpeedGame;
//-----------------------------------------------------------------------
// Declaration menu
MENU main;	// Main menu
	MENU menu1;	// Snake game
		MENU menu11;	// Play game
		MENU menu12;	// Level
			MENU menu121;	// Easy
			MENU menu122;	// Medium
			MENU menu123;	// Hard
			MENU menu124;	// Insane
		MENU menu13;	// Highscores
	MENU menu2;	// Visualization
		MENU menu21;	// Enable usart
	MENU menu3;	// Credits
//-----------------------------------------------------------------------
// Menu functions (callbacks)
void menuPlayGame(void);
void menuSetEasy(void);
void menuSetMedium(void);
void menuSetHard(void);
void menuSetInsane(void);
void menuHighscores(void);
void menuEnableUsart(void);
void menuCredits(void);
//-----------------------------------------------------------------------
// Function to move on menu
inline void menuUp() { if(currentMenu->currentSubmenu > 0) currentMenu->currentSubmenu--; }
inline void menuDown() { if(currentMenu->currentSubmenu < currentMenu->submenuSize-1) currentMenu->currentSubmenu++; }
inline void menuEnter() { currentMenu = currentMenu->submenu[currentMenu->currentSubmenu]; }
inline void menuBack() { currentMenu = currentMenu->parent; }
inline void menuBackTo(MENU * menu) { currentMenu = menu; }
//-----------------------------------------------------------------------
// Function to change menu base on current event, must be call every time an event occurs
void changeMenu();
//-----------------------------------------------------------------------
// Function to draw current menu on lcd (+lcd buffer)
void drawMenu();
//-----------------------------------------------------------------------
// Function to operate on ANSI C string - insert whole strFrom string to strTo string, the beginning of the insertion starts at toPos
char * strPut(char * strTo, const char * strFrom, uint8_t toPos);
//-----------------------------------------------------------------------
#endif
