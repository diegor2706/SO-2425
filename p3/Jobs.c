//
// Created by diegoroman on 14/11/24.
// para el allocate

#include "Jobs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/mman.h>


void createEmptyJobList(tListJ *J) {
    *J = JNULL;
}

bool isEmptyJobList(tListJ J){
    return (J == JNULL);
}

bool createJobNode(tPosJ *p){
    *p = malloc(100 * sizeof (struct jNode));
    return *p != JNULL;
}

bool insertJobItem(void *direccion, long tam, char *fecha, char *funcion, int df,int id, key_t  llave, tPosJ p, tListJ *J) {

    tPosJ q, r;

    if (!createJobNode(&q)) {
        return false;
    } else {
        q->elemento.tam = tam;
        strcpy(q->elemento.fecha, fecha);
        strcpy(q->elemento.funcion, funcion);
        q->elemento.direccion = direccion; // Guardar la dirección directamente como puntero
        q->elemento.df = df;
        q->elemento.id = id;
        q->elemento.llave = llave;

        q->siguiente = JNULL;
        q->anterior = JNULL;

        if (isEmptyJobList(*J)) {
            *J = q;
        } else if (p == JNULL) {
            // Insertar al final
            for (r = *J; r->siguiente != JNULL; r = r->siguiente);
            r->siguiente = q;
            q->anterior = r;

        } else if (p == *J) {
            // Insertar al inicio
            q->siguiente = p;
            p->anterior = q;
            *J = q;

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

            if (p->siguiente != JNULL) {
                p->siguiente->anterior = q;
            }
            q->siguiente = p->siguiente;
            p->siguiente = q;
            q->anterior = p;
        }
        return true;
    }
}

tPosJ firstJob(tListJ J){
    return J;
}

tPosJ lastJob(tListJ M){
    tPosJ p;

    for(p = M; p->siguiente != JNULL; p = p->siguiente);
    return p;
}

tPosJ nextJob(tPosJ p, tListJ J){
    return p->siguiente;
}

bool deleteJobPosition(tPosJ p, tListJ *J) {
    tPosJ q;

    if (isEmptyJobList(*J) || p == JNULL) {
        return false;
    }

    if (p == *J) {
        *J = p->siguiente;
        if (*J != JNULL) {
            (*J)->anterior = JNULL;
        }
    } else {
        q = p->anterior;
        q->siguiente = p->siguiente;
        if (p->siguiente != JNULL) {
            p->siguiente->anterior = q;
        }
    }

    free(p);
    return true;
}

void deleteJobList(tListJ *J){

    tPosJ p;

    while(!isEmptyJobList(*J)) {
        p = *J;
        *J = (*J)->siguiente;
        free(p);
    }

}

