/*
 * snake.c
 *
 *  Created on: Jul 21, 2013
 *  Author: Patryk Chrabaszcz
 */

#include <stdlib.h>
#include "snake.h"
#include "lcd.h"

uint16_t snakeSize = 1; //Start game - size=1

// Game loop and initialization
int snakeGame(uint16_t gameSpeed) // Game Speed is a time delay between moves
{
	speed= gameSpeed;
	startGame();
	score=0;

	while(1)
	{
		if(lastKey != NONE_KEY && lastKey != START_KEY) // Start key ends the game
			lastDirection=lastKey;

		moveSnake(lastDirection);

		// Checks if game is over
		if(isCrush())
			return score;

		updateCube(); // Paint snake and food on the Cube

		// Repeat to get better keys readings, could be done using interrupts but it's not
		for(int i=0;i<4;i++)
		{
			delayMs(speed/4);
			lastKey=getKey();

			if(lastKey != NONE_KEY && lastKey != START_KEY)
				lastDirection=lastKey;
		}


		if(lastKey==START_KEY) // Start key ends the game
		{
			return score;
		}
	}
	return score;
}

void startGame() 
{
	CUBE_OFF;

	setFood();

	snake[0][X] = 3;
	snake[0][Y] = 3;
	snake[0][Z] = 3;

	snakeSize = 1;
}

//Display random food
void setFood()
{
	food[X] = rand() % 8;
	food[Y] = rand() % 8;
	food[Z] = rand() % 8;

	//Dispay score on LCD
	score = (snakeSize-1)*(((EASY_SPEED-speed)+100)/100);
	lcdCls();
	lcdGoTo(1,0);
	lcdStr("SCORE: ");
	lcdGoTo(1,7);
	char buff[5];
	lcdStr( itoa(score, buff, 10) );

}

// Updates snake table, this have to be updated into Cube matrix
void moveSnake(uint8_t direction)
{
	if (direction == SELECT_KEY) return;

	// Moving snake table and making space for new head
	for (int i = 0; i < snakeSize; i++)
	{
		snake[snakeSize-i][X] = snake[snakeSize-i-1][X];
		snake[snakeSize-i][Y] = snake[snakeSize-i-1][Y];
		snake[snakeSize-i][Z] = snake[snakeSize-i-1][Z];
	}
	// Making new head which depends on actual direction, if direction is not recognized then PAUSE GAME
	switch (direction)
	{
		case UP_KEY:
			if (snake[0][X] > 0)
				snake[0][X]--;
			else
				snake[0][X] = 7;
			break;

		case DOWN_KEY:
			if (snake[0][X] < 7)
				snake[0][X]++;
			else
				snake[0][X] = 0;
			break;

		case RIGHT_KEY:
			if (snake[0][Y] <7 )
				snake[0][Y]++;
			else
				snake[0][Y] = 0;
			break;

		case LEFT_KEY:
			if (snake[0][Y] > 0)
				snake[0][Y]--;
			else
				snake[0][Y] = 7;
			break;

		case UPT_KEY:
			if (snake[0][Z] <7)
				snake[0][Z]++;
			else
				snake[0][Z] = 0;
			break;

		case DOWNT_KEY:
			if (snake[0][Z] > 0)
				snake[0][Z]--;
			else
				snake[0][Z] = 7;
			break;

		default:
			break;

	}

	// Check if snake is eating(if True then snake grows and new food appears on board)
	if(	(snake[0][X]==food[X])&&
		(snake[0][Y]==food[Y])&&
		(snake[0][Z]==food[Z]) )
	{
		snakeSize++;
		setFood();
	}
}
// Updates Cube matrix by snake table
void updateCube()
{
	// Clear the Cube
	CUBE_OFF;


	// Set snake body leds ON
	for(int i=0;i<snakeSize;i++)
	{
		setLedOn(snake[i][X],snake[i][Y],snake[i][Z]);
	}

	// Food blinking
	foodBlink++;
	if(foodBlink%2)
		setLedOn(food[X],food[Y],food[Z]);
	else
		setLedOff(food[X],food[Y],food[Z]);
}
// Checks if there is a crush
int isCrush()
{
	for(int i=0;i<snakeSize-1;i++)
	{
		for(int j=i+1;j<snakeSize;j++)
			if(snake[i][X]==snake[j][X] &&
			   snake[i][Y]==snake[j][Y] &&
			   snake[i][Z]==snake[j][Z])
				return 1;
	}
	return 0;
}
