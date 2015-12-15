#include <stdlib.h>
#include "object.h"
#include "CACaves/CACaves.h"
#include "level.h"

extern level_t* currentLevel;

int OBJECT_move(object_t* obj, int row, int col){
    tile_t* dest;
    
    dest = CAVE_getTile(currentLevel->cave, row, col);
    
    if(!dest)
        return 0;
    
    if(dest->data)
        return 0;
    
    obj->tile->data = NULL;
    dest->data = obj;
    obj->tile = dest;
    
    return 1;
    
}

object_t* OBJECT_at(int row, int col){
    tile_t *t;
    
    t = CAVE_getTile(currentLevel->cave, row, col);
    
    return t ? t->data : NULL;
}

int OBJECT_swap(object_t* obj1, object_t* obj2){
    int aux_col, aux_row;
    
    aux_col = obj1->tile->col;
    aux_row = obj1->tile->row;
    
    
    
}
