// para el open
#include "File.h"
#include <stdlib.h>
#include <string.h>

void createEmptyFileList(tList *L){
    *L = FNULL;
}
bool isEmptyFileList(tList L){
    return (L == FNULL);
}
bool createFileNode(tPosL *p){
    *p = malloc(sizeof(struct tNode));
    return *p != FNULL;
}
bool insertFileItem(Descriptor j,tItemL m, tItemL d, tPosL p, tList *L){

    tPosL q, r;

    if(!createFileNode(&q)) {
        return false;
    } else {

        
    }

}


tPosL firstFile(tList L){
    return L;
}

tPosL lastFile(tList L){
    tPosL p;

    for(p = L; p->siguiente != FNULL; p = p->siguiente);
    return p;
}

tPosL nextFile(tPosL p, tList L){
    return p->siguiente;
}

void deleteFile(tPosL p, tList *L) {
    tPosL q;

    if(p == *L) {
        *L = (*L)->siguiente;

        if(!isEmptyFileList(*L)) {
            (*L)->siguiente = FNULL;
        }

    } else {
        q = p->siguiente;
        q->siguiente = p->siguiente;

        if(p->siguiente != FNULL) {
            p->siguiente->anterior = p->anterior;
        }
    }

    free(p);
}

