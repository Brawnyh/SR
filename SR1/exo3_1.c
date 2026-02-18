#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>




int main(void){
    int f=open("/mnt/c/Users/Mamadou/workspace/SR1/fichier.txt",O_WRONLY|O_APPEND|O_EXCL);
    if (f==-1){
        perror("Erreur de ouverture");
        exit(1);
    }
    for (uint8_t i=0;i<3;i++){
        if(write(fd;&i,1)!=1){
            perror("Write");
            exit(1);
        }
    }
    if (close(f)!=0){
        perror("close");
        exit(1)
    }
    return 0;
}