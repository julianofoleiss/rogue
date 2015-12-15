#ifndef MSGBOX_H
#define	MSGBOX_H

typedef struct msg_t{
    char* message;
    struct msg_t* next;
}msg_t;

typedef struct msgBox_t{
    msg_t* first;
    int capacity;
    int size;
}msgBox_t;

msgBox_t* MSGBOX_new(int size);
void MSGBOX_add(msgBox_t* msg, char* message);
void MSGBOX_clear(msgBox_t* msg);
void MSGBOX_update(msgBox_t* msg);
void MSGBOX_print(msgBox_t* msg);
void MSGBOX_addConstString(msgBox_t* msgBox, const char* message);

typedef struct stringPool_t{
    int poolSize;
    char** pool;
    int pos;
}stringPool_t;

#define STRING_POOL_STRING_SIZE 255

stringPool_t* STRING_POOL_new(int size);
char* STRING_POOL_getString(stringPool_t* s);

#endif	/* MSGBOX_H */

