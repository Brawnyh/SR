#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void ecrire_dans_stdout(char nom_fich[]){
    //ssize_t nb_lu;

    int f=open(nom_fich,O_RDONLY);
    if (f==-1){
        perror("erreur de ouverture");
        exit(1);
    }
    if (dup2(f,STDIN_FILENO)==-1){//redi entrée stand->fichier
        perror("erreur dup2");
        close(f);
        exit(1);
    }

    close(f);
    execlp("cat","cat",NULL);

    
}


int main(int argc,char *argv[]){
    if (argc!=2){
        perror("il faut un argument");
        exit(1);
    }
    ecrire_dans_stdout(argv[1]);
    return 0;
}