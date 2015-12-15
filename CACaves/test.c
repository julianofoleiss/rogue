#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "CACaves.h"

int main(int argc, char** argv) {
    cave_t* cave;
    int seed;
    int i;

    printf("Give me the initial seed: ");
    scanf("%d", &seed);

    srand(seed);
    
    i = seed;

    struct winsize size;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, (char*) &size);

    cave = CAVE_getRooms(size.ws_col-10, size.ws_row-5, .5f, 100);
    cave = CAVE_getRoomsAverage(cave, 0.0f);
    //CAVE_print(cave);

    printf("window width: %d, height: %d\n", size.ws_col, size.ws_row);

    ROOM_getRandomTile(cave->rooms[0]);
    ROOM_getRandomTile(cave->rooms[0]);
    ROOM_getRandomTile(cave->rooms[0]);
    ROOM_getRandomTile(cave->rooms[0]);
    ROOM_getRandomTile(cave->rooms[0]);
    ROOM_getRandomTile(cave->rooms[0]);
    ROOM_getRandomTile(cave->rooms[0]);
    ROOM_getRandomTile(cave->rooms[0]);
    
    CAVE_print(cave);
    
    return (EXIT_SUCCESS);
}

