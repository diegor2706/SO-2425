//
// Created by diegoroman on 14/11/24.
// para el allocate

#include "Memory.h"
#include <stdlib.h>
#include <string.h>

void createEmptyMemoryList(tListM *M) {
    *M = MNULL;
}

bool isEmptyMemoryList(tListM M){
    return (M == MNULL);
}

bool createMemoryNode(tPosM *p){
    *p = malloc(100 * sizeof (struct mNode));
    return *p != MNULL;
}

bool insertMItem(void *direccion, long tam, char *fecha, char *funcion, char *identificador,int id, tPosM p, tListM *L) {
    tPosM q, r;

    if (!createMemoryNode(&q)) {
        return false;
    } else {
        q->elemento.tam = tam;
        strcpy(q->elemento.fecha, fecha);
        strcpy(q->elemento.funcion, funcion);
        q->elemento.direccion = direccion; // Guardar la dirección directamente como puntero
        strcpy(q->elemento.identificador, identificador);
        q->elemento.id = id;

        q->siguiente = MNULL;
        q->anterior = MNULL;

        if (isEmptyMemoryList(*L)) {
            *L = q;
        } else if (p == MNULL) {
            // Insertar al final
            for (r = *L; r->siguiente != MNULL; r = r->siguiente);
            r->siguiente = q;
            q->anterior = r;

        } else if (p == *L) {
            // Insertar al inicio
            q->siguiente = p;
            p->anterior = q;
            *L = q;

        } else {
            // Insertar en el medio
            q->elemento.tam = p->elemento.tam;
            strcpy(q->elemento.fecha, p->elemento.fecha);
            strcpy(q->elemento.funcion, p->elemento.funcion);
            q->elemento.direccion = p->elemento.direccion; // Copiar puntero
            strcpy(q->elemento.identificador, p->elemento.identificador);
            q->elemento.id = p->elemento.id;

            p->elemento.tam = tam;
            strcpy(p->elemento.fecha, fecha);
            strcpy(p->elemento.funcion, funcion);
            p->elemento.direccion = direccion; // Actualizar dirección del nodo actual
            strcpy(p->elemento.identificador, identificador);
            p->elemento.id = id;

            if (p->siguiente != MNULL) {
                p->siguiente->anterior = q;
            }
            q->siguiente = p->siguiente;
            p->siguiente = q;
            q->anterior = p;
        }
        return true;
    }
}



tPosM firstMemory(tListM M){
    return M;
}

tPosM lastMemory(tListM M){
    tPosM p;

    for(p = M; p->siguiente != MNULL; p = p->siguiente);
    return p;
}

tPosM nextMemory(tPosM p, tListM M){
    return p->siguiente;
}

void deleteMemoryList(tListM *M){
    tPosM p;

    while(!isEmptyMemoryList(*M)){
        p = *M;
        *M = (*M)->siguiente;
        free(p);
    }
}
