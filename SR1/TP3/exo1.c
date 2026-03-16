#include <stdio.h>
#include <fcntl.h>   
#include <sys/stat.h>
#include <time.h>


void Afficheinode (struct stat * Infos){
    //printf("%-3d",Infos->st_mode);
    printf(" %10ld octets",Infos->st_size);
    printf(" %s",ctime(&Infos->st_mtime));
    
}


int main(){
    struct stat Infos;

    printf("exo1.c         ");
    lstat("exo1.c",&Infos);
    if(S_ISREG(Infos.st_mode)){
        printf("fichier ordinaire");
    }

    if(S_ISDIR(Infos.st_mode)){
        printf("répertoire");
    }
    
    Afficheinode(&Infos);
    printf("\n");
    
    
    return 0;
}

