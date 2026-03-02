#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        perror("arguments");
        return 1;
    }
    int i;
    int redir = 0;
    char *dest;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], ">") == 0) {
            redir = 1;
            dest = argv[i+1];
            break;
        }
    }
    if (redir) {
        int fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        dup2(fd, 1); //redi stdout
        close(fd);
    }
// test 2
//test de los commits(no cuenta los forks)
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], ">") == 0)
            break;
        pid_t pid = fork();
        if (pid == 0) {
            FILE *f = fopen(argv[i], "r");
            if (f == NULL) {
                printf("erreur ouverture %s\n", argv[i]);
                exit(1);
            }
            char c;
            while ((c = fgetc(f)) != EOF) {
                putchar(c);
            }
            fclose(f);
            exit(0);
        }
        wait(NULL);
    }
    return 0;
}