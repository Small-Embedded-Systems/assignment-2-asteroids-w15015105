/* Asteroids model */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "model.h"
#include "utils.h"
#include "asteroids.h"

static int shieldDelay;

//move player
ship move_ship(ship player, float Dt) {

	player.p.x = player.p.x + (player.v.x * Dt);
	player.p.y = player.p.y + (player.v.y * Dt);
	
	//acc need to work out what Delta v is not use v.x/y
	double x_acc = cos(radians(player.heading)) * (player.v.x * Dt);
	double y_acc = cos(radians(player.heading)) * (player.v.y * Dt);
	
	player.v.x = player.v.x + (x_acc * Dt);
	player.v.y = player.v.y + (y_acc * Dt);	
	
	return player;
} 

//updates missile position per tick
void move_missiles(missile *shots) {

	for( ; shots ; shots=shots->next ) {
		shots->p.x += sin(radians(shots->heading));
		shots->p.y += -cos(radians(shots->heading));
		}
}


//to check lists don't exceed their own size
int getShotListLength(struct missile *shots) {
	int len = 0;
	for ( ; shots ; shots = shots->next, ++len) {};
		return len;
}

int getRockListLength(struct rock *asteroids) {
	int len = 0;
	for ( ; asteroids ; asteroids = asteroids->next, ++len) {};
		return len;
}

//deals with first element of linked list remove during missile hit
struct missile *shothead(struct missile *shots) {
	if( shots ){
		if (shots->hit) {
				struct missile *discard = shots;
				shots = shots->next;
				discard->next = NULL;
		}
	}
	return shots; 
}

//deals with first element of linked list remove during missile timeout
struct missile *shotDeadhead(struct missile *shots) {
	if( shots ){
				struct missile *discard = shots;
				shots = shots->next;
				discard->next = NULL;
	}
	return shots;
}

//removes timed out missile from list
void *removeDeadShot(struct missile *shots) {
	shots = shotDeadhead(shots);
	for( ; shots ; shots=shots->next ) {
	shots->next = shotDeadhead( shots->next );
	}
	return shots;
}

//removes collided missile from list
void *removeHitShots(struct missile *shots) {
	shots = shothead(shots);
	for( ; shots ; shots=shots->next ) {
	shots->next = shothead( shots->next );
	}
	return shots;
}

//deals with first element of linked list remove
struct rock *asteroidhead(struct rock *asteroids) {
	if( asteroids ){
		if (asteroids->hit) {
				struct rock *discard = asteroids;
				asteroids = asteroids->next;
				discard->next = NULL;
		}
	}
	return asteroids;
}

//removes asteroid from list
void *removeRock(struct rock *asteroids) {
	asteroids = asteroidhead(asteroids);
	for( ; asteroids ; asteroids=asteroids->next ) {
	asteroids->next = asteroidhead( asteroids->next );
	}
	return asteroids;
}

//sets asteroids to a new asteroid if space available *****
rock* new_asteroids(rock *asteroids) {

    if (getRockListLength(asteroids) < 10) {
		struct rock *newAsteroids = (struct rock*) malloc(sizeof(struct rock));
		newAsteroids->p.x = randrange(0,480); 
		newAsteroids->p.y = randrange(30,300);
		newAsteroids->heading = randrange(0,361);
		newAsteroids->hit = false;
	  if (randrange(0,100) > 75) {
			newAsteroids->type = 1;
			newAsteroids->size.x = 45;
			newAsteroids->size.y = 45;
		}
		else { 
			newAsteroids->type = 2;
			newAsteroids->size.x = 20;
			newAsteroids->size.y = 20;
		}
		newAsteroids->next = asteroids;
		return newAsteroids;
		}
		else 
			return asteroids;
}

//updates asteroid position per tick
void move_asteroids(rock *asteroids, float Dt) {

	for( ; asteroids ; asteroids=asteroids->next ) {
		if(asteroids->type == 1) {
			asteroids->p.x += sin(radians(asteroids->heading))*0.4f;
			asteroids->p.y += -cos(radians(asteroids->heading))*0.4f;
		}
		else if (asteroids->type == 2) {
			asteroids->p.x += sin(radians(asteroids->heading))*0.8f;
			asteroids->p.y += -cos(radians(asteroids->heading))*0.8f;
		}

		if(asteroids->p.x > 480) {
			asteroids->p.x = 0;
		}
		if(asteroids->p.x < 0) {
			asteroids->p.x = 480;	
		}
		if(asteroids->p.y > 280) {
			asteroids->p.y = 0;
		}
		if(asteroids->p.y < 0) {
			asteroids->p.y = 280;
		}
		if(asteroids->p.x < 65 + asteroids->size.x && asteroids->p.y < 45 + asteroids->size.y) {
		asteroids->p.y += 235;
		asteroids->p.x += 415;
		}
	}
}

//checks for collisions between missles and rocks
void missiles_hit_rocks(missile *shots, rock *asteroids) {
	
	for ( ; shots ; shots=shots->next ) {
		for ( ; asteroids ; asteroids=asteroids->next ) {
			bool xHit = false;
			bool yHit = false;
			if (shots->p.x >asteroids->p.x && shots->p.x <= (asteroids->p.x + asteroids->size.x)) {
				xHit = true;	
			}
			if (shots->p.y <= (asteroids->p.y + asteroids->size.y) && shots->p.y > asteroids->p.y) {
				yHit = true;	
			}
			if (xHit && yHit) {
				shots->hit = true;
				asteroids->hit = true;				
			}
		}	
	}
}

//code for when asteroids collide with ships
bool ship_hits(rock *asteroids) {
	
		for ( ; asteroids ; asteroids=asteroids->next ) {
			bool xHit = false;
			bool yHit = false;
			if (player.p.x > asteroids->p.x && player.p.x <= (asteroids->p.x + asteroids->size.x)) {
				xHit = true;	
			}
			if (player.p.y <= (asteroids->p.y + asteroids->size.y) && player.p.y > asteroids->p.y) {
				yHit = true;	
			}
			if (xHit && yHit) {
				return true;			
			}
		}	
		return false;
}

void shield_hits(rock *asteroids) {
	
		for ( ; asteroids ; asteroids=asteroids->next ) {
			bool xHit = false;
			bool yHit = false;
			if (player.shield.x > asteroids->p.x && player.shield.x <= (asteroids->p.x + asteroids->size.x)) {
				xHit = true;	
			}
			if (player.shield.y <= (asteroids->p.y + asteroids->size.y) && player.shield.y > asteroids->p.y) {
				yHit = true;	
			}
			if (xHit && yHit) {
				asteroids->hit = true;			
			}
		}	
}

//triggered by input to generate a new missile
void fire() {

	if (getShotListLength(shots) < 10 && player.respawnTime <= 0) {
		struct missile *newShots = (struct missile*) malloc(sizeof(struct missile));
		newShots->p.x = player.p.x + (cos(player.heading)); 
		newShots->p.y = player.p.y + (sin(player.heading)); 
		newShots->heading = player.heading + 90;
		newShots->hit = false;
		newShots->lifetime = 0;
		
		newShots->next = shots;
		shots = newShots;
	}
	
} 

//updates missiles
void update_missile_list() {

	struct missile *update = shots;
	for (; update ; update = update->next) {
		if (update->lifetime > 200) {
			removeDeadShot(update);
		} 
		else {
			removeHitShots(update);
			update->lifetime++;;
		}
	}
}

//updates asteroids
void update_rock_list() {
	
	struct rock *update = asteroids;
	for (; update ; update = update->next) {
				removeRock(update); 
	}		
}

//shield functions
void spawnShield() {
	if (shieldDelay/10 > 200 && !player.shielded) {
		player.shieldAvailable = true;
		shieldDelay = 0;
	}
	if (player.shielded) {
		player.shield.x = player.p.x;
		player.shield.y = player.p.y;
	} 
}


void physics(void)
{
		if (mode == 1) {
			player = move_ship(player, Dt);
			move_missiles(shots);
			asteroids = new_asteroids(asteroids);
			move_asteroids(asteroids,Dt);
			missiles_hit_rocks( shots, asteroids);
			spawnShield();
			if( ship_hits(asteroids)){
				if (!player.shielded) {
					if (player.respawnTime <= 0) {
						lives--;
						elapsed_time = 0;
						player.respawnTime = 200;
					}
				}
				else
						shield_hits(asteroids);
						player.shielded = false;
						player.shieldAvailable = false;
			} 
			else {
				player.respawnTime--;
				if (player.respawnTime <= 0) score++;
				if (!player.shieldAvailable && !player.shielded) shieldDelay ++;
				elapsed_time += 0.025f;
			}	
			update_missile_list();
			update_rock_list();
	}
}

