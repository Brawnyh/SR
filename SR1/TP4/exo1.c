#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void){
    int tube[2];
    pipe(tube);
    int pid=fork();
    int N;
    

    if (pid==-1){perror("Erreur fork");exit(1);}

    if (pid==0){
        close(tube[1]);
        while(read(tube[0],&N,sizeof(int))>0){
            printf("[fils] = %d\n",N);
        }
        
        close(tube[0]);
        exit(1);

    }else{
        close(tube[0]);
        for (int i=0;i<5;i++){
            write(tube[1],&i,sizeof(int));
            
        }
        
        close(tube[1]);
        wait(NULL);
        printf("[Pere]: Mon fils %d est terminé avec le code 1.",getppid());
    }


    return 0;
}