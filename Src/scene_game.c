#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <string.h>
#include "game.h"
#include "shared.h"
#include "utility.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "scene_win.h"
#include "pacman_obj.h"
#include "ghost.h"
#include "map.h"


// [HACKATHON 2-0]
// Just modify the GHOST_NUM to 1
#define GHOST_NUM 3

/* global variables*/
int game_main_score = 0;
bool game_over = false;
bool game_win = false;

/* Internal variables*/
static ALLEGRO_TIMER* power_up_timer;
static const int power_up_duration = 10;
static Pacman* pman;
static Map* basic_map;
static Ghost** ghosts;
static ALLEGRO_SAMPLE_ID gameBGM;
static bool debug_mode = false;
static bool cheat_mode = false;

/* Declare static function prototypes */
static void init(void);
static void step(void);
static void checkItem(void);
static void status_update(void);
static void update(void);
static void draw(void);
static void printinfo(void);
static void destroy(void);
static void on_key_down(int key_code);
static void on_mouse_down(void);
static void render_init_screen(void);
static void draw_hitboxes(void);


static void init(void) {
    game_over = false;
    game_win = false;
    game_main_score = 0;

    // For debugging
//    basic_map = create_map("Assets/map_test.txt");

    // [TODO]
    // Create map from .txt file and design your own map !!
    basic_map = create_map("Assets/map_custom.txt");
    if (!basic_map) {
        game_abort("error on creating map");
    }
    // create pacman
    pman = pacman_create();
    if (!pman) {
        game_abort("error on creating pacman\n");
    }

    // allocate ghost memory
    // [HACKATHON 2-1]
    // TODO: Allocate dynamic memory for ghosts array.
    ghosts = (Ghost**)malloc(sizeof(Ghost*) * GHOST_NUM);
    if(!ghosts) {
        game_log("We haven't create any ghosts!\n");
    }
    else {
        // [HACKATHON 2-2]
        // TODO: create a ghost.
        // Try to look the definition of ghost_create and figure out what should be placed here.
        for (GhostType i = 0; i < GHOST_NUM; i++) {
            game_log("Creating ghost: %d", i);
            ghosts[i] = ghost_create(i);
            if (!ghosts[i])
                game_abort("error creating ghost\n");

        }
    }

    GAME_TICK = 0;
    render_init_screen();

    power_up_timer = al_create_timer(1.0f); // 1 tick / sec
    if (!power_up_timer)
        game_abort("Error creating timer\n");

    // Play game scene bgm
    stop_bgm(gameBGM);
    gameBGM = play_bgm(gameMusic_v1, music_volume);

    return ;
}

static void step(void) {
    if (pman->objData.moveCD > 0)
        pman->objData.moveCD -= pman->speed;
    for (int i = 0; i < GHOST_NUM; i++) {
        // important for movement
        if (ghosts[i]->objData.moveCD > 0)
            ghosts[i]->objData.moveCD -= ghosts[i]->speed;
    }
}

/**
 * Checks whether an item was eaten, and handles the corresponding event for eating that item.
 */
static void checkItem(void) {
    int Grid_x = pman->objData.Coord.x, Grid_y = pman->objData.Coord.y;
    if (Grid_y >= basic_map->row_num - 1 || Grid_y <= 0 || Grid_x >= basic_map->col_num - 1 || Grid_x <= 0)
        return;
    // [HACKATHON 1-3]
    // TODO: check which item you are going to eat and use `pacman_eatItem` to deal with it.

    switch (basic_map->map[Grid_y][Grid_x]) {
    case '.':
        pacman_eatItem(pman, '.');
        basic_map->map[Grid_y][Grid_x] = ' ';   // erase beans from map
        basic_map->beansCount -= 1;             // Update beans count of map
        game_main_score += 10;                  // update score
        break;
    default:
        break;
    }
    // [HACKATHON 1-4]
    // erase the item you eat from map
    // be careful no erasing the wall block.
}

static void status_update(void) {
    // Check if all beans are eaten
    if (!basic_map->beansCount) {
        game_log("All beans are eaten");
        stop_bgm(gameBGM);
        pacman_win();
        game_win = true;
    }

    // Check status of each ghost
    for (int i = 0; i < GHOST_NUM; i++) {
        if (ghosts[i]->status == GO_IN)
            continue;
        // [TODO]
        // use `getDrawArea(..., GAME_TICK_CD)` and `RecAreaOverlap(..., GAME_TICK_CD)` functions to detect
        // if pacman and ghosts collide with each other.
        // And perform corresponding operations.
        // [NOTE]
        // You should have some branch here if you want to implement power bean mode.
        // Uncomment Following Code

        // Game over if not cheat_mode and (collision of ghost with pacman)
        if (
            !cheat_mode &&
            RecAreaOverlap(
                getDrawArea(ghosts[i]->objData, GAME_TICK_CD),
                getDrawArea(pman->objData, GAME_TICK_CD)
            )
        ) {
            game_log("collide with ghost");
            stop_bgm(gameBGM);
            al_rest(1.0);
            pacman_die();
            game_over = true;
            break;
        }
    }
}

static void update(void) {
    if (game_over) {
        /*
        	[TODO]
        	start pman->death_anim_counter and schedule a game-over event (e.g change scene to menu) after Pacman's death animation finished
        	game_change_scene(...);
        */
        // start the death counter timer to create death animation
        al_start_timer(pman->death_anim_counter);

        // Change scene 2*PMAN_DEATH_ANIM_CD ticks after Pacman's death animation.
        if (al_get_timer_count(pman->death_anim_counter) >= 2 * PMAN_DEATH_ANIM_CD) {
            al_stop_timer(pman->death_anim_counter);

            // update the scores
            update_scores(scores, game_main_score);

            // switch to menu scene after death animation
            game_change_scene(scene_menu_create());
        }
        return;
    }
    else if (game_win) {
        // start the win counter timer to create victory animation
        al_start_timer(pman->win_anim_counter);

        // if counter ticks reaches 2*PMAN_WIN_ANIM_CD, then stop timer and change the scene
        if (al_get_timer_count(pman->win_anim_counter) >= 2 * PMAN_WIN_ANIM_CD) {
            al_stop_timer(pman->win_anim_counter);

            // update the scores
            update_scores(scores, game_main_score);

            // switch to win scene
            game_change_scene(scene_win_create());
        }
        // We want to keep moving the pacman to eat the last bean and create winning animation.
        // But we don't want to update the status and ghosts, so we use this extra else statement.
        else {
            step();
            checkItem();
            pacman_move(pman, basic_map);
        }

        return;
    }

    // Update the game
    step();
    checkItem();
    status_update();
    pacman_move(pman, basic_map);
    for (int i = 0; i < GHOST_NUM; i++)
        ghosts[i]->move_script(ghosts[i], basic_map, pman);

}

static void draw(void) {

    al_clear_to_color(al_map_rgb(0, 0, 0));

    //	[TODO]
    //	Draw scoreboard, something you may need is sprinf();
    /*
    	al_draw_text(...);
    */
    // Draw scoreboard
    al_draw_textf(
        menuFont,
        al_map_rgb(255, 255, 255),
        100, 15,
        ALLEGRO_ALIGN_CENTER,
        "Score: %d",
        game_main_score
    );

    draw_map(basic_map);
    pacman_draw(pman);

    if (game_over || game_win)
        return;
    // no drawing below when game over or player wins

    for (int i = 0; i < GHOST_NUM; i++)
        ghost_draw(ghosts[i]);

    //debugging mode
    if (debug_mode) {
        draw_hitboxes();
    }

}

static void draw_hitboxes(void) {
    RecArea pmanHB = getDrawArea(pman->objData, GAME_TICK_CD);
    al_draw_rectangle(
        pmanHB.x, pmanHB.y,
        pmanHB.x + pmanHB.w, pmanHB.y + pmanHB.h,
        al_map_rgb_f(1.0, 0.0, 0.0), 2
    );

    for (int i = 0; i < GHOST_NUM; i++) {
        RecArea ghostHB = getDrawArea(ghosts[i]->objData, GAME_TICK_CD);
        al_draw_rectangle(
            ghostHB.x, ghostHB.y,
            ghostHB.x + ghostHB.w, ghostHB.y + ghostHB.h,
            al_map_rgb_f(1.0, 0.0, 0.0), 2
        );
    }

}

static void printinfo(void) {
    game_log("pacman:");
    game_log("coord: %d, %d", pman->objData.Coord.x, pman->objData.Coord.y);
    game_log("PreMove: %d", pman->objData.preMove);
    game_log("NextTryMove: %d", pman->objData.nextTryMove);
    game_log("Speed: %f", pman->speed);
}


static void destroy(void) {
    stop_bgm(gameBGM);
    /*
    	[TODO]
    	free map array, Pacman, ghosts and timers
    */
    delete_map(basic_map);
    pacman_destroy(pman);

    for (int i = 0; i < GHOST_NUM; i++)
        ghost_destroy(ghosts[i]);
    free(ghosts);

    al_destroy_timer(power_up_timer);
}

static void on_key_down(int key_code) {
    switch (key_code) {
    // [HACKATHON 1-1]
    // TODO: Use allegro pre-defined enum ALLEGRO_KEY_<KEYNAME> to controll pacman movement
    // we provided you a function `pacman_NextMove` to set the pacman's next move direction.
    case ALLEGRO_KEY_W:
        pacman_NextMove(pman, UP);
        break;
    case ALLEGRO_KEY_A:
        pacman_NextMove(pman, LEFT);
        break;
    case ALLEGRO_KEY_S:
        pacman_NextMove(pman, DOWN);
        break;
    case ALLEGRO_KEY_D:
        pacman_NextMove(pman, RIGHT);
        break;
    case ALLEGRO_KEY_C:
        cheat_mode = !cheat_mode;
        if (cheat_mode)
            game_log("cheat mode on");
        else
            game_log("cheat mode off");
        break;
    case ALLEGRO_KEY_G:
        debug_mode = !debug_mode;
        break;
    default:
        break;
    }

}

static void on_mouse_down(void) {
    // nothing here

}

static void render_init_screen(void) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    draw_map(basic_map);
    pacman_draw(pman);
    for (int i = 0; i < GHOST_NUM; i++) {
        ghost_draw(ghosts[i]);
    }

    al_draw_text(
        menuFont,
        al_map_rgb(255, 255, 0),
        400, 400,
        ALLEGRO_ALIGN_CENTER,
        "READY!"
    );

    al_flip_display();
    al_rest(2.0);

}
// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_main_create(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "Game";
    scene.initialize = &init;
    scene.update = &update;
    scene.draw = &draw;
    scene.destroy = &destroy;
    scene.on_key_down = &on_key_down;
    scene.on_mouse_down = &on_mouse_down;
    // TODO: Register more event callback functions such as keyboard, mouse, ...
    game_log("Start scene created");
    return scene;
}
