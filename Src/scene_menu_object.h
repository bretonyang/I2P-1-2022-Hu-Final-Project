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

typedef struct DropdownMenu {
    Button* dropdownBtn;
    Button* settingsBtn;
    Button* highScoreBtn;
    bool isOpened;
} DropdownMenu;

typedef struct Slider {
    RecArea handle_body;
    RecArea track_body;
    ALLEGRO_BITMAP* handle_img;
    ALLEGRO_BITMAP* track_img;
    bool handle_hovered;
    bool handle_held;
} Slider;

/* functions for buttons */
extern Button* button_create(float, float, float, float, const char*, const char*);
extern void button_draw(Button*);
extern void button_destroy(Button*);
extern void button_update_hover_state(Button*, int, int);

/* functions for sliders */
extern Slider* slider_create(float, float, float, float, float, float, float, float, const char*, const char*);
extern void slider_draw(Slider*);
extern void slider_destroy(Slider*);
extern void slider_update_value(Slider*, int, float*);
extern void slider_update_hover_state(Slider*, int, int);


#endif // MENU_OBJECT_H
