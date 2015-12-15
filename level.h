#include "CACaves/CACaves.h"
#include "object.h"

#ifndef __LEVEL__
#define	__LEVEL__

#define OBJSALLOCSTEP 10

typedef struct level_t{
    int number;
    int seed;
    cave_t *cave;
    char** fow;
    object_t **obj;
    int nObjs;
    int allocObjs;
}level_t;

typedef struct roulette_t{
    int *p;
    int *label;
    int num;
    int pos;
    int totalAcum;
}roulette_t;

roulette_t* ROULETTE_new(int nLabels);
void ROULETTE_addLabel(roulette_t* r, int label, int p);
void ROULETTE_calcP(roulette_t* r);
int ROULETTE_get(roulette_t* r);
void ROULETTE_clear(roulette_t* r);


level_t* LEVEL_new(player_t *player, int n, int randomSeed, int width, int height, float wallRatio, int iterations);
void LEVEL_draw();
void LEVEL_clearFOW(level_t* l, int centerRow, int centerCol, int radius);
void LEVEL_clearAllFOW(level_t* l);
tile_t* LEVEL_getEmptyTileInRoom(room_t* r);

#endif	/* __LEVEL__ */

