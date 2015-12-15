#include "teleport.h"
#include "player.h"
#include "object.h"
#include "level.h"
#include "CACaves/CACaves.h"
#include "KeyPress/color.h"

#include <stdlib.h>

extern object_t* player;

void TELEPORT_interact(object_t* teleport){
    object_t* obj;
    tile_t* tile;
    
    obj = OBJECT_at(teleport->data.teleport->targetTile->row, teleport->data.teleport->targetTile->col);
    
    if(obj){
       tile = LEVEL_getEmptyTileInRoom(teleport->data.teleport->targetRoom);
       OBJECT_move(obj, tile->row, tile->col);
    }
    
    OBJECT_move(player, teleport->data.teleport->targetTile->row, teleport->data.teleport->targetTile->col);
}

void TELEPORT_step(object_t* teleport){
    return;
}

object_t* TELEPORT_new(tile_t* tile, tile_t* dest, room_t* targetRoom){
    object_t* new;
    teleport_t* teleport;
    
    new = malloc(sizeof(object_t));
    teleport = malloc(sizeof(teleport_t));
    
    new->tile = tile;
    new->data.teleport = teleport;
    new->flags = OBJ_F_INTERACTIVE;
    new->type = OBJ_TELEPORT;
    new->description = "Teleport to another room in this level!";
    new->objectName = "Teleport";
    new->sprite = 'O';
    teleport->targetTile = dest;
    teleport->targetRoom = targetRoom;
    new->interact = TELEPORT_interact;
    new->step = TELEPORT_step;
    new->spriteColor = COLOR_CODE_KCYN;
    new->textAttrib = ATTRIB_CODE_BOLD;
    
    tile->data = new;
    
    return new;
    
}
