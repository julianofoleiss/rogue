#include <stdlib.h>
#include <stdio.h>
#include "msgBox.h"

extern stringPool_t *stringPool;

msgBox_t* MSGBOX_new(int size){
    msgBox_t* msgBox;
    
    msgBox = malloc(sizeof(msgBox_t));
    
    msgBox->first = NULL;
    msgBox->capacity = size;
    msgBox->size = 0;
    
    return msgBox;
}

void MSGBOX_add(msgBox_t* msgBox, char* message){
    msg_t* msg;
    msg_t* p, *ant;

    msg = malloc(sizeof(msgBox_t));
    msg->message = message;
    msg->next = NULL;
    
    if(msgBox->first){
        p = msgBox->first;
        
        while(p->next){
            p = p->next;
        }        
        
        p->next = msg;
        
        if((msgBox->size == msgBox->capacity) && msgBox->first){
            p = msgBox->first;
            msgBox->first = msgBox->first->next;
            free(p->message);
            free(p);
        }        
        
         msgBox->size = msgBox->size < msgBox->capacity ? msgBox->size + 1 : msgBox->size;
    }
    else{
        msgBox->first = msg;
        msgBox->size = 1;
    }
   
    
}


void MSGBOX_addConstString(msgBox_t* msgBox, const char* message){
    char* msg;
    
    msg = STRING_POOL_getString(stringPool);
    
    sprintf(msg, message);
    
    MSGBOX_add(msgBox, msg);
}

void MSGBOX_clear(msgBox_t* msgBox){
    msg_t* p, *ant;
    ant = NULL;
    for(p = msgBox->first; p != NULL; p = p->next){
        if(ant)
            free(ant);
        ant = p;
    }
    free(ant);
}

void MSGBOX_print(msgBox_t* msgBox){
    msg_t* p;
    
    for(p = msgBox->first; p!= NULL; p = p->next)
        printf("%s\n", p->message);
}

stringPool_t* STRING_POOL_new(int size){
    stringPool_t* pool;
    
    pool = malloc(sizeof(stringPool_t));
    
    pool->pos = 0;
    pool->poolSize = size;
    pool->pool = malloc(sizeof(char*) * size);
    
    return pool;
}

char* STRING_POOL_getString(stringPool_t* s){
    char* ret;
    
    s->pool[s->pos] = calloc(STRING_POOL_STRING_SIZE, sizeof(char));
    ret = s->pool[s->pos];
    s->pos = (s->pos % s->poolSize);
    
    return ret;
}
