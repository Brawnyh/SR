#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_BASSIN 5
#define N 10

int main(void) {
    int Remplir[2];
    int Etat[2];
    pipe(Remplir);
    pipe(Etat);

    int bassin = 0;
    int pid = fork();

    if (pid == -1) {
        perror("Erreur:fork");
        exit(2);
    }

    if (pid == 0) {
        close(Remplir[1]);
        close(Etat[0]);

        int val;
        while (read(Remplir[0], &val, sizeof(int)) > 0) {
            if (val > 0) {
                bassin += val;
                printf("*");
                fflush(stdout);
                if (bassin > MAX_BASSIN)
                    write(Etat[1], &bassin, sizeof(int));
            } else {
                bassin = 0;
                printf("bassin vidé\n");
                fflush(stdout);
            }
        }

        close(Remplir[0]);
        close(Etat[1]);
        exit(1);

    } else {
        int f_flags = fcntl(Etat[0], F_GETFL);
        f_flags |= O_NONBLOCK;
        fcntl(Etat[0], F_SETFL, f_flags);

        close(Remplir[0]);
        close(Etat[1]);

        int x = 1;
        int y = -1;
        int etat_val;

        for (int i = 0; i < N; i++) {
            write(Remplir[1], &x, sizeof(int));
            sleep(1);
            if (read(Etat[0], &etat_val, sizeof(int)) > 0)
                write(Remplir[1], &y, sizeof(int));
        }

        close(Remplir[1]);
        close(Etat[0]);

        int status;
        wait(&status);
        if (WIFEXITED(status))
            printf("fils terminé avec code %d\n", WEXITSTATUS(status));
    }

    return 0;
}