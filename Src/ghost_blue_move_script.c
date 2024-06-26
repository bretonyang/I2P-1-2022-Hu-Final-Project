
#include "ghost.h"
#include "pacman_obj.h"
#include "map.h"

#define GO_OUT_TIME 1024

/* Shared variables */
extern const uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;
extern const int cage_grid_x, cage_grid_y;

/* Declare static function prototypes */
static void ghost_blue_move_script_FREEDOM(Ghost* ghost, Map* M);
static void ghost_blue_move_script_BLOCKED(Ghost* ghost, Map* M);

/**
 * Movement script for FREEDOM (free at the map) blue ghost.
 * Ghost should move randomly without walking back into the room.
 * Also, walking back and forth is not allowed.
 */
static void ghost_blue_move_script_FREEDOM(Ghost* ghost, Map* M) {
    // [HACKATHON 2-4]
    // Uncomment the following code and finish pacman picking random direction.

    // Find the direction of the ghost's previous position
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
        ghost_NextMove(ghost, proba[generateRandomNumber(0, cnt - 1)]); // randomly choose an allowed moving direction

}

/**
 * Movement script for BLOCKED (inside the ghost room) blue ghost.
 * The ghost is expected to keep moving up and down inside the room.
 */
static void ghost_blue_move_script_BLOCKED(Ghost* ghost, Map* M) {

    switch (ghost->objData.preMove) {
    case UP:
        if (ghost->objData.Coord.y == 10)
            ghost_NextMove(ghost, DOWN);
        else
            ghost_NextMove(ghost, UP);
        break;
    case DOWN:
        if (ghost->objData.Coord.y == 12)
            ghost_NextMove(ghost, UP);
        else
            ghost_NextMove(ghost, DOWN);
        break;
    default:
        ghost_NextMove(ghost, UP);
        break;
    }
}

void ghost_blue_move_script(Ghost* ghost, Map* M, Pacman* pacman) {
    if (!movetime(ghost->speed))
        return;

    switch (ghost->status) {
    case BLOCKED:
        ghost_blue_move_script_BLOCKED(ghost, M);
        if (al_get_timer_count(game_tick_timer) > GO_OUT_TIME)
            ghost->status = GO_OUT;
        break;
    case FREEDOM:
        ghost_blue_move_script_FREEDOM(ghost, M);
        break;
    case GO_OUT:
        ghost_move_script_GO_OUT(ghost, M);
        break;
    case GO_IN:
        ghost_move_script_GO_IN(ghost, M);
        if (M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') {
            ghost->status = GO_OUT;
            ghost->speed = 2;
        }
        break;
    case FLEE:
        ghost_move_script_FLEE(ghost, M, pacman);
        break;
    default:
        break;
    }

    if(ghost_movable(ghost, M, ghost->objData.nextTryMove, false)) {
        ghost->objData.preMove = ghost->objData.nextTryMove;
        ghost->objData.nextTryMove = NONE;
    }
    else if (!ghost_movable(ghost, M, ghost->objData.preMove, false))
        return;

    switch (ghost->objData.preMove) {
    case RIGHT:
        ghost->objData.Coord.x += 1;
        break;
    case LEFT:
        ghost->objData.Coord.x -= 1;
        break;
    case UP:
        ghost->objData.Coord.y -= 1;
        break;
    case DOWN:
        ghost->objData.Coord.y += 1;
        break;
    default:
        break;
    }
    ghost->objData.facing = ghost->objData.preMove;
    ghost->objData.moveCD = GAME_TICK_CD;
}
