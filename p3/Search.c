// para el seach
#include "Search.h"
#include <stdlib.h>
#include <string.h>

void createEmptyListSE(tListS *S){
    *S = SNULL;
}
bool isEmptyListSE(tListS S){
    return (S == SNULL);
}
bool createNodeSE(tPosS *p){
    *p = malloc(sizeof(struct tNodeS));
    return *p != SNULL;
}

bool insertItemSE(tItemS d, tPosS p, tListS *S) {
    /* q -> elemento que se va a insertar
     * r -> elemento previo a q */
    tPosS q, r;

    if(!createNodeSE(&q)) {
        return false;
    } else {
        strcpy(q->elemento.datos, d);
        q->siguiente = SNULL;
        q->anterior = SNULL;
        if(isEmptyListSE(*S)) {
            *S = q;
        } else if(p == SNULL) {
            for(r = *S; r->siguiente != SNULL; r = r->siguiente);
            r->siguiente = q;
            q->anterior = r;

        } else if(p == *S) {
            q->siguiente = p;
            p->anterior = q;
            *S = q;

        } else {

            strcpy(q->elemento.datos, p->elemento.datos);
            strcpy(p->elemento.datos , d);
            if(p->siguiente != SNULL) {
                p->siguiente->siguiente = q;
            }
            q->siguiente = p->siguiente;
            p->siguiente = q;
            q->anterior = p;
        }
        return true;
    }

}

tPosS firstSE(tListS L){
    return L;
}

tPosS lastSE(tListS L){
    tPosS p;

    for(p = L; p->siguiente != SNULL; p = p->siguiente);
    return p;
}

tPosS nextSE(tPosS p, tListS S){
    return p->siguiente;
}

bool deleteSearchPosition(tPosS p, tListS *S) {
    tPosS q;

    if (isEmptyListSE(*S) || p == SNULL) {
        return false;
    }

    if (p == *S) {
        *S = p->siguiente;
        if (*S != SNULL) {
            (*S)->anterior = SNULL;
        }
    } else {
        q = p->anterior;
        q->siguiente = p->siguiente;
        if (p->siguiente != SNULL) {
            p->siguiente->anterior = q;
        }
    }
    free(p);
    return true;
}



void deleteListSE(tListS *S) {
    tPosS p;

    while(!isEmptyListSE(*S)) {
        p = *S;
        *S = (*S)->siguiente;
        free(p);
    }
}