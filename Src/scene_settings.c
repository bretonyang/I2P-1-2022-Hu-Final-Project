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

/* Define your static vars / function prototypes below. */
static ALLEGRO_BITMAP* settingsTitle = NULL;
static ALLEGRO_SAMPLE_ID settingsBGM;
static int settingsTitleW;
static int settingsTitleH;

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

static void init(void) {
    // Load title image and get its width and height
    settingsTitle = load_bitmap("Assets/settings_title.png");
    settingsTitleW = al_get_bitmap_width(settingsTitle);
    settingsTitleH = al_get_bitmap_height(settingsTitle);

    // Play settings scene bgm
    stop_bgm(settingsBGM);
    settingsBGM = play_bgm(themeMusic, music_volume);
}

static void draw(void ) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // Draw settings title
    const float title_x = (SCREEN_W - settingsTitleW) >> 1;
    const float title_y = settingsTitleH >> 1;
    al_draw_bitmap(settingsTitle, title_x, title_y, 0);
}

static void destroy() {
    stop_bgm(settingsBGM);
    al_destroy_bitmap(settingsTitle);
}

static void on_key_down(int keycode) {

    switch (keycode) {
//    case ALLEGRO_KEY_ENTER:
//        // Change to menu scene when "ENTER" pressed
//        game_change_scene(scene_menu_create());
//        break;
    case ALLEGRO_KEY_ESCAPE:
        // Close settings scene and switch to menu scene when "ESCAPE" pressed
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
    scene.on_key_down = &on_key_down;

    // TODO: Register more event callback functions such as keyboard, mouse, ...
    game_log("Settings scene created");
    return scene;
}
