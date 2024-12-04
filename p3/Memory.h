// Created by diegoroman on 14/11/24.
// Listas empleadas para el allocate

#ifndef P3_MEMORY_H
#define P3_MEMORY_H
#define MNULL NULL
#define  MAXI 50
#include <stdbool.h>
#include <sys/types.h>

typedef struct elementM{
    void *direccion;
    char fecha [MAXI];
    char funcion[MAXI];
    int  df;
    int id;
    key_t llave;
    long tam;

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
bool insertMItem(void *direc,long tam, char *fecha, char *funcion, int df , int id,key_t llave,tPosM p, tListM *L);
tPosM firstMemory(tListM M);
tPosM lastMemory(tListM M);
tPosM nextMemory(tPosM p, tListM M);
void deleteMallocMemoryPosition(tPosM p, tListM *M);
void deleteSharedMemoryPosition(tPosM p, tListM *M);
void deleteMmapMemoryPosition(tPosM p, tListM *M);
void deleteMemoryList(tListM *M);


#endif //P3_MEMORY_H
