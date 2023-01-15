// [shared.c]
// you should define the shared variable declared in the header here.

#include <stdlib.h>
#include "shared.h"
#include "utility.h"
#include "game.h"

/* Global variables */

// Font
ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;
ALLEGRO_FONT* menuFont = NULL;

// Audio
ALLEGRO_SAMPLE* themeMusic = NULL;
ALLEGRO_SAMPLE* gameMusic_v1 = NULL;
ALLEGRO_SAMPLE* PACMAN_MOVESOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_WIN_SOUND = NULL;

int fontSize = 30;
float music_volume = 0.5;
float effect_volume = 0.5;
bool gameDone = false;

const int NUM_OF_SCORES = 5;
ScoreData* scores; // array of high scores

/*
	A way to accelerate is load assets once.
	And delete them at the end.
	This method does provide you better management of memory.
*/
void shared_init(void) {

    menuFont = load_font("Assets/Minecraft.ttf", fontSize);
    themeMusic = load_audio("Assets/Music/original_theme.ogg");
    gameMusic_v1 = load_audio("Assets/Music/ghost_siren_v1.ogg");
    PACMAN_MOVESOUND = load_audio("Assets/Music/pacman-chomp.ogg");
    PACMAN_DEATH_SOUND = load_audio("Assets/Music/pacman_death.ogg");
    PACMAN_WIN_SOUND = load_audio("Assets/Music/pacman_win.ogg");

    scores = load_scores("Assets/high_scores.txt");
}

void shared_destroy(void) {

    al_destroy_font(menuFont);
    al_destroy_sample(themeMusic);
    al_destroy_sample(gameMusic_v1);
    al_destroy_sample(PACMAN_MOVESOUND);
    al_destroy_sample(PACMAN_DEATH_SOUND);
    al_destroy_sample(PACMAN_WIN_SOUND);

    store_scores(scores, "Assets/high_scores.txt"); // store the new scores before program ended
    destroy_scores(scores);
}
