
/* Scene for highest 5 scores */

#include "scene_scores.h"
#include "scene_menu.h"
#include "scene_menu_object.h"
#include "game.h"
#include "utility.h"
#include "shared.h"

/* Internal variables */

// image and sample
static ALLEGRO_BITMAP* scoresTitle;
static ALLEGRO_SAMPLE_ID scoresBGM;

// width and height settings
static int scoresTitleW, scoresTitleH;
static const int btnImgW = 177, btnImgH = 54; // width and height of button image

// table's text dimensions settings
static const int rankTextX = 130;
static const int scoreTextX = 330;
static const int dateTextX = 600;
static const int tableStartY = 180;
static const int tableDiffY = 80;

// Buttons
static Button* btnMenu; // button to switch to menu scene


/* Internal functions */

static void init(void) {
    // Create buttons
    btnMenu = button_create((SCREEN_W - btnImgW) >> 1, SCREEN_H - 120, btnImgW, btnImgH,
                            "Assets/scene_scores/menu.png", "Assets/scene_scores/menu2.png");

    // Load title image
    scoresTitle = load_bitmap("Assets/scene_scores/scores_title.png");
    scoresTitleW = al_get_bitmap_width(scoresTitle);
    scoresTitleH = al_get_bitmap_height(scoresTitle);

    // Play victory music
    stop_bgm(scoresBGM);
    scoresBGM = play_bgm(themeMusic, music_volume);
}

static void draw(void) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // Draw win title and trophy
    al_draw_bitmap(scoresTitle, (SCREEN_W - scoresTitleW) >> 1, 60, 0);

    // Draw table header
    al_draw_text(
        menuFont, al_map_rgb(255, 255, 255),
        rankTextX, tableStartY,
        ALLEGRO_ALIGN_CENTER, "Rank"
    );
    al_draw_text(
        menuFont, al_map_rgb(255, 255, 255),
        scoreTextX, tableStartY,
        ALLEGRO_ALIGN_CENTER, "Score"
    );
    al_draw_text(
        menuFont, al_map_rgb(255, 255, 255),
        dateTextX, tableStartY,
        ALLEGRO_ALIGN_CENTER, "Date"
    );

    // Draw each row of table body
    for (int i = 1; i <= NUM_OF_SCORES; i++) {
        // Don't draw empty data
        if (scores[i - 1].score == -1) {
            break;
        }

        // Draw ranking
        al_draw_textf(
            menuFont, al_map_rgb(255, 255, 255),
            rankTextX, tableStartY + tableDiffY * i,
            ALLEGRO_ALIGN_CENTER, "%d", i
        );
        // Draw score value
        al_draw_textf(
            menuFont, al_map_rgb(255, 255, 255),
            scoreTextX, tableStartY + tableDiffY * i,
            ALLEGRO_ALIGN_CENTER, "%4d", scores[i - 1].score
        );
        // Draw date info
        al_draw_textf(
            menuFont, al_map_rgb(255, 255, 255),
            dateTextX, tableStartY + tableDiffY * i,
            ALLEGRO_ALIGN_CENTER, "%s", scores[i - 1].date
        );
    }

    // Draw buttons
    button_draw(btnMenu);
}

static void destroy(void) {
    // Audio
    stop_bgm(scoresBGM);

    // Bitmap
    al_destroy_bitmap(scoresTitle);

    // Button
    button_destroy(btnMenu);
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
    button_update_hover_state(btnMenu, mouse_x, mouse_y);
}

static void on_mouse_down(void) {
    if (btnMenu->hovered) {
        game_change_scene(scene_menu_create());
    }
}


/* Global function */

// The only function that is shared across files.
Scene scene_scores_create(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "High scores scene";

    // Register callbacks
    scene.initialize = &init;
    scene.draw = &draw;
    scene.destroy = &destroy;
    scene.on_mouse_move = &on_mouse_move;
    scene.on_mouse_down = &on_mouse_down;

    game_log("High scores scene created");
    return scene;
}
