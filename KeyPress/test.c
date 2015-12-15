#include <stdio.h>
#include <stdlib.h>
#include "KeyPress.h"

int main(int argc, char** argv) {

    char c = 0;
    
    SetupConsole();
    
    do{
        c = (char)GetKey();
    }while(c != 'q');
    
    RestoreConsole();
    
    return (EXIT_SUCCESS);
}

