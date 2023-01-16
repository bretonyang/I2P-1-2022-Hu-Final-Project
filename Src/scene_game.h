// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

#ifndef SCENE_GAME_H
#define SCENE_GAME_H
#include "game.h"

Scene scene_main_create(void);

extern int game_main_score;
extern bool game_over;
extern bool game_win;
extern bool ghost_twinkle;

#endif
