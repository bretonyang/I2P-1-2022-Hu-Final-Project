//#pragma once
// [game.h]
// declare shared variables and allegro5 related routines.

// Extern variables are "declare without defining". When
// a variable is defined, the compiler allocates memory for that
// variable and possibly also initializes its contents to some value.
// The declaration informs the compiler that a variable by that name
// and type exists, but the compiler does not need to allocate memory
// for it since it is allocated elsewhere.

// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

// Use `include guard` to avoid multiple inclusion
#ifndef GAME_H
#define GAME_H
#define _CRT_SECURE_NO_DEPRECATE
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
typedef struct Ghost Ghost;
typedef struct Map Map;
typedef struct Pacman Pacman;
// If defined, logs will be shown on console and written to file.
// If commented out, logs will not be shown nor be saved.
#define LOG_ENABLED
; // Question
// Types of function pointers.
static ALLEGRO_EVENT_QUEUE* game_event_queue;
typedef void(*func_ptr)(void);
typedef void(*script_func_ptr)(Ghost* ghost, Map*M, Pacman* pacman);
typedef void(*func_ptr_keyboard)(int keycode);
typedef void(*func_ptr_mouse)(int btn, int x, int y, int dz);
// TODO: More function pointer typedefs for other events.

// Structure containing all scene functions / event callbacks.
typedef struct {
    char* name;
    func_ptr initialize;
    func_ptr update;
    func_ptr draw;
    func_ptr destroy;
    func_ptr_keyboard on_key_down;
    func_ptr_keyboard on_key_up;
    func_ptr_mouse on_mouse_down;
    func_ptr_mouse on_mouse_move;
    func_ptr_mouse on_mouse_up;
    func_ptr_mouse on_mouse_scroll;
    // TODO: More event callbacks such as timer tick, video finished, ...
} Scene;

// Frame rate (frame per second)
extern const int FPS;
// Display (screen) width.
extern const int SCREEN_W;
// Display (screen) height.
extern const int SCREEN_H;
// At most 4 audios can be played at a time.
extern const int RESERVE_SAMPLES;
// game ticking counter
extern uint32_t GAME_TICK;
// # of times the game should update the scene's status, it's set to update 2*GAME_TICK_CD / 1sec.
extern const uint32_t GAME_TICK_CD;
// allegro timer for game tick
extern ALLEGRO_TIMER* game_tick_timer;


/* Input states */

// The active scene. Events will be triggered through function pointers.
extern Scene active_scene;
// Keyboard state, whether the key is down or not.
extern bool key_state[ALLEGRO_KEY_MAX];
// Mouse state, whether the key is down or not.
// 1 is for left, 2 is for right, 3 is for middle.
extern bool* mouse_state;
// Mouse position.
extern int mouse_x, mouse_y;
// TODO: More variables to store input states such as joysticks, ...


/* Game states */

// Whether game is finished
extern bool gameDone;


/* Function prototypes */

// Create and start running the game.
void game_create(void);
// Function to change from one scene to another.
void game_change_scene(Scene next_scene);

// Display error message and exit the program, used like 'game_log'.
// Write formatted output to stdout and file from the format string.
// If the program crashes unexpectedly, you can inspect "log.txt" for
// further information.
void game_abort(const char* format, ...);
// Log events for later debugging, used like 'game_log'.
// Write formatted output to stdout and file from the format string.
// You can inspect "log.txt" for logs in the last run.
void game_log(const char* format, ...);

/* Functions and variables that should be implemented in other files. */

// Initialize shared variables and resources.
// Allows the game to perform any initialization it needs before
// starting to run.
extern void shared_init(void);
// Free shared variables and resources.
extern void shared_destroy(void);
#endif
