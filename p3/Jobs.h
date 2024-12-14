//
// Created by diegoroman on 12/12/24.
// para el ListJobs y para el DelJobs

#ifndef P3_JOBS_H
#define P3_JOBS_H
#define JNULL NULL
#define  MAXI 50
#include <stdbool.h>
#include <sys/types.h>

typedef struct elementJ{
    int  PID;
    char fecha [MAXI];
    char Status[MAXI];
    char nombre_usuario[MAXI];
    char tipo_signal[MAXI];
    int prioridad;
    char nombre_archivo[MAXI];

} elementJ;

typedef struct jNode *tPosJ;

struct jNode{
    elementJ elemento;
    tPosJ siguiente;
    tPosJ anterior;
};

typedef tPosJ tListJ;

void createEmptyJobList(tListJ *J);
bool isEmptyJobList(tListJ J);
bool createJobNode(tPosJ *p);
bool insertJobItem(int PID, char *fecha, char *Status, char *nombre_usuario, char *tipo_signal, int prioridad, char *nombre_archivo, tPosJ p, tListJ *J);
tPosJ firstJob(tListJ J);
tPosJ lastJob(tListJ J);
tPosJ nextJob(tPosJ p, tListJ J);
bool deleteJobPosition(tPosJ p, tListJ *J);
void deleteJobList(tListJ *J);

#endif //P3_JOBS_H
