#include "level.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "CACaves/CACaves.h"
#include "player.h"
#include "teleport.h"
#include "monster.h"
#include "KeyPress/color.h"
#include "msgBox.h"
#include "utils.h"

extern level_t* currentLevel;
extern object_t* player;
extern msgBox_t* msgBox;
extern monsterFactory_t *monsterFactory;

roulette_t* ROULETTE_new(int nLabels){
    roulette_t* r;
    r = malloc(sizeof(roulette_t));
    
    r->num = nLabels;
    r->pos = 0;
    r->label = malloc(sizeof(int) * nLabels);
    r->p = malloc(sizeof(int) * nLabels);
    
    return r;
}

void ROULETTE_addLabel(roulette_t* r, int label, int p){
    r->label[r->pos] = label;
    r->p[r->pos] = p;
    r->pos++;
}

void ROULETTE_calcP(roulette_t* r){
    int i;
    int acum = 0;
    int aux;
    for(i = 0; i < r->num;i++){
        aux = r->p[i];
        r->p[i] = acum + aux;
        acum += aux;
    }
    r->totalAcum = acum;
}

int ROULETTE_get(roulette_t* r){
    int i;
    int random;
    
    random = getRandomNumber(0, r->totalAcum);
    
    for(i = 0; i < r->num; i++){
        if(random < r->p[i]){
            return r->label[i]; 
        }
    }
    
    return -200;
}

void ROULETTE_clear(roulette_t* r){
    r->totalAcum = 0;
    r->pos = 0;
}

/* Arrange the N elements of ARRAY in random order.
   Only effective if N is much smaller than RAND_MAX;
   if this may not be the case, use a better random
   number generator. */
void shuffle(int *array, int n)
{
    if (n > 1) {
        int i;
	for (i = 0; i < n - 1; i++) {
	  int j = i + rand() / (RAND_MAX / (n - i) + 1);
	  int t = array[j];
	  array[j] = array[i];
	  array[i] = t;
	}
    }
}

void LEVEL_addObj(level_t *l, object_t* o){
    
    if(l->nObjs >= l->allocObjs){
        l->allocObjs += OBJSALLOCSTEP;
        l->obj = realloc(l->obj, l->allocObjs);
    }
    
    l->obj[l->nObjs] = o;
    l->nObjs++;
    
    return;
}

tile_t* LEVEL_freeAround(level_t *l, tile_t* t){
    
    tile_t* t2;
    
    t2 = CAVE_getTile(l->cave, t->row-1, t->col);
    if(t2)
        return t2;
    
    t2 = CAVE_getTile(l->cave, t->row+1, t->col);
    if(t2)
        return t2;
    
    t2 = CAVE_getTile(l->cave, t->row, t->col-1);
    if(t2)
        return t2;
    
    t2 = CAVE_getTile(l->cave, t->row, t->col+1);
    if(t2)
        return t2;
    
    t2 = CAVE_getTile(l->cave, t->row-1, t->col-1);
    if(t2)
        return t2;
    
    t2 = CAVE_getTile(l->cave, t->row-1, t->col+1);
    if(t2)
        return t2;
    
    t2 = CAVE_getTile(l->cave, t->row+1, t->col-1);
    if(t2)
        return t2;
    
    t2 = CAVE_getTile(l->cave, t->row+1, t->col+1);
    if(t2)
        return t2;
    
    return NULL;
}

void LEVEL_clearFOW(level_t* l, int centerRow, int centerCol, int radius){
    int i, j;
    for(i = -1*radius; i < radius+1; i++){
        for(j = -1*radius; j < radius+1; j++){
            if((centerRow + i < 0) || (centerRow + i >= l->cave->height) || (centerCol + j < 0) || (centerCol + j >= l->cave->width)){
                continue;
            }
            l->fow[centerRow + i][centerCol + j] = 1;
        }
    }
    
}

void LEVEL_clearAllFOW(level_t* l){
    int i, j;
    for(i = 0; i < l->cave->height; i++){
        for(j = 0; j < l->cave->width; j++){
            l->fow[i][j] = 1;
        }
    }
}

tile_t* LEVEL_getEmptyTileInRoom(room_t* r){
    tile_t* ret;
    
    while(ret = ROOM_getRandomTile(r)){
        if(!(ret->data))
            break;
    }    
    
    return ret;
}

int LEVEL_spawnEnemies(level_t* l){
    int numEnemies;
    int roomEnemies;
    int totalEnemies = 0;
    int totalArea;
    int i, k;
    int monsterLvl;
    float p;
    tile_t* tile;
    object_t* monster_obj;
    
    MonsterFactory f;
    roulette_t* r;
    
    r = ROULETTE_new(3);
    ROULETTE_addLabel(r, -1, 70);
    ROULETTE_addLabel(r, 0, 20);
    ROULETTE_addLabel(r, 1, 10);
    ROULETTE_calcP(r);
    
    for(i = 0; i < l->cave->nRooms; i++){
        totalArea += l->cave->rooms[i]->size;
    }
    
    numEnemies = ceil((l->number * 1.25f) + (player->data.player->level * 1.33f));
    
    //printf("number of enemies to spawn: %d\n", numEnemies);
    
    for(i = 0; i < l->cave->nRooms; i++){
        p = ((float)l->cave->rooms[i]->size) / totalArea;
        //printf("room %c is %.3f of the cave floor\n", (char)i+ 'a', p);
        
        roomEnemies = ceil(p*(float)numEnemies);
        
        totalEnemies = 0;
        
        //printf("will have %d enemies\n", roomEnemies);
        
        for(k = 0; k < roomEnemies; k++){
            monsterLvl = player->data.player->level + ROULETTE_get(r);
            f = MONSTER_FACTORY_getRandomFactory(monsterFactory, monsterLvl);
            tile = LEVEL_getEmptyTileInRoom(l->cave->rooms[i]);
            monster_obj = f(tile);
            LEVEL_addObj(l, monster_obj);
        }
        
    }
    
    
    
/*tile = LEVEL_getEmptyTileInRoom(new->cave->rooms[0]);
    MonsterFactory f = MONSTER_FACTORY_getRandomFactory(monsterFactory, 2);
    object_t*  monster = f(tile);
    LEVEL_addObj(new, monster);*/    
    
    return numEnemies;
    
}

int LEVEL_spawnChests(level_t* l){
    
}

level_t* LEVEL_new(player_t *player_data, int n, int randomSeed, int width, int height, float wallRatio, int iterations){
    level_t *new;
    tile_t *tile, *destination, *player_tile;
    object_t *obj;
    tile_t* farthest, *t;
    float farthest_dist, dist;
    int i;
    
    new = malloc(sizeof(level_t));
    
    new->number = n;
    new->allocObjs = OBJSALLOCSTEP;
    new->nObjs = 0;
    new->obj = calloc(new->allocObjs, sizeof(object_t*));
    new->seed = randomSeed;
    
    srand(randomSeed);
    
    new->cave = CAVE_getRooms(width, height, wallRatio, iterations);
    new->cave = CAVE_getRoomsAverage(new->cave, 0.0f);
    new->cave = CAVE_clearRooms(new->cave);
    
    new->fow = malloc(sizeof(char*) * new->cave->height);
    for(i = 0; i < height; i++){
        new->fow[i] = calloc(new->cave->width, sizeof(char));
    }
    
    int nMonsters;
    int nItems;
    
    //Place player on far top room of the first room
    player_tile = new->cave->rooms[0]->floor->far_top;
    player = PLAYER_obj(player_tile, player_data);
    player_tile->data = player;
    LEVEL_clearFOW(new, player_tile->row, player_tile->col, 5);        
    
    //make teleports! (except for the last room)
    for(i = 0; i < (new->cave->nRooms-1); i++){
        //First, choose the farthest tile from the initial player position.
        farthest = player_tile;
        farthest_dist = 0;
        if( (dist = TILE_eucDistance(player_tile, new->cave->rooms[i]->floor->far_left)) > farthest_dist){
            farthest_dist = dist;
            farthest = new->cave->rooms[i]->floor->far_left;
        }
        
        if( (dist = TILE_eucDistance(player_tile, new->cave->rooms[i]->floor->far_right)) > farthest_dist){
            farthest_dist = dist;
            farthest = new->cave->rooms[i]->floor->far_right;
        }
        
        if( (dist = TILE_eucDistance(player_tile, new->cave->rooms[i]->floor->far_bottom)) > farthest_dist){
            farthest_dist = dist;
            farthest = new->cave->rooms[i]->floor->far_bottom;
        }        
        
        //teleport destination is always the far top tile of the next room
        destination = new->cave->rooms[i+1]->floor->far_top;
        
        t = LEVEL_freeAround(new, destination);
        
        obj = TELEPORT_new(farthest, t, new->cave->rooms[i+1]);
        farthest->data = obj;
        
        t = LEVEL_freeAround(new, farthest);
        
        obj = TELEPORT_new(destination, t, new->cave->rooms[i]);
        destination->data = obj;
        
        player_tile = destination;
    }
    
    LEVEL_spawnEnemies(new);
    
    /*tile = LEVEL_getEmptyTileInRoom(new->cave->rooms[0]);
    MonsterFactory f = MONSTER_FACTORY_getRandomFactory(monsterFactory, 2);
    object_t*  monster = f(tile);
    LEVEL_addObj(new, monster);*/

    MSGBOX_addConstString(msgBox, "Welcome to rogue!");
    
    return new;
}

void LEVEL_draw(){
    int i, j;
    tile_t* tile;
    
    for(i = 0; i < currentLevel->cave->height; i++){
        for(j = 0; j < currentLevel->cave->width; j++){
            
            if(currentLevel->fow[i][j] == 0){
                printf( "%s " COLOR_CODE_RESET, BCOLOR_CODE_LGRY);
                continue;
            }
            
            tile = CAVE_getTile(currentLevel->cave, i, j);
            
            if(tile){
                if (tile->data){    
                    printf( "%s%s%c" COLOR_CODE_RESET, ((object_t*)tile->data)->spriteColor, ((object_t*)tile->data)->textAttrib, ((object_t*)tile->data)->sprite);
                    continue;
                }
            }
            
            printf("%c", currentLevel->cave->mData[i][j]);
            
        }
        printf("\n");
    }
    
}