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

bool insertJobItem(int PID, char *fecha, char *Status, char *nombre_usuario,
                   int comandLine, int prioridad, char *nombre_archivo, tPosJ p, tListJ *J) {
    tPosJ q, r;

    if (!createJobNode(&q)) {
        return false;
    } else {

        strcpy(q->elemento.fecha, fecha);
        strcpy(q->elemento.Status, Status);
        strcpy(q->elemento.nombre_usuario, nombre_usuario);
        strcpy(q->elemento.nombre_archivo, nombre_archivo);
        q->elemento.PID = PID;
        q->elemento.comandLine = comandLine;
        q->elemento.prioridad = prioridad;



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

            strcpy(q->elemento.fecha, p->elemento.fecha);
            strcpy(q->elemento.Status, p->elemento.Status);
            strcpy(q->elemento.nombre_usuario, p->elemento.nombre_usuario);
            strcpy(q->elemento.nombre_archivo, p->elemento.nombre_archivo);
            q->elemento.PID= p->elemento.PID;
            q->elemento.comandLine = p->elemento.comandLine;
            q->elemento.prioridad = p->elemento.prioridad;


            strcpy(p->elemento.fecha, fecha);
            strcpy(p->elemento.Status, Status);
            strcpy(p->elemento.nombre_usuario, nombre_usuario);
            strcpy(p->elemento.nombre_archivo, nombre_archivo);

            p->elemento.PID = PID;
            p->elemento.comandLine = comandLine;
            p->elemento.prioridad = prioridad;

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

