#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#define MAX_BASSIN 5 

int main(void){
    int Remplir[2];
    int Etat[2];
    pipe(Remplir);
    pipe(Etat);
    int bassin=0;
    int pid=fork();

    if (pid==-1){
        perror("Erreur:fork");
        exit(2);
    }

    if(pid==0){
        close(Remplir[1]);
        close(Etat[0]);
        int N;
        while(read(Remplir[0],&N,sizeof(int))!=-1){
            if (read(Remplir[0],&N,sizeof(int))>=0){
                bassin+=1;
                fflush(stdout);
                printf("*");
                
            }
            if (bassin>MAX_BASSIN){
                write(Etat[1],&bassin,sizeof(int));
            }
             if (read(Remplir[0],&N,sizeof(int))<0){
                bassin=0;
                printf("\n");
            }
        }
        close(Remplir[0]);
        close(Etat[1]);
        exit(1);

    }else{
        int f_flags;
        int N=10;
        int x=1;
        int y=-1;
        f_flags=fcntl(Etat[0],F_GETFL);
        f_flags |= O_NONBLOCK;
        fcntl(Etat[0],F_SETFL,f_flags);

        close(Remplir[0]);
        close(Etat[1]);
        for (int i=0;i<N;i++){
            write(Remplir[1],&x,sizeof(int));
            if (read(Etat[0],&N,sizeof(int))!=-1){
                write(Remplir[1],&y,sizeof(int));
            }
            //printf("*");
            
        }
        

        //printf("\n");
        close(Remplir[1]);
        close(Etat[0]);
        wait(NULL);
    } 
   




    return 0;
}