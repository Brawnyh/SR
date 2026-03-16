//tubos de comunicacion introduccion de la clase
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
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
