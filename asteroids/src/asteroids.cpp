/* Asteroids
    Sample solution for assignment
    Semester 2 -- Small Embedded Systems
    Dr Alun Moon
*/

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* hardware platform libraries */
#include <display.h>
#include <mbed.h>

/* Main game elements */
#include "model.h"
#include "view.h"
#include "controller.h"

/* Game state */
float elapsed_time; 
int   score;
int   lives;
struct ship player;
struct missile *shots; 
struct rock *asteroids;

int mode = 1;


float Dt = 0.01f;

Ticker model, view, controller;

/* The single user button needs to have the PullUp resistor enabled */
DigitalIn userbutton(P2_10,PullUp);
int main()
{
	  player.p.x = 240;
	  player.p.y = 140;
		player.respawnTime = 0;
		lives = 4;
	
    init_DBuffer();
	
    view.attach( draw, 0.025);
    model.attach( physics, Dt);
    controller.attach( controls, 0.1);   			


}

