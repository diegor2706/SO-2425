// para el search

#ifndef p3_SEARCH_H
#define p3_SEARCH_H
#include <stdbool.h>

#define SNULL NULL
typedef char tItemS[30];

typedef struct elementS{
    tItemS datos;
}elementS;

typedef struct tNodeS *tPosS;

struct tNodeS{
    elementS elemento;
    tPosS siguiente;
    tPosS anterior;
};

typedef tPosS tListS;

void createEmptyListSE(tListS *S);
bool isEmptyListSE(tListS S);
bool createNodeSE(tPosS *p);
bool insertItemSE(tItemS d, tPosS p, tListS *S);
tPosS firstSE(tListS S);
tPosS lastSE(tListS S);
tPosS nextSE(tPosS p, tListS S);
void deleteListSE(tListS *S);

#endif //p3_SEARCH_H