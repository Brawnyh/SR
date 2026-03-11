#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
//find
//hay que modificar esto del 4_2 para que funcione:tiene que dar la lista de archivos en el directorio
long findR(void){
    DIR *dir=opendir(".");

    if (dir=NULL){
        perror("erreur repertoire");
        exit(1);
    }

    long taille=0;
    struct dirent *element;
    while((element=readdir(dir))!=NULL){
        struct stat infos;
        if (lstat(element->d_name,&infos)==0){
            if(S_ISREG(infos.st_mode))
                taille+=infos.st_size;
        }
        else
            perror(element->d_name);
    }
    
    
    closedir(dir);

    strcpy(buff,dir->d_name);
    snprintf(buff,256,"%s")
   
}


int main(int argc,char * argv[]){
    return 0;
}