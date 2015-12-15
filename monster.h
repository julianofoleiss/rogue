#ifndef __MONSTER__
#define	__MONSTER__

#include "CACaves/CACaves.h"
#include "object.h"

#define MONSTER_NOT_ATTACKED -10000

typedef enum MONSTER_TYPE{
    MON_RAT,
    MON_SLIME,
    MON_SKELETON,
    MON_KABOLD
}MONSTER_TYPE;

typedef struct rat_t{
    
}rat_t;

typedef struct slime_t{
    
}slime_t;

typedef struct skeleton_t{
    
}skeleton_t;

typedef struct kabold_t{
    
}kabold_t;

typedef struct monster_t{
    MONSTER_TYPE type;
    int level;
    int hp;
    int maxHp;
    int def;
    int atk;
    int xp;
    int gold;
    int speed;
    int attackDelay;
    int magic;
    int magDef;
    int evasion;
    int hit;
    int moveDelay;
    int moveDelayIni;
    union{
        rat_t* rat;
        slime_t* slime;
        skeleton_t* skeleton;
        kabold_t* kabold;
    }monsterData;
}monster_t;

typedef object_t* (*MonsterFactory)(tile_t*);

typedef struct monsterFactory_t{
    MonsterFactory** factory;
    int levels;
    int *aloc;
    int *size;
}monsterFactory_t;

object_t* MONSTER_newRat(tile_t* tile);
object_t* MONSTER_newSlime(tile_t *tile);
object_t* MONSTER_newSkeleton(tile_t* tile);
object_t* MONSTER_newKabold(tile_t* tile);

#define MONSTER_FACTORY_ALLOC_INC   10

monsterFactory_t* MONSTER_FACTORY_new(int levels);
void MONSTER_FACTORY_addMonster(monsterFactory_t* factory,int monsterLevel, MonsterFactory f);
MonsterFactory MONSTER_FACTORY_getRandomFactory(monsterFactory_t *factory, int level);

#endif	/* __MONSTER__ */

