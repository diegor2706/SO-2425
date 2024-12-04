// para el open
#include "File.h"
#include <stdlib.h>
#include <string.h>


void createEmptyFileList(tListF *F) {
    *F = LNULL;
}

bool isEmptyFileList(tListF F) {
    return (F == LNULL);
}

bool createFileNode(tPosF *p) {

    *p = malloc(100 * sizeof(struct tNodeF));
    return *p != LNULL;
}

bool insertFileItem(Descriptor j, char  *m,char  *n,long offset, tPosF p, tListF *L) {

    tPosF q, r;

    if(!createFileNode(&q)) {
        return false;
    }
    else {
        strcpy(q->data.nombre, n);
        strcpy(q->data.modo, m);
        q->data.descriptor = j;
        q->data.offset = offset;

        q->siguiente = LNULL;

        if (isEmptyFileList(*L)) {
            *L = q;
        }

        else if(p == LNULL) {
            for(r = *L; r->siguiente != LNULL; r = r->siguiente);

            r->siguiente = q;
        }

        else if(p == *L) {
            q->siguiente = p;
            *L = q;
        }

        else {
            q->data = p->data;

            strcpy(p->data.nombre, n);
            strcpy(p->data.modo, m);
            p->data.descriptor = j;
            p->data.offset = offset;


            q->siguiente = p->siguiente;
            p->siguiente = q;
        }
        return true;
    }
}

tPosF nextFile(tPosF p, tListF F) {
    return p->siguiente;
}

tPosF firstFile(tListF L) {
    return L;
}

tPosF lastFile(tListF F){
    tPosF i;
    for (i = F; i->siguiente != FNULL ; i = i->siguiente);
    return i;
}

tPosF findFileItem(int x, tListF F) {
    tPosF i = F;

    while (i != FNULL) {
        if (i->data.descriptor == x) {
            break;
        }
        i = i->siguiente;
    }

    return i;
}


void deleteFileAtPosition(tPosF p, tListF *F) {

    tPosF q;

    if(p == *F) { // Eliminado en la primera posición de la lista
        *F = (*F)->siguiente;

    } else if(p->siguiente == FNULL) { // Eliminado en la última posición
        for(q = *F; q->siguiente->siguiente != LNULL; q = q->siguiente);
        q->siguiente = LNULL;

    } else { // Eliminado de un elemento que no se encuentra en los extremos
        q = p->siguiente;
        p->data = q->data;
        p->siguiente = q->siguiente;
        p = q;
    }

    free(p);
}

void deleteFileList(tListF *F) {
    tPosF p;

    while(!isEmptyFileList(*F)) {
        p = *F;
        *F = (*F)->siguiente;
        free(p);
    }
}
