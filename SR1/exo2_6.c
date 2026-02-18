#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//ejercicios similar en el sigueinte TP
int main(void){
    //sequence de commande ls -l et 
    //ls -l child
    switch (fork())
    {
    case -1:
        perror("fork");
        exit(1);
    case 0:
        execlp("ls","ls","-l",NULL);//se separan los parametros de ls
        perror("exec ls");
        exit(1);
    default:
        while(wait(NULL)==-1); //esta parte no la enteindo
    }

    //pwd child
    switch (fork()){
    case -1:
        perror("fork");
        exit(1);
    case 0:
        execlp("pwd","pwd",NULL);
        perror("exec pwd");
        exit(1);
    default:
        while(wait(NULL)==-1);
    }
    return 0;
}