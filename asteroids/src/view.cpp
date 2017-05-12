/* Asteroids view
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

#include "asteroids.h"
#include "model.h"
#include "utils.h"
#include "view.h"

static float flashTimer;

Display *graphics = Display::theDisplay();

const colour_t background = BLACK; /* Midnight Blue */


const coordinate_t shape[] = {
    {-5,-5}, {10,0}, {-5,5}
};

/* double buffering functions */
void init_DBuffer(void)
{   /* initialise the LCD driver to use second frame in buffer */
    uint16_t *bufferbase = graphics->getFb();
    uint16_t *nextbuffer = bufferbase+ (480*272);
    LPC_LCD->UPBASE = (uint32_t)nextbuffer;
}

void swap_DBuffer(void)
{   /* swaps frames used by the LCD driver and the graphics object */
    uint16_t *buffer = graphics->getFb();
    graphics->setFb( (uint16_t*) LPC_LCD->UPBASE);
    LPC_LCD->UPBASE = (uint32_t)buffer;
}

void initialise(int score, int elapsed_time, int lives) {
		graphics->setTextColor(WHITE);
		graphics->setCursor(10,9);
		graphics->setTextSize(2);
		graphics->printf("%d", score/10);
		graphics->setCursor(10,30);
		if (lives == 4) {
			graphics->drawTriangle(10,45,15,30,20,45, WHITE);
			graphics->drawTriangle(25,45,30,30,35,45, WHITE);
			graphics->drawTriangle(40,45,45,30,50,45, WHITE);
			graphics->drawTriangle(55,45,60,30,65,45, WHITE);
		}
		else if (lives == 3) {
			graphics->drawTriangle(10,45,15,30,20,45, WHITE);
			graphics->drawTriangle(25,45,30,30,35,45, WHITE);
			graphics->drawTriangle(40,45,45,30,50,45, WHITE);
			graphics->drawTriangle(55,45,60,30,65,45, BLACK);
		}
		else if (lives == 2) {
			graphics->drawTriangle(10,45,15,30,20,45, WHITE);
			graphics->drawTriangle(25,45,30,30,35,45, WHITE);
			graphics->drawTriangle(40,45,45,30,50,45, BLACK);
			graphics->drawTriangle(55,45,60,30,65,45, BLACK);
		}
		else if (lives == 1) {
			graphics->drawTriangle(10,45,15,30,20,45, WHITE);
			graphics->drawTriangle(25,45,30,30,35,45, BLACK);
			graphics->drawTriangle(40,45,45,30,50,45, BLACK);
			graphics->drawTriangle(55,45,60,30,65,45, BLACK);
		}		
		graphics->setCursor(440,9);
		graphics->printf("%d", elapsed_time);
}

void gameOverScreen() {
	graphics->fillScreen(BLACK);
	graphics->setTextSize(5);
	graphics->setTextColor(WHITE);
	graphics->setCursor(100,120);
	graphics->printf("GAME OVER");
	graphics->setTextSize(2);
	graphics->setCursor(130,180);
	graphics->printf("Total Score : %d", score/10);
	mode = 0;
	
}
void drawmissiles(missile *shots) {
		for( ; shots ; shots=shots->next ) {	
		graphics->drawCircle(shots->p.x,shots->p.y,1,WHITE);	
		}
};
	
unsigned char reverse(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

void drawrocks(rock *asteroids) {
	  for( ; asteroids ; asteroids=asteroids->next ) {
			if(asteroids->type == 1) {
				unsigned char pic1[sizeof(asteroid1_bits)];
				memcpy (pic1, asteroid1_bits, sizeof(pic1));
				for (int c = 0; c < sizeof(asteroid1_bits) ; c++) {
					pic1[c] = reverse(pic1[c]);
				}
				graphics->drawBitmap(asteroids->p.x,asteroids->p.y, pic1, asteroid1_width, asteroid1_height, WHITE);
			}		
			else if (asteroids->type == 2) {
				unsigned char pic2[sizeof(asteroid2_bits)];
				memcpy (pic2, asteroid2_bits, sizeof(pic2));
				for (int c = 0; c < sizeof(asteroid2_bits) ; c++) {
					pic2[c] = reverse(pic2[c]);
				}
				graphics->drawBitmap(asteroids->p.x,asteroids->p.y, pic2, asteroid2_width, asteroid2_height, WHITE);
			
			}
		}
};
	
void drawship(ship player) {
	
	//code to fine player position and rotation
	
	//code to wraparound player ship
	if (player.p.x < 0) 
		player.p.x = 480;
	else if (player.p.x > 480) 
		player.p.x = 0;
	
	if (player.p.y < 0) 
		player.p.y = 280;
	else if (player.p.y > 280) 
		player.p.y = 0;
	
	//code to draw player ship around position
	float x_right, x_left, x_top, y_right, y_left, y_top;
	float x_rOld,x_lOld,x_tOld,y_rOld,y_lOld,y_tOld;
	
	
	float angle = radians(player.heading);
	
	x_rOld = shape[0].x;
	y_rOld = shape[0].y;
	x_tOld = shape[1].x;
	y_tOld = shape[1].y;
	x_lOld = shape[2].x;
	y_lOld = shape[2].y;
	
	
	x_right = player.p.x + (x_rOld*cos(angle)) - (y_rOld*sin(angle));
	y_right = player.p.y + (x_rOld*sin(angle)) + (y_rOld*cos(angle));
	
	x_top = player.p.x + (x_tOld*cos(angle)) - (y_tOld*sin(angle));
	y_top = player.p.y +(x_tOld*sin(angle)) + (y_tOld*cos(angle));
	
	x_left = player.p.x +(x_lOld*cos(angle)) - (y_lOld*sin(angle));
	y_left = player.p.y +(x_lOld*sin(angle)) + (y_lOld*cos(angle));
	
	x_right = player.p.x + (x_rOld*cos(angle)) - (y_rOld*sin(angle));
	y_right = player.p.y + (x_rOld*sin(angle)) + (y_rOld*cos(angle));
	
	//graphics->setCursor(20,10);
	//graphics->printf(" x1 : %1.1f, y1 : %1.1f, x2 : %1.1f, y2 : %1.1f, y3 : %1.1f, y3 : %1.1f", x_right,y_right,x_left,y_left,x_top,y_top);
	
	if (player.respawnTime > 0) {
		if (int(flashTimer*10)%4 == 0) {
		graphics->drawTriangle(x_left, y_left,
        x_top, y_top,
        x_right, y_right, BLACK);
		}
		else {
			graphics->drawTriangle(x_left, y_left,
        x_top, y_top,
        x_right, y_right, WHITE);
		}

	} else {	
	graphics->drawTriangle(x_left, y_left,
        x_top, y_top,
        x_right, y_right, WHITE);
	}
				

}

void drawShield() {
	if(player.shieldAvailable && !player.shielded) {
		graphics->setTextSize(2);
		graphics->setCursor(140,9);
		if (int(flashTimer*10)%4 == 0) {
			graphics->setTextColor(rgb(0,102,102));
			graphics->printf("SHIELD AVAILABLE");
		}
		else {
			graphics->setTextColor(BLACK);
			graphics->printf("SHIELD AVAILABLE");
		}
	}
	if(player.shielded) {
			graphics->drawCircle(player.shield.x,player.shield.y,16,rgb(0,102,102));
	}	
}	



void draw(void)
{
  graphics->fillScreen(background);
	
	if (lives > 0) {
		initialise(score,elapsed_time,lives);
		drawmissiles(shots);
		drawrocks(asteroids);
		drawship(player);
		drawShield();
		flashTimer += 0.025f;
	}
	else {
		gameOverScreen();
	}

	//add pause print
	
	swap_DBuffer();
}
