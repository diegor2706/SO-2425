// para el open

#ifndef SO_P0_FILE_H
#define SO_P0_FILE_H
#define FNULL NULL

#include <stdbool.h>
typedef int Descriptor;
typedef char tItemL[30];

typedef struct element{
    tItemL nombre;
    tItemL modo;
    Descriptor descriptor;

}element;

typedef struct tNode *tPosL;

struct tNode{

    element elemento;
    tPosL siguiente;
    tPosL anterior;

};

typedef tPosL tList;

void createEmptyFileList(tList *L);

bool createFileNode(tPosL *p);
bool insertFileItem(Descriptor j,tItemL m, tItemL d, tPosL p, tList *L);
tPosL firstFile(tList L);
tPosL lastFile(tList L);
tPosL nextFile(tPosL p, tList L);
void deleteFile(tPosL p, tList *L);


#endif //SO_P0_FILE_H
