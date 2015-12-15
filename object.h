#ifndef __OBJECT__
#define	__OBJECT__

#include "CACaves/CACaves.h"

typedef enum objType{
    OBJ_PLAYER,
    OBJ_TELEPORT,
    OBJ_MONSTER,
    OBJ_COLLECTABLE
} objType;

#define OBJ_F_PASSABLE      0x01
#define OBJ_F_INTERACTIVE   0x02

typedef struct teleport_t teleport_t;
typedef struct player_t player_t;
typedef struct monster_t monster_t;

typedef struct object_t{
    objType type;
    tile_t *tile;
    const char* objectName;
    const char* description;
    int flags;
    int active;
    union{
        player_t* player;
        teleport_t* teleport;
        monster_t* monster;
    }data;
    
    char sprite;
    const char* spriteColor;
    const char* textAttrib;
    
    void(*interact)(struct object_t*);
    void(*step)(struct object_t*);
    
}object_t;

int OBJECT_move(object_t* obj, int row, int col);
int OBJECT_swap(object_t* obj1, object_t* obj2);
object_t* OBJECT_at(int row, int col);

#endif	/* OBJECT_H */

