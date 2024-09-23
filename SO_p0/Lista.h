#ifndef SO_P0_LISTA_H
#define SO_P0_LISTA_H
#include <stdbool.h>

#define LNULL NULL
typedef char tItemL[30];

typedef struct element{
    tItemL datos;
}element;

typedef struct tNode *tPosL;

struct tNode{
    element elemento;
    tPosL siguiente;
    tPosL anterior;
};

typedef tPosL tList;

void createEmptyList(tList *L);
bool isEmptyList(tList L);
bool createNode(tPosL *p);
bool insertItem(tItemL d, tPosL p, tList *L);
void updateItem(tItemL d, tPosL p, tList *L);   //quitar
tPosL findItem(tItemL d, tList L);
element getItem(tPosL p, tList L);
tPosL first(tList L);
tPosL last(tList L);
tPosL next(tPosL p, tList L);
void deleteAtPosition(tPosL p, tList *L);


#endif //SO_P0_LISTA_H
