// para el open

#ifndef SO_P0_FILE_H
#define SO_P0_FILE_H
#define FNULL NULL

#include <stdbool.h>
typedef int Descriptor;
typedef char tItemF[100];

typedef struct elementF{
    Descriptor descriptor;
    tItemF nombre;
    tItemF modo;
}elementF;

typedef struct FNode *tPosF;

struct FNode{
    elementF elemento;
    tPosF siguiente;
    tPosF anterior;
};

typedef tPosF tListF;

void createEmptyFileList(tListF *L);
bool isEmptyFileList(tListF L);
bool createFileNode(tPosF *p);
bool insertFileItem(Descriptor j, tItemF m,tItemF n, tPosF p, tListF *L);
tPosF firstFile(tListF L);
tPosF lastFile(tListF L);
tPosF nextFile(tPosF p, tListF L);
void deleteFile(tPosF p, tListF *L);


#endif //SO_P0_FILE_H
