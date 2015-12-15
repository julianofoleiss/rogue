#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include "KeyPress.h"
#include <sys/ioctl.h>

struct termios old_tio, new_tio;
struct winsize consoleSize;

void SetupConsole(){
    /* get terminal window size */
    ioctl(STDOUT_FILENO, TIOCGWINSZ, (char*) &consoleSize);
    
    /* get the terminal settings for stdin */
    tcgetattr(STDIN_FILENO,&old_tio);

    /* we want to keep the old setting to restore them a the end */
    new_tio=old_tio;

    /* disable canonical mode (buffered i/o) and local echo */
    new_tio.c_lflag &=(~ICANON & ~ECHO);

    /* set the new settings immediately */
    tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);
    
#ifdef __linux__
    system("setterm -cursor off");
#endif
    
}

void RestoreConsole(){
    /* restore the former settings */
    tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
    
#ifdef __linux__
    system("setterm -cursor on");
#endif    
}

int getTerminalHeight(){
    return consoleSize.ws_row;
}

int getTerminalWidth(){
    return consoleSize.ws_col;
}

int GetKey(){
    char c;
    
    c=getchar();

    /*if(c == 27){
        c = getchar();
        if(c == 91){
            c = getchar();
            switch(c){
                case 65:
                        printf("up! ");
                break;
                case 66:
                        printf("down! ");
                break;	 						 			
                case 67:
                        printf("right! ");
                break;		 			
                case 68:
                        printf("left! ");
                break;
            }
        }
    }*/ 
    
    return c;
}