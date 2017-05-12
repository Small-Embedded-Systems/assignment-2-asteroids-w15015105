/* Controller */

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* hardware platform libraries */
#include <mbed.h>

/* asteroids */
#include "model.h"
#include "asteroids.h"

/* Joystick 5-way switch
*/
enum position { left,down,right,up,centre };
DigitalIn joystick[] = {P5_0, P5_1, P5_4, P5_2, P5_3};

void controls(void)
{
 if (joystick[right].read() == 0) {
		player.heading += 25;
 }
 if (joystick[left].read() == 0) {
		player.heading -= 25;
 }
 if (joystick[centre].read() == 0) {
		if (lives > 0) {
			fire();		
		}
 }
 if (joystick[up].read() == 0) {
		if (player.shieldAvailable) {
			player.shielded = true;
		}
 }
}

