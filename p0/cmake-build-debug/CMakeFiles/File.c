// para el open
#include "File.h"
#include <stdlib.h>
#include <string.h>

void createEmptyFileList(tListF *L){
    *L = FNULL;
}
bool isEmptyFileList(tListF L){
    return (L == FNULL);
}
bool createFileNode(tPosF *p){
    *p = malloc(sizeof(struct FNode));
    return *p != FNULL;
}

bool insertFileItem(Descriptor s, tItemF n,tItemF m, tPosF p, tListF *L) {
    /* q -> elemento que se va a insertar
     * r -> elemento previo a q */
    tPosF q, r;

    if(!createFileNode(&q)) {
        return false;
    } else {

        q->elemento.descriptor = s;
        strcpy(q->elemento.nombre, n);
        strcpy(q->elemento.modo, m);
        q->siguiente = LNULL;
        q->anterior = LNULL;
        if(isEmptyFileList(*L)) {
            *L = q;
        } else if(p == LNULL) {
            for(r = *L; r->siguiente != FNULL; r = r->siguiente);
            r->siguiente = q;
            q->anterior = r;

        } else if(p == *L) {
            q->siguiente = p;
            p->anterior = q;
            *L = q;

        } else {
            q->elemento.descriptor = p->elemento.descriptor;
            strcpy(q->elemento.nombre, p->elemento.nombre);
            strcpy(q->elemento.modo, p->elemento.modo);

            p->elemento.descriptor = s;
            strcpy(p->elemento.nombre , n);
            strcpy(p->elemento.modo, m);



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


tPosF firstFile(tListF L){
    return L;
}

tPosF lastFile(tListF L){
    tPosF p;

    for(p = L; p->siguiente != FNULL; p = p->siguiente);
    return p;
}

tPosF nextFile(tPosF p, tListF L){
    return p->siguiente;
}

void deleteFile(tPosF p, tListF *L) {
    tPosF q;

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

