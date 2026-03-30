#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/wait.h>

#define CHEMIN_MAX 1024
#define N 3

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <magic> <dir>\n", argv[0]);
        return 1;
    }

    char *magic = argv[1];
    char *start_dir = argv[2];

    int tube1[2];
    int tube2[2];

    pipe(tube1);
    pipe (tube2);

    //traiterFichier
    for (int i = 0; i < N; i++) {
        if (fork() == 0) {
            close(tube1[1]);
            close(tube2[0]);

            char chemin[CHEMIN_MAX];
            while (read(tube1[0], chemin, CHEMIN_MAX) > 0) {
                int fd = open(chemin, O_RDONLY);
                if (fd < 0) continue;

                int len = strlen(magic);
                char buf[16];
                if (read(fd, buf, len) == len) {
                    if (strncmp(buf, magic, len) == 0) {
                        struct stat st;
                        if (stat(chemin, &st) == 0) {
                            write (tube2[1], chemin, CHEMIN_MAX);
                            write (tube2[1], &st.st_ino, sizeof(ino_t));
                            write (tube2[1], &st.st_size, sizeof(off_t));
                        }
                    }
                }
                close(fd);
            }
            close(tube1[0]);
            close(tube2[1]);
            exit(0);
        }
    }

    // "traiterRéponse"
    if (fork() == 0) {
        close(tube1[0]);
        close(tube1[1]);
        close(tube2[1]);

        char chemin[CHEMIN_MAX];
        ino_t inode;
        off_t size;

        while (1) {
            if (read(tube2[0], chemin, CHEMIN_MAX) <= 0) break;
            if (read(tube2[0], &inode, sizeof(ino_t)) <= 0) break;
            if (read(tube2[0], &size, sizeof(off_t)) <= 0) break;

            printf("Fichier: %s | inode: %ld | taille: %ld\n",
                   chemin, inode, size);
        }

        close(tube2[0]);
        exit(0);
    }

    //pere direc
    close(tube1[0]);
    close(tube2[0]);
    close(tube2[1]);

    char stack[100][CHEMIN_MAX];
    int top = 0;
    strncpy(stack[top++], start_dir, CHEMIN_MAX);

    while (top > 0) {
        char current[CHEMIN_MAX];
        strncpy(current, stack[--top], CHEMIN_MAX);

        DIR *dir = opendir(current);
        if (!dir) continue;

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
                continue;

            char fullchemin[CHEMIN_MAX];
            snprintf(fullchemin, CHEMIN_MAX, "%s/%s", current, entry->d_name);

            if (entry->d_type == DT_DIR) {
                strncpy(stack[top++], fullchemin, CHEMIN_MAX);
            } else if (entry->d_type == DT_REG) {
                write(tube1[1], fullchemin, CHEMIN_MAX);
            }
        }
        closedir(dir);
    }
    close(tube1[1]);
    for (int i=0; i<N+1; i++) wait(NULL);

    return 0;
}