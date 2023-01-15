// [HACKATHON 3-9]
// TODO: Create scene_settings.h and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.
#include "scene_settings.h"
#include "scene_menu.h"
#include "scene_menu_object.h"
#include "game.h"
#include "utility.h"
#include "shared.h"

// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/// TODO: implement a reset to default button

/* Internal variables */

static ALLEGRO_BITMAP* settingsTitle;
static ALLEGRO_SAMPLE_ID settingsBGM;

static int settingsTitleW;
static int settingsTitleH;
static int settingsTextX;
static int settingsTextY;
static const int okImgW = 80; // width of the ok button image
static const int okImgH = 61; // height of the ok button image

// UI components
static Button* btnOK;
static Slider* musicSlider;
static Slider* soundSlider;

// Width and height of track and handle images (in px).
static const int trackImgW = 400;
static const int trackImgH = 10;
static const int handleImgW = 40;
static const int handleImgH = 40;

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

static void init(void) {
    // Setup x and y starting position for each setting text
    settingsTextX = SCREEN_W >> 3;
    settingsTextY = SCREEN_H >> 2;

    // Create the "OK" button
    btnOK = button_create((SCREEN_W - okImgW) >> 1, SCREEN_H - 200, okImgW, okImgH,
                          "Assets/scene_settings/ok_button.png", "Assets/scene_settings/ok_button2.png");

    // Create the music and sound volume slider
    musicSlider = slider_create(settingsTextX + 150,
                                settingsTextY + (trackImgH >> 1),
                                trackImgW, trackImgH,
                                settingsTextX + 150 + 40 * (music_volume * 10) - (handleImgW >> 1),
                                settingsTextY + (trackImgH >> 1) - 15,
                                handleImgW, handleImgH,
                                "Assets/scene_settings/slider_track.png", "Assets/scene_settings/slider_handle.png"
                               );

    soundSlider = slider_create(settingsTextX + 150,
                                settingsTextY + (trackImgH >> 1) + 150,
                                trackImgW, trackImgH,
                                settingsTextX + 150 + 40 * (effect_volume * 10) - (handleImgW >> 1),
                                settingsTextY + (trackImgH >> 1) - 15 + 150,
                                handleImgW, handleImgH,
                                "Assets/scene_settings/slider_track.png", "Assets/scene_settings/slider_handle.png"
                               );

    // Load title image and get its width and height
    settingsTitle = load_bitmap("Assets/scene_settings/settings_title.png");
    settingsTitleW = al_get_bitmap_width(settingsTitle);
    settingsTitleH = al_get_bitmap_height(settingsTitle);

    // Play settings scene bgm
    stop_bgm(settingsBGM);
    settingsBGM = play_bgm(themeMusic, music_volume);
}

static void draw(void ) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // Draw settings title
    al_draw_bitmap(settingsTitle, (SCREEN_W - settingsTitleW) >> 1, 60, 0);

    // Draw music volume title text
    al_draw_text(
        menuFont,
        al_map_rgb(255, 255, 255),
        settingsTextX,
        settingsTextY,
        ALLEGRO_ALIGN_CENTER,
        "Music"
    );

    // Draw sound volume title text
    al_draw_text(
        menuFont,
        al_map_rgb(255, 255, 255),
        settingsTextX,
        settingsTextY + 150,
        ALLEGRO_ALIGN_CENTER,
        "Sound"
    );

    // Draw confirmation button
    button_draw(btnOK);

    // Draw music volume slider
    slider_draw(musicSlider);
    slider_draw(soundSlider);

    // Draw music volume value
    al_draw_textf(
        menuFont,
        al_map_rgb(255, 255, 255),
        700,
        settingsTextY,
        ALLEGRO_ALIGN_CENTER,
        "%d", (int)(music_volume * 100)
    );

    // Draw sound volume value
    al_draw_textf(
        menuFont,
        al_map_rgb(255, 255, 255),
        700,
        settingsTextY + 150,
        ALLEGRO_ALIGN_CENTER,
        "%d", (int)(effect_volume * 100)
    );
}

static void destroy(void) {
    // Audio
    stop_bgm(settingsBGM);

    // Bitmaps
    al_destroy_bitmap(settingsTitle);

    // Buttons
    button_destroy(btnOK);

    // Sliders
    slider_destroy(musicSlider);
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
    // Update button hover state
    button_update_hover_state(btnOK, mouse_x, mouse_y);

    // Update slider handle hover state
    slider_update_hover_state(musicSlider, mouse_x, mouse_y);
    slider_update_hover_state(soundSlider, mouse_x, mouse_y);

    // If handle is held, then update slider handle's x position and change corresponding float value
    if (musicSlider->handle_held)
        slider_update_value(musicSlider, mouse_x, &music_volume);
    if (soundSlider->handle_held)
        slider_update_value(soundSlider, mouse_x, &effect_volume);
}

static void on_mouse_down(void) {
    // change to menu scene when ok button clicked
    if (btnOK->hovered) {
        game_change_scene(scene_menu_create());
    }
    else {
        // If handle is hovered, then update the handle state to "held"
        if (musicSlider->handle_hovered)
            musicSlider->handle_held = true;


        if (soundSlider->handle_hovered)
            soundSlider->handle_held = true;
    }
}

static void on_mouse_up(int a, int mouse_x, int mouse_y, int f) {
    // If handle is "held", then change handle state to "unheld".
    if (musicSlider->handle_held)
        musicSlider->handle_held = false;

    else if (soundSlider->handle_held)
        soundSlider->handle_held = false;
}

static void on_key_down(int keycode) {

    switch (keycode) {
    case ALLEGRO_KEY_ESCAPE:
        // Close settings scene and switch to menu scene when "ESCAPE" is pressed
        game_change_scene(scene_menu_create());
        break;
    default:
        break;
    }
}

// The only function that is shared across files.
Scene scene_settings_create(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    // Register callbacks
    scene.name = "Settings";
    scene.initialize = &init;
    scene.draw = &draw;
    scene.destroy = &destroy;
    scene.on_mouse_move = &on_mouse_move;
    scene.on_mouse_down = &on_mouse_down;
    scene.on_mouse_up = &on_mouse_up;
    scene.on_key_down = &on_key_down;

    // TODO: Register more event callback functions such as keyboard, mouse, ...
    game_log("Settings scene created");
    return scene;
}
