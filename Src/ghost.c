#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include "ghost.h"
#include "game.h"
#include "scene_game.h"
#include "map.h"
#include "pacman_obj.h"

/* global variables*/
// [ NOTE ]
// if you change the map .txt to your own design.
// You have to modify cage_grid_{x,y} to corressponding value also.
// Or you can do some change while loading map (reading .txt file)
// Make the start position metadata stored with map.txt.
const int cage_grid_x=22, cage_grid_y=11;

/* Internal variables */
static const int fix_draw_pixel_offset_x = -3;
static const int fix_draw_pixel_offset_y = -3;
static const int draw_region = 30;
// [ NOTE - speed again ]
// Again, you see this notificationd. If you still want to implement something
// fancy with speed, objData->moveCD and GAME_TICK, you can first start on
// working on animation of ghosts and pacman. // Once you finished the animation
// part, you will have more understanding on whole mechanism.
static const int basic_speed = 2;
// speed: how many times `movetime` will return true per GAME_TICK_CD
// (i.e. # of moves allowed per GAME_TICK_CD)

/**
 * Create a ghost of GhostType `flag`.
 */
Ghost* ghost_create(GhostType flag) {

    // NOTODO
    Ghost* ghost = (Ghost*)malloc(sizeof(Ghost));
    if (!ghost)
        return NULL;

    ghost->typeFlag = flag;
    ghost->objData.Size.x = block_width;
    ghost->objData.Size.y = block_height;

    ghost->objData.nextTryMove = NONE;
    ghost->speed = basic_speed;
    ghost->status = BLOCKED;


    ghost->flee_sprite = load_bitmap("Assets/ghost_flee.png");
    ghost->dead_sprite = load_bitmap("Assets/ghost_dead.png");

    switch (ghost->typeFlag) {
    case Blinky:
        ghost->objData.Coord.x = cage_grid_x;
        ghost->objData.Coord.y = cage_grid_y;
        ghost->move_sprite = load_bitmap("Assets/ghost_move_red.png");
        ghost->move_script = &ghost_red_move_script;
        break;
    case Pinky:
        ghost->objData.Coord.x = cage_grid_x - 1;
        ghost->objData.Coord.y = cage_grid_y;
        ghost->move_sprite = load_bitmap("Assets/ghost_move_pink.png");
        ghost->move_script = &ghost_pink_move_script;
        break;
    case Inky:
        ghost->objData.Coord.x = cage_grid_x + 1;
        ghost->objData.Coord.y = cage_grid_y;
        ghost->move_sprite = load_bitmap("Assets/ghost_move_blue.png");
        ghost->move_script = &ghost_blue_move_script;
        break;
    default:
        ghost->objData.Coord.x = cage_grid_x;
        ghost->objData.Coord.y = cage_grid_y;
        ghost->move_sprite = load_bitmap("Assets/ghost_move_red.png");
        ghost->move_script = &ghost_red_move_script;
        break;
    }
    return ghost;
}

void ghost_destroy(Ghost* ghost) {
    // [TODO]
    // free ghost resource
    al_destroy_bitmap(ghost->move_sprite);
    al_destroy_bitmap(ghost->dead_sprite);
    al_destroy_bitmap(ghost->flee_sprite);
    free(ghost);
}

/**
 * Draw the ghost's animatoin according to its facing and moveCD.
 */
void ghost_draw(Ghost* ghost) {
    // getDrawArea return the drawing RecArea defined by objData and GAME_TICK_CD
    RecArea drawArea = getDrawArea(ghost->objData, GAME_TICK_CD);

    /*
    	[TODO]
    	Draw ghost according to its status
    	hint : use ghost->objData.moveCD value to determine which frame of the animation to draw.

    		A not so good way is:

    		if(ghost->objData.moveCD % 16 == 0){
    			al_draw_scaled_bitmap(...);
    		}
    		else if(ghost->objData.moveCD % 16 == 1){
    			al_draw_scaled_bitmap(...);
    		}...

    		since modulo operation is expensive, better avoid using it.
    */

    int bitmap_x_offset = 0;
    // [TODO] below is for animation usage, change the sprite you want to use.
    if (ghost->status == FLEE) {
        if (ghost_twinkle) {
            bitmap_x_offset = 32 * (ghost->objData.moveCD * 2 / GAME_TICK_CD);
        }

        // draw 2 different motion images per GAME_TICK_CD
        if (ghost->objData.moveCD * 2 / GAME_TICK_CD) { // == moveCD / (GAME_TICK_CD / 2)
            al_draw_scaled_bitmap(ghost->flee_sprite, bitmap_x_offset, 0,
                                  16, 16,
                                  drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                                  draw_region, draw_region, 0
                                 );
        }
        else {
            al_draw_scaled_bitmap(ghost->flee_sprite, bitmap_x_offset + 16, 0,
                                  16, 16,
                                  drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                                  draw_region, draw_region, 0
                                 );
        }
    }
    else if (ghost->status == GO_IN) {
        // decide which image to apply in bitmap
        // NOTE: each bitmap is 16px wide.
        switch (ghost->objData.facing) {
        case RIGHT:
            bitmap_x_offset = 0;
            break;
        case LEFT:
            bitmap_x_offset = 16;
            break;
        case UP:
            bitmap_x_offset = 32;
            break;
        case DOWN:
            bitmap_x_offset = 48;
            break;
        default:
            break;
        }

        al_draw_scaled_bitmap(ghost->dead_sprite, bitmap_x_offset, 0,
                                  16, 16,
                                  drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                                  draw_region, draw_region, 0
                                 );
    }
    else {
        // decide which image to apply in bitmap
        // NOTE: each bitmap is 16px wide.
        switch (ghost->objData.facing) {
        case RIGHT:
            bitmap_x_offset = 0;
            break;
        case LEFT:
            bitmap_x_offset = 32;
            break;
        case UP:
            bitmap_x_offset = 64;
            break;
        case DOWN:
            bitmap_x_offset = 96;
            break;
        default:
            break;
        }

        // draw 2 different motion images per GAME_TICK_CD
        if (ghost->objData.moveCD * 2 / GAME_TICK_CD) // == moveCD / (GAME_TICK_CD / 2)
            al_draw_scaled_bitmap(ghost->move_sprite, bitmap_x_offset, 0,
                                  16, 16,
                                  drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                                  draw_region, draw_region, 0
                                 );
        else
            al_draw_scaled_bitmap(ghost->move_sprite, bitmap_x_offset + 16, 0,
                                  16, 16,
                                  drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                                  draw_region, draw_region, 0
                                 );
    }

}

void ghost_NextMove(Ghost* ghost, Directions next) {
    ghost->objData.nextTryMove = next;
}

void printGhostStatus(GhostStatus S) {

    switch(S) {

    case BLOCKED: // stay inside the ghost room
        game_log("BLOCKED");
        break;
    case GO_OUT: // going out the ghost room
        game_log("GO_OUT");
        break;
    case FREEDOM: // free at the map
        game_log("FREEDOM");
        break;
    case GO_IN:
        game_log("GO_IN");
        break;
    case FLEE:
        game_log("FLEE");
        break;
    default:
        game_log("status error");
        break;
    }
}

bool ghost_movable(Ghost* ghost, Map* M, Directions targetDirec, bool room) {
    // [HACKATHON 2-3]
    // TODO: Determine if the current direction is movable.
    // Basically, this is a ghost version of `pacman_movable`.
    // So if you have finished (and you should), you can just "copy and paste"
    // and do some small alternation.
    int next_x = ghost->objData.Coord.x, next_y = ghost->objData.Coord.y;

    switch (targetDirec) {
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

    // if room == true, then ghost can NOT move into room
    if (is_wall_block(M, next_x, next_y) || (room && is_room_block(M, next_x, next_y)))
        return false;

    return true;

}

void ghost_toggle_FLEE(Ghost* ghost, bool setFLEE) {
    // [TODO]
    // TODO: Here is reserved for power bean implementation.
    // The concept is "When pacman eats the power bean, only
    // ghosts who are in state FREEDOM will change to state FLEE.
    // For those who are not (BLOCK, GO_IN, etc.), they won't change state."
    // This implementation is based on the classic PACMAN game.
    // You are allowed to do your own implementation of power bean system.
    /*
    	if(setFLEE){
    		if(... == FREEDOM){
    			... = FLEE;
    			... speed = ...
    		}
    	}else{
    		if(... == FLEE)
    			..
    	}
    */
    if (setFLEE) {
        if (ghost->status == FREEDOM) {
            ghost->status = FLEE;
            ghost->speed = basic_speed / 2;
        }
    }
    else {
        if (ghost->status == FLEE) {
            ghost->status = FREEDOM;
        }
    }
}

void ghost_collided(Ghost* ghost) {
    if (ghost->status == FLEE) {
        ghost->status = GO_IN;
        ghost->speed = 2 * basic_speed;
    }
}

void ghost_move_script_GO_IN(Ghost* ghost, Map* M) {
    // Description
    // `shortest_path_direc` is a function that returns the direction of shortest path.
    // Check `map.c` for its detail usage.
    // For GO_IN state.
    ghost->objData.nextTryMove = shortest_path_direc(M, ghost->objData.Coord.x, ghost->objData.Coord.y, cage_grid_x, cage_grid_y);
}
void ghost_move_script_GO_OUT(Ghost* ghost, Map* M) {
    // Description
    // Here we always assume the room of ghosts opens upward.
    // And used a greedy method to drag ghosts out of room.
    // You should modify here if you have different implementation/design of room.
    if(M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B')
        ghost_NextMove(ghost, UP);
    else
        ghost->status = FREEDOM;
}
void ghost_move_script_FLEE(Ghost* ghost, Map* M, const Pacman* const pacman) {
    // [TODO]
    Directions shortestDirection = shortest_path_direc(M, ghost->objData.Coord.x, ghost->objData.Coord.y, pacman->objData.Coord.x, pacman->objData.Coord.y);
    // Description:
    // The concept here is to simulate ghosts running away from pacman while pacman is having power bean ability.
    // To achieve this, think in this way. We first get the direction to shortest path to pacman, call it K (K is either UP, DOWN, RIGHT or LEFT).
    // Then we choose other available direction rather than direction K.
    // In this way, ghost will escape from pacman.
    int dx = ghost->objData.Coord.x - pacman->objData.Coord.x;
    int dy = ghost->objData.Coord.y - pacman->objData.Coord.y;
    bool isFar = (dx * dx + dy * dy) >= 100; // 10 block away

    // Find the direction of the ghost's previous position
//    Directions prevDirec;
//    switch (ghost->objData.preMove) {
//    case UP:
//        prevDirec = DOWN;
//        break;
//    case DOWN:
//        prevDirec = UP;
//        break;
//    case LEFT:
//        prevDirec = RIGHT;
//        break;
//    case RIGHT:
//        prevDirec = LEFT;
//        break;
//    default:
//        prevDirec = NONE;
//        break;
//    }

    static Directions proba[4]; // possible movements
    int cnt = 0; // count of possible movements

    // Use the normal direction choosing method
    if (isFar) {
        Directions prevDirec;
        switch (ghost->objData.preMove) {
        case UP:
            prevDirec = DOWN;
            break;
        case DOWN:
            prevDirec = UP;
            break;
        case LEFT:
            prevDirec = RIGHT;
            break;
        case RIGHT:
            prevDirec = LEFT;
            break;
        default:
            prevDirec = NONE;
            break;
        }

        static Directions proba[4]; // possible movements
        int cnt = 0; // count of possible movements
        for (Directions i = 1; i <= 4; i++) {
            if (i != prevDirec && ghost_movable(ghost, M, i, true)) // `true` argument means shouldn't walk into room
                proba[cnt++] = i;
        }

        if (!cnt)
            ghost_NextMove(ghost, prevDirec); // move backwards if no possible movements available
        else
            ghost_NextMove(ghost, proba[generateRandomNumber(0, cnt - 1)]);
    }
    else {
        for (Directions i = 1; i <= 4; i++) {
        if (i != shortestDirection && ghost_movable(ghost, M, i, true)) // `true` argument means shouldn't walk into room
                proba[cnt++] = i;
        }

        if (!cnt) {
            ghost_NextMove(ghost, shortestDirection);
        }
        else
            ghost_NextMove(ghost, proba[generateRandomNumber(0, cnt - 1)]); // randomly choose a possible moving direction
    }
}

