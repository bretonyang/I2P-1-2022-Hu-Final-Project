#ifndef MENU_OBJECT_H
#define MENU_OBJECT_H

#include "utility.h"
#include "shared.h"
#include "game.h"

typedef struct Button {
    RecArea body;
    ALLEGRO_BITMAP* default_img;
    ALLEGRO_BITMAP* hovered_img;
    bool hovered;
} Button;

extern Button button_create(float, float, float, float, const char*, const char*);
extern void button_draw(Button);
extern void button_destroy(Button);
extern bool button_hovered(Button, int, int);

#endif // MENU_OBJECT_H
