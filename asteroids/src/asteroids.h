/* Game state */

extern float elapsed_time; /* time this ship has been active */
extern int   score;        /* total score so far */
extern int   lives;        /* lives remaining */

extern struct ship player;
extern void fire(void);

extern struct rock *asteroids; /* array of rocks / pointer to linked-list */

extern struct missile *shots;  /* array of missiles / pointer to linked-list */

extern const float Dt; /* Time step for physics, needed for consistent motion */

extern int mode; /* to stop gameplay once lives hit 0 */
