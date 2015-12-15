#include <time.h>
#include <stdint.h>
#include <stdlib.h>

int getRandomNumber(int min, int max){
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}
