/*
 * snake.c
 *
 *  Created on: Jun 5, 2013
 *      Author: Patryk Chrabaszcz
 */

#include "snake.h"

int snakeSize = 1;

void startGame()
{
	CUBE_OFF;

	setFood();

	snake[0][X] = 3;
	snake[0][Y] = 3;
	snake[0][Z] = 3;

	snakeSize = 1;
}

//Updates food randomly
void setFood()
{
	food[X] = rand() % 8;
	food[Y] = rand() % 8;
	food[Z] = rand() % 8;

}
//Updates snake table which must be updated into Cube matrix
void moveSnake(int direction)
{
	//Moving snake table and making space for new head
	for (int i = 0; i < snakeSize; i++)
	{
		snake[snakeSize-i][X] = snake[snakeSize-i-1][X];
		snake[snakeSize-i][Y] = snake[snakeSize-i-1][Y];
		snake[snakeSize-i][Z] = snake[snakeSize-i-1][Z];
	}
	//Making new head which depends on actual direction, if direction is not recognized then PAUSE GAME
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

	//Check if snake is eating(if True then snake grows and new food appears on board)
	if(	(snake[0][X]==food[X])&&
		(snake[0][Y]==food[Y])&&
		(snake[0][Z]==food[Z]) )
	{
		snakeSize++;
		setFood();
	}


}
//Updates Cube matrix by snake table
void updateCube()
{
	//Clear Cube
	CUBE_OFF;
	foodBlink++;
	//Set on snake body
	for(int i=0;i<snakeSize;i++)
	{
		setLedOn(snake[i][X],snake[i][Y],snake[i][Z]);
	}
		if(foodBlink%2)
			setLedOn(food[X],food[Y],food[Z]);

		else
			setLedOff(food[X],food[Y],food[Z]);
}



int snakeGame(int gameSpeed)
{
	speed= gameSpeed;
	startGame();

	while(1)
	{

		if(lastKey!=NONE_KEY)
			lastDirection=lastKey;

		moveSnake(lastDirection);
		updateCube();

		// Repeat rutine to get better keys reading
		for(int i=0;i<4;i++)
		{
			delayMs(speed/4);
			lastKey=getKey();

			if(lastKey!=NONE_KEY)
				lastDirection=lastKey;
		}

		if(lastKey==SELECT_KEY)
		{
			// To do: Saving hi-Scores
			return 0;
		}
	}
}





