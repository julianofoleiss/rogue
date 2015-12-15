#include <stdio.h>
#include <stdlib.h>
#include "object.h"
#include "level.h"
#include "player.h"
#include "KeyPress/KeyPress.h"

extern level_t* currentLevel;
extern object_t* player;

/*
 * 
 */
int main(int argc, char** argv) {

    char key;
    PLAYER_COMMAND cmd;
    player_t* newplayer;
    
    SetupConsole();
    
    system("clear");
    
    newplayer = PLAYER_new();
    currentLevel = LEVEL_new(newplayer, 0, 50, getTerminalWidth(), getTerminalHeight() - 10, .5f, 100);
    
    while(1){
        
        int new_row, new_col;
        player_t* player_data;
        player_data = player->data.player;
        
        LEVEL_draw();
        PLAYER_infoDraw();    

        key = GetKey();
        
        cmd = PLAYER_getCommand(key);
        
        new_row = player->tile->row;
        new_col = player->tile->col;
        
        switch(cmd){
            case PLAYER_CMD_UP:
                new_row = player->tile->row - 1;
            break;
            
            case PLAYER_CMD_DOWN:
                new_row = player->tile->row + 1;
            break;
            case PLAYER_CMD_LEFT:
                new_col = player->tile->col - 1;
            break;
            
            case PLAYER_CMD_RIGHT:
                new_col = player->tile->col + 1;
            break;            
        }
        
        tile_t* new_pos;
        object_t* obj;
        
        new_pos = CAVE_getTile(currentLevel->cave, new_row, new_col);
        
        //is it floor/object or wall? Returns a tile if so.
        if(new_pos){
            
            if(new_pos->data){
                obj = new_pos->data;
                if(obj->flags & OBJ_F_INTERACTIVE){
                    obj->interact(obj);
                }
            }
            else{
                //just a floor.. ok to walk.
                OBJECT_move(player, new_row, new_col);
                
            }
            
        }
        
        system("clear");
    }
    
    RestoreConsole();
    
    return (EXIT_SUCCESS);
}

