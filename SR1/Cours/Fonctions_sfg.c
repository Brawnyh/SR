#include <stdio.h>
#include <sys/stat.h>


/*stat permite obtener la informacion de un archivo

int main(){
    struct stat info;
    if (stat("fichero.txt",&info)==-1){
        fprintf(stdout,"Error al leer el archivo\n");
        return 1;
    }


    printf("Tamaño del archivo: %ld bytes\n",info.st_size);
    
    return 0;
}

*/

/*st_mode permite saber si es un archivo o directorio
int main(){
    struct stat info;
    stat("test",&info); 
    if(S_ISREG(info.st_mode)){
        printf("archivo normal\n");
    }

    if(S_ISDIR(info.st_mode)){
        printf("Es un directorio\n");
    }
    
    return 0;
}
*/

/*
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
int main(){
    struct stat info;
    stat("fichero.txt";&info)
    printf("ultima momdificacion : %s",ctime(&info.st_mtime));
    return 0

}

*/

/*
#include <stdio.h>
#include <dirent.h>

int main(){

    DIR *rep;
    rep=opendir(".");
    if(rep==NULL){
        fprintf(stdout,"Erreur de ouverture");
    }
    printf("directorio abierto correctamente");

    closedir(dir);


    //readir lee los archivos dentro del directorio

    struct dirent *element;
    while((element=readdir(rep))!=NULL){
        printf("%s\n",element->d_name),
    
    }

    closedir(rep);

    return 0;




}



*/