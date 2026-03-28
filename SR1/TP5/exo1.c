#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>


#define _POSIX_C_SOURCE 200809L
#define N 4
#define CheminMax 1024

int pipe_pf[2];
int pipe_fr[2];

void parcourir(char *dirpath){
    DIR *dir=opendir(dirpath);
    if(dir==NULL){perror("Ouverture reperotire");exit(1);}

    struct dirent *rep;
    char cheminEnt[cheminMax];

    struct stat Infos;

    while((rep=readdir(dir))!=NULL){
        if (strcmp(rep->d_name,".") && strcmp(rep->d_name,"..")){
            continue;
        }
        snprintf(cheminEnt,cheminMax,"%s,%s",dirpath,rep->d_name)
        lstat(cheminEnt,&Infos);

        if (S_ISREG(Infos.st_mode)){
            write(pipe_pf[1],cheminEnt,cheminMax);
        }
        if (S_ISDIR(Infos.st_mode)){
            parcourir(cheminEnt);
        }
        closedir(dir);
    }



}



int main(int argc,char * argv[]){

    return 0;
}