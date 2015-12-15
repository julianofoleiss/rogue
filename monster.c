#include <stdlib.h>
#include <stdio.h>
#include "CACaves/CACaves.h"
#include "object.h"
#include "monster.h"
#include "player.h"
#include "game.h"
#include "KeyPress/color.h"
#include "msgBox.h"
#include "utils.h"

extern object_t* player;
extern GAME_STATES gameState;
extern int gameNotOver;
extern msgBox_t* msgBox;
extern stringPool_t *stringPool;

void MONSTER_followPlayer(object_t* mon){
monster_t* monster;
    int random;
    
    monster = mon->data.monster;
    
    if(TILE_eucDistance(mon->tile, player->tile) < 7){
    
        if(monster->moveDelay != -1){
            monster->moveDelay--;
            if(monster->moveDelay <= 0){
                monster->moveDelay = monster->moveDelayIni;
                random = rand() / (RAND_MAX / 101);

                if(random < 50){
                    if(mon->tile->col > player->tile->col)
                        OBJECT_move(mon, mon->tile->row, mon->tile->col-1);
                    else{
                        if(mon->tile->col < player->tile->col)
                            OBJECT_move(mon, mon->tile->row, mon->tile->col+1);
                    }
                }
                else{
                    if(mon->tile->row > player->tile->row)
                        OBJECT_move(mon, mon->tile->row-1, mon->tile->col);
                    else{
                        if(mon->tile->row < player->tile->row)
                            OBJECT_move(mon, mon->tile->row+1, mon->tile->col);
                    }
                }

            }
        }    
    }
}

void MONSTER_endOfBattle(object_t* obj){
    
    char* msg = STRING_POOL_getString(stringPool);
    
    obj->tile->data = NULL;
    //TODO:delete all data.
    player->data.player->gold += obj->data.monster->gold;
    player->data.player->exp += obj->data.monster->xp;
    player->data.player->next -= obj->data.monster->xp;
    obj->active = 0;

    sprintf(msg, "You've smashed a %s%s%s!%s Gained %d exp and %d gold!", obj->textAttrib, obj->spriteColor, obj->objectName, COLOR_CODE_RESET, obj->data.monster->xp, obj->data.monster->gold);
    MSGBOX_add(msgBox, msg);
}

void MONSTER_battle(object_t* obj){
 
    monster_t* monster;
    player_t* player_data;
    int random;
    
    int weapon = 3; //isso é um placeholder para o item de ataque
    int armor = 2; //placeholder para as armaduras
    int base;
    int damage;
    int multiplier;
    int h;
    char* msg;
    
    
    monster = obj->data.monster;
    
    player_data = player->data.player;
    
    //check if the attack hits!
    if(player_data->hit >= monster->evasion)
        h = 99;
    else{
        h = (player_data->hit * 100) / monster->evasion;
    }
    random = getRandomNumber(0, 100);
    
    if(h >= random){
        //TODO:move this calculation to PLAYER (this will allow many classes)
        base = ((player->data.player->power * 12) + (weapon*5))/18;

        random = player->data.player->power / 3;
        
        multiplier = 256 - monster->def;
        
        damage = ((base * multiplier)/256) + (((char)getRandomNumber(0, random)) & 0xFF);
        if(damage <= 0)
            damage = 1;
        
        //critical hit?
        if(getRandomNumber(0, 100) <= player->data.player->hit){
            damage = damage * 2;
            msg = STRING_POOL_getString(stringPool);
            sprintf(msg, "%sA critical hit!%s", COLOR_CODE_KRED, COLOR_CODE_RESET);
            MSGBOX_add(msgBox, msg);
        }
        
        msg = STRING_POOL_getString(stringPool);
        sprintf(msg, "You've dealt %d damage to the %s%s%s%s!", damage, obj->textAttrib, obj->spriteColor, obj->objectName, COLOR_CODE_RESET);
        MSGBOX_add(msgBox, msg);
        
        monster->hp -=  damage;

        if(monster->hp <= 0){
            MONSTER_endOfBattle(obj);
            return;
        }
    
    }
    else{
        MSGBOX_addConstString(msgBox, "You missed the attack!");
    }
   
    if(monster->attackDelay == MONSTER_NOT_ATTACKED){
        monster->attackDelay = player_data->speed;
    }
    
    monster->attackDelay -= monster->speed;
    
    if(monster->attackDelay < 0){
    
        damage = (monster->atk * (256 - (player->data.player->stamina + armor)))/256;
        if(damage <= 0)
            damage = 1;

        msg = STRING_POOL_getString(stringPool);
        sprintf(msg, "Received %s%d damage!%s", COLOR_CODE_KRED, damage, COLOR_CODE_RESET);
        MSGBOX_add(msgBox, msg);

        player->data.player->hp -= damage;

        if(player->data.player->hp <= 0){
            gameNotOver = 0;
        }  
        
        monster->attackDelay+= player_data->speed;
        
    }
    
}

void MONSTER_slimeStep(object_t* obj){
    
    MONSTER_followPlayer(obj);
    
}

void MONSTER_slimeInteract(object_t* obj){
    
    MONSTER_battle(obj);
    
}

object_t* MONSTER_newSlime(tile_t *tile){
    //blue imp
    object_t* new;
    monster_t* monster;
    slime_t* slime;
    
    new = malloc(sizeof(object_t));
    monster = malloc(sizeof(monster_t));
    slime = malloc(sizeof(slime_t));
    
    monster->level = 0;
    monster->type = MON_SLIME;
    monster->xp = 2;
    monster->hp = 13;
    monster->def = 127;
    monster->gold = 12;
    monster->atk = 4;
    monster->speed = 8;
    monster->maxHp = monster->hp;
    monster->attackDelay = MONSTER_NOT_ATTACKED;
    monster->magic = 1;
    monster->magDef = 50;
    monster->evasion = 6;
    monster->hit = 8;    
    monster->monsterData.slime = slime;
    monster->moveDelayIni = 2;
    monster->moveDelay = monster->moveDelayIni;
    
    new->active = 1;
    new->data.monster = monster;
    new->description = "Harmless industrial slime";
    new->objectName = "Slime";
    new->flags = OBJ_F_INTERACTIVE;
    new->sprite = '&';
    new->tile = tile;
    new->type = OBJ_MONSTER;
    new->spriteColor = COLOR_CODE_KGRN;
    new->textAttrib = ATTRIB_CODE_BOLD;
            
    new->step = MONSTER_slimeStep;
    new->interact = MONSTER_slimeInteract;
    
    tile->data = new;
    
    return new;    
}

void MONSTER_ratStep(object_t* obj){
    
    MONSTER_followPlayer(obj);
    
}

void MONSTER_ratInteract(object_t* obj){
    
    MONSTER_battle(obj);
    
}

object_t* MONSTER_newRat(tile_t* tile){
    //hetake
    object_t* new;
    monster_t* monster;
    rat_t* rat;
    
    new = malloc(sizeof(object_t));
    monster = malloc(sizeof(monster_t));
    rat = malloc(sizeof(rat_t));
            
    monster->level = 1; // na realidade é 1
    monster->type = MON_RAT;
    monster->xp = 3;
    monster->hp = 14;
    monster->def = 127;
    monster->gold = 16;
    monster->atk = 4;
    monster->speed = 5;
    monster->maxHp = monster->hp;
    monster->attackDelay = MONSTER_NOT_ATTACKED;
    monster->magic = 2;
    monster->magDef = 50;
    monster->evasion = 6;
    monster->hit = 8;     
    monster->monsterData.rat = rat;
    monster->moveDelayIni = 1;
    monster->moveDelay = monster->moveDelayIni;
    
    new->active = 1;
    new->data.monster = monster;
    new->description = "Rats are usually found under tavern basements.";
    new->objectName = "Nasty rat";
    new->flags = OBJ_F_INTERACTIVE;
    new->sprite = '&';
    new->tile = tile;
    new->type = OBJ_MONSTER;
    new->spriteColor = COLOR_CODE_KYEL;
    new->textAttrib = ATTRIB_CODE_BOLD;
            
    new->step = MONSTER_ratStep;
    new->interact = MONSTER_ratInteract;
    
    tile->data = new;
    
    return new;
}

void MONSTER_skeletonStep(object_t* obj){
    
    MONSTER_followPlayer(obj);
    
}

void MONSTER_skeletonInteract(object_t* obj){
    
    MONSTER_battle(obj);
    
}


object_t* MONSTER_newSkeleton(tile_t* tile){
    //roly rider
    object_t* new;
    monster_t* monster;
    skeleton_t* skeleton;
    
    new = malloc(sizeof(object_t));
    monster = malloc(sizeof(monster_t));
    skeleton = malloc(sizeof(skeleton_t));
            
    monster->level = 2; // na realidade é 1
    monster->type = MON_SKELETON;
    monster->xp = 5;
    monster->hp = 30;
    monster->def = 127;
    monster->gold = 40;
    monster->atk = 6;
    monster->speed = 7;
    monster->maxHp = monster->hp;
    monster->attackDelay = MONSTER_NOT_ATTACKED;
    monster->magic = 1;
    monster->magDef = 50;
    monster->evasion = 6;
    monster->hit = 8;     
    monster->monsterData.skeleton = skeleton;
    monster->moveDelayIni = 1;
    monster->moveDelay = monster->moveDelayIni;
    
    new->active = 1;
    new->data.monster = monster;
    new->description = "Living-dead monster";
    new->objectName = "Skeleton";
    new->flags = OBJ_F_INTERACTIVE;
    new->sprite = '&';
    new->tile = tile;
    new->type = OBJ_MONSTER;
    new->spriteColor = COLOR_CODE_KWHT;
    new->textAttrib = ATTRIB_CODE_BOLD;
            
    new->step = MONSTER_skeletonStep;
    new->interact = MONSTER_skeletonInteract;
    
    tile->data = new;
    
    return new;    
}



object_t* MONSTER_newKabold(tile_t* tile){
    
}

monsterFactory_t* MONSTER_FACTORY_new(int levels){
    monsterFactory_t* new;
    int i;
    
    new = malloc(sizeof(monsterFactory_t));
    
    new->levels = levels;
    new->aloc = malloc(sizeof(int) * levels);
    new->size = calloc(levels, sizeof(int));
    new->factory = malloc(sizeof(MonsterFactory**) * levels);
    
    for(i = 0; i < levels; i++){
        new->aloc[i] = MONSTER_FACTORY_ALLOC_INC;
        new->factory[i] = malloc(sizeof(void*) * new->aloc[i]);
    }
    
    return new;
}

void MONSTER_FACTORY_addMonster(monsterFactory_t* factory,int monsterLevel, MonsterFactory f){
    if(factory->size[monsterLevel] >= factory->aloc[monsterLevel]){
        factory->aloc[monsterLevel] += MONSTER_FACTORY_ALLOC_INC;
        factory->factory[monsterLevel] = realloc(factory->factory[monsterLevel], factory->aloc[monsterLevel] * sizeof(void*));
    }
    
    factory->factory[monsterLevel][factory->size[monsterLevel]] = f;
    
    factory->size[monsterLevel]++;
}

MonsterFactory MONSTER_FACTORY_getRandomFactory(monsterFactory_t *factory, int level){
    int random;
    
    random = rand() / (RAND_MAX / factory->size[level]);
   
    return factory->factory[level][random];
}
