// Created by diegoroman on 14/11/24.
// Listas empleadas para el allocate

#ifndef P2_MEMORY_H
#define P2_MEMORY_H
#define MNULL NULL
#define  MAXI 50
#include <stdbool.h>

typedef struct elementM{
    void *direccion;
    char fecha [MAXI];
    char funcion[MAXI];
    char identificador[MAXI];

    int tam;
} elementM;

typedef struct mNode *tPosM;

struct mNode{
    elementM elemento;
    tPosM siguiente;
    tPosM anterior;
};

typedef tPosM tListM;

void createEmptyMemoryList(tListM *M);
bool isEmptyMemoryList(tListM M);
bool createMemoryNode(tPosM *p);
bool insertMItem(void *direc,int tam, char *fecha, char *funcion, char *identificador, tPosM p, tListM *L);
tPosM firstMemory(tListM M);
tPosM lastMemory(tListM M);
tPosM nextMemory(tPosM p, tListM M);
void deleteMemoryList(tListM *M);

#endif //P2_MEMORY_H
