/*
 * snake.h
 *
 *  Created on: Jun 5, 2013
 *      Author: Patryk Chrabaszcz
 */
#include "config.h"

// x,y,z in snake[][x] for easier use
#define X 0
#define Y 1
#define Z 2

#define MAX_SNAKE_SIZE 512


// Actual snake Size
int snake[MAX_SNAKE_SIZE][3];	// Table to store snake
int food[3];			// Food that can be randomly set by setFood()
int lastDirection; 		// Stores information about last pressed direction key
int speed;

void startGame();

// Updates snake table which must be updated into Cube matrix
void moveSnake(int);

// Updates Cube matrix using snake table
void updateCube();

// Starts Snake Game
int snakeGame(int);

void setFood();

//TO DO
void saveScore();

