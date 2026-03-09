#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc,char *argv[]){
    int f=open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);//crea el primer archivo en argumento
    if (f==-1){
        perror("Erreur de ouverture fichier");
    }
    if (argc<1){
        perror("manque de arguments");
    }else{
        dup2(f,1);
        printf("hola\n");
    }
    return 0;
}