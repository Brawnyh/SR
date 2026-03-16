//tubos de comunicacion introduccion de la clase
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
//exo1 CM tubes cosas que pueden salir en el CC
//leer en el teclado ->read(0)
int main(int argc,char *argv[]){
    int tube[2];

    if(pipe(tube)==-1){
        perror("échec creation tube");
    }
    int pid=fork();


    // Père = lecteur, Fils = écrivain
    if (pid == 0) {          
        close(tube[0]);      
        write(tube[1], "hola",5);
        close(tube[1]);
    } else {                 
        close(tube[1]);      
        read(tube[0], "hola",5);
        close(tube[0]);
    }
    return 0;
}


//exercices 2 tubes

#include <ctype.h>
int main(){
    
}


int main(){
    int tube[2];
    char msg[]="Bonjour du pére";
    char buf[64];
    if (pipe(tube)==-1){ perror("pipe");exit(1);}

    int pid=fork();
    if (pid==-1){ perror("fork");exit 2;}
    if(pid==0){ //fils lecteure
        close(tube[1]);//ferme ecriture
        read(tube[0],buf,sizeof(buf));
        close(tube[0]);
        printf("fils reçoit %s\n",buf);
        close(tube[0]);
        exit(0);
    }else{ //pere ecrivain
        close(tube[0]);
        write(tube[1],msg,strlen(msg)+1);
        close(tube[1]);
        wait(NULL);
    }


    //Lecture et ecriture non bloquante
    //par deafaut read sur un tube vide est bloqué
    int flags=fcntl(tube[0],F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(tube[0],F_SETFL,flags);
    //maintenant si tube vide:
    int n=read(tube[0],buf,sizeof(buf));
    if (n==-1 && errno ==EAGAIN){
        //podemos hacer aqui otra cosa si tubo vacio
    }

}



