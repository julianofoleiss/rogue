#include <stdlib.h>
#include "level.h"
#include "player.h"
#include "game.h"
#include "msgBox.h"
#include "monster.h"

level_t* currentLevel = NULL;
object_t* player = NULL;
GAME_STATES gameState = GAME_CAVES;
int gameNotOver = 1;
msgBox_t* msgBox = NULL;
monsterFactory_t* monsterFactory = NULL;
stringPool_t* stringPool = NULL;