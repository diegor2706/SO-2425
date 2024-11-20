// para el open

#ifndef p2_FILE_H
#define p2_FILE_H
#define FNULL NULL

#include <stdbool.h>

#define LNULL NULL
#define  MAX 30

typedef int Descriptor;

typedef struct elementF{
    Descriptor descriptor;
    char nombre[MAX];
    char modo[MAX];
    long offset;
}elementF;

typedef struct tNodeF *tPosF;

struct tNodeF {
    elementF data;
    tPosF siguiente;
};

typedef tPosF tListF;

void createEmptyFileList(tListF *F);
bool isEmptyFileList(tListF F);
bool createFileNode(tPosF *p);
bool insertFileItem(Descriptor j, char *m,char *n, long offset,  tPosF p, tListF *L);
tPosF firstFile(tListF L);
tPosF lastFile(tListF L);
tPosF nextFile(tPosF p, tListF L);
tPosF findFileItem(int x, tListF L);
void deleteFileAtPosition(tPosF p, tListF *L);
void deleteFileList(tListF *F);


#endif //p2_FILE_H
