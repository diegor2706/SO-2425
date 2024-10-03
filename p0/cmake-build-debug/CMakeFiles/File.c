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

bool insertFileItem(Descriptor j, char  *m,char  *n, tPosF p, tListF *L) {


    tPosF q, r;

    if(!createFileNode(&q)) {
        return false;
    }
    else {
        strcpy(q->data.nombre, n);
        strcpy(q->data.modo, m);
        q->data.descriptor = j;

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


            q->siguiente = p->siguiente;
            p->siguiente = q;
        }
        return true;
    }
}

tPosF nextFile(tPosF p, tListF L) {
    return p->siguiente;
}

tPosF firstFile(tListF L) {
    return L;
}

/*
void updateItem(tItemF d, tPosF p, tListF *L) {
    p->d = d;
}


tPosF findItem(tItemF d, tListF L) {
    tPosL p;

    for(p = L; (p != LNULL) && (p->d != d); p = p->siguiente);
    return p;
}


tItemF getIFiletem(tPosF p, tList L) {
    return p->data;
}



tPosL last(tList L) {
    tPosL p;

    for(p = L; p->next != LNULL; p = p->next);
    return p;
}

tPosL previous(tPosL p, tList L) {
    tPosL q; // q es el elemento previo

    if(p == L) {
        return LNULL;
    }
    else {
        for(q = L; q->next != p; q = q->next);
        // Mientras el siguiente elemento a q sea distinto al introducido, se seguirá ejecutando el bucle
        return q;
    }
}



void deleteAtPosition(tPosL p, tList *L) {

    tPosL q;

    if(p == *L) { // Eliminado en la primera posición de la lista
        *L = (*L)->next;

    } else if(p->next == LNULL) { // Eliminado en la última posición
        for(q = *L; q->next->next != LNULL; q = q->next);
        q->next = LNULL;

    } else { // Eliminado de un elemento que no se encuentra en los extremos
        q = p->next;
        p->data = q->data;
        p->next = q->next;
        p = q;
    }

    free(p);
}

void deleteList(tList *L) {
    tPosL p;

    while(!isEmptyList(*L)) {
        p = *L;
        *L = (*L)->next;
        free(p);
    }
}
*/