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

typedef struct Slider {
    RecArea handle_body;
    RecArea track_body;
    ALLEGRO_BITMAP* handle_img;
    ALLEGRO_BITMAP* track_img;
    bool handle_hovered;
    bool handle_held;
} Slider;

typedef struct DropdownMenu {
    Button* dropdownBtn;
    Button* settingsBtn;
    Button* highScoreBtn;
    bool isOpened;
} DropdownMenu;

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

/* functions for dropdown menus */
extern DropdownMenu* dropdownMenu_create(float dropdown_x, float dropdown_y, float dropdown_w, float dropdown_h,
                                  float item_x, float item_w, float item_h,
                                  const char* dropdown_img, const char* dropdown_img_hov,
                                  const char* settings_img, const char* settings_img_hov,
                                  const char* scores_img, const char* scores_img_hov);
extern void dropdownMenu_draw(DropdownMenu* dropdownMenu);
extern void dropdownMenu_update_hover_state(DropdownMenu* dropdownMenu, int mouse_x, int mouse_y);
extern void dropdownMenu_destroy(DropdownMenu* dm);


#endif // MENU_OBJECT_H
