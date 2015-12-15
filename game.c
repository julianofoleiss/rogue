#include <stdlib.h>
#include "game.h"
#include "caves.h"
#include "object.h"
#include "level.h"
#include "monster.h"
#include "KeyPress/KeyPress.h"
#include "msgBox.h"

#define STRING_POOL_SIZE 5

extern level_t* currentLevel;
extern object_t* player;
extern GAME_STATES gameState;
extern int gameNotOver;
extern monsterFactory_t *monsterFactory;
extern stringPool_t *stringPool;

void setupMonsters(){
    monsterFactory = MONSTER_FACTORY_new(10);
    MONSTER_FACTORY_addMonster(monsterFactory, 0, MONSTER_newSlime);
    MONSTER_FACTORY_addMonster(monsterFactory, 1, MONSTER_newRat);
    MONSTER_FACTORY_addMonster(monsterFactory, 2, MONSTER_newSkeleton);
}

int main(int argc, char** argv) {

    SetupConsole();
    setupMonsters();
    stringPool = STRING_POOL_new(STRING_POOL_SIZE);
    
    system("clear");
    
    while(gameNotOver){
        switch(gameState){
            case GAME_CAVES:
                CAVES_run();
            break;                
        }
    }
    
    RestoreConsole();
    
    return 0;
}

