#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


//exercices 2.2 anneau unicdirectionnel 
int main(void){
    pid_t p1,p2,p3;
    p1=getpid();
    switch (fork())
    {
    case -1:
        perror("fork");
        exit(1);//termine le code de execution current et renvoie le code de retour 1
        break;
        
    default:
        sleep(4);
        p2=getpid();
        printf("%d",p2);
        break;
    }
    return 0;
    //esto voy a tener que hacerlo solo todo lo de los suceros dan te
}

//PRIMITIVES UNIX
