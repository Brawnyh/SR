#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
//find

long findR(void){
    DIR *dir=opendir(".");
    char buff[256];
    
    if (dir=NULL){
        perror("erreur repertoire");
        exit(1);
    }

    strcpy(buff,dir.d_name);
    snprintf(buff,256,"%s")
    while()
}


int main(int argc,char * argv[]){

    while ()


    return 0;
}