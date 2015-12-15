#ifndef __CACAVES__
#define __CACAVES__

#define CAVEWALL  '#'
#define CAVEFLOOR ' '
#define CAVEFILLING '%'
#define TILEINCREMENT 10
#define ROOMINCREMENT 10
//#define CA_DEBUG 1

#ifndef TILESTRUCT
#define TILESTRUCT void*
#endif

/**
 * tile_t struct: this struct represents a tile of a room.
 */
typedef struct tile_t{
    TILESTRUCT data;        //pointer to the tile data.
    int row;                //row of the tile
    int col;                //column of the tile
    char roomID;              //this tile's room id
}tile_t;

/**
 * floor_t struct: this struct represents the floor of a room. 
 * Has a pointer to every reachable tile.
 */
typedef struct floor_t{
    tile_t** tiles;         //An array of arrays with the tiles. The first dimension
                            //selects the row, while the second selects the column.
                            //Use numTiles below to determine the number of tiles each
                            //row has.
    
    int *numTiles;          //Tells how many tiles are actually being used in each row.
                            //Useful for traversing! If greater than zero, the row has
                            //tiles. Use the number as an index to the tiles array of arrays.
    
    int *alocTiles;         //How many tiles are allocated to each row. ignore it.
    int maxHeight;          //This is used to keep the total matrix size. ignore this.
    
    int height;             //The height of this floor.
                            //Don't use this to traverse the tiles.
    
    int width;              //The width of this floor.
                            //Don't use this to traverse the tiles.
    
    int top;                //First y position of any tile in this floor.
                            //Don't use this to traverse the tiles.
    
    int bottom;             //Last y position of any tile in this floor.
                            //Don't use this to traverse the tiles.
    
    int left;               //First x position of any tile in this floor.
                            //Don't use this to traverse the tiles.
    
    int right;              //Furthest x position of any  tile in this floor. 
                            //Don't use this to traverse the tiles.
    
    tile_t *far_left;       //A pointer to one of the dar left tiles
    tile_t *far_right;      //A pointer to one of the far right tiles
    tile_t *far_top;        //A pointer to one of the far top tiles
    tile_t *far_bottom;     //A pointer to one of the far bottom tiles
}floor_t;

/**
 * Gets the floor tile from (row, col)
 * @param f The floor to be searched
 * @param row Tile row
 * @param col Tile column
 * @return the floor tile structure or NULL if tile not found.
 */
tile_t* FLOOR_getTile(floor_t *f, int row, int col);

/**
 * room_t struct: represents a self-contained room within a cavern.
 */

typedef struct room_t{
    char id;                //the id of the room.
    int size;               //how many tiles in this room
    floor_t* floor;         //pointer to the floor structure of this room.
    int xOffset;            //row of the leftmost top tile of this room.
    int yOffset;            //column of the leftmost top tile of this room.
} room_t;

/**
 * Returns a random tile from within a room.
 * @param r The room.
 * @return a random tile within a room.
 */
tile_t* ROOM_getRandomTile(room_t* r);

/**
 * Gets the floor tile from (row, col) from a given room
 * @param r The room to be searched
 * @param row Tile row
 * @param col Tile column
 * @return  the floor tile structure or NULL if tile not found.
 */

tile_t* ROOM_getTile(room_t *r, int row, int col);

/**
 * cave_t struct: represents a generated cellular-automata-based cavern and its rooms.
 */
typedef struct cave_t{  
    char *data;             //Pointer to wall/floor data
    char **mData;           //row/column representation of the wall/floor data
    int height;             //Total height of the cave
    int width;              //Total width of the cave
    float wallRatio;        //The initial The wall/floor ratio of the cave
    int randomSeed;         //The random seed used to generate the cave.
    float currentWallRatio; //UNUSED.
    room_t** rooms;         //Array of pointers to room structures.
    int nRooms;             //The amount of rooms that are present in this cave.
                            //Use this to traverse the rooms array (determine its size).
    int alocRooms;          //Used to keep track of how many pointers to rooms are currently allocated. ignore this.
} cave_t;

/**
 * Returns a new cave with every room from the cellular automata.
 * @param width The width of the cave
 * @param height The height of the cave
 * @param wallRatio Wall to floor ratio. 0..1 (usually .5f for many rooms)
 * @param randomSeed Used for cave generation determinism. The same seed always returns the same cave configuration
 * @param iterations How many CA generations to compute
 * @return a new cave.
 */
cave_t* CAVE_getRooms(int width, int height, float wallRatio, int iterations);

/**
 * Removes all the rooms from a certain cave if their sizes are smaller than the average room size throughout the cave.
 * @param cave A cave, usually generated with CAVE_getRooms
 * @param deviation An acceptable lower threshold relative to the average cave size to accept. 0 means that only caves larger than the average cave will remain. 
 * 0.1f means that caves up to 10% smaller than the average will be accepted.
 * @return a modified cave.
 * @see cave_t, CAVE_getRooms
 */
cave_t* CAVE_getRoomsAverage(cave_t* cave, float deviation);

/**
 * Removes all the rooms from a given cave if theirs sizes are smaller than a given ratio to the largest room.
 * @param cave A cave, usually generated with CAVE_getRooms 
 * @param keepRooms Only rooms that whose room_size/largestroom_size is above this ratio will be left.
 * @return a modified cave.
 * @see CAVE_getRooms
 */
cave_t* CAVE_getRoomsSize(cave_t* cave, float keepRooms);

/**
 * Returns a cave containing only large rooms. The same as CAVE_getRoomsSize(cave, 0.5f)
 * @param cave A cave, usually generated with CAVE_getRooms 
 * @return a modified cave.
 * @see CAVE_getRooms
 */
cave_t* CAVE_getLargeRooms(cave_t* cave);

/**
 * Prints the cave for debugging purposes.
 * @param cave The cave.
 */
void CAVE_print(cave_t* cave);

tile_t* CAVE_getTile(cave_t* cave, int row, int col);

cave_t* CAVE_clearRooms(cave_t* cave);

float TILE_eucDistance(tile_t* a, tile_t* b);

#endif