//
// Created by diegoroman on 14/11/24.
// para el allocate

#include "Memory.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/mman.h>

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

bool insertMItem(void *direccion, long tam, char *fecha, char *funcion, int df,int id, key_t  llave,tPosM p, tListM *L) {

    tPosM q, r;

    if (!createMemoryNode(&q)) {
        return false;
    } else {
        q->elemento.tam = tam;
        strcpy(q->elemento.fecha, fecha);
        strcpy(q->elemento.funcion, funcion);
        q->elemento.direccion = direccion; // Guardar la dirección directamente como puntero
        q->elemento.df = df;
        q->elemento.id = id;
        q->elemento.llave = llave;

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
            q->elemento.df = p->elemento.df;
            q->elemento.id = p->elemento.id;
            q->elemento.llave = p->elemento.llave;

            p->elemento.tam = tam;
            strcpy(p->elemento.fecha, fecha);
            strcpy(p->elemento.funcion, funcion);
            p->elemento.direccion = direccion; // Actualizar dirección del nodo actual
            p->elemento.df = df;
            p->elemento.id = id;
            p->elemento.llave = llave;

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



void deleteMallocMemoryPosition(tPosM p, tListM *M) {

    if (p->elemento.direccion != NULL) {
        free(p->elemento.direccion);
    }


    if (p == *M) { // Si es el primer nodo
        *M = (*M)->siguiente;
        if (!isEmptyMemoryList(*M)) {
            (*M)->anterior = MNULL;
        }
    } else { // Nodo intermedio o final
        if (p->anterior != MNULL) {
            p->anterior->siguiente = p->siguiente;
        }
        if (p->siguiente != MNULL) {
            p->siguiente->anterior = p->anterior;
        }
    }


    free(p);
}

void deleteMmapMemoryPosition(tPosM p, tListM *M) {
    if (p->elemento.direccion != NULL) {
        // Liberar memoria mapeada con munmap
        if (munmap(p->elemento.direccion, p->elemento.tam) == -1) {
            perror("Error al liberar memoria mapeada");
        }
    }

    // Ajustar los enlaces de la lista
    if (p == *M) { // Si es el primer nodo
        *M = (*M)->siguiente;
        if (!isEmptyMemoryList(*M)) {
            (*M)->anterior = MNULL;
        }
    } else { // Nodo intermedio o final
        if (p->anterior != MNULL) {
            p->anterior->siguiente = p->siguiente;
        }
        if (p->siguiente != MNULL) {
            p->siguiente->anterior = p->anterior;
        }
    }

    // Liberar el nodo
    free(p);
}
void deleteSharedMemoryPosition(tPosM p, tListM *M) {
    int shmid;

    // Obtener el shmid usando la clave almacenada
    if ((shmid = shmget(p->elemento.llave, p->elemento.tam, 0666)) == -1) {
        perror("Error al obtener el shmid para eliminar memoria compartida");
    } else {
        // Desasociamos la memoria compartida del proceso
        if (shmdt(p->elemento.direccion) == -1) {
            perror("Error al desasociar memoria compartida");
        }

        // Eliminamos el segmento de memoria compartida
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("Error al eliminar segmento de memoria compartida");
        }
    }

    // Eliminamos el nodo de la lista
    if (p == *M) { // Si es el primer nodo
        *M = (*M)->siguiente;
        if (!isEmptyMemoryList(*M)) {
            (*M)->anterior = MNULL;
        }
    } else { // Nodo intermedio o final
        if (p->anterior != MNULL) {
            p->anterior->siguiente = p->siguiente;
        }
        if (p->siguiente != MNULL) {
            p->siguiente->anterior = p->anterior;
        }
    }

    // Liberamos la memoria ocupada por el nodo
    free(p);
}

void deleteMemoryList(tListM *M){
    tPosM p;

    while(!isEmptyMemoryList(*M)){

        p = *M;
        if (p->elemento.id == 0){
            deleteMallocMemoryPosition(p,M);
        }else if(p->elemento.id == 1){
            deleteMmapMemoryPosition(p,M);
        }
        else if (p->elemento.id == 2){
            deleteSharedMemoryPosition(p,M);
        }  //*M = (*M)->siguiente;
        //free(p);
    }
}
