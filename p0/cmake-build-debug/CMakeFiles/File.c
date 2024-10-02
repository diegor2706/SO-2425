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
    *p = malloc(50* sizeof(struct FNode));
    return *p != FNULL;
}
bool insertFileItem(Descriptor j,  tItemF m,tItemF n, tPosF p, tListF *L) {
    tPosF q, r;

    if (!createFileNode(&q)) {
        return false;
    } else {
        q->elemento.descriptor = j;
        // Usamos strncpy para evitar desbordamiento
        strncpy(q->elemento.nombre, n, sizeof(q->elemento.nombre) - 1);
        q->elemento.nombre[sizeof(q->elemento.nombre) - 1] = '\0'; // Aseguramos el terminador nulo

        strncpy(q->elemento.modo, m, sizeof(q->elemento.modo) - 1);
        q->elemento.modo[sizeof(q->elemento.modo) - 1] = '\0'; // Aseguramos el terminador nulo

        q->siguiente = LNULL;
        q->anterior = LNULL;

        if (isEmptyFileList(*L)) {
            *L = q;
        } else if (p == LNULL) {
            for (r = *L; r->siguiente != FNULL; r = r->siguiente);
            r->siguiente = q;
            q->anterior = r;
        } else if (p == *L) {
            q->siguiente = p;
            p->anterior = q;
            *L = q;
        } else {
            q->elemento.descriptor = p->elemento.descriptor;
            strncpy(q->elemento.nombre, p->elemento.nombre, sizeof(q->elemento.nombre) - 1);
            q->elemento.nombre[sizeof(q->elemento.nombre) - 1] = '\0';

            strncpy(q->elemento.modo, p->elemento.modo, sizeof(q->elemento.modo) - 1);
            q->elemento.modo[sizeof(q->elemento.modo) - 1] = '\0';

            p->elemento.descriptor = j;
            strncpy(p->elemento.nombre, n, sizeof(p->elemento.nombre) - 1);
            p->elemento.nombre[sizeof(p->elemento.nombre) - 1] = '\0';

            strncpy(p->elemento.modo, m, sizeof(p->elemento.modo) - 1);
            p->elemento.modo[sizeof(p->elemento.modo) - 1] = '\0';

            if (p->siguiente != LNULL) {
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