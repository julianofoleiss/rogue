#include <stdlib.h>
#include <stdio.h>
#include "KeyPress/KeyPress.h"
#include "player.h"
#include "object.h"
#include "KeyPress/color.h"

extern object_t* player;

PLAYER_COMMAND PLAYER_getCommand(char c){
    if(c == 27){
        c = GetKey();
        if(c == 91){
            c = GetKey();
            switch(c){
                case 65:
                    return PLAYER_CMD_UP;
                case 66:
                    return PLAYER_CMD_DOWN;
                case 67:
                    return PLAYER_CMD_RIGHT;
                case 68:
                    return PLAYER_CMD_LEFT;
                case 72:
                    return PLAYER_CMD_UP_LEFT;
                case 53:
                    return PLAYER_CMD_UP_RIGHT;
                case 70:
                    return PLAYER_CMD_DOWN_LEFT;
                case 54:
                    return PLAYER_CMD_DOWN_RIGHT;
            }
        }
    }
    
    if(c == 'c')
        return PLAYER_CLEAR_FOW;
    if(c == 'q')
        return PLAYER_QUIT_GAME;
}

player_t* PLAYER_new(){
    player_t* player;
    
    player = malloc(sizeof(player_t));
    
    player->exp = 0;
    player->next = 20;
    player->level = 1;
    player->power = 5;
    player->magic = 5;
    player->mdef = 2;
    player->hit = 8;
    player->def = 0;
    player->evd = 8;
    player->stamina = 8;
    player->gold = 0;
    player->speed = 13;
    player->hp = 70;
    player->maxhp = 70;
    
    return player;
}

void PLAYER_infoDraw(){
    player_t* player_data;
    
    player_data = player->data.player;
    
    printf("LEVEL %2d\tXP: %d\tNext: %d\tHP: %d/%d\tPWR: %d\tHIT: %d\tDEF: %d\t EVD: %d\tSPD: %d\tGOLD: %d\t(%d,%d)\n", player_data->level, 
            player_data->exp, 
            player_data->next, 
            player_data->hp,
            player_data->maxhp,
            player_data->power,
            player_data->hit,
            player_data->def, 
            player_data->evd,
            player_data->speed,
            player_data->gold,
            player->tile->row,
            player->tile->col);
}

object_t* PLAYER_obj(tile_t* tile, player_t* player){
    object_t *o;
    
    o = malloc(sizeof(object_t));
    o->data.player = player;
    o->description = "The brave player!";
    o->objectName = "Player";
    o->flags = 0;
    o->interact = NULL;
    o->step = NULL;
    o->type = OBJ_PLAYER;
    o->tile = tile;
    o->sprite = '@';
    o->spriteColor = COLOR_CODE_KRED;
    o->textAttrib = ATTRIB_CODE_BOLD;
    
    return o;
}
