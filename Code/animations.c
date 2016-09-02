/*
 * animations.c
 *
 *  Created on: Jul 21, 2013
 *  Author: Patryk Chrabaszcz
 */

#include "animations.h"

void cubeOnePlaneDown()
{
	for (int z=0;z<7;z++)
			for (int y=0;y<8;y++)
				Cube[z][y]=Cube[z+1][y];

	for (int y=0;y<8;y++)
		Cube[7][y]=0x00;

}
void cubeOnePlaneUp()
{
	for (int z=1;z<8;z++)
		for (int y=0;y<8;y++)
			Cube[z][y]=Cube[z-1][y];

	for (int y=0;y<8;y++)
		Cube[0][y]=0x00;
}

void rain()
{
	setLedOn(rand()%8,rand()%8,7);
	setLedOn(rand()%8,rand()%8,7);
	_delay_ms(300);
	cubeOnePlaneDown();
}


