#ifndef __PLAYER__
#define	__PLAYER__

#include "object.h"
#include "CACaves/CACaves.h"

typedef struct player_t{
    int level;
    int exp;
    int next;
    int hp;
    int maxhp;
    int power;
    int magic;
    int hit;
    int mdef;
    int def;
    int stamina;
    int evd;
    int gold;
    int speed;
}player_t;

typedef enum PLAYER_COMMAND{
    PLAYER_CMD_UP,
    PLAYER_CMD_LEFT,
    PLAYER_CMD_RIGHT,
    PLAYER_CMD_DOWN,
    PLAYER_CMD_UP_LEFT,
    PLAYER_CMD_UP_RIGHT,
    PLAYER_CMD_DOWN_LEFT,
    PLAYER_CMD_DOWN_RIGHT,
    PLAYER_QUIT_GAME,
    PLAYER_CLEAR_FOW
}PLAYER_COMMAND;

PLAYER_COMMAND PLAYER_getCommand(char c);
object_t* PLAYER_obj(tile_t* tile, player_t* player);
player_t* PLAYER_new();
void PLAYER_infoDraw();

#endif	/* __PLAYER__ */

