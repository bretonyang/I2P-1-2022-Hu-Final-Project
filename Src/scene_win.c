/* Scene for the victory animation after pacman has eaten all beans */

#include "scene_settings.h"
#include "scene_menu.h"
#include "scene_game.h"
#include "scene_menu_object.h"
#include "game.h"
#include "share.h"


/* Internal variables */

// image and sample
static ALLEGRO_BITMAP* winTitle;
static ALLEGRO_BITMAP* trophyImg;
static ALLEGRO_SAMPLE_ID winMusic;

// width and height settings
static int winTitleW;
static int winTitleH;
static int trophyImgW;
static int trophyImgH;
static const int btnImgW = 177; // width of button images
static const int btnImgH = 54; // height of button images

// Buttons
static Button btnRestart; // button to switch to main game scene
static Button btnMenu; // button to switch to menu scene


/* Internal functions */

static void init(void) {
    // Create buttons
    btnRestart = button_create((SCREEN_W >> 1) - btnImgW - 50, 550, btnImgW, btnImgH,
                               "Assets/restart.png", "Assets/restart2.png"
                              );
    btnMenu = button_create((SCREEN_W >> 1) + 50, 550, btnImgW, btnImgH, "Assets/menu.png", "Assets/menu2.png");

    // Load title image
    winTitle = load_bitmap("Assets/win_title.png");
    winTitleW = al_get_bitmap_width(winTitle);
    winTitleH = al_get_bitmap_height(winTitle);

    // Load trophy image
    trophyImg = load_bitmap("Assets/win_trophy.png");
    trophyImgW = al_get_bitmap_width(trophyImg);
    trophyImgH = al_get_bitmap_height(trophyImg);

    // Play victory music
    stop_bgm(winMusic);
    winMusic = play_audio(PACMAN_WIN_SOUND, music_volume);
}

static void draw(void) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // Draw win title and trophy
    al_draw_bitmap(winTitle, (SCREEN_W - winTitleW) >> 1, SCREEN_H >> 2, 0);
    al_draw_bitmap(trophyImg, (SCREEN_W - trophyImgW) >> 1, (SCREEN_H - trophyImgH) >> 1, 0);

    // Draw buttons
    button_draw(btnRestart);
    button_draw(btnMenu);
}

static void destroy(void) {
    // Audio
    stop_bgm(winMusic);

    // Bitmap
    al_destroy_bitmap(winTitle);
    al_destroy_bitmap(trophyImg);

    // Button
    button_destroy(btnRestart);
    button_destroy(btnMenu);
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
    btnRestart.hovered = button_hovered(btnRestart, mouse_x, mouse_y);
    btnMenu.hovered = button_hovered(btnMenu, mouse_x, mouse_y);
}

static void on_mouse_down(void) {
    if (btnRestart.hovered)
        game_change_scene(scene_main_create());
    if (btnMenu.hovered)
        game_change_scene(scene_menu_create());
}


/* Global function */

// The only function that is shared across files.
Scene scene_win_create(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    // Register callbacks
    scene.initialize = &init;
    scene.draw = &draw;
    scene.destroy = &destroy;
    scene.on_mouse_move = &on_mouse_move;
    scene.on_mouse_down = &on_mouse_down;

    game_log("Win scene created");
    return scene;
}
