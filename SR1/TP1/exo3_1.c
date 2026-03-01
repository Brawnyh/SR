#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "decoupe.h"

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++){
        pid_t pid = fork();
        if (pid < 0){
            perror("fork");
            exit(1);
        }
        if (pid == 0){
            char commande[256];
            char *mots[NBMOTSMAX + 1];
            strcpy(commande, argv[i]);
            Decoupe(commande, mots);
            printf("[%d] Je lance %s\n", getpid(), argv[i]);
            execvp(mots[0], mots);

            perror("execvp");
            exit(1);
        }
        else{
            printf("[%d] J'attends %d\n", getpid(), pid);
            wait(NULL);
        }
    }
    
    printf("[%d] J'ai fini\n", getpid());
    return 0;
}
