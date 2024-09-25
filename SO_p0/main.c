/* Tomás Cotelo Álvarez -> tomas.cotelo */
/* Diego Román Pose -> diego.roman */
/* Grupo 3.2 */
/* practica 0 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "Lista.h"
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h> // para el pid y el ppid
#include <sys/utsname.h> // para el infosys

#define MAX 150

void historial (char cadena[], tList L, bool terminado);


int esNumero(char cadena[]) {

    for (int i = 0; i < strlen(cadena); i++) {
        if (!isdigit(cadena[i])) {
            return 0;
        }
    }

    return 1;
}

int TrocearCadena(char * cadena, char * trozos[]) {
    int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}

void imprimirPrompt(){
    printf("$");
}

void leerEntrada(char * cadena, tList *L){
    fgets(cadena, 100, stdin);

    size_t len = strlen(cadena);
    if (len > 0 && cadena[len-1] == '\n') {
        cadena[len-1] = '\0';
    }
    insertItem(cadena, LNULL, L);
}

void quit(bool *terminado){
    *terminado = true;
}

void authors (char cadena[]){
    if (cadena == NULL ) {
        printf("login 1: tomas.cotelo \tname 1: Tomás Cotelo Álvarez \nlogin 2: diego.roman \tname 2: Diego Román Pose\n");
    }
    else if (strcmp("-l", cadena) == 0){
        printf("login 1: tomas.cotelo \nlogin 2: diego.roman\n");
    }
    else if (strcmp("-n", cadena) == 0){
        printf("name 1: Tomás Cotelo Álvarez \nname 2: Diego Román Pose \n");
    }else{
        printf("Comando 'authors %s' no reconocido\n", cadena);
    }

}

void date (char cadena[]){
    time_t tiempo_actual;
    struct tm *tiempo_local;
    char buffer[80];

    time(&tiempo_actual);   //Pone en segundos el tiempo actual.

    tiempo_local = localtime(&tiempo_actual);   //Divide en el struct tiempo_local el tiempo transcurrido en día, mes, año, hora, minutos y segundos.

    if (cadena == NULL ) {
        strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", tiempo_local);    //Convierte el struct en una cadena de caracteres segun el % que se le indique.
        printf("Fecha y hora actuales: %s\n", buffer);
    }
    else if (strcmp("-t", cadena) == 0){
        strftime(buffer, sizeof(buffer), "%H:%M:%S", tiempo_local);
        printf("Hora actual: %s\n", buffer);
    }
    else if (strcmp("-d", cadena) == 0){
        strftime(buffer, sizeof(buffer), "%d/%m/%Y", tiempo_local);
        printf("Fecha actual: %s\n", buffer);
    }else{
        printf("Comando 'date %s' no reconocido\n", cadena);
    }

}

void infosys() {
    struct utsname sys_info;

    if (uname(&sys_info) == 0) {
        printf("Nombre del sistema operativo: %s\n", sys_info.sysname);
        printf("Nombre de la máquina (hostname): %s\n", sys_info.nodename);
        printf("Versión del sistema operativo: %s\n", sys_info.version);
        printf("Revisión del sistema operativo: %s\n", sys_info.release);
        printf("Arquitectura de hardware: %s\n", sys_info.machine);
    } else {
        perror("Error obteniendo información del sistema");
    }
}

void cd (char cadena[]){

    char Actual_Directory[MAX];
    if (cadena == NULL ) {
        getcwd(Actual_Directory,sizeof (Actual_Directory));
        printf("%s",Actual_Directory);
    }
    else if (strcmp("-l", cadena) == 0){
        printf("login 1: tomas.cotelo \nlogin 2: diego.roman\n");
    }
    else if (strcmp("-n", cadena) == 0){
        printf("name 1: Tomás Cotelo Álvarez \nname 2: Diego Román Pose \n");
    }else{
        printf("Comando 'authors %s' no reconocido\n", cadena);
    }


}

void help(char cadena[]){
    if (cadena == NULL) {
        printf("authors / pid / ppid / cd / date / historic / open / close / dup / infosys / quit / exit / bye\n ");
    }
    else if (strcmp("authors",cadena) == 0){
        printf("authors: muestra el nombre y el login de los autores.\n[-n]: muestra solo el nombre.\n[-l]: muestra el login de los autores.\n");
    }
    else if (strcmp("pid",cadena) == 0) {
        printf("pid: muestra el pid del shell.\n");
    }
    else if (strcmp("ppid",cadena) == 0) {
        printf("Prints the names and logins of the program authors. authors -l prints only the logins and authors -n prints only the names\n");
    }
    else if (strcmp("cd",cadena) == 0){
        printf("Prints the names and logins of the program authors. authors -l prints only the logins and authors -n prints only the names\n");
    }
    else if (strcmp("date",cadena) == 0){
        printf("date: imprime la fecha actual en formato DD/MM/YYYY y la hora actual en formato hh:mm:ss.\n[-d]: muestra solo el día en formato DD/MM/YYYY\n[-t]: muestra solo la fecha en formato hh:mm:ss\n");;
    }
    else if (strcmp("historic",cadena) == 0){
        printf("Prints the names and logins of the program authors. authors -l prints only the logins and authors -n prints only the names\n");
    }
    else if (strcmp("open",cadena) == 0){
        printf("Prints the names and logins of the program authors. authors -l prints only the logins and authors -n prints only the names\n");
    }
    else if (strcmp("close",cadena) == 0){
        printf("Prints the names and logins of the program authors. authors -l prints only the logins and authors -n prints only the names\n");
    }
    else if (strcmp("dup",cadena) == 0){
        printf("Prints the names and logins of the program authors. authors -l prints only the logins and authors -n prints only the names\n");
    }
    else if (strcmp("infosys",cadena) == 0){
        printf("Imprime información sobre la máquina que ejecuta el Shell\n");
    }
    else if  ((strcmp("quit", cadena) == 0)|| (strcmp("exit", cadena) == 0) || (strcmp("bye", cadena) == 0) ){
        printf("Finaliza la ejecución del shell\n");
    }
}

void procesarEntrada(char * cadena, char *trozos[], bool *terminado, tList L){
    TrocearCadena(cadena, trozos);

    if (trozos[0] != NULL){
        if ((strcmp("quit", trozos[0]) == 0)|| (strcmp("exit", trozos[0]) == 0) || (strcmp("bye", trozos[0]) == 0) ){
            quit(terminado);
        }
        else if(strcmp("authors", trozos[0]) == 0){
            authors(trozos[1]);
        }
        else if(strcmp("date", trozos[0]) == 0){
            date(trozos[1]);
        }
        else if(strcmp("help", trozos[0]) == 0){
            help(trozos[1]);
        }
        else if(strcmp("cd", trozos[0]) == 0) {
            cd (trozos[1]);
        }
        else if(strcmp("historic", trozos[0]) == 0) {
            historial(trozos[1], L, *terminado);
        }
        else if (strcmp("infosys", trozos[0]) == 0){
            infosys();
        }
        else if (strcmp("pid", trozos[0]) == 0){
            pid_t pid = getpid();
            printf("PID del proceso ejecutando el shell: %d\n", pid);
        }
        else if (strcmp("ppid", trozos[0]) == 0){
            pid_t ppid = getpid();
            printf("PPID del proceso ejecutando el shell: %d\n", ppid);
        }
        else if (strcmp("open", trozos[0]) == 0){
            void Cmd_open (char * tr[]);
        }
        else if (strcmp("close", trozos[0]) == 0){

        }
        else if (strcmp("dup", trozos[0]) == 0){

        }
        else{
            printf("Comando no reconocido\n");
        }
    }

    else {
        printf("Comando no reconocido\n");
    }
}


void historial (char cadena[], tList L, bool terminado) {

    tPosL i, j; // posición del comando en la lista
    int p = 0; // posición del comando en el historial
    int aux;
    char auxi[MAX];

    char *trozos[10];

    if (cadena == NULL) { // imprimir el historial entero
        for (i = first(L); i != LNULL; i = next(i,L)){
            printf("%i -> %s\n",p,i->elemento.datos);
            p++;
        }
    }
    else if (esNumero(cadena)){ //  repetir el comando número N
        aux = atoi(cadena);

        for (i = first(L); p != aux && i != LNULL; i = next(i,L)){
            p++;
        }
        if (i ==LNULL){
            printf("No hay elemento %s en el historial\n", cadena);
        }
        else{
            printf("Ejecutando hist (%d): %s\n", aux, i->elemento.datos);

            strcpy(auxi,i->elemento.datos);
            procesarEntrada(auxi, trozos, &terminado, L);
        }
    }
    else if (('-' == cadena[0]) && esNumero(cadena+1) && (cadena[1] != '\0')) { ;// repetir los últimos N comandos
        aux = atoi(cadena+1);

        for (i = last(L); p != aux; i = i->anterior)  {
            p++;
        }
        for ( j = i->siguiente; j != LNULL; j = next(j, L) ){
            printf("%i -> %s\n",p+aux, j->elemento.datos );
            p++;
        }

    }else{
        printf("Comando 'historic %s' no reconocido\n", cadena);
    }
}

int main() {

    bool terminado = false;
    char cadena[MAX]= "asignatura de sistemas operativos";
    char *trozos[10];
    tList L;
    createEmptyList(&L);

    while (!terminado){
        imprimirPrompt();
        leerEntrada(cadena,&L);
        procesarEntrada(cadena, trozos, &terminado,L);
    }
    return 0;
}
