#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void afficher_infos(void) {
    printf("PID  : %d\n", getpid());
    printf("PPID : %d\n", getppid());
    //printf("GetEnv :%s\n",getenv("Logname"));
}

int main(void) {

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        printf("\n--- FILS ---\n");
        afficher_infos();
        printf("Le fils se termine\n");
        exit(0);
    }

    else {
        printf("\n--- PERE ---\n");
        afficher_infos();

        wait(NULL); 

        printf("Le père se termine\n");
    }

    return 0;
}
