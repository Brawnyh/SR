#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

long currentDirSize(void){
    DIR *dir=opendir(".");

    if (dir=NULL){
        perror("erreur repertoire");
        exit(1);
    }

   
    long taille=0;
    struct dirent *element;
    while((element=readdir(dir))!=NULL){
        struct stat infos;
        if (lstat(element->d_name,&infos)==0){
            if(S_ISREG(infos.st_mode))
                taille+=infos.st_size;
        }
        else
            perror(element->d_name);
    }
    
    
    closedir(dir);

    return taille;
   
}


int main(int argc,char * argv[]){
    //long m =currentDirSize();
    printf("%ld",currentDirSize());
    return 0;
}
