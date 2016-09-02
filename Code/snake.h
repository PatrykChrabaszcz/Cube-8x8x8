/*
 * snake.h
 *
 *  Created on: Jul 21, 2013
 *  Author: Patryk Chrabaszcz
 */


#ifndef SNAKE_H_
#define SNAKE_H_

#include "config.h"

// x,y,z in snake[][x] for easier use
#define X 0
#define Y 1
#define Z 2

#define MAX_SNAKE_SIZE 512
#define EASY_SPEED 400
#define MEDIUM_SPEED 300
#define HARD_SPEED 200
#define INSANE_SPEED 100


uint8_t snake[MAX_SNAKE_SIZE][3];	// Table to store snake
uint8_t food[3];			// Food which can be randomly set by setFood()
uint8_t lastDirection; 			// Stores information about last pressed direction key
uint16_t speed;
uint16_t score;
uint8_t foodBlink;

// Init the game
void startGame();

// Updates snake table which must be updated into Cube matrix
void moveSnake(uint8_t delay);

// Updates Cube matrix by snake table
void updateCube();

// Starts Snake Game
int snakeGame(uint16_t speed);

// Sets random food
void setFood();

// Checks if there is crush
int isCrush();

#endif
