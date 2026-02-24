#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>




int main(int argc,char *argv[]){
    //int f=open("/mnt/c/Users/Mamadou/workspace/SR1/fichier.txt",O_WRONLY|O_APPEND|O_EXCL);
    int f=open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IEXEC|S_IWRITE);//habria que ver como va esto
    if (f==-1){
        perror("Erreur de ouverture");
        exit(1);
    }
    if(argc<2){
        perror("not enought arguments");
    }
    //habia que usar read en este contexto


    for (uint8_t i=0;i<3;i++){
        if(write(fd;&i,1)!=1){
            perror("Write");
            exit(1);
        }
    }
    if (close(f)!=0){
        perror("close");
        exit(1)
    }
    return 0;
}