// [utility.c]
// you should implement the utility functions defined in the header.

#include "utility.h"
#include "game.h"
#include <math.h>
#include <stdlib.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
extern uint32_t GAME_TICK;
extern const uint32_t GAME_TICK_CD;
const int GridSize = 22;
float VOLUME = 1.0;
extern map_offset_x;
extern map_offset_y;
extern block_width, block_height;

/**
 * Load the given file as an ALLEGRO_SAMPLE and return it.
 */
ALLEGRO_SAMPLE* load_audio(const char* filename) {
    ALLEGRO_SAMPLE* sample = al_load_sample(filename);
    if (!sample)
        game_abort("failed to load audio: %s", filename);
    else
        game_log("loaded audio: %s", filename);
    return sample;
}

/**
 * Play the given loaded music sample once.
 * Returns the played sample id.
 */
ALLEGRO_SAMPLE_ID play_audio(ALLEGRO_SAMPLE* sample, float volume) {
    ALLEGRO_SAMPLE_ID id;
    if (!al_play_sample(sample, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &id))
        game_abort("failed to play audio (once)");
    return id;
}

/**
 * Play the given loaded music sample in loop mode
 * Returns the played sample id.
 */
ALLEGRO_SAMPLE_ID play_bgm(ALLEGRO_SAMPLE* sample, float volume) {
    ALLEGRO_SAMPLE_ID id;
    if (!al_play_sample(sample, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id))
        game_abort("failed to play audio (bgm)");

    //game_log("played audio (bgm)");
    return id;
}

/**
 * Stop playing the bgm with the given sample id.
 */
void stop_bgm(ALLEGRO_SAMPLE_ID sample) {
    al_stop_sample(&sample);
    //game_log("stop audio (bgm)");
}

/**
 * Returns the loaded font in the given file path and font size.
 */
ALLEGRO_FONT* load_font(const char* filename, int size) {
    ALLEGRO_FONT* font = al_load_font(filename, size, 0);
    if (!font)
        game_abort("failed to load font: %s with size %d", filename, size);
    else
        game_log("loaded font: %s with size %d", filename, size);
    return font;
}

ALLEGRO_BITMAP* load_bitmap(const char* filename) {
    ALLEGRO_BITMAP* bmp = al_load_bitmap(filename);
    if (!bmp)
        game_abort("failed to load image: %s", filename);
    else
        game_log("loaded image: %s", filename);
    return bmp;
}

ALLEGRO_BITMAP* load_bitmap_resized(const char* filename, int w, int h) {
    ALLEGRO_BITMAP* loaded_bmp = load_bitmap(filename);
    ALLEGRO_BITMAP* resized_bmp = al_create_bitmap(w, h);
    ALLEGRO_BITMAP* prev_target = al_get_target_bitmap();

    if (!resized_bmp)
        game_abort("failed to create bitmap when creating resized image: %s", filename);
    al_set_target_bitmap(resized_bmp);
    al_draw_scaled_bitmap(loaded_bmp, 0, 0,
                          al_get_bitmap_width(loaded_bmp),
                          al_get_bitmap_height(loaded_bmp),
                          0, 0, w, h, 0);
    al_set_target_bitmap(prev_target);
    al_destroy_bitmap(loaded_bmp);

    game_log("resized image: %s", filename);

    return resized_bmp;
}

// [HACKATHON 3-5]
// TODO: Define bool pnt_in_rect(int px, int py, RecArea field)
// Uncomment and fill in the code below.

/**
 * Returns true if the point (px, py) is in the area specified by a RecArea object, and false otherwise.
 */
bool pnt_in_rect(int px, int py, RecArea field) {
    return px >= field.x && px <= field.x + field.w && py >= field.y && py <= field.y + field.h;
}


void setRecArea(RecArea* RA, float x, float y, float w, float h) {
    RA->x = x;
    RA->y = y;
    RA->w = w;
    RA->h = h;
}

// Hakathon
/**
 * Returns true if the 2 RecAreas overlap each other, and false otherwise.
 */
bool RecAreaOverlap(const RecArea RA,const RecArea RB) {
    // [TODO]
    // Detect if two RecArea is overlapped.
    float RA_x2 = RA.x + RA.w;
    float RA_y2 = RA.y + RA.h;
    float RB_x2 = RB.x + RB.w;
    float RB_y2 = RB.y + RB.h;
    if ((fmin(RA_x2, RB_x2) > fmax(RA.x, RB.x)) && (fmin(RA_y2, RB_y2) > fmax(RA.y, RB.y)))
        return true;
    return false;
}

RecArea getDrawArea(object obj, uint32_t TOTAL_TICK) {
    // NOTODO
    RecArea target;

    target.x = map_offset_x + obj.Coord.x * block_width;
    target.y = map_offset_y + obj.Coord.y * block_height;
    target.w = block_width;
    target.h = block_height;

    switch (obj.preMove) {
    case UP:
        target.y += (obj.moveCD) * block_width / TOTAL_TICK;
        break;
    case DOWN:
        target.y -= (obj.moveCD) * block_width / TOTAL_TICK;
        break;
    case LEFT:
        target.x += (obj.moveCD) * block_width / TOTAL_TICK;
        break;
    case RIGHT:
        target.x -= (obj.moveCD) * block_width / TOTAL_TICK;
        break;
    case NONE:
        break;
    default:
        break;
    }

    return target;
}

void printRecAreaInfo(const RecArea* RA) {
    // NOTODO
    game_log("RecArea info: \nx: %f, y: %f, h: %f\n",
             RA->x, RA->y, RA->w, RA->h);
}

void printDirection(const Directions a) {
    // NOTODO
    switch (a) {
    case NONE:
        game_log("NONE");
        break;
    case UP:
        game_log("UP");
        break;
    case DOWN:
        game_log("DOWN");
        break;
    case LEFT:
        game_log("LEFT");
        break;
    case RIGHT:
        game_log("RIGHT");
        break;
    default:
        game_log("direction error");
        break;
    }
}

/**
 * Returns true if the object with the passed `speed` should move.
 * The number of times an object moves in 1 GAME_TICK_CD is the same as the value of `speed`.
 */
bool movetime(int speed) {
    // game_log("%d %d %d %d", GAME_TICK, GAME_TICK_CD, speed, GAME_TICK % (GAME_TICK_CD / speed));
    // NOTODO
    return (GAME_TICK % (GAME_TICK_CD / speed)) == 0;
}

/**
 * Returns a random integer in [a, b].
 */
uint32_t generateRandomNumber(uint32_t a, uint32_t b) {
    // NOTODO
    if (b < a)
        game_abort("Error in RandomNumber, b is less than a");
//	srand(time(NULL));
    return rand() % (b - a + 1) + a;
}

/**
 * Returns a random double in [0, 1].
 */
double generateRandomFloat() {
    // NOTODO
//	srand(time(NULL));
    return (double)rand() / RAND_MAX;
}

bool bernoulliTrail(double p) {
    // NOTODO
    if (p >= 1 || p <= 0)
        game_abort("Error range of p = %lf in BernoulliTrail func\n But p should be between 0.0 and 1.0", p);
    return generateRandomFloat() < p;

}
