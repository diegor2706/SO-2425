#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int terminado = 0;

int TrocearCadena(char * cadena, char * trozos[])
{ int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}

void imprimirPrompt(){
    printf("$" );
}

void leerEntrada(char * cadena){
    fgets(cadena, 100, stdin);
}

void procesarEntrada(char * cadena, char *trozos[]){
    TrocearCadena(cadena, trozos);

    if (trozos[0] != NULL){
        if (strcmp("1", trozos[0]) == 0){
            printf("hola\n");
        }
        else if(strcmp("bye", trozos[0]) == 0){

        }
    }else{
        printf("Comando no reconocido\n");
    }
}


void quit(){
    terminado = 1;
}

void bye (){
    terminado = 1;
}
void exit(){
    terminado = 1;
}

int main() {


    char cadena[]= "asignatura de sistemas operativos";
    char *trozos[10];



    while (!terminado){
        imprimirPrompt();
        leerEntrada(cadena);
        procesarEntrada(cadena, trozos);
    }

      //PRUEBA DE COMANDO 1
    printf("%i\n", TrocearCadena(cadena,trozos));
    printf("%s %s", trozos[2], trozos[3]);


    return 0;
}

