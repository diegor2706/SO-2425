/* Tomás Cotelo Álvarez -> tomas.cotelo */
/* Diego Román Pose -> diego.roman */
/* Grupo 3.2 */
/* práctica 3 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "Lista.c"          // Incluye el archivo de encabezado correspondiente
#include "File.c"           // Incluye el archivo de encabezado correspondiente
#include "Memory.c"         // Incluye el archivo de encabezado correspondiente
#include "Search.c"         // Incluye el archivo de encabezado correspondiente
#include "Jobs.c"           // Incluye el archivo de encabezado correspondiente
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>         // para el pid y el ppid
#include <limits.h>
#include <sys/utsname.h>    // para el infosys
#include <fcntl.h>          // para el open
#include <dirent.h>         // para el listfile
#include <sys/stat.h>       // necesaria para el mkdir y listdir
#include <sys/types.h>
#include <pwd.h>            // necesaria para el listfile -long
#include <grp.h>            // necesaria para el listfile -long
#include <sys/resource.h>   // necesaria para el exec
#include <signal.h>         // necesaria para el back
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define TAMANO 2048

void historial (char cadena[], tList L, bool terminado, tListF f, tListM M, tListS S,tListJ J, char *envp[]);

int CambiarVariable(char *var, char *valor, char *e[]);

int BuscarVariable(char *var, char *e[]);

void Cmd_open (char * tr[], tListF F);

void Cmd_close (char *tr[], tListF F);

char* fecha_allocate() {
    static char formatted_date[20];
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    strftime(formatted_date, sizeof(formatted_date), "%b %d %H:%M", local);

    return formatted_date;
}

int esNumero(char cadena[]) { // función aux

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

// funcion auxiliar para la impresion de LISTFILE
int Trocear_Direccion(char * cadena, char * trozos[]) {
    int i=1;
    if ((trozos[0]=strtok(cadena,"/"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL,"/"))!=NULL)
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

void quit(bool *terminado, tList *L, tListF *F,tListM *M,tListS *S,tListJ *J) {
    *terminado = true;
    deleteList(L);
    deleteFileList(F);
    deleteMemoryList(M);
    deleteListSE(S);
    deleteJobList(J);
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

char *dateJobs() {
    time_t tiempo_actual;
    struct tm *tiempo_local;

    tiempo_actual = time(NULL);

    tiempo_local = localtime(&tiempo_actual);

    char *buffer = malloc(80 * sizeof(char));

    strftime(buffer, 80, "%d/%m/%Y %H:%M:%S", tiempo_local);
    return buffer;
}

void date (char cadena[]) {

    time_t tiempo_actual;
    struct tm *tiempo_local;
    char buffer[80];

    time(&tiempo_actual);   //Pone en segundos el tiempo actual.

    tiempo_local = localtime(&tiempo_actual);   //Divide en el struct tiempo_local el tiempo transcurrido en día, mes, año, hora, minutos y segundos.

    if (cadena == NULL ) {
        strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", tiempo_local);  //Convierte el struct en una cadena de caracteres segun el % que se le indique.
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
        perror("Error obteniendo información del sistema\n");
    }
}

void cd (char cadena[]) {

    char New_Directory[100]; // le damos 100 para evitar problemas de espacio
    char cwd[PATH_MAX];
    char dest[100] = "/";    // le damos 100 para evitar problemas de espacio

    // Obtener el directorio de trabajo actual y verificar si se obtiene correctamente
    if (cadena == FNULL){
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("Directorio de trabajo actual: %s\n", cwd);
        }
    }
    else if (strcmp(cadena, "..") == 0) {
        chdir("..");
    }
    else {
        getcwd(New_Directory, sizeof(New_Directory));
        strcat(dest, cadena); //añadir automaticamente '/' para cambiar al nuevo directorio
        strcat(New_Directory, dest);
        if (chdir(New_Directory) != 0) {
            printf("No ejecutado: No such file or directory\n");
        }
    }
}

void cwd(const char cadena[]) {
    char cwd[PATH_MAX];  // Array para almacenar la ruta


    if ( cadena == FNULL){
        // Obtener el directorio de trabajo actual y verificar si se obtiene correctamente
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("Directorio de trabajo actual: %s\n", cwd);
        } else {
            perror("Error al obtener el directorio de trabajo actual\n");
        }
    }else{
        printf("Comando no reconocido\n");
    }
}

void help(char cadena[]){
    if (cadena == NULL) {
        printf("authors / pid / ppid / cd / date / historic / open / close / dup / infosys / quit / exit / bye\n");
        printf("makefile / makedir / listfile / cwd / listdir / reclist / revlist / erase / delrec\n");
    }
    else if (strcmp("authors",cadena) == 0){
        printf("authors: muestra el nombre y el login de los autores.\n[-n]: muestra solo el nombre.\n[-l]: muestra el login.\n");
    }
    else if (strcmp("pid",cadena) == 0) {
        printf("pid: muestra el PID (Process ID) del proceso que ejecuta el shell.\n");
    }
    else if (strcmp("ppid",cadena) == 0) {
        printf("ppid: muestra el PPID (Parent Process ID) del proceso padre que está ejecutando el shell.\n");
    }
    else if (strcmp("cd",cadena) == 0){
        printf("cd [dir]: cambia la ruta actual a la ruta indicada. si no se indica ninguna ruta, se muestra la ruta actual.\n");
    }
    else if (strcmp("date",cadena) == 0){
        printf("date: imprime la fecha actual en formato DD/MM/YYYY y la hora actual en formato hh:mm:ss.\n");
        printf("[-d]: muestra solo el día en formato DD/MM/YYYY\n");
        printf("[-t]: muestra solo la fecha en formato hh:mm:ss\n");
    }
    else if (strcmp("historic",cadena) == 0){
        printf("historic: muestra el historial de los comandos ejecutados en el shell.\n");
        printf("[]: muestra el historial completo.\n");
        printf("[N]: repite el comando N del historial.\n");
        printf("[-N]: muestra los últimos N comandos.\n");
    }
    else if (strcmp("open",cadena) == 0){
        printf("open: abre un archivo y lo añade (junto con el descriptor de archivo y el modo de apertura) a la lista de archivos abiertos de la shell.\n\n");

        printf("[cr] para O_CREAT (crear archivo si no existe)\n");
        printf("[ap] para O_APPEND (añadir al final del archivo)\n");
        printf("[ex] para O_EXCL (fallar si el archivo ya existe)\n");
        printf("[ro] para O_RDONLY (lectura)\n");
        printf("[rw] para O_RDWR (lectura y escritura)\n");
        printf("[wo] para O_WRONLY (solo escritura)\n");
        printf("[tr] para O_TRUNC (truncar el archivo)\n\n");

        printf("si se usa sin argumentos, muestra la línea de comandos abiertos por el shell.\n");
    }
    else if (strcmp("close",cadena) == 0){
        printf("close [df]: cierra el descriptor de archivo df y elimina el elemento correspondiente de la lista de archivos abiertos.\n");
    }
    else if (strcmp("dup",cadena) == 0){
        printf("dup [df]: duplica el descriptor de archivo df, creando la entrada correspondiente en la lista de archivos.\n");
    }
    else if ((strcmp("help",cadena) == 0) || (strcmp("command", cadena) == 0)) {
        printf("help / command -?: muestra información sobre los comandos.\n[cmd]: muestra información sobre el comando cmd indicado.\n");
    }
    else if (strcmp("infosys",cadena) == 0){
        printf("infosys: imprime información sobre la máquina que ejecuta el shell\n");
    }
    else if  ((strcmp("quit", cadena) == 0) || (strcmp("exit", cadena) == 0) || (strcmp("bye", cadena) == 0)){
        printf("quit / exit / bye: finaliza la ejecución del shell\n");
    }
    else if (strcmp("makefile",cadena) == 0){
        printf("makefile [name]: crea un nuevo fichero\n");
    }
    else if (strcmp("makedir",cadena) == 0){
        printf("makedir [name]: crea un nuevo directorio\n");
    }
    else if ((strcmp("listfile",cadena) == 0) || (strcmp("listfile -long", cadena) == 0) || (strcmp("listfile -link", cadena) == 0) || (strcmp("listfile -acc", cadena) == 0)){
        printf("listfile []: brinda información sobre ficheros o directorios\n");
        printf("[-long]: listado largo. \n");
        printf("[-acc]: acesstime. \n");
        printf("[-link]: si el enlace es simbólico, el path contenido. \n");
    }
    else if (strcmp("cwd",cadena) == 0){
        printf("cwd: imprime el directorio de trabajo actual\n");
    }
    else if ((strcmp("listdir",cadena) == 0) || (strcmp("listdir -long", cadena) == 0) || (strcmp("listdir -hid", cadena) == 0)
             || (strcmp("listdir -acc", cadena) == 0) || (strcmp("listfile -link", cadena) == 0)) {
        printf("listdir []: enumera el contenido de los directorios\n");
        printf("[-long]: listado largo. \n");
        printf("[-hid]: incluye los ficheros ocultos. \n");
        printf("[-acc]: acesstime. \n");
        printf("[-link]: si el enlace es simbólico, el path contenido. \n");
    }
    else if ((strcmp("reclist",cadena) == 0) || (strcmp("reclist -long", cadena) == 0) || (strcmp("reclist -hid", cadena) == 0)
             || (strcmp("reclist -acc", cadena) == 0) || (strcmp("reclist -link", cadena) == 0)) {
        printf("reclist: enumera directorios de forma recursiva (subdirectorios después)\n");
        printf("[-long]: listado largo. \n");
        printf("[-hid]: incluye los ficheros ocultos. \n");
        printf("[-acc]: acesstime. \n");
        printf("[-link]: si el enlace es simbólico, el path contenido. \n");
    }
    else if ((strcmp("revlist",cadena) == 0) || (strcmp("revlist -long", cadena) == 0) || (strcmp("revlist -hid", cadena) == 0)
             || (strcmp("revlist -acc", cadena) == 0) || (strcmp("revlist -link", cadena) == 0)) {
        printf("revlist: enumera directorios de forma recursiva (subdirectorios antes)\n");
        printf("[-long]: listado largo. \n");
        printf("[-hid]: incluye los ficheros ocultos. \n");
        printf("[-acc]: acesstime. \n");
        printf("[-link]: si el enlace es simbólico, el path contenido. \n");
    }
    else if (strcmp("erase",cadena) == 0){
        printf("erase: elimina archivos y/o directorios vacíos\n");
    }
    else if (strcmp("delrec",cadena) == 0){
        printf("delrec: elimina archivos y/o directorios no vacíos de forma recursiva\n");
    }
    else if (strcmp("allocate", cadena) == 0){
        printf("allocate [-malloc|-shared|-createshared|-mmap]... Asigna un bloque de memoria\n");
        printf("[-malloc] [tam]: asigna un bloque malloc de tamano tam\n");
        printf("[-createshared] [cl] [tam]: asigna (creando) el bloque de memoria compartida de clave cl y tamano tam\n");
        printf("[-shared] [cl]: asigna el bloque de memoria compartida (ya existente) de clave cl\n");
        printf("[-mmap] [fich] [perm]: mapea el fichero fich, perm son los permisos\n");
    }
    else if(strcmp("deallocate", cadena) == 0){
        printf("deallocate [-malloc|-shared|-delkey|-mmap|addr]..\tDesasigna un bloque de memoria\n");
        printf("[-malloc] [tam]: desasigna el bloque malloc de tamano tam\n");
        printf("[-shared] [cl]: desasigna (desmapea) el bloque de memoria compartida de clave cl\n");
        printf("[-delkey] [cl]: elimina del sistema (sin desmapear) la clave de memoria cl\n");
        printf("[-mmap] [fich]: desmapea el fichero mapeado fich\n");
        printf("addr: desasigna el bloque de memoria en la direccion addr\n");
    }
    else if(strcmp("memfill", cadena) == 0){
        printf("memfill addr cont byte \tLlena la memoria a partir de addr con byte\n");
    }
    else if(strcmp("memdump", cadena) == 0){
        printf("memdump addr cont \tVuelca en pantallas los contenidos (cont bytes) de la posicion de memoria addr\n");
    }
    else if(strcmp("memory", cadena) == 0){
        printf("memory [-blocks|-funcs|-vars|-all|-pmap] ..\tMuestra muestra detalles de la memoria del proceso\n");
        printf("[-blocks]: los bloques de memoria asignados\n");
        printf("[-funcs]: las direcciones de las funciones\n");
        printf("[-vars]: las direcciones de las variables\n");
        printf("[:-all]: todo (-funcs, -vars y -blocks)\n");
        printf("[-pmap]: muestra la salida del comando pmap(o similar)\n");
    }
    else if(strcmp("readfile", cadena) == 0){
        printf("readfile fiche addr cont \tLee cont bytes desde fich a la direccion addr\n");
    }
    else if(strcmp("writefle", cadena) == 0){
        printf("writefile [-o] fiche addr cont \tEscribe cont bytes desde la direccion addr a fich (-o sobreescribe)\n");
    }
    else if(strcmp("read", cadena) == 0) {
        printf("read df addr cont\tTransfiere cont bytes del fichero descrito por df a la dirección addr\n");
    }
    else if(strcmp("write", cadena) == 0){
        printf("write df addr cont\tTransfiere cont bytes desde la dirección addr al fichero descrito por df\n");
    }
    else if(strcmp("recurse", cadena) == 0){
        printf("recurse [n]\tInvoca a la funcion recursiva n veces\n");
    }
    else if(strcmp("getuid", cadena) == 0){
        printf("getuid: Muestra las credenciales del proceso que ejecuta el shell\n");
    }
    else if(strcmp("recurse", cadena) == 0){
        printf("recurse [n]\tInvoca a la funcion recursiva n veces\n");
    }
    else if(strcmp("setuid", cadena) == 0){
        printf("setuid [-l] id\tCambia las credenciales del proceso que ejecuta el shell\n");
        printf("id: establece la credencial al valor numerico id\n");
        printf("-l id: establece la credencial a login id\n");
    }
    else if(strcmp("showbar", cadena) == 0){
        printf("showbar [var]: Muestra el valor y las direcciones de la variable de entorno var\n");
    }
    else if(strcmp("recurse", cadena) == 0){
        printf("recurse [n]\tInvoca a la funcion recursiva n veces\n");
    }
    else if(strcmp("changevar", cadena) == 0){
        printf("changevar [-a|-e|-p] var valor\tCambia el valor de una variable de entorno\n");
        printf("-a: accede por el tercer arg de main\n");
        printf("-e: accede mediante environ\n");
        printf("-p: accede mediante putenv\n");
    }
    else if(strcmp("subsvar", cadena) == 0){
        printf("subsvar [-a|-e] var1 var2 valor\tSustituye la variable de entorno var1 con var2=valor\n");
        printf("-a: accede por el tercer arg de main\n");
        printf("-e: accede mediante environ\n");
    }
    else if(strcmp("environ", cadena) == 0){
        printf("environ [-environ|-addr] \t Muestra el entorno del proceso\n");
        printf("-environ: accede usando environ (en lugar del tercer arg de main)\n");
        printf("-addr: muestra el valor y donde se almacenan environ y el 3er arg main\n");
    }
    else if(strcmp("search", cadena) == 0){
        printf("search [-add|-del|-clear|-path]..\tManipula o muestra la ruta de busqueda del shell (path)\n");
        printf("-add dir: aniade dir a la ruta de busqueda(equiv +dir)\n");
        printf("-del dir: elimina dir de la ruta de busqueda (equiv -dir)\n");
        printf("-clear: vacia la ruta de busqueda\n");
        printf("-path: importa el PATH en la ruta de busqueda\n");
    }
    else if(strcmp("exec", cadena) == 0){
        printf("rexec VAR1 VAR2 ..prog args....[@pri]\tEjecuta, sin crear proceso,prog con argumentos\n"
               "\t en un entorno que contiene solo las variables VAR1, VAR2...\n");
    }
    else if(strcmp("fg", cadena) == 0){
        printf("fg prog args...\tCrea un proceso que ejecuta en primer plano prog con argumentos\n");
    }
    else if(strcmp("fgpri", cadena) == 0){
        printf("fgpri prio prog args...\tCrea un proceso que ejecuta en primer plano prog con argumentos  con la prioridad cambiada a prio\n");
    }
    else if(strcmp("back", cadena) == 0){
        printf("back prog args...\tCrea un proceso que ejecuta en segundo plano prog con argumentos\n");
    }
    else if(strcmp("backpri", cadena) == 0){
        printf("backpri prio prog args...\tCrea un proceso que ejecuta en segundo plano prog con argumentos con la prioridad cambiada a prio\n");
    }
    else if(strcmp("listjobs", cadena) == 0){
        printf("listjobs \tLista los procesos en segundo plano\n");
    }
    else if(strcmp("deljobs", cadena) == 0){
        printf("deljobs [-term][-sig]\tElimina los procesos de la lista procesos en sp\n");
        printf("-term: los terminados\n");
        printf("-sig: los terminados por senal\n");
    }
    else printf("comando no reconocido\n");
}

void inicicializarFileLIst(tListF *F){
    insertFileItem(0,"estandar O_RDWR", "entrada",-1, FNULL, F);
    insertFileItem(1,"estandar O_RDWR", "salida",-1 ,FNULL, F);
    insertFileItem(2,"estandar O_RDWR", "error", -1,FNULL, F);
}

void ListarFicherosAbiertos(tListF F){
    tPosF i;
    int j = 0;

    for (i = firstFile(F); i != FNULL; i = nextFile(i, F)){
        while (j < i->data.descriptor){
            printf("descriptor: %i, offset: (  )-> no usado\n",j);
            j++;
        }if (i->data.descriptor < 3){
            printf("descriptor: %i, offset: (  ) -> %s %s\n", i->data.descriptor, i->data.nombre, i->data.modo);
            j ++;
        }else{
            printf("descriptor: %i, offset: ( %ld ) -> %s %s\n", i->data.descriptor, i->data.offset, i->data.nombre, i->data.modo);
            j ++;
        }
    }

    while (j < 20 ){
        printf("descriptor: %i, offset: (  ) -> no usado\n",j);
        j++;
    }
}

void AnadirAFicherosAbiertos(tListF *F, int df, char *nombre, char *modo) {

    tItemL fileName, fileMode;
    tPosF i,q = lastFile(*F);

    // Copiar el nombre del archivo y el modo de apertura
    strcpy(fileName, nombre);
    strcpy(fileMode, modo);
    off_t offset = lseek(df, 0, SEEK_CUR);

    if (q->data.descriptor < df) {
        if (insertFileItem(df, fileMode, fileName,offset, NULL, F)) {
            printf("Archivo %s con descriptor %d y modo %s añadido a la lista de ficheros abiertos\n", nombre, df,modo);

        }
    } else {
        for (i = firstFile(*F); i->data.descriptor < df; i = i->siguiente);
        if (insertFileItem(df, fileMode, fileName, offset,i, F)) {
            printf("Archivo %s con descriptor %d y modo %s añadido a la lista de ficheros abiertos\n", nombre, df,modo);

        }else{
            printf("Error al añadir el archivo a la lista\n");
        }
    }
}

char* NombreFicheroDescriptor(int df, tListF F) {
    tPosF i;

    // Recorrer la lista de archivos abiertos
    for (i = firstFile(F); i != FNULL; i = nextFile(i, F)) {
        if (i->data.descriptor == df) {
            return i->data.nombre;  // Devolver el nombre del archivo
        }
    }
    return NULL;  // Si no se encuentra el descriptor
}

void Cmd_dup (char * tr[], tListF F)
{
    int df, duplicado;
    char aux[MAX],*p;

    if (tr[0]==NULL || (df=atoi(tr[0]))<0) { /*no hay parametro*/
        ListarFicherosAbiertos(F);                 /*o el descriptor es menor que 0*/
        return;
    }

    duplicado=dup(df);
    p = NombreFicheroDescriptor(df, F);
    sprintf (aux,"dup %d (%s)",df, p);
    AnadirAFicherosAbiertos(&F, duplicado, aux, "");
}

void Cmd_close (char *tr[], tListF F) {
    int df, x;
    tPosF pos;

    if (tr[0]==NULL || (df=atoi(tr[0]))<0) {
        ListarFicherosAbiertos(F);
        return;
    }

    if (close(df)==-1){
        perror("Inposible cerrar descriptor");
    }
    else{
        x = atoi(tr[0]);

        pos = findFileItem( x, F);
        deleteFileAtPosition(pos, &F);
    }
}

void makefile(const char filename[]) {
    int fd;

    if (filename == FNULL){
        printf("Error, introduce un nombre para el archivo a continuacion de makefile.\n");
    }
    else{
        if ((fd = open(filename, O_CREAT, 0777)) == -1) {
            perror("Imposible crear fichero\n");
        }
        fopen(filename, "O_CREAT");



        printf("Archivo %s creado con éxito.\n", filename);
        close(fd);
    }
}

void makedir(const char *dirname){
    if(mkdir(dirname, 0755) == -1) {
        perror("Error al crear el directorio");
    }
    else printf("Directorio %s creado exitosamente.\n", dirname);

}

char LetraTF (mode_t m) { // función auxiliar - help code

    switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio */
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no deberia aparecer*/
    }
}

char * ConvierteModo2 (mode_t m)
{
    static char permisos[12];
    strcpy (permisos,"---------- ");

    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';

    return permisos;
}

//listfile gives information on files or directories
void listfile(char *filename[]) {
    struct stat fileStat;
    char formatted_date[50];
    int aux = -1;
    char acces_date[50];

    if (filename[0] == NULL) {
        cd (NULL);
        return;
    }

    if (strcmp("-long", filename[0]) == 0) {
        aux = 0;
    } else if (strcmp("-acc", filename[0]) == 0) {
        aux = 1;
    } else if (strcmp("-link", filename[0]) == 0) {
        aux = 2;
    }
    if (filename[1] == NULL && aux != -1) {
        cd(NULL);
        return;
    }

    if (lstat(filename[aux == -1 ? 0 : 1], &fileStat) == -1) {
        perror("Error al obtener la información del archivo");
        return;
    }

    strftime(formatted_date, sizeof(formatted_date), "%Y/%m/%d-%H:%M", localtime(&fileStat.st_mtime));

    if (aux == 0) {  // -long
        struct passwd *pw = getpwuid(fileStat.st_uid);
        struct group  *gr = getgrgid(fileStat.st_gid);
        char *permisos = ConvierteModo2(fileStat.st_mode);

        printf("%s   %ld (%ld) %s %s %s %8ld %s\n",
               formatted_date,
               fileStat.st_nlink,
               fileStat.st_ino,
               pw->pw_name,
               gr->gr_name,
               permisos,
               fileStat.st_size,
               filename[1]);
    }
    else if (aux == 1) {
        struct tm *lt = localtime(&fileStat.st_atime);

        strftime(acces_date, sizeof(acces_date), "%Y/%m/%d-%H:%M", lt);

        printf("%8ld   %s %s\n", fileStat.st_size, acces_date, filename[1]);
    }
    else if (aux == 2) {  // -link
        if (LetraTF(fileStat.st_mode) == 'l') {
            char link_path[PATH_MAX];
            ssize_t len = readlink(filename[1], link_path, sizeof(link_path) - 1);
            if (len != -1) {
                link_path[len] = '\0';
                printf("%8ld   %s -> %s\n", fileStat.st_size, filename[1],link_path);
            } else {
                perror("Error al leer el enlace simbólico");
            }
        } else {
            printf("%8ld   %s\n", fileStat.st_size, filename[1]);
        }
    } else {
        printf("%8ld   %s\n", fileStat.st_size, filename[0]);
    }
}

//listdir lists directories contents
void listdir(char *path[]) {
    DIR *dir;
    struct dirent *entry;
    char fullpath[1024];
    struct stat fileStat;
    int aux = -1;
    int es_doble;
    char *envio_file[15] = {NULL};

    if (path[0] == NULL) {
        cd(NULL);
        return;
    }


    if (strcmp("-long", path[0]) == 0) {
        aux = 0;
    } else if (strcmp("-acc", path[0]) == 0) {
        aux = 1;
    } else if (strcmp("-link", path[0]) == 0) {
        aux = 2;
    } else if (strcmp("-hid", path[0]) == 0) {
        aux = 3;
    }

    if (path[1] == NULL && aux != -1) {
        cd(NULL);
        return;
    }

    es_doble = (aux == -1 ? 0 : 1);

    if ((dir = opendir(path[es_doble])) == NULL) {
        perror("Error al abrir el directorio");
        return;
    }

    printf("************%s\n", path[es_doble]);

    while ((entry = readdir(dir)) != NULL) {

        if (aux != 3){
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
        }
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path[es_doble], entry->d_name);

        if (lstat(fullpath, &fileStat) == -1) {
            perror("Error al obtener la información del archivo");
            continue;
        }


        if (aux == -1) {
            printf("%8ld  %s\n", fileStat.st_size, entry->d_name);
        } else {

            if (aux == 0 || aux == 1 || aux == 2 ) {
                envio_file[0] = path[0];
                envio_file[1] = fullpath;
            }
            else {
                envio_file[0] = fullpath;
            }

            listfile(envio_file);
        }
    }

    closedir(dir);
}

void reclist(char *path[]) {
    DIR *dir;
    struct dirent *entry;
    char fullpath[1024];
    int aux = -1;
    int es_doble;
    char *envio_file[15] = {NULL};

    //reclist
    if (path[0] == NULL) {
        cd(NULL);
        return;
    }

    if (strcmp("-long", path[0]) == 0) {
        aux = 0;
    } else if (strcmp("-acc", path[0]) == 0) {
        aux = 1;
    } else if (strcmp("-link", path[0]) == 0) {
        aux = 2;
    } else if (strcmp("-hid", path[0]) == 0) {
        aux = 3;
    }
    // reclist -long // reclist -[...]
    if (path[1] == NULL && aux != -1) {
        cd(NULL);
        return;
    }

    es_doble = (aux == -1 ? 0 : 1);

    if (aux != -1) {
        char *listdir_args[] = {path[0], path[1], NULL};
        listdir(listdir_args);
    } else {
        char *listdir_args[] = {path[0], NULL};
        listdir(listdir_args);
    }
    if ((dir = opendir(path[es_doble])) == NULL) {
        perror("Error al abrir el directorio");
        return;
    }


    while ((entry = readdir(dir)) != NULL) {
        // Ignorar "." y ".."
        if (aux != 3){
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
        }

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path[es_doble], entry->d_name);

        struct stat fileStat;
        if (lstat(fullpath, &fileStat) == -1) {
            perror("Error al obtener la información del archivo");
            continue;
        }


        if (S_ISDIR(fileStat.st_mode)) {
            if (aux == 0 || aux == 1 || aux == 2 ) {
                envio_file[0] = path[0];
                envio_file[1] = fullpath;
            }
            else {
                envio_file[0] = fullpath;
            }
            reclist(envio_file);
        }
    }
    closedir(dir);
}

void revlist(char *path[]) {
    DIR *dir;
    struct dirent *entry;
    char fullpath[1024];
    int aux = -1;
    int es_doble;
    char *envio_file[15] = {NULL};

    if (path[0] == NULL) {
        cd(NULL);
        return;
    }

    if (strcmp("-long", path[0]) == 0) {
        aux = 0;
    } else if (strcmp("-acc", path[0]) == 0) {
        aux = 1;
    } else if (strcmp("-link", path[0]) == 0) {
        aux = 2;
    } else if (strcmp("-hid", path[0]) == 0) {
        aux = 3;
    }

    if (path[1] == NULL && aux != -1) {
        cd(NULL);
        return;
    }

    es_doble = (aux == -1 ? 0 : 1);

    if ((dir = opendir(path[es_doble])) == NULL) {
        perror("Error al abrir el directorio");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {

        if (aux != 3){
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
        }

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path[es_doble], entry->d_name);

        struct stat fileStat;
        if (lstat(fullpath, &fileStat) == -1) {
            perror("Error al obtener la información del archivo");
            continue;
        }

        if (S_ISDIR(fileStat.st_mode)) {
            if (aux == 0 || aux == 1 || aux == 2 ) {
                envio_file[0] = path[0];
                envio_file[1] = fullpath;
            }
            else {
                envio_file[0] = fullpath;
            }
            revlist(envio_file);
        }
    }

    if (aux != -1) {
        char *listdir_args[] = {path[0], path[1], NULL};
        listdir(listdir_args);
    } else {
        char *listdir_args[] = {path[0], NULL};
        listdir(listdir_args);
    }

    closedir(dir);
}

void erase(const char *path) {
    struct stat path_stat;

    if (path == NULL) {
        cd(NULL);
        return;
    }

    if (lstat(path, &path_stat) == -1) {
        perror("Error al obtener información del archivo o directorio");
        return;
    }
    if (S_ISREG(path_stat.st_mode)) {

        if (unlink(path) == 0) {
            printf("Archivo %s eliminado con éxito.\n", path);
        } else {
            perror("Error al eliminar el archivo");
        }
    }
    else if (S_ISDIR(path_stat.st_mode)) {

        if (rmdir(path) == 0) {
            printf("Directorio %s eliminado con éxito.\n", path);
        } else {
            perror("Error al eliminar el directorio (¿puede que no esté vacío?)");
        }
    }
}

void delrec(const char *path) {
    struct stat path_stat;

    if (lstat(path, &path_stat) == -1) {
        perror("Error al obtener información del archivo o directorio");
        return;
    }

    if (S_ISREG(path_stat.st_mode) || S_ISLNK(path_stat.st_mode)) {
        if (unlink(path) == 0) {

        } else {
            perror("Error al eliminar el archivo");
        }
        return;
    }

    if (S_ISDIR(path_stat.st_mode)) {
        DIR *dir = opendir(path);
        struct dirent *entry;
        char subpath[512];

        if (!dir) {
            perror("Error al abrir el directorio");
            return;
        }

        while ((entry = readdir(dir)) != NULL) {

            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            snprintf(subpath, sizeof(subpath), "%s/%s", path, entry->d_name);

            delrec(subpath);
        }
        closedir(dir);

        if (rmdir(path) == 0) {
        } else {
            perror("Error al eliminar el directorio");
        }
    }
}

void imprimirGeneral(tListM M){

    char descriptor[MAXI];
    pid_t pid = getpid();
    printf("******Lista de bloques asignados shared para el proceso: %d\n", pid);

    tPosM i ;
    for ( i = M ; i != MNULL; i = i->siguiente){
        if (i->elemento.id == 0){
            printf("\t\t %p \t\t %ld %s %s \n", i->elemento.direccion, i->elemento.tam, i->elemento.fecha,
                   i->elemento.funcion);
        }else if (i->elemento.id == 1){
            sprintf(descriptor, "(descriptor %d)", i->elemento.df);
            printf("\t\t %p \t\t %ld %s %s %s\n", i->elemento.direccion, i->elemento.tam, i->elemento.fecha,
                   i->elemento.funcion, descriptor);
        }else if(i->elemento.id == 2){
            sprintf(descriptor, "(key %i)", i->elemento.llave);
            printf("\t\t %p \t\t %ld %s %s %s\n", i->elemento.direccion, i->elemento.tam, i->elemento.fecha,
                   i->elemento.funcion, descriptor);
        }
    }
}

void ImprimirListaShared(tListM  M){

    tPosM i;
    char descriptor[MAXI];
    pid_t pid = getpid();
    printf("******Lista de bloques asignados shared para el proceso: %d\n", pid);

    for ( i = M ; i != MNULL; i = i->siguiente){

        if (i->elemento.id == 2){
            sprintf(descriptor, "(key %d)", i->elemento.llave);
            printf("\t\t %p \t\t %ld %s %s %s\n", i->elemento.direccion, i->elemento.tam, i->elemento.fecha,
                   i->elemento.funcion, descriptor);        }
    }
}

void imprimirMalloc(tListM M){

    tPosM i ;
    pid_t pid = getpid();
    printf("******Lista de bloques asignados shared para el proceso: %d\n", pid);

    for ( i = M ; i != MNULL; i = i->siguiente){

        if (i->elemento.id == 0){
            printf("\t\t %p \t\t %ld %s %s\n", i->elemento.direccion, i->elemento.tam, i->elemento.fecha, i->elemento.funcion);
        }
    }
}

void imprimirMmap (tListM M) {

    tPosM i;

    char descriptor[MAXI];
    pid_t pid = getpid();
    printf("******Lista de bloques asignados shared para el proceso: %d\n", pid);
    for (i = M; i != MNULL; i = i->siguiente) {

        if (i->elemento.id == 1){
            sprintf(descriptor, "(descriptor %d)", i->elemento.df);
            printf("\t\t %p \t\t %ld %s %s %s\n", i->elemento.direccion, i->elemento.tam, i->elemento.fecha,
                   i->elemento.funcion, descriptor);
        }
    }
}

void Recursiva (int n)
{
    char automatico[TAMANO];
    static char estatico[TAMANO];

    printf ("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico, estatico);

    if (n>0)
        Recursiva(n-1);
}

void LlenarMemoria (void *p, size_t cont, unsigned char byte)
{
    unsigned char *arr=(unsigned char *) p;
    size_t i;

    for (i=0; i<cont;i++)
        arr[i]=byte;
}

void * ObtenerMemoriaShmget(key_t clave, size_t tam, tListM *M) { // tListM por referencia
    void *p;
    int aux, id, flags = 0777;
    struct shmid_ds s;

    if (tam)
        flags = flags | IPC_CREAT | IPC_EXCL;
    if (clave == IPC_PRIVATE) {
        errno = EINVAL;
        return NULL;
    }
    if ((id = shmget(clave, tam, flags)) == -1)
        return (NULL);
    if ((p = shmat(id, NULL, 0)) == (void *)-1) {
        aux = errno;
        if (tam)
            shmctl(id, IPC_RMID, NULL);
        errno = aux;
        return (NULL);
    }
    shmctl(id, IPC_STAT, &s); /* si no es crear, necesitamos el tamaño, que es s.shm_segsz */


    insertMItem(p, s.shm_segsz, fecha_allocate(), "shared", -1, 2, clave, MNULL, M);
    return (p);
}

void do_AllocateCreateshared(char *tr[], tListM *M) { // tListM por referencia
    key_t cl;
    size_t tam;
    void *p;

    if (tr[0] == NULL || tr[1] == NULL) {
        ImprimirListaShared(*M);
        return;
    }

    cl = (key_t)strtoul(tr[0], NULL, 10);
    tam = (size_t)strtoul(tr[1], NULL, 10);
    if (tam == 0) {
        printf("No se asignan bloques de 0 bytes\n");
        return;
    }
    if ((p = ObtenerMemoriaShmget(cl, tam, M)) != NULL) // Pasamos la lista por referencia
        printf("Asignados %lu bytes en %p\n", (unsigned long)tam, p);
    else
        printf("Imposible asignar memoria compartida clave %lu: %s\n", (unsigned long)cl, strerror(errno));
}

void do_AllocateShared (char *tr[], tListM *L) {
    key_t cl;
    //size_t tam = 0;
    void *p;

    if (tr[0]==NULL) {
        ImprimirListaShared(*L);
        return;
    }

    cl=(key_t)  strtoul(tr[0],NULL,10);

    if ((p=ObtenerMemoriaShmget(cl,0, L))!=NULL)
        printf ("Asignada memoria compartida de clave %lu en %p\n",(unsigned long) cl, p);
    else
        printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
}

void hacer_malloc(tListM *M, char *cadena){

    long aux = atoi(cadena);

    void *dir = malloc(aux);
    printf("Asignados %s bytes en %p\n",cadena, dir);

    insertMItem(dir, aux, fecha_allocate(), "malloc", -1, 0 ,-1, MNULL, M);

}

void *MapearFichero(char *fichero[], int protection, tListF *F, tListM *M) {
    int df, map = MAP_PRIVATE, modo = O_RDONLY;
    struct stat s;
    void *p;
    char descriptor[MAXI];
    char descriptor1[MAXI];

    if(protection & PROT_WRITE)
        modo = O_RDWR;
    if(stat(fichero[0],&s)==-1||(df=open(fichero[0],modo))==-1)
        return NULL;
    if((p=mmap(NULL,s.st_size,protection,map,df,0))==MAP_FAILED)
        return NULL;

    off_t offset = lseek(df, 0, SEEK_CUR);

    sprintf(descriptor, "Mapeado de %s", fichero[0]);

    if (modo == O_RDWR){
        strcpy(descriptor1,"O_RDWR");
    }

    insertFileItem(df,descriptor1,descriptor,offset,NULL,F);


    insertMItem(p,s.st_size,fecha_allocate(),fichero[0],df,1,-1,MNULL ,M);

    return p;
}

void do_AllocateMmap(char *arg[], tListM *L, tListF *F) {
    char *perm;
    void *p;
    int protection = 0;

    if (arg[0] == NULL) {
        imprimirMmap(*L);
        return;
    }

    if ((perm=arg[1]) != NULL && strlen(perm) < 4) {
        if (strchr(perm,'r')!=NULL) protection |= PROT_READ;
        if (strchr(perm,'w')!=NULL) protection |= PROT_WRITE;
        if (strchr(perm,'x')!=NULL) protection |= PROT_EXEC;
    }

    if ((p = MapearFichero(arg,protection,F,L)) == NULL)
        perror("Imposible mapear fichero");
    else
        printf("fichero %s mapeado en %p\n",arg[0],p);
}

void allocate(char *cadena[], tListM *M, tListF *F) {


    if (cadena[0] == NULL) {

        imprimirGeneral(*M);
        return;

    } else if (strcmp(cadena[0], "-malloc") == 0) {
        if (cadena[1] == NULL) {

            imprimirMalloc(*M);
        } else if (esNumero(cadena[1])) {
            hacer_malloc(M, cadena[1]);
        } else {
            printf("uso: allocate [-malloc|-shared|-createshared|-mmap] ....\n");
        }

    } else if (strcmp(cadena[0], "-mmap") == 0) {
        do_AllocateMmap(cadena + 1, M, F);
    }
    else if (strcmp(cadena[0], "-createshared") == 0) {

        do_AllocateCreateshared(cadena+1,M);

    } else if (strcmp(cadena[0], "-shared") == 0) {

        do_AllocateShared(cadena+1,M);


    } else {
        printf("uso: allocate [-malloc|-shared|-createshared|-mmap] ....\n");
    }
}

void deallocate_malloc(char *args[], tListM *M) {
    tPosM pos;

    if (args[0] == NULL) {
        imprimirGeneral(*M);
        return;
    }

    long size = atol(args[0]);

    for (pos = *M; pos != MNULL; pos = pos->siguiente) {
        if (pos->elemento.id == 0 && pos->elemento.tam == size) {

            printf("Memoria malloc de tamaño %ld liberada en %p\n", size, pos->elemento.direccion);
            deleteMallocMemoryPosition(pos, M);
            return;
        }
    }
    printf("No se encontró un bloque malloc de tamaño %ld\n", size);
}

void deallocate_mmap(char *args[], tListM *M,tListF *F){
    tPosM pos;
    if (args[0] == NULL) {
        imprimirMmap(*M);
        return;
    }
    for (pos = *M; pos != MNULL; pos = pos->siguiente) {
        if (pos->elemento.id == 1 && strcmp(pos->elemento.funcion, args[0]) == 0) {

            munmap(pos->elemento.direccion, pos->elemento.tam);
            printf("Archivo mapeado %s desmapeado de %p\n", args[0], pos->elemento.direccion);
            deleteFileAtPosition(findFileItem( pos->elemento.df, *F) ,F);
            deleteMmapMemoryPosition(pos, M);
            return;
        }
    }
    printf("No se encontró el archivo mapeado %s\n", args[1]);
}

void deallocate_shared(char *args[], tListM *M) {
    tPosM pos;
    key_t key;

    if (args[0] == NULL) {
        imprimirMmap(*M);
        return;
    }

    key = (key_t)atoi(args[0]);

    for (pos = *M; pos != MNULL; pos = pos->siguiente) {
        if (pos->elemento.id == 2 && pos->elemento.llave == key) {
            printf("Memoria compartida clave %d desasociada de %p\n", key, pos->elemento.direccion);
            deleteSharedMemoryPosition(pos, M);
            return;
        }
    }
    printf("No se encontró la memoria compartida clave %d\n", key);
}

void do_DeallocateDelkey (char *args[]) {
    key_t clave;
    int id;
    char *key=args[0];

    if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
        printf ("      delkey necesita clave_valida\n");
        return;
    }
    if ((id=shmget(clave,0,0666))==-1){
        perror ("shmget: imposible obtener memoria compartida");
        return;
    }
    if (shmctl(id,IPC_RMID,NULL)==-1)
        perror ("shmctl: imposible eliminar memoria compartida\n");
}

void do_Addr(void *addr, tListM *M, tListF *F) {
    tPosM pos;
    void *direccion;
    sscanf(addr, "%p", &direccion);

    for (pos = *M; pos != MNULL; pos = pos->siguiente) {
        if (pos->elemento.direccion == direccion) {

            switch (pos->elemento.id) {
                case 0: // malloc
                    printf("Memoria malloc liberada en %p\n", direccion);
                    deleteMallocMemoryPosition(pos, M);
                    return;

                case 1: // mmap
                    printf("Archivo mapeado desmapeado de %p\n", direccion);
                    deleteFileAtPosition(findFileItem(pos->elemento.df, *F), F);
                    deleteMmapMemoryPosition(pos, M);
                    return;

                case 2: // shared
                    printf("Memoria compartida desasociada de %p\n", direccion);
                    deleteSharedMemoryPosition(pos, M);
                    return;
            }
        }
    }
    printf("No se encontró ningún bloque de memoria en la dirección %p\n", direccion);
}

void deallocate(char *args[], tListM *M, tListF *F) {

    if (args[0] == NULL) {
        imprimirGeneral(*M);
        return;
    }
    if (strcmp(args[0], "-malloc") == 0) {
        deallocate_malloc(args + 1, M);
    }
    else if (strcmp(args[0], "-mmap") == 0) {
        deallocate_mmap(args + 1, M, F);
    }
    else if (strcmp(args[0], "-shared") == 0) {
        deallocate_shared(args + 1, M);
    }
    else if (strcmp(args[0], "-delkey") == 0) {
        do_DeallocateDelkey(args + 1);
    }
    else
        do_Addr((void *) args[0], M, F);
}

void memdump(char *args[], tListM M) {
    void *addr;
    size_t cont = 25;  // valor por defecto
    unsigned char *p;

    sscanf(args[0], "%p", &addr);

    if (args[1] != NULL) {
        cont = (size_t)strtoul(args[1], NULL, 10);
    }

    tPosM pos;
    bool direccionValida = false;
    for (pos = M; pos != MNULL; pos = pos->siguiente) {
        char *inicio = (char *)pos->elemento.direccion;
        char *fin = inicio + pos->elemento.tam;
        char *addr_check = (char *)addr;

        if (addr_check >= inicio && addr_check + cont <= fin) {
            direccionValida = true;
            break;
        }
    }

    if (!direccionValida) {
        printf("Dirección de memoria no válida o fuera de rango: %p\n", addr);
        return;
    }

    printf("Volcando %zu bytes desde la direccion %p\n", cont, addr);
    p = (unsigned char *)addr;

    for (size_t i = 0; i < cont; i++) {
        if (isprint(p[i])) {
            printf("%c ", p[i]);
        } else {
            printf("   ");
        }
    }
    printf("\n");

    for (size_t i = 0; i < cont; i++) {
        printf("%02x ", p[i]);
    }
    printf("\n");
}

void memfill(char *args[], tListM M) {
    void *addr;
    size_t cont;
    unsigned char byte;

    if (args[0] == NULL || args[1] == NULL || args[2] == NULL) {
        printf("Uso: memfill [addr] [cont] [ch]\n");
        return;
    }

    sscanf(args[0], "%p", &addr);
    cont = (size_t)strtoul(args[1], NULL, 10);
    byte = (unsigned char)args[2][1];

    tPosM pos;
    bool direccionValida = false;
    for (pos = M; pos != MNULL; pos = pos->siguiente) {
        if (addr >= pos->elemento.direccion &&
            (char *)addr + cont <= (char *)pos->elemento.direccion + pos->elemento.tam) {
            direccionValida = true;
            break;
        }
    }

    if (!direccionValida) {
        printf("Dirección de memoria no válida o fuera de rango: %p\n", addr);
        return;
    }

    printf("Llenando %zu bytes de memoria con el byte %c(%02x) a partir de la direccion %p\n",
           cont, byte, byte, addr);
    LlenarMemoria(addr, cont, byte);
}

void Do_pmap (void) /*sin argumentos*/{
    pid_t pid;       /*hace el pmap (o equivalente) del proceso actual*/
    char elpid[32];
    char *argv[4]={"pmap",elpid,NULL};

    sprintf (elpid,"%d", (int) getpid());
    if ((pid=fork())==-1){
        perror ("Imposible crear proceso");
        return;
    }
    if (pid==0){
        if (execvp(argv[0],argv)==-1)
            perror("cannot execute pmap (linux, solaris)");

        argv[0]="procstat"; argv[1]="vm"; argv[2]=elpid; argv[3]=NULL;
        if (execvp(argv[0],argv)==-1)/*No hay pmap, probamos procstat FreeBSD */
            perror("cannot execute procstat (FreeBSD)");

        argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;
        if (execvp(argv[0],argv)==-1)  /*probamos procmap OpenBSD*/
            perror("cannot execute procmap (OpenBSD)");

        argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
        if (execvp(argv[0],argv)==-1) /*probamos vmmap Mac-OS*/
            perror("cannot execute vmmap (Mac-OS)");
        exit(1);
    }
    waitpid (pid,NULL,0);
}

void programFunction1() {}
void programFunction2() {}
void programFunction3() {}

void function() {
    void (*progFunc1)() = programFunction1;
    void (*progFunc2)() = programFunction2;
    void (*progFunc3)() = programFunction3;

    void *libFunc1 = malloc;
    void *libFunc2 = free;
    void *libFunc3 = strcpy;

    printf("Funciones programa     %p,    %p,    %p\n", (void *)progFunc1, (void *)progFunc2, (void *)progFunc3);
    printf("Funciones libreria     %p,    %p,    %p\n", libFunc1, libFunc2, libFunc3);
}

void do_MemVars() {
    int local1 = 1;
    int local2 = 2;
    int local3 = 3;

    static int global1 = 10;
    static int global2 = 20;
    static int global3 = 30;

    static int global_ni1;
    static int global_ni2;
    static int global_ni3;

    static int static1 = 100;
    static int static2 = 200;
    static int static3 = 300;

    static int static_ni1;
    static int static_ni2;
    static int static_ni3;

    printf("Variables locales      %p,    %p,    %p\n",
           (void*)&local1, (void*)&local2, (void*)&local3);

    printf("Variables globales     %p,    %p,    %p\n",
           (void*)&global1, (void*)&global2, (void*)&global3);

    printf("Var (N.I.)globales     %p,    %p,    %p\n",
           (void*)&global_ni1, (void*)&global_ni2, (void*)&global_ni3);

    printf("Variables staticas     %p,    %p,    %p\n",
           (void*)&static1, (void*)&static2, (void*)&static3);

    printf("Var (N.I.)staticas     %p,    %p,    %p\n",
           (void*)&static_ni1, (void*)&static_ni2, (void*)&static_ni3);
}

void memory(char *args[],tListM M){

    if (strcmp(args[0],"-funcs") == 0){
        function();
    }
    else if(strcmp(args[0],"-vars") == 0){
        do_MemVars();
    }
    else if(strcmp(args[0],"-blocks") == 0){
        imprimirMalloc(M);
    }
    else if(strcmp(args[0],"-all") == 0){
        do_MemVars();
        function();
        imprimirMalloc(M);
    }
    else if(strcmp(args[0],"-pmap") == 0){
        Do_pmap();

    }
}

ssize_t LeerFichero (char *f, void *p, size_t cont)
{
    struct stat s;
    ssize_t  n;
    int df,aux;

    if (stat (f,&s)==-1 || (df=open(f,O_RDONLY))==-1)
        return -1;
    if (cont==-1)   /* si pasamos -1 como bytes a leer lo leemos entero*/
        cont=s.st_size;
    if ((n=read(df,p,cont))==-1){
        aux=errno;
        close(df);
        errno=aux;
        return -1;
    }
    close (df);
    return n;
}

void* cadtop(char *cadena) {
    void *p;
    sscanf(cadena, "%p", &p);
    return p;
}

void Cmd_ReadFile (char *ar[])
{
    void *p;
    size_t cont=-1;  /*si no pasamos tamano se lee entero */
    ssize_t n;
    if (ar[0]==NULL || ar[1]==NULL){
        printf ("faltan parametros\n");
        return;
    }
    p=cadtop(ar[1]);  /*convertimos de cadena a puntero*/
    if (ar[2]!=NULL)
        cont=(size_t) atoll(ar[2]);

    if ((n=LeerFichero(ar[0],p,cont))==-1)
        perror ("Imposible leer fichero");
    else
        printf ("leidos %lld bytes de %s en %p\n",(long long) n,ar[0],p);
}

void do_WriteFile(char *args[], tListM M) {
    void *addr;
    size_t cont;
    int fd;
    ssize_t written;

    sscanf(args[1], "%p", &addr);
    cont = (size_t)strtoul(args[2], NULL, 10);

    // Verificar que la dirección pertenece a un bloque asignado
    tPosM pos;
    bool direccionValida = false;
    for (pos = M; pos != MNULL; pos = pos->siguiente) {
        char *inicio = (char *)pos->elemento.direccion;
        char *fin = inicio + pos->elemento.tam;
        char *addr_check = (char *)addr;

        if (addr_check >= inicio && addr_check + cont <= fin) {
            direccionValida = true;
            break;
        }
    }

    if (!direccionValida) {
        printf("Dirección de memoria no válida o fuera de rango: %p\n", addr);
        return;
    }

    // Abrir el archivo
    if ((fd = open(args[0], O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
        perror("Error al abrir el archivo");
        return;
    }

    // Escribir en el archivo
    if ((written = write(fd, addr, cont)) == -1) {
        perror("Error al escribir en el archivo");
        close(fd);
        return;
    }

    printf("Escritos %zd bytes desde %p al fichero %s\n", written, addr, args[0]);
    close(fd);
}

void do_Read(char *args[], tListM M) {
    void *addr;
    size_t cont;
    int df;
    ssize_t bytes_read;

    df = atoi(args[0]);
    sscanf(args[1], "%p", &addr);
    cont = (size_t)strtoul(args[2], NULL, 10);

    tPosM pos;
    bool direccionValida = false;
    for (pos = M; pos != MNULL; pos = pos->siguiente) {
        char *inicio = (char *)pos->elemento.direccion;
        char *fin = inicio + pos->elemento.tam;
        char *addr_check = (char *)addr;

        if (addr_check >= inicio && addr_check + cont <= fin) {
            direccionValida = true;
            break;
        }
    }

    if (!direccionValida) {
        printf("Dirección de memoria no válida o fuera de rango: %p\n", addr);
        return;
    }

    if ((bytes_read = read(df, addr, cont)) == -1) {
        perror("Error al leer del descriptor");
        return;
    }

    printf("Leidos %zd bytes del descriptor %d en %p\n", bytes_read, df, addr);
}

void do_Write(char *args[], tListM M) {
    void *addr;
    size_t cont;
    int df;
    ssize_t written;

    df = atoi(args[0]);
    sscanf(args[1], "%p", &addr);
    cont = (size_t)strtoul(args[2], NULL, 10);

    tPosM pos;
    bool direccionValida = false;
    for (pos = M; pos != MNULL; pos = pos->siguiente) {
        char *inicio = (char *)pos->elemento.direccion;
        char *fin = inicio + pos->elemento.tam;
        char *addr_check = (char *)addr;

        if (addr_check >= inicio && addr_check + cont <= fin) {
            direccionValida = true;
            break;
        }
    }

    if (!direccionValida) {
        printf("Dirección de memoria no válida o fuera de rango: %p\n", addr);
        return;
    }

    if ((written = write(df, addr, cont)) == -1) {
        perror("Error al escribir en el descriptor");
        return;
    }

    printf("Escritos %zd bytes desde %p al descriptor %d\n", written, addr, df);
}

void do_getuid() {
    uid_t real_uid = getuid();         //UID real
    uid_t effective_uid = geteuid();   //UID efectivo

    struct passwd *real_user = getpwuid(real_uid);
    printf("Credencial real: %d, (%s)\n", real_uid, real_user->pw_name);

    struct passwd *effective_user = getpwuid(effective_uid);
    printf("Credencial efectiva: %d, (%s)\n", effective_uid, effective_user->pw_name);

}

void do_setuid(char *args[]){
    struct passwd *pw;

    if (args[0] == NULL) {
        do_getuid();
        return;
    }
    else if (strcmp(args[0], "-l") == 0) {

        if (args[1] == NULL) {
            do_getuid();
            return;
        }

        if ((pw = getpwnam(args[1])) == NULL) {
            printf("Usuario no existente %s\n", args[1]);
            return;
        }
        setuid(pw->pw_uid);
    }
    else {
        printf("Imposible cambiar credencial: Operation not permitted\n");
    }
}

int BuscarVariable (char * var, char *e[]) {
    int pos=0;
    char aux[MAX];

    strcpy (aux,var);
    strcat (aux,"=");

    while (e[pos]!=NULL)
        if (!strncmp(e[pos],aux,strlen(aux)))
            return (pos);
        else
            pos++;
    errno=ENOENT;   /*no hay tal variable*/
    return(-1);
}

/*cambia una variable en el entorno que se le pasa como parÃ¡metro*/
/*lo hace directamente, no usa putenv*/
int CambiarVariable(char * var, char * valor, char *e[]) {
    int pos;
    char *aux;

    if ((pos=BuscarVariable(var,e))==-1)
        return(-1);
    if ((aux=(char *)malloc(strlen(var)+strlen(valor)+2))==NULL)
        return -1;
    strcpy(aux,var);
    strcat(aux,"=");
    strcat(aux,valor);
    e[pos]=aux;
    return (pos);
}

int CambiarVariableSUS(char * var1,char * var, char * valor, char *e[]) {
    int pos;
    char *aux;

    if ((pos=BuscarVariable(var1,e))==-1)
        return(-1);
    printf("paso\n");
    if ((aux=(char *)malloc(strlen(var)+strlen(valor)+2))==NULL)
        return -1;
    strcpy(aux,var);
    strcat(aux,"=");
    strcat(aux,valor);
    e[pos]=aux;
    return (pos);
}

void do_showvar(char *args[], char *envp[]){
    extern char **environ;

    if(args[0] == NULL ) {
        for (int i = 0; envp[i] != NULL; i++) {
            printf("0x%p->main arg3[%d]=(0x%p) %s\n",
                   (void*)&envp[i],    // Dirección donde se almacena el puntero
                   i,                  // Índice
                   (void*)envp[i],     // Dirección del contenido
                   envp[i]);           // Contenido de la variable
        }
        return;
    }

    for (int i = 0; args[i] != NULL; i++) {
        char *value = getenv(args[i]);
        if (value != NULL) {
            // Buscar en arg3
            for (int j = 0; envp[j] != NULL; j++) {
                if (strncmp(envp[j], args[i], strlen(args[i])) == 0 &&
                    envp[j][strlen(args[i])] == '=') {
                    printf("Con arg3 main %s(0x%p) @0x%p\n",
                           envp[j], (void*)envp[j], (void*)&envp[j]);
                    break;
                }
            }
            // Buscar en environ
            for (int j = 0; environ[j] != NULL; j++) {
                if (strncmp(environ[j], args[i], strlen(args[i])) == 0 &&
                    environ[j][strlen(args[i])] == '=') {
                    printf("  Con environ %s(0x%p) @0x%p\n",
                           environ[j],
                           (void*)environ[j],
                           (void*)&environ[j]);
                    break;
                }
            }
            // Mostrar valor con getenv
            printf("   Con getenv %s(0x%p)\n",
                   value, (void*)value);
        }
    }
}

// función auxiliar para cambiar variable
void Cambio(char *args[], char *envp[]){
    extern char **environ;
    char buffer[1024];
    if (strcmp(args[0],"-a") == 0){
        CambiarVariable(args[1],args[2], envp );
    }
    else if (strcmp(args[0],"-e") == 0){
        CambiarVariable(args[1],args[2], environ );
    }
    else if ((strcmp(args[0], "-p") == 0)) {
        // Using putenv - can create new variables
        snprintf(buffer, sizeof(buffer), "%s=%s", args[1], args[2]);
        if (putenv(strdup(buffer)) != 0) {
            perror("Error al cambiar variable con putenv");
        }
    }
    else printf("Opción no válida. Use -a, -e o -p\n");
}

void do_subsvar(char *args[], char *env[]) {
    extern char **environ;

    if (args[0] == NULL || args[1] == NULL || args[2] == NULL || args[3] == NULL) {
        printf("Uso: subsvar [-a|-e] var1 var2 valor\n");
        return;
    }

    char *v1 = args[1];    // Variable a sustituir (DISPLAY)
    char *v2 = args[2];    // Nueva variable (biblioteca)
    char *val = args[3];   // Nuevo valor (ruido)

    if (strcmp(args[0], "-a") == 0) {
        // Usando el tercer argumento de main
        CambiarVariable(v2,val,env);
        CambiarVariableSUS(v1,v2, val, env);

    }
    else if (strcmp(args[0], "-e") == 0) {
        // Usando environ
        CambiarVariableSUS(v1,v2,val,environ);
    }
    else {
        printf("Opción no válida. Use -a o -e\n");
    }
}

void do_Environ(char *args[], char *envp[]){
    extern char **environ;

    if (args[0] == NULL){
        do_showvar(args,envp);
    }
    else if (strcmp(args[0], "-environ") == 0) {
        for (int i = 0; environ[i] != NULL; i++) {
            printf("0x%p->environ[%d]=(0x%p) %s\n",
                   (void*)&environ[i],
                   i,
                   (void*)environ[i],
                   environ[i]);
        }
    }
    else if (strcmp(args[0], "-addr") == 0) {
        printf("environ: %p (almacenado en %p)\n", (void *)environ, (void *)&environ);
        printf("main arg3: %p (almacenado en %p)\n", (void *)envp, (void *)&envp);
    }

    else {
        printf("Uso: environ [-environ|-addr]\n");
    }
}

void Cmd_fork (char *tr[], tListJ *J) {
    pid_t pid;

    if ((pid=fork())==0){
        deleteJobList(J);
        printf ("ejecutando proceso %d\n", getpid());
    }
    else if (pid!=-1)
        waitpid (pid,NULL,0);
}

void do_search(char * args[], tListS *S){

    tPosS i;
    int j = 0;

    if (args[0] == NULL){
        for (i = *S;i != NULL ; i = i->siguiente){
            printf("%s\n", i->elemento.datos);
        }
    }
    else if ((strcmp(args[0], "-add") == 0)){
        if (args[1] == NULL){
            printf("Imposible realizar operacion -add: Bad address\n");
        }
        else{
            insertItemSE(args[1], NULL, S);
        }

    }
    else if ((strcmp(args[0], "-del") == 0)){
        if (args[1] == NULL) {
            printf("Imposible realizar operacion -del: Bad address\n");
            return;
        }
        tPosS pos = *S;
        while (pos != NULL) {
            if (strcmp(pos->elemento.datos, args[1]) == 0) {
                if (deleteSearchPosition(pos, S)) {
                    return;
                }
                break;
            }
            pos = pos->siguiente;
        }
    }
    else if ((strcmp(args[0], "-clear") == 0)){
        deleteListSE(S);
    }
    else if ((strcmp(args[0], "-path") == 0)){
        char *cadena =getenv("PATH");
        char *trozos[30];
        if (cadena == NULL){
            printf("Error al encontrar los directorios\n");
            return;
        }
        else {

            if ((trozos[0]=strtok(cadena,":"))==NULL)
                return;

            while ((trozos[j]=strtok(NULL,":"))!=NULL){
                j++;
            }
            printf("Importados %d directorios en la ruta de busqueda\n",j);
            for (j = 0; trozos[j] != NULL; j++){
                insertItemSE(trozos[j], NULL, S);
            }
        }
    }
    else printf("Argumento incorrecto %s\n", args[0]);
}

char *Ejecutable(char *s, tListS *S) {
    static char path[MAX];
    struct stat st;
    tPosS pos;

    if (s == NULL || isEmptyListSE(*S))
        return s;

    if (s[0] == '/' || !strncmp(s, "./", 2) || !strncmp(s, "../", 3))
        return s; /* Es una ruta absoluta */

    for (pos = firstSE(*S); pos != SNULL; pos = nextSE(pos, *S)) {
        strncpy(path, pos->elemento.datos, MAX - 1);
        strncat(path, "/", MAX - strlen(path) - 1);
        strncat(path, s, MAX - strlen(path) - 1);

        if (lstat(path, &st) != -1)
            return path;
    }

    return s;
}

int Execpve(char *tr[], char **NewEnv, const int *pprio, tListS *S) {
    char *p; /* NewEnv contiene la dirección del nuevo entorno */
    /* pprio contiene la dirección de la nueva prioridad */
    /* NULL indica que no hay cambios en el entorno o prioridad */

    if (tr[0] == NULL || (p = Ejecutable(tr[0], S)) == NULL) {
        errno = EFAULT;
        return -1;
    }

    if (pprio != NULL && setpriority(PRIO_PROCESS, getpid(), *pprio) == -1 && errno) {
        printf("Imposible cambiar prioridad: %s\n", strerror(errno));
        return -1;
    }

    if (NewEnv == NULL)
        return execv(p, tr);
    else
        return execve(p, tr, NewEnv);
}


void do_exec(char *args[], tListS *S) {
    char *new_env[1024] = { NULL };  // Entorno nuevo
    int env_count = 0;               // Contador de variables de entorno
    int i = 0;                       // Índice para recorrer `args`

    if (args[0] == NULL) {
        fprintf(stderr, "Imposible ejecutar: Bad address.\n");
        return;
    }

    //Añada las variables de entorno necesarias
    while (args[i] != NULL && getenv(args[i]) != NULL) {
        new_env[env_count] = strdup(args[i]); //Copia y hace un malloc de las variabales de entorno
        env_count++;
        i++;
    }

    if (args[i] == NULL) {
        fprintf(stderr, "Imposible ejecutar: Bad address.\n");
        return;
    }

    //Si hay variables de entorno pone de ultimo valor NULL en el array
    if (env_count > 0) {
        new_env[env_count] = NULL;
    }

    //Hace un array del ejecutable sin las variables de entorno
    char *exec_args[1024];
    int j = 0;
    while (args[i] != NULL) {
        exec_args[j++] = args[i++];
    }
    exec_args[j] = NULL; // Terminar la lista de argumentos con NULL

    // Ejecutar el programa con el entorno adecuado
    if (Execpve(exec_args, (env_count > 0 ? new_env : NULL), NULL, S) == -1) {
        perror("Error al ejecutar el comando");
    }
    // Liberar memoria asignada para las variables de entorno
    for (int k = 0; k < env_count; k++) {
        free(new_env[k]);
    }
}

void do_ExecPri(char *args[], tListS *S) {
    char **new_env = NULL;
    char *exec_args[MAX];
    int i = 0, j = 0, env_vars = 0;
    int priority;

    if (args[0] == NULL || args[1] == NULL) {
        printf("Imposible ejecutar: Bad adress\n");
        return;
    }
    priority = atoi(args[0]);
    i = 1;

    while (args[i] != NULL) {
        char *value = getenv(args[i]);
        if (value == NULL) break;
        env_vars++;
        i++;
    }
    if (args[i] == NULL) {
        printf("Error: no se especificó el ejecutable\n");
        return;
    }
    if (env_vars > 0) {
        new_env = malloc((env_vars + 1) * sizeof(char *));
        if (new_env == NULL) {
            perror("Error al asignar memoria");
            return;
        }

        for (j = 0; j < env_vars; j++) {
            char *value = getenv(args[j + 1]);
            char *env_str = malloc(strlen(args[j + 1]) + strlen(value) + 2);
            if (env_str == NULL) {
                perror("Error al asignar memoria");
                while (--j >= 0) free(new_env[j]);
                free(new_env);
                return;
            }
            sprintf(env_str, "%s=%s", args[j + 1], value);
            new_env[j] = env_str;
        }
        new_env[j] = NULL;
    }

    j = 0;
    exec_args[j++] = args[i];  // Nombre del programa
    i++;
    while (args[i] != NULL && j < MAX - 1) {
        exec_args[j++] = args[i++];
    }
    exec_args[j] = NULL;

    if (Execpve(exec_args, new_env, &priority, S) == -1) {
        perror("Error en execpri");

        if (new_env != NULL) {
            for (i = 0; i < env_vars; i++) {
                free(new_env[i]);
            }
            free(new_env);
        }
    }
}

void do_Fg (char *args[], tListS *S) {
    char **new_env = NULL;
    char *exec_args[MAX];
    int i = 0, j = 0, env_vars = 0;
    pid_t pid;

    if (args[0] == NULL) {
        printf("No ejecutado: Bad adress\n");
        return;
    }
    while (args[i] != NULL) {
        char *value = getenv(args[i]);
        if (value == NULL) break;
        env_vars++;
        i++;
    }
    if (args[i] == NULL) {
        printf("Error: no se especificó el ejecutable\n");
        return;
    }
    if (env_vars > 0) {
        new_env = malloc((env_vars + 1) * sizeof(char *));
        if (new_env == NULL) {
            perror("Error al asignar memoria");
            return;
        }
        for (j = 0; j < env_vars; j++) {
            char *value = getenv(args[j]);
            char *env_str = malloc(strlen(args[j]) + strlen(value) + 2);
            if (env_str == NULL) {
                perror("Error al asignar memoria");
                while (--j >= 0) free(new_env[j]);
                free(new_env);
                return;
            }
            sprintf(env_str, "%s=%s", args[j], value);
            new_env[j] = env_str;
        }
        new_env[j] = NULL;
    }
    j = 0;
    exec_args[j++] = args[i];
    i++;
    while (args[i] != NULL && j < MAX - 1) {
        exec_args[j++] = args[i++];
    }
    exec_args[j] = NULL;

    if ((pid = fork()) == -1) {
        perror("Error en fork");
        return;
    }

    if (pid == 0) {

        if (Execpve(exec_args, new_env, NULL, S) == -1) {
            perror("No ejecutado:");
            exit(EXIT_FAILURE);
        }
    } else {
        waitpid(pid, NULL, 0);
        if (new_env != NULL) {
            for (i = 0; i < env_vars; i++) {
                free(new_env[i]);
            }
            free(new_env);
        }
    }
}

void do_FgPri(char *args[], tListS *S) {
    char **new_env = NULL;
    char *exec_args[MAX];
    int i = 0, j = 0, env_vars = 0;
    pid_t pid;
    int priority;

    if (args[0] == NULL || args[1] == NULL) {
        printf("No ejecutado: Bad adress\n");
        return;
    }
    priority = atoi(args[0]);
    i = 1;

    while (args[i] != NULL) {
        char *value = getenv(args[i]);
        if (value == NULL) break;
        env_vars++;
        i++;
    }

    if (args[i] == NULL) {
        printf("Error: no se especificó el ejecutable\n");
        return;
    }
    if (env_vars > 0) {
        new_env = malloc((env_vars + 1) * sizeof(char *));
        if (new_env == NULL) {
            perror("Error al asignar memoria");
            return;
        }
        for (j = 0; j < env_vars; j++) {
            char *value = getenv(args[j + 1]);
            char *env_str = malloc(strlen(args[j + 1]) + strlen(value) + 2);
            if (env_str == NULL) {
                perror("Error al asignar memoria");
                while (--j >= 0) free(new_env[j]);
                free(new_env);
                return;
            }
            sprintf(env_str, "%s=%s", args[j + 1], value);
            new_env[j] = env_str;
        }
        new_env[j] = NULL;
    }
    j = 0;
    exec_args[j++] = args[i];
    i++;
    while (args[i] != NULL && j < MAX - 1) {
        exec_args[j++] = args[i++];
    }
    exec_args[j] = NULL;

    if ((pid = fork()) == -1) {
        perror("Error en fork");
        return;
    }

    if (pid == 0) {
        if (Execpve(exec_args, new_env, &priority, S) == -1) {
            perror("Error en exec");
            exit(EXIT_FAILURE);
        }
    } else {
        waitpid(pid, NULL, 0);
        if (new_env != NULL) {
            for (i = 0; i < env_vars; i++) {
                free(new_env[i]);
            }
            free(new_env);
        }
    }
}

struct SEN {
    char *nombre;
    int senal;
};

static struct SEN sigstrnum[]={
        {"HUP", SIGHUP}, {"INT", SIGINT}, {"QUIT", SIGQUIT},
        {"ILL", SIGILL}, {"TRAP", SIGTRAP},{"ABRT", SIGABRT},
        {"IOT", SIGIOT}, {"BUS", SIGBUS}, {"FPE", SIGFPE},
        {"KILL", SIGKILL},{"USR1", SIGUSR1}, {"SEGV", SIGSEGV},
        {"USR2", SIGUSR2}, {"PIPE", SIGPIPE}, {"ALRM", SIGALRM},
        {"TERM", SIGTERM}, {"CHLD", SIGCHLD}, {"CONT", SIGCONT},
        {"STOP", SIGSTOP}, {"TSTP", SIGTSTP},{"TTIN", SIGTTIN},
        {"TTOU", SIGTTOU}, {"URG", SIGURG}, {"XCPU", SIGXCPU},
        {"XFSZ", SIGXFSZ},{"VTALRM", SIGVTALRM}, {"PROF", SIGPROF},
        {"WINCH", SIGWINCH}, {"IO", SIGIO}, {"SYS", SIGSYS},
/*senales que no hay en todas partes*/
#ifdef SIGPOLL
        {"POLL", SIGPOLL},
#endif
#ifdef SIGPWR
        {"PWR", SIGPWR},
#endif
#ifdef SIGEMT
        {"EMT", SIGEMT},
#endif
#ifdef SIGINFO
        {"INFO", SIGINFO},
#endif
#ifdef SIGSTKFLT
        {"STKFLT", SIGSTKFLT},
#endif
#ifdef SIGCLD
        {"CLD", SIGCLD},
#endif
#ifdef SIGLOST
        {"LOST", SIGLOST},
#endif
#ifdef SIGCANCEL
        {"CANCEL", SIGCANCEL},
#endif
#ifdef SIGTHAW
        {"THAW", SIGTHAW},
#endif
#ifdef SIGFREEZE
        {"FREEZE", SIGFREEZE},
#endif
#ifdef SIGLWP
        {"LWP", SIGLWP},
#endif
#ifdef SIGWAITING
        {"WAITING", SIGWAITING},
#endif
        {NULL,-1},
};    /*fin array sigstrnum */

int ValorSenal(char * sen)  /*devuelve el numero de senial a partir del nombre*/
{
    int i;
    for (i=0; sigstrnum[i].nombre!=NULL; i++)
        if (!strcmp(sen, sigstrnum[i].nombre))
            return sigstrnum[i].senal;
    return -1;
}

char *NombreSenal(int sen)  /*devuelve el nombre senal a partir de la senal*/
{			/* para sitios donde no hay sig2str*/
    int i;
    for (i=0; sigstrnum[i].nombre!=NULL; i++)
        if (sen==sigstrnum[i].senal)
            return sigstrnum[i].nombre;
    return ("SIGUNKNOWN");
}

void do_Back(char *args[], tListS *S, tListJ *J) {

    char **new_env = NULL;
    char *exec_args[MAX];
    int i = 0, j = 0, env_vars = 0, status, priority;
    pid_t pid;
    char *job_status;
    char *date = dateJobs();
    char tipo_signal[MAX] = "000";

    if (args[0] == NULL) {
        printf("No ejecutado: Bad address\n");
        return;
    }

    // Procesar variables de entorno
    while (args[i] != NULL) {
        char *value = getenv(args[i]);
        if (value == NULL) break;
        env_vars++;
        i++;
    }
    if (args[i] == NULL) {
        printf("Error: no se especificó el ejecutable\n");
        return;
    }
    if (env_vars > 0) {
        new_env = malloc((env_vars + 1) * sizeof(char *));
        if (new_env == NULL) {
            perror("Error al asignar memoria");
            return;
        }
        for (j = 0; j < env_vars; j++) {
            char *value = getenv(args[j]);
            char *env_str = malloc(strlen(args[j]) + strlen(value) + 2);
            if (env_str == NULL) {
                perror("Error al asignar memoria");
                while (--j >= 0) free(new_env[j]);
                free(new_env);
                return;
            }
            sprintf(env_str, "%s=%s", args[j], value);
            new_env[j] = env_str;
        }
        new_env[j] = NULL;
    }

    j = 0;
    exec_args[j++] = args[i];
    i++;
    while (args[i] != NULL && j < MAX - 1) {
        exec_args[j++] = args[i++];
    }
    exec_args[j] = NULL;

    if ((pid = fork()) == -1) {
        perror("Error en fork");
        return;
    }

    if (pid == 0) {

        if (Execpve(exec_args, new_env, NULL, S) == -1) {
            perror("No ejecutado:");
            exit(EXIT_FAILURE);
        }
    } else {
        waitpid(pid, &status, 0);

        // Determinar estado
        if (WIFEXITED(status)) {
            job_status = "FINISHED";
        } else if (WIFSIGNALED(status)) {
            job_status = "SIGNALED";
            int signal_num = WTERMSIG(status);
            strcpy(tipo_signal,"KEEEE");
            strcpy(tipo_signal,NombreSenal(signal_num));

        } else if (WIFSTOPPED(status)) {
            job_status = "STOPPED";
        } else {
            job_status = "ACTIVE";
        }


        priority = (strcmp(job_status, "STOPPED") == 0) ? 0 : -1;


        insertJobItem(pid, date, job_status, getlogin(), tipo_signal, priority, exec_args[0], NULL, J);

        if (new_env != NULL) {
            for (i = 0; i < env_vars; i++) {
                free(new_env[i]);
            }
            free(new_env);
        }
    }
}



void do_backPRI(char *args[],int priority ,tListS *S,tListJ  *J){

    char **new_env = NULL;
    char *exec_args[MAX];
    int i = 0, j = 0, env_vars = 0, status;
    pid_t pid;
    char *job_status;
    char *date = dateJobs();
    char tipo_signal[MAX] = "000";


    if (args[0] == NULL) {
        printf("No ejecutado: Bad address\n");
        return;
    }


    while (args[i] != NULL) {
        char *value = getenv(args[i]);
        if (value == NULL) break;
        env_vars++;
        i++;
    }
    if (args[i] == NULL) {
        printf("Error: no se especificó el ejecutable\n");
        return;
    }
    if (env_vars > 0) {
        new_env = malloc((env_vars + 1) * sizeof(char *));
        if (new_env == NULL) {
            perror("Error al asignar memoria");
            return;
        }
        for (j = 0; j < env_vars; j++) {
            char *value = getenv(args[j]);
            char *env_str = malloc(strlen(args[j]) + strlen(value) + 2);
            if (env_str == NULL) {
                perror("Error al asignar memoria");
                while (--j >= 0) free(new_env[j]);
                free(new_env);
                return;
            }
            sprintf(env_str, "%s=%s", args[j], value);
            new_env[j] = env_str;
        }
        new_env[j] = NULL;
    }


    j = 0;
    exec_args[j++] = args[i];
    i++;
    while (args[i] != NULL && j < MAX - 1) {
        exec_args[j++] = args[i++];
    }
    exec_args[j] = NULL;

    if ((pid = fork()) == -1) {
        perror("Error en fork");
        return;
    }

    if (pid == 0) {
        // Proceso hijo
        if (Execpve(exec_args, new_env, NULL, S) == -1) {
            perror("No ejecutado:");
            exit(EXIT_FAILURE);
        }
    } else {

        // Proceso padre
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            job_status = "TERMINADO";
        } else if (WIFSIGNALED(status)) {
            job_status = "SENALADO";
            int signal_num = WTERMSIG(status);
            strcpy(tipo_signal,NombreSenal(signal_num));

        } else if (WIFSTOPPED(status)) {
            job_status = "PARADO";
        } else {
            job_status = "ACTIVO";
        }

        priority = (strcmp(job_status, "PARADO") == 0) ? priority : -1;

        insertJobItem(pid, date, job_status, getlogin(), tipo_signal, priority, exec_args[0], NULL, J);

        if (new_env != NULL) {
            for (i = 0; i < env_vars; i++) {
                free(new_env[i]);
            }
            free(new_env);
        }
    }

}

void do_listjobs(char *args[],tListJ J){

    tPosJ i;

    if (args[0] == NULL){

        for (i = J; i !=NULL; i = i->siguiente)
        printf(" %i\t%s p=%i %s %s (%s) %s\n",i->elemento.PID,i->elemento.nombre_usuario,i->elemento.prioridad, i->elemento.fecha, i->elemento.Status, i->elemento.tipo_signal, i->elemento.nombre_archivo);
    }
}

void do_deljobs(char *args[], tListJ *J) {
    tPosJ i;

    if (args[0] == NULL) {
        do_listjobs(args, *J);
        return;
    }

    if (strcmp("-term", args[0]) == 0) {
        for (i = *J; i != NULL;i = i->siguiente) {

            if (strcmp(i->elemento.Status, "TERMINADO") == 0) {
                deleteJobPosition(i, J);
            }

        }
    } else if (strcmp("-sig", args[0]) == 0) {
        for (i = *J; i != NULL; i = i->siguiente) {

            if (strcmp(i->elemento.Status, "SEÑALADO") == 0) {
                deleteJobPosition(i, J);
            }
            
        }
    } else {
        do_listjobs(args, *J);
    }
}

void procesarEntrada(char * cadena, char *trozos[], bool *terminado, tList L, tListF *F, tListM *M, tListS *S,
                     tListJ *J, char *envp[]) {

    TrocearCadena(cadena, trozos);

    if (trozos[0] != NULL){
        if ((strcmp("quit", trozos[0]) == 0)|| (strcmp("exit", trozos[0]) == 0) || (strcmp("bye", trozos[0]) == 0)) {
            quit(terminado, &L, F, M, S, J);
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
        else if (strcmp("command", trozos[0]) == 0) {
            if (trozos[1] != NULL && trozos[1][0] == '-') {
                help(trozos[1] + 1);  // Salta el guión para enviar el comando sin el prefijo "-"
            } else {
                printf("Uso: command -comando\n");
            }
        }
        else if(strcmp("cd", trozos[0]) == 0) {
            cd (trozos[1]);
        }
        else if(strcmp("historic", trozos[0]) == 0) {
            historial(trozos[1], L, *terminado, *F, *M, *S, *J, envp);
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
            Cmd_open(trozos+1, *F);
        }
        else if (strcmp("close", trozos[0]) == 0){
            Cmd_close(trozos+1, *F);
        }
        else if (strcmp("makefile", trozos[0]) == 0){
            makefile(trozos[1]);
        }
        else if (strcmp("makedir", trozos[0]) == 0){
            makedir(trozos[1]);
        }
        else if (strcmp("listfile", trozos[0]) == 0){
            listfile(trozos+1);
        }
        else if (strcmp("cwd", trozos[0]) == 0){
            cwd (trozos[1]);
        }
        else if (strcmp(trozos[0], "listdir") == 0) {
            listdir(trozos+1);
        }
        else if (strcmp("reclist", trozos[0]) == 0){
            reclist(trozos+1);
        }
        else if (strcmp("revlist", trozos[0]) == 0){
            revlist(trozos+1);
        }
        else if (strcmp("erase", trozos[0]) == 0) {
            erase(trozos[1]);
        }
        else if (strcmp("delrec", trozos[0]) == 0){
            if (trozos[1] != NULL) {
                delrec(trozos[1]);
            } else {
                printf("Uso: delrec <ruta>\n");
            }
        }
        else if (strcmp("dup", trozos[0]) == 0){
            Cmd_dup(trozos+1,*F);
        }
        else if(strcmp("allocate", trozos[0]) == 0){
            allocate(trozos+1, M, F);
        }
        else if(strcmp("deallocate", trozos[0]) == 0){
            deallocate(trozos+1, M, F);
        }
        else if(strcmp("memfill", trozos[0]) == 0){
            memfill(trozos+1,*M);
        }
        else if(strcmp("memdump", trozos[0]) == 0){
            memdump(trozos+1,*M);
        }
        else if(strcmp("memory", trozos[0]) == 0){
            memory(trozos+1,*M);
        }
        else if(strcmp("readfile", trozos[0]) == 0){
            Cmd_ReadFile(trozos+1);
        }
        else if(strcmp("writefile", trozos[0]) == 0){
            do_WriteFile(trozos+1, *M);
        }
        else if(strcmp("read", trozos[0]) == 0){
            do_Read(trozos+1, *M);
        }
        else if(strcmp("write", trozos[0]) == 0){
            do_Write(trozos+1, *M);
        }
        else if(strcmp("recurse", trozos[0]) == 0){
            Recursiva(atoi(trozos[1]));
        }
        else if(strcmp("getuid", trozos[0]) == 0){
            do_getuid();
        }
        else if (strcmp("fork", trozos[0]) == 0){
            Cmd_fork(trozos+1, J);
        }
        else if (strcmp("showvar", trozos[0]) == 0){
            do_showvar(trozos+1, envp);
        }
        else if(strcmp("setuid", trozos[0]) == 0){
            do_setuid(trozos+1);
        }
        else if (strcmp("changevar", trozos[0]) == 0){
            Cambio(trozos+1,envp);
        }
        else if (strcmp("subsvar", trozos[0]) == 0){
            do_subsvar(trozos+1, envp);
        }
        else if (strcmp("environ", trozos[0]) == 0){
            do_Environ(trozos+1, envp);
        }
        else if (strcmp("search", trozos[0]) == 0){
            do_search(trozos+1, S);
        }
        else if (strcmp("exec", trozos[0]) == 0){
            do_exec(trozos+1, S);
        }
        else if (strcmp("execpri", trozos[0]) == 0){
            do_ExecPri(trozos+1,S);
        }
        else if (strcmp("fg", trozos[0]) == 0){
            do_Fg(trozos+1, S);
        }
        else if (strcmp("fgpri", trozos[0]) == 0){
            do_FgPri(trozos+1, S);
        }
        else if (strcmp("back", trozos[0]) == 0){
            do_Back(trozos+1,S,J);
        }
        else if (strcmp("backpri", trozos[0]) == 0){
            do_backPRI(trozos+2, atoi(trozos[1]),S ,J);
        }
        else if (strcmp("listjobs", trozos[0]) == 0){
            do_listjobs(trozos+1, *J);
        }
        else if (strcmp("deljobs", trozos[0]) == 0){
            do_deljobs(trozos+1, J);
        }
        else{
            printf("Comando no reconocido\n");
        }
    }
    else {
        printf("Comando no reconocido\n");
    }
}

void Cmd_open (char *tr[], tListF F) {
    int i;
    int df ;
    int mode = 0;
    char Mod[MAX];
    strcpy(Mod,"");

    if (tr[0] == NULL) {
        ListarFicherosAbiertos(F);  //Imprime el historial de archivos abiertos (open)
        return;
    }
    for (i = 1; tr[i] != NULL; i++) {
        if (!strcmp(tr[i], "cr")) mode |= O_CREAT;
        else if (!strcmp(tr[i], "ex")) mode |= O_EXCL;
        else if (!strcmp(tr[i], "ro")) mode |= O_RDONLY;
        else if (!strcmp(tr[i], "wo")) mode |= O_WRONLY;
        else if (!strcmp(tr[i], "rw")) mode |= O_RDWR;
        else if (!strcmp(tr[i], "ap")) mode |= O_APPEND;
        else if (!strcmp(tr[i], "tr")) mode |= O_TRUNC;
        else break;
    }
    if ((df = open(tr[0], mode, 0777)) == -1) {
        perror("Imposible abrir fichero\n");
    } else {
        // Abre el archivo con fopen() basado en el modo
        if (mode == O_CREAT) {
            fopen(tr[0], "mode");
            strcpy(Mod, "O_CREAT");
        } else if (mode == O_EXCL) {
            fopen(tr[0], "mode");
            strcpy(Mod, "O_EXCL");
        } else if (mode == O_RDONLY) {
            fopen(tr[0], "mode");
        } else if (mode == O_WRONLY) {
            fopen(tr[0], "mode");
        } else if (mode == O_RDWR) {
            fopen(tr[0], "mode");
        } else if (mode == O_APPEND) {
            fopen(tr[0], "mode");
            strcpy(Mod, "O_APPEND");
        } else if (mode == O_TRUNC) {
            fopen(tr[0], "mode");
            strcpy(Mod, "O_TRUNC");
        }
        printf("Añadida entrada a la tabla ficheros abiertos..................\n");
        AnadirAFicherosAbiertos(&F, df, tr[0], Mod);
    }
}

void historial (char cadena[], tList L, bool terminado, tListF F, tListM M, tListS S, tListJ J, char *envp[] ) {

    tPosL i, j; // posición del comando en la lista
    int p = 0;  // posición del comando en el historial
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
            procesarEntrada(auxi, trozos, &terminado, L, &F, &M, &S, &J, envp);
        }
    }
    else if (('-' == cadena[0]) && esNumero(cadena+1) && (cadena[1] != '\0')) { // repetir los últimos N comandos
        aux = atoi(cadena+1);

        for (i = last(L); p != aux; i = i->anterior)  {
            p++;
        }
        for ( j = i->siguiente; j != LNULL; j = next(j, L) ){
            printf("%i -> %s\n",p+aux, j->elemento.datos );
            p++;
        }
    }
    else{
        printf("Comando 'historic %s' no reconocido\n", cadena);
    }
}

int main(int argc, char *argv[], char *envp[]) {

    bool terminado = false;
    char cadena[MAX];
    char *trozos[15];

    tList L;
    tListF F;
    tListM M;
    tListS S;
    tListJ J;

    createEmptyList(&L);
    createEmptyFileList(&F);
    createEmptyMemoryList(&M);
    inicicializarFileLIst(&F);
    createEmptyListSE(&S);
    createEmptyJobList(&J);

    while (!terminado){
        imprimirPrompt();
        leerEntrada(cadena,&L);
        procesarEntrada(cadena, trozos, &terminado,L, &F, &M, &S, &J, envp);
    }

    return 0;
}
