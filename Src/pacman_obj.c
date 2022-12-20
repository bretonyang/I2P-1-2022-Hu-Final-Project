#include <allegro5/allegro_primitives.h>
#include "pacman_obj.h"
#include "map.h"
/* Static variables */
static const int start_grid_x = 25, start_grid_y = 25;		// where to put pacman at the beginning
static const int fix_draw_pixel_offset_x = -3, fix_draw_pixel_offset_y = -3;  // draw offset
static const int draw_region = 30;							// pacman bitmap draw region
static ALLEGRO_SAMPLE_ID PACMAN_MOVESOUND_ID;
// [ NOTE - speed ]
// If you want to implement something regarding speed.
// You may have to modify the basic_speed here.
// But before you modify this, make sure you are
// totally understand the meaning of speed and function
// `step()` in `scene_game.c`, also the relationship between
// `speed`, `GAME_TICK`, `GAME_TICK_CD`, `objData->moveCD`.
static const int basic_speed = 2;

/* Shared variables */
extern ALLEGRO_SAMPLE* PACMAN_MOVESOUND;
extern ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND;
extern uint32_t GAME_TICK;
extern uint32_t GAME_TICK_CD;
extern bool game_over;
extern float effect_volume;

/* Declare static function */
static bool pacman_movable(Pacman* pacman, Map* M, Directions targetDirec) {
	// [HACKATHON 1-2]
	// TODO: Determine if the current direction is movable.
	// That is to say, your pacman shouldn't penetrate 'wall' and 'room'
	// , where room is reserved for ghost to set up.
	// 1) For the current direction `targetDirec`, use pre-implemented function
	// `is_wall_block` and `is_room_block` to check if the block is wall or room. (they are both defined in map.c)
	// 2) the coordinate data of pacman is stored in pacman->objData.Coord
	// it is a self-defined pair IntInt type. Trace the code and utilize it.

	int next_x = pacman->objData.Coord.x, next_y= pacman->objData.Coord.y;

	switch (targetDirec)
	{
    case UP:
        next_y -= 1;
        break;
    case DOWN:
        next_y += 1;
        break;
    case LEFT:
        next_x -= 1;
        break;
    case RIGHT:
        next_x += 1;
        break;
    default:
        // for none UP, DOWN, LEFT, RIGHT direction u should return false.
        return false;
	}
	if (is_wall_block(M, next_x, next_y) || is_room_block(M, next_x, next_y))
		return false;

	return true;
}

Pacman* pacman_create() {

	/*
		[TODO]
		Allocate dynamic memory for pman pointer;
	*/
	Pacman* pman = (Pacman*)malloc(sizeof(Pacman));
	if (!pman)
		return NULL;

	/* Init pman data */
	/* set starting point, Size, */
	/* TODO? */
	/* hint / just put it */
	pman->objData.Coord.x = 24;
	pman->objData.Coord.y = 24;
	pman->objData.Size.x = block_width;
	pman->objData.Size.y = block_height;

	pman->objData.preMove = NONE;
	pman->objData.nextTryMove = NONE;
	pman->speed = basic_speed;

	pman->death_anim_counter = al_create_timer(1.0f / 64);
	pman->powerUp = false;
	/* load sprites */
	pman->move_sprite = load_bitmap("Assets/pacman_move.png");
	pman->die_sprite = load_bitmap("Assets/pacman_die.png");
	return pman;

}

void pacman_destroy(Pacman* pman) {
	/*
		[TODO]
		free pacman resource
		al_destroy_bitmap(pman->...);
		al_destroy_timer(pman->...);
		...
		free(pman);
	*/
}


void pacman_draw(Pacman* pman) {
	/*
        [HW-TODO]
		Draw Pacman and animations
		hint: use pman->objData.moveCD to determine which frame of the animation to draw, you may refer to discription in ghost_draw in ghost.c
	*/

	/*  NOTE:
        1. moveCD will decrease by its speed value when GAME_TICK ticks (i.e. game updates).
        2. moveCD reaches 0 and is reset to GAME_TICK_CD when the pacman is allowed to move
           (i.e. movetime returns true).
        3.
	*/
	RecArea drawArea = getDrawArea(pman->objData, GAME_TICK_CD);

	//Draw default image
	al_draw_scaled_bitmap(pman->move_sprite, 0, 0,
		16, 16,
		drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
		draw_region, draw_region, 0
	);

	int offset = 0;
	if (game_over) {
		/*
			hint: instead of using pman->objData.moveCD, use Pacman's death_anim_counter to create animation
		*/
	}
	else {
		/*
			switch(pman->objData.facing)
			{
			case LEFT:
				...
			}
		*/
	}
}
void pacman_move(Pacman* pacman, Map* M) {
	if (!movetime(pacman->speed))
		return;
	if (game_over)
		return;

	int probe_x = pacman->objData.Coord.x, probe_y = pacman->objData.Coord.y;

	if (pacman_movable(pacman, M, pacman->objData.nextTryMove))
		pacman->objData.preMove = pacman->objData.nextTryMove; // record valid nextMove as preMove.
	else if (!pacman_movable(pacman, M, pacman->objData.preMove))
		return; // Don't move if nextTryMove and preMove are both invalid (e.g. when player

    // move pacman according to the recorded valid nextMove
    // or if nextMove is not valid, we'll use the original preMove
	switch (pacman->objData.preMove)
	{
	case UP:
		pacman->objData.Coord.y -= 1;
		pacman->objData.preMove = UP;
		break;
	case DOWN:
		pacman->objData.Coord.y += 1;
		pacman->objData.preMove = DOWN;
		break;
	case LEFT:
		pacman->objData.Coord.x -= 1;
		pacman->objData.preMove = LEFT;
		break;
	case RIGHT:
		pacman->objData.Coord.x += 1;
		pacman->objData.preMove = RIGHT;
		break;
	default:
		break;
	}
	pacman->objData.facing = pacman->objData.preMove; // record current facing direction
	pacman->objData.moveCD = GAME_TICK_CD; // reset moveCD
}
void pacman_eatItem(Pacman* pacman, const char Item) {
	switch (Item)
	{
	case '.':
		stop_bgm(PACMAN_MOVESOUND_ID);
		PACMAN_MOVESOUND_ID = play_audio(PACMAN_MOVESOUND, effect_volume);
		break;
	default:
		break;
	}
}

void pacman_NextMove(Pacman* pacman, Directions next) {
	pacman->objData.nextTryMove = next;
}

void pacman_die() {
	stop_bgm(PACMAN_MOVESOUND_ID);
	PACMAN_MOVESOUND_ID = play_audio(PACMAN_DEATH_SOUND, effect_volume);
}





