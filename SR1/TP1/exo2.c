#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        execlp("ls", "ls", "-al", NULL);
        perror("execlp");
        exit(1);
    }
    else {
        wait(NULL); 
        execlp("date", "date", NULL);
        perror("execlp");
        exit(1);
    }
}
