#include "Lista.h"
#include <stdlib.h>
#include <string.h>

void createEmptyList(tList *L){
    *L = LNULL;
}

bool isEmptyList(tList L){
    return (L == LNULL);
}

bool createNode(tPosL *p){
    *p = malloc(sizeof(struct tNode));
    return *p != LNULL;
}

bool insertItem(tItemL d, tPosL p, tList *L) {
    /* q -> elemento que se va a insertar
     * r -> elemento previo a q */
    tPosL q, r;

    if(!createNode(&q)) {
        return false;
    } else {
        strcpy(q->elemento.datos, d);
        q->siguiente = LNULL;
        q->anterior = LNULL;
        if(isEmptyList(*L)) {
            *L = q;
        } else if(p == LNULL) {
            for(r = *L; r->siguiente != LNULL; r = r->siguiente);
            r->siguiente = q;
            q->anterior = r;

        } else if(p == *L) {
            q->siguiente = p;
            p->anterior = q;
            *L = q;

        } else {

            strcpy(q->elemento.datos, p->elemento.datos);
            strcpy(p->elemento.datos , d);
            if(p->siguiente != LNULL) {
                p->siguiente->siguiente = q;
            }
            q->siguiente = p->siguiente;
            p->siguiente = q;
            q->anterior = p;
        }
        return true;
    }
}

tPosL first(tList L){
    return L;
}

tPosL last(tList L){
    tPosL p;

    for(p = L; p->siguiente != LNULL; p = p->siguiente);
    return p;
}

tPosL next(tPosL p, tList L){
    return p->siguiente;
}

void deleteAtPosition(tPosL p, tList *L) {
    tPosL q;

    if(p == *L) {
        *L = (*L)->siguiente;

        if(!isEmptyList(*L)) {
            (*L)->siguiente = LNULL;
        }

    } else {
        q = p->siguiente;
        q->siguiente = p->siguiente;

        if(p->siguiente != LNULL) {
            p->siguiente->anterior = p->anterior;
        }
    }

    free(p);
}
