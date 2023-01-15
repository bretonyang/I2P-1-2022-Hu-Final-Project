#ifndef PACMAN_OBJ_H
#define PACMAN_OBJ_H

#include "utility.h"
#include "game.h"

// <Forward Declaration>
// forward declarations can be useful when you need to have looping struct declarations,
// but you want to (should) prevent mutually include dependency.
// Only applicable when you only need the declaration but no need the definition in you sturct,
// which means that you only used the struct as pointer(no access to struct data), param, return type in this file.
// If you have to access the data in struct ex. `A->data`,
// you should include the corresponding header file for its definition.
// Advance reading : https://stackoverflow.com/questions/18658438/what-is-forward-declaration-and-the-difference-between-typedef-struct-x-and

typedef struct Ghost Ghost;
typedef struct Map Map;

typedef struct Pacman {
    bitmapdata imgdata;
    object objData;
    func_ptr move;
    int speed;
    bool speedUp;
    bool powerUp;
    ALLEGRO_TIMER* death_anim_counter;
    ALLEGRO_TIMER* win_anim_counter;
    ALLEGRO_TIMER* speed_up_counter;
    ALLEGRO_BITMAP* move_sprite;
    ALLEGRO_BITMAP* die_sprite;
    ALLEGRO_BITMAP* win_sprite;
} Pacman;

/* Global variables */
extern const int PMAN_DEATH_ANIM_CD; // Pacman's death animation will be finished at 96 death_anim_counter ticks.
extern const int PMAN_WIN_ANIM_CD;
extern const int PMAN_SPEED_UP_CD;

extern Pacman* pacman_create(void);
extern void pacman_destroy(Pacman* pman);
extern void pacman_update_status(Pacman* pacman);

extern void pacman_draw(Pacman* pacman);
extern void pacman_move(Pacman* pacman, Map* M);
extern void pacman_eatItem(Pacman* pacman, const char Item);

extern void pacman_NextMove(Pacman* pacman, Directions next);
extern void pacman_die(void);
extern void pacman_win(void);

#endif // !PACMAN_OBJ_H
