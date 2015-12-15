#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "CACaves.h"

void FLOOR_destroy(floor_t* f);
void delRoom(cave_t *cave, int i, int j, char mark);
floor_t* FLOOR_new(int maxRows);

room_t* ROOM_new(int x, int y, char id, int maxHeight){
    room_t* new;
    new = malloc(sizeof(room_t));
    
    new->id = id;
    new->size = 0;
    new->floor = FLOOR_new(maxHeight);
    new->xOffset = x;
    new->yOffset = y;
    
    return new;
}

void* ROOM_destroy(room_t* r){
    FLOOR_destroy(r->floor);
    free(r);
}

tile_t* ROOM_getRandomTile(room_t* r){
    int i, j;
    
    i = r->floor->top + rand() / (RAND_MAX / (r->floor->bottom - r->floor->top + 1) + 1);
    j = rand() / (RAND_MAX / r->floor->numTiles[i] + 1);
    
    return &(r->floor->tiles[i][j]);
}

floor_t* FLOOR_new(int maxRows){
    floor_t *new;
    int i;
    
    new = malloc(sizeof(floor_t));
    
    new->maxHeight = maxRows;
    new->height = maxRows;
    new->width = 0;
    new->numTiles = calloc(maxRows, sizeof(int));
    new->alocTiles = calloc(maxRows, sizeof(int));
    
    new->tiles = calloc(maxRows, sizeof(tile_t*));
    for(i = 0; i < maxRows; i++){
        new->tiles[i] = calloc(TILEINCREMENT, sizeof(tile_t));
        new->alocTiles[i] = TILEINCREMENT;
    }
    
    new->top = 0;
    new->bottom = 0;
    new->left = 0;
    new->right = 0;
    new->far_bottom = NULL;
    new->far_left = NULL;
    new->far_top = NULL;
    new->far_right = NULL;
    
    return new;
}

void FLOOR_destroy(floor_t* f){
    int i;
    
    free(f->numTiles);
    free(f->alocTiles);
    
    for(i = 0; i < f->maxHeight; i++){
        free(f->tiles[i]);
    }
    free(f->tiles);
    
    free(f);
}

tile_t* getTileBSearch(tile_t *t, int col, int l, int r){
    int i;
    int m;
    
    if(l > r)
        return NULL;
    
    m = (l + r)/2;
    
    if(t[m].col == col){
        return &t[m];
    }
    else{
        if(col < t[m].col)
            return getTileBSearch(t, col, l, m-1);
        else
            return getTileBSearch(t, col, m+1, r);
    }
}

tile_t* FLOOR_getTile(floor_t *f, int row, int col){
    if(f->numTiles[row])
        return getTileBSearch(f->tiles[row], col, 0, f->numTiles[row]);
    return NULL;
}

void insert(tile_t* t, int row, int col, void* data, char roomID, int k){
    k--;
    while((t[k].col > col) && (k >= 0)){
        t[k+1] = t[k];
        k--;
    }
    t[k+1].col = col;
    t[k+1].row = row;
    t[k+1].data = data;
    t[k+1].roomID = roomID;
    
    return;
}

int FLOOR_addTile(floor_t *f, int row, int col, void* data, char roomID){
    tile_t* t;
    t = FLOOR_getTile(f, row, col);
    
    if(t)
        return 0;
    
    f->numTiles[row]++;
    
    if(f->numTiles[row] >= f->alocTiles[row]){
        f->alocTiles[row] += TILEINCREMENT;
        f->tiles[row] = realloc(f->tiles[row], (sizeof(tile_t)) * f->alocTiles[row]);
    }
    
    insert(f->tiles[row], row, col, data, roomID, f->numTiles[row]-1);
    
    return 1;
}

int FLOOR_removeTile(floor_t *f, int row, int col){
    
}

void FLOOR_calcLimits(floor_t *f){
    int i, top, bottom, left, right;
    int m;
       
    //find the top line
    i = 0;
    while((!f->numTiles[i]) && (i < f->height)) i++;
    top = i;
    bottom  = i;
    
    //find the last line
    while(i < f->height){
        if(f->numTiles[i])
            bottom = i;
        i++;
    }
    
    //setting far top tile
    f->far_top = &(f->tiles[top][0]);
    
    //setting far bottom tile
    f->far_bottom = &(f->tiles[bottom][f->numTiles[bottom]-1]);    
    
    //find first and last columns
    left = f->tiles[top][0].col;
    right = f->tiles[top][f->numTiles[top]-1].col;
    for(i = top+1; i <= bottom; i++){
        if(f->numTiles[i]){
            //Find left limit
            if(f->tiles[i][0].col < left){
                left = f->tiles[i][0].col;
                f->far_left = &(f->tiles[i][0]);
            }
            
            //find right limit
            if(f->tiles[i][f->numTiles[i]-1].col > right){
                right = f->tiles[i][f->numTiles[i]-1].col;
                f->far_right = &(f->tiles[i][f->numTiles[i]-1]);
            }
        }
    }
    
    f->height = bottom - top + 1;
    f->width = right - left + 1;
    f->right = right;
    f->left = left;
    f->top = top;
    f->bottom = bottom;
    
    return;
}

void FLOOR_print(floor_t *f){
    int i, k;
    printf("The Floor:\n");
    for(i = f->top; i <= f->bottom; i++){
        printf("Row %d: ", i);
        for(k = 0; k < f->numTiles[i]; k++){
            printf("(%d, %p) ", f->tiles[i][k].col, f->tiles[i][k].data);
        }
        printf("\n");
    }
    return;
}

tile_t* ROOM_getTile(room_t *r, int row, int col){
    return FLOOR_getTile(r->floor, row, col);
}

room_t* CAVE_addRoom(cave_t* cave, char id, int x, int y, int maxHeight){
    room_t *r;
    if(cave->nRooms >= cave->alocRooms){
        cave->alocRooms += ROOMINCREMENT;
        cave->rooms = realloc(cave->rooms, cave->alocRooms * sizeof(room_t*));
    }
    
    r = ROOM_new(x, y, id, maxHeight);
    
    cave->rooms[cave->nRooms] = r;

    cave->nRooms++;
    
    return r;
}

int CAVE_removeRoom(cave_t* cave, char id){
    int caveIndex = -1;
    room_t *r;
    int i;
    
    for(i = 0; i < cave->nRooms; i++){
        if(cave->rooms[i]->id == id){
            caveIndex = i;
            break;
        }
    }
    
    if(caveIndex == -1){
        return 0;
    }
    
    r = cave->rooms[caveIndex];
    
    delRoom(cave, r->floor->far_top->row, r->floor->far_top->col, id);
    
    for(i = caveIndex; i < (cave->nRooms-1); i++){
        cave->rooms[i] = cave->rooms[i+1];
    }
    
    cave->nRooms--;
    
    ROOM_destroy(r);
    
    return 1;
}

cave_t* randomCave(int width, int height, float wallRatio){
    cave_t* cave;
    int i;

    cave = (cave_t*) malloc(sizeof(cave_t));

    cave->data = (char*) malloc(sizeof(char)* width * height);

    cave->height = height;
    cave->width = width;
    cave->wallRatio = wallRatio;
    cave->currentWallRatio = wallRatio;
    cave->nRooms = 0;
    cave->alocRooms =  ROOMINCREMENT;
    cave->rooms = calloc(ROOMINCREMENT, sizeof(room_t*));
    
    for (i = 0; i < width * height; i++){
        float random = ((rand() % 100) / 100.0f);
        cave->data[i] =  random < wallRatio ? CAVEWALL : CAVEFLOOR;
    }

    cave->mData = (char**)malloc(sizeof(char*)* cave->height);

    for (i = 0; i < cave->height; i++){
        cave->mData[i] = &(cave->data[i*cave->width]);
    }

    return cave;
}

void CAVE_print(cave_t* cave){
	int i;

	printf("Wall/Floor Ratio: %f%%\n", cave->currentWallRatio * 100);

	for (i = 0; i < cave->height * cave->width; i++){
		if (!(i % cave->width))
			printf("\n   ");
		else{
			printf("%c", cave->data[i]);
			/*if (cave->data[i] == CAVEFLOOR || cave->data[i] == CAVEWALL)
				cfprintf(stdout, "whiteblack{%c}", cave->data[i]);
			else
				cfprintf(stdout, "redblack{%c}", cave->data[i]);*/
		}
	}

	printf("\n\n");
        
        printf("Rooms:\n\n");
        
        for(i = 0; i < cave->nRooms; i++){
            printf("Room %c\n", cave->rooms[i]->id);
            printf("Room size: %d\n", cave->rooms[i]->size);
            FLOOR_print(cave->rooms[i]->floor);
            printf("far left room: (%d, %d)\n", cave->rooms[i]->floor->far_left->row, cave->rooms[i]->floor->far_left->col);
            printf("far right room: (%d, %d)\n", cave->rooms[i]->floor->far_right->row, cave->rooms[i]->floor->far_right->col);
            printf("far top room: (%d, %d)\n", cave->rooms[i]->floor->far_top->row, cave->rooms[i]->floor->far_top->col);
            printf("far bottom room: (%d, %d)\n", cave->rooms[i]->floor->far_bottom->row, cave->rooms[i]->floor->far_bottom->col);
            printf("\n\n");
        }
        
}

void caCave(cave_t* cave, int mutations){
    int i, j, k, l, sum, totalWalls = 0;
    char** newCave;

    newCave = (char**)malloc(sizeof(char*) * cave->height);

    for (i = 0; i < cave->height; i++){
        newCave[i] = (char*)calloc(cave->width, sizeof(char));
    }

    for (i = 0; i < cave->height; i++){
        for (j = 0; j < cave->width; j++){
            if (!i || !j || j == 1 || i == 1 || (i == cave->height - 2) || (j == cave->width - 2) ||(i == cave->height - 1) || (j == cave->width)){
                newCave[i][j] = CAVEWALL;
                totalWalls++;
                continue;
            }
            else{
                sum = 0;

                for (k = -1; k < 2; k++){
                        for (l = -1; l < 2; l++){
                                sum += (cave->mData[i + k][j + l] == CAVEWALL) ? 1 : 0;
                        }
                }
                newCave[i][j] = (sum >= 5) ? CAVEWALL : CAVEFLOOR;


                //newCave[i][j]

                if (mutations){
                        if (((rand() % 100) / 100.0f) < .005f){
                                newCave[i][j] = (newCave[i][j] == CAVEWALL) ? CAVEFLOOR : CAVEWALL;
                        }
                }

                totalWalls += (newCave[i][j] == CAVEWALL) ? 1 : 0;
            }
        }
    }

    for (i = 0; i < cave->height; i++){
        for (j = 0; j < cave->width; j++){
            cave->mData[i][j] = newCave[i][j];
        }
        free(newCave[i]);
    }

    free(newCave);

    cave->currentWallRatio = (float)totalWalls / (float)(cave->height * cave->width);
}

int fillCave(cave_t *cave, int i, int j, char mark, floor_t *f){
    int k, l;
    int size = 0;

    cave->mData[i][j] = CAVEFILLING;

    for (k = -1; k < 2; k++){
        if ((i + k) < 0 || (i + k) >= cave->height)
                continue;

        for (l = -1; l < 2; l++){
            if ((j + l) < 0 || (j + l) >= cave->width)
                continue;

            if (cave->mData[i + k][j + l] == CAVEFLOOR){
                size += fillCave(cave, i + k, j + l, mark, f);

            }

        }

    }

    cave->mData[i][j] = mark;
    
    FLOOR_addTile(f, i, j, NULL, mark);
    
    return 1+size;

}

int fillCaves(cave_t *cave){
    int i, j, mark = 0;
    int size = 0;

    for (i = 0; i < cave->height; i++){
            for (j = 0; j < cave->width; j++){
                    if (cave->mData[i][j] == CAVEFLOOR){
                            room_t* r = CAVE_addRoom(cave, 'A' + mark, i, j, cave->height);
                            size = fillCave(cave, i, j, 'A' + mark, r->floor);
#ifdef CA_DEBUG
                            printf("Cave %c size: %d\n", 'A' + mark, size);
#endif
                            r->size = size;
                            FLOOR_calcLimits(r->floor);
                            size = 0;
                            mark++;
                    }
            }
    }

}

int any(char c, char* s){
    int tam, i;
    tam = strlen(s);
    for(i = 0; i < tam; i++){
        if(s[i]>=c){
            return s[i] > c ? 0 : 1;
        }
    }
    return 0;
}

void delRoom(cave_t *cave, int i, int j, char mark){
    int k, l;
    
    cave->mData[i][j] = CAVEFILLING;

    for (k = -1; k < 2; k++){
        if ((i + k) < 0 || (i + k) >= cave->height)
            continue;
        	for (l = -1; l < 2; l++){
                    if ((j + l) < 0 || (j + l) >= cave->width)
                        continue;

                    if (cave->mData[i + k][j + l] == mark){
                        delRoom(cave, i + k, j + l, mark);
                    }

		}
    }

    cave->mData[i][j] = CAVEWALL;
}

void CAVE_removeRooms(cave_t *cave, char *caves){
	
    int i;
    
    for(i = 0; i < strlen(caves); i++){
        CAVE_removeRoom(cave, caves[i]);
    }
    
        /*int i, j;

	for (i = 0; i < cave->height; i++){
		for (j = 0; j < cave->width; j++){
			if (any(cave->mData[i][j], caves)){
				delRoom(cave, i, j);
			}
		}
	}*/

}


void writePPM(cave_t *cave, char* outfile){
	FILE *out = NULL;
	int i, j;
	char c;

	out = fopen(outfile, "w");

	if (!out)
		exit(1);

	fprintf(out, "P1\n");

	fprintf(out, "%d %d\n", cave->width, cave->height);

	for (i = 0; i < cave->height; i++){
		for (j = 0; j < cave->width; j++){
			c = cave->mData[i][j] == CAVEFLOOR ? '1' : '0';
			c = (i == 0) || (j == 0) || (j == cave->width - 1) || (i == cave->height - 1) ? '0' : c;
			fprintf(out, "%c ", c);
		}
		fprintf(out, "\n");
	}

	fclose(out);

}

cave_t* CAVE_getRooms(int width, int height, float wallRatio, int iterations){
    cave_t* cave;
    int it;
    
    it = 0;
    cave = randomCave(width, height, wallRatio);
    
    do{
        caCave(cave, 0);
        it++;
    } while((it <=iterations)); 
    
    fillCaves(cave);
    
    return cave;
}

cave_t* CAVE_getRoomsAverage(cave_t* cave, float deviation){
    
    float acum;
    float average;
    char* toRemove;
    char room[2];
    int i;
    
    room[1] = 0;
    
    toRemove = calloc(cave->nRooms+1, sizeof(char));
    acum = 0;
    
    for(i = 0; i < cave->nRooms; i++){
        acum += ((float)cave->rooms[i]->size);
    }
    average = acum / cave->nRooms;
    
    for(i = 0; i < cave->nRooms; i++){
        if((((float)cave->rooms[i]->size)) < (average - (average*deviation))){
            room[0] = cave->rooms[i]->id;
            strcat(toRemove, room);
        }
    }
    
    #ifdef CA_DEBUG
    CAVE_print(cave);
    printf("average room size: %.2f (cut = %.2f)\n", average, average - (average*deviation));
    #endif

    CAVE_removeRooms(cave, toRemove);
    
    return cave;    
    
}

cave_t* CAVE_getRoomsSize(cave_t* cave, float keepRooms){
    char* toRemove;
    char room[2];
    int largest;
    int i;
    
    room[1] = 0;
    
    toRemove = calloc(cave->nRooms+1, sizeof(char));
    largest  = 0;
    
    for(i = 0; i < cave->nRooms; i++){
        if(cave->rooms[i]->size > cave->rooms[largest]->size){
            largest = i;
        }
    }
    
    for(i = 0; i < cave->nRooms; i++){
        if((((float)cave->rooms[i]->size) / cave->rooms[largest]->size ) < keepRooms){
            room[0] = cave->rooms[i]->id;
            strcat(toRemove, room);
        }
    }

    CAVE_removeRooms(cave, toRemove);
    
    return cave;
}

cave_t* CAVE_getLargeRooms(cave_t* cave){
    return CAVE_getRoomsSize(cave, 0.5f);
}

tile_t* CAVE_getTile(cave_t* cave, int row, int col){
    tile_t* ret = NULL;
    int i;
    
    for(i = 0; i < cave->nRooms; i++){
        ret = ROOM_getTile(cave->rooms[i], row, col);
        if (ret)
            break;
    }
    
    return ret;
}

void clearRoom(cave_t *cave, int i, int j, char mark, char roomID){
    int k, l;
    tile_t *t;
    
    cave->mData[i][j] = CAVEFILLING;

    for (k = -1; k < 2; k++){
        if ((i + k) < 0 || (i + k) >= cave->height)
            continue;
        	for (l = -1; l < 2; l++){
                    if ((j + l) < 0 || (j + l) >= cave->width)
                        continue;

                    if (cave->mData[i + k][j + l] == mark){
                        clearRoom(cave, i + k, j + l, mark, roomID);
                    }

		}
    }
    //t = CAVE_getTile(cave, i, j);
    //t->roomID = roomID;
    cave->mData[i][j] = CAVEFLOOR;
}

cave_t* CAVE_clearRooms(cave_t* cave){
    int i;
    char mark;
    for(i = 0; i < cave->nRooms; i++){
        mark = cave->rooms[i]->id;
        clearRoom(cave, cave->rooms[i]->floor->far_top->row, cave->rooms[i]->floor->far_top->col, mark, (char)i + 'a');
    }
    
    return cave;
}

float TILE_eucDistance(tile_t* a, tile_t* b){
    float x1, y1, x2, y2;
    
    x1 = a->col;
    x2 = b->col;
    y1 = a->row;
    y2 = b->row;
    
    return sqrt(pow((y2-y1),2) + pow((x2-x1),2));
    
}

