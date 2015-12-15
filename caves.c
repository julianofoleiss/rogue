#include <stdio.h>
#include <stdlib.h>
#include "object.h"
#include "level.h"
#include "player.h"
#include "KeyPress/KeyPress.h"
#include "msgBox.h"

extern level_t* currentLevel;
extern object_t* player;
extern int gameNotOver;
extern msgBox_t* msgBox;

void CAVES_run() {
    char key;
    PLAYER_COMMAND cmd;
    player_t* newplayer;
    
    if(currentLevel == NULL){
        newplayer = PLAYER_new();
        msgBox = MSGBOX_new(5);
        currentLevel = LEVEL_new(newplayer, 1, 1, getTerminalWidth(), getTerminalHeight() - 10, .5f, 100);
    }
    
    int new_row, new_col;
    player_t* player_data;
    player_data = player->data.player;
    
    LEVEL_draw();
    PLAYER_infoDraw();    
    MSGBOX_print(msgBox);
    
    key = GetKey();

    system("clear");
    
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
        
        case PLAYER_CMD_UP_LEFT:
            new_row = player->tile->row - 1;
            new_col = player->tile->col - 1;
        break;

        case PLAYER_CMD_UP_RIGHT:
            new_row = player->tile->row - 1;
            new_col = player->tile->col + 1;
        break;
        
        case PLAYER_CMD_DOWN_LEFT:
            new_col = player->tile->col - 1;
            new_row = player->tile->row + 1;
        break;        
        
        case PLAYER_CMD_DOWN_RIGHT:
            new_col = player->tile->col + 1;
            new_row = player->tile->row + 1;
        break;      
        
        case PLAYER_CLEAR_FOW:
            LEVEL_clearAllFOW(currentLevel);
        break;
        
        case PLAYER_QUIT_GAME:
            gameNotOver = 0;
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
            
            LEVEL_clearFOW(currentLevel, player->tile->row, player->tile->col, 5);
            
        }
        else{
            //just a floor.. ok to walk.
            OBJECT_move(player, new_row, new_col);
            
            LEVEL_clearFOW(currentLevel, player->tile->row, player->tile->col, 5);
            
            int i;
            
            for(i = 0; i < currentLevel->nObjs; i++){
                if(currentLevel->obj[i]->active){
                    if(currentLevel->obj[i]->tile->roomID == player->tile->roomID){
                        currentLevel->obj[i]->step(currentLevel->obj[i]);
                    }
                }
            }
            
        }

    }
    
    
    
    
}


