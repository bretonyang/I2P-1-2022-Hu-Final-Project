#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <math.h>
#include "scene_menu_object.h"
#include "scene_settings.h"
#include "scene_scores.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "utility.h"
#include "shared.h"

/* Internal Variables*/
static ALLEGRO_BITMAP* gameTitle;
static ALLEGRO_SAMPLE_ID menuBGM;
static int gameTitleW, gameTitleH;

// [HACKATHON 3]
// TARGET : use a clickable button to enter setting scene.
// For `Button` struct(object) is defined in `scene_menu_object.h` and `scene_menu_object.c`
// STRONGLY recommend you trace both of them first.

//	[HACKATHON 3-1]
//	TODO: Declare variable for button
//	Uncomment and fill the code below

// UI Components
static DropdownMenu* dropdownMenu;

static void init() {

    // [HACKATHON 3-2]
    // TODO: Create button to settings
    //	Uncomment and fill the code below
//    btnSettings = button_create(730, 20, 50, 50, "Assets/scene_menu/settings.png", "Assets/scene_menu/settings2.png");

    dropdownMenu = dropdownMenu_create(730, 20, 50, 50,
                                       670, 110, 51,
                                       "Assets/scene_menu/dropdown.png", "Assets/scene_menu/dropdown2.png",
                                       "Assets/scene_menu/settings.png", "Assets/scene_menu/settings2.png",
                                       "Assets/scene_menu/scores.png", "Assets/scene_menu/scores2.png");

    // Load menu title image and get its width and height
    gameTitle = load_bitmap("Assets/scene_menu/menu_title.png");
    gameTitleW = al_get_bitmap_width(gameTitle);
    gameTitleH = al_get_bitmap_height(gameTitle);

    // Play menu scene bgm
    stop_bgm(menuBGM);
    menuBGM = play_bgm(themeMusic, music_volume);

}

static void draw() {

    al_clear_to_color(al_map_rgb(0, 0, 0));

    const float scale = 0.7;
    const float offset_w = (SCREEN_W >> 1) - 0.5 * scale * gameTitleW;
    const float offset_h = (SCREEN_H >> 1) - 0.5 * scale * gameTitleH;

    //draw menu title image
    al_draw_scaled_bitmap(
        gameTitle,
        0, 0,
        gameTitleW, gameTitleH,
        offset_w, offset_h,
        gameTitleW * scale, gameTitleH * scale,
        0
    );

    // draw game entering prompt text
    al_draw_text(
        menuFont,
        al_map_rgb(255, 255, 255),
        SCREEN_W >> 1,
        SCREEN_H - 150,
        ALLEGRO_ALIGN_CENTER,
        "PRESS \"ENTER\""
    );

    // [HACKATHON 3-3]
    // TODO: Draw button
    // Uncomment and fill the code below
//    button_draw(btnSettings);
    dropdownMenu_draw(dropdownMenu);
}

static void destroy() {
    stop_bgm(menuBGM);
    al_destroy_bitmap(gameTitle);
    //	[HACKATHON 3-10]
    //	TODO: Destroy button images
    //	Uncomment and fill the code below
//    al_destroy_bitmap(btnSettings.default_img);
//    al_destroy_bitmap(btnSettings.hovered_img);
//    button_destroy(btnSettings);
    dropdownMenu_destroy(dropdownMenu);
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
    //	[HACKATHON 3-7]
    //	TODO: Update button's status(hovered), and utilize the function `pnt_in_rect`, which you just implemented
    //	Uncomment and fill the code below
//    button_update_hover_state(btnSettings, mouse_x, mouse_y);
    dropdownMenu_update_hover_state(dropdownMenu, mouse_x, mouse_y);
}


//	[HACKATHON 3-8]
//	TODO: When btnSettings clicked, switch to settings scene
//  `game_change_scene` is defined in `game.h`.
//  You can check line 121 `scene_menu.c` to see how to use this function.
//  And find the corresponding function for `scene_setting`
// 	Utilize the member defined in struct `Button`.
//  The logic here is
//  `if clicked the mouse` && `mouse position is in the area of button`
// 	      `Enter the setting scene`
//	Uncomment and fill the code below
static void on_mouse_down() {
//    if (btnSettings->hovered) {
//        game_change_scene(scene_settings_create()); // mouse down + hovered = clicked
//    }

    if (dropdownMenu->isOpened) {
        if (dropdownMenu->dropdownBtn->hovered) {
            // Close dropdown
            dropdownMenu->isOpened = false;
        }
        else if (dropdownMenu->settingsBtn->hovered) {
            // Switch to settings scene
            game_change_scene(scene_settings_create());
        }
        else if (dropdownMenu->highScoreBtn->hovered) {
            // Switch to high scores scene
            game_change_scene(scene_scores_create());
        }
    }
    else {
        if (dropdownMenu->dropdownBtn->hovered) {
            // Open dropdown
            dropdownMenu->isOpened = true;
        }
    }
}

static void on_key_down(int keycode) {

    switch (keycode) {
    case ALLEGRO_KEY_ENTER:
        // Change to game scene when "ENTER" pressed
        game_change_scene(scene_main_create());
        break;
//    case ALLEGRO_KEY_ESCAPE:
//        // Change to settings scene when "ESCAPE" pressed
//        game_change_scene(scene_scores_create());
//        break;
    default:
        break;
    }
}

// TODO: Add more event callback functions such as update, ...

// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_menu_create(void) {

    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    // Register callbacks
    scene.name = "Menu";
    scene.initialize = &init;
    scene.draw = &draw;
    scene.destroy = &destroy;
    scene.on_mouse_move = &on_mouse_move;
    // [HACKATHON 3-9]
    // TODO: Register on_mouse_down.
    // Uncomment the code below.
    scene.on_mouse_down = &on_mouse_down;
    scene.on_key_down = &on_key_down;

    // TODO: Register more event callback functions such as keyboard, mouse, ...
    game_log("Menu scene created");
    return scene;
}
