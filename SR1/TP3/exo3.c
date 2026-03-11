#include <stdio.h>
#include <fcntl.h>   
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>

void Afficheinode (struct stat * Infos){
    //printf("%-3d",Infos->st_mode);
    printf(" %10ld octets",Infos->st_size);
    printf(" %s",ctime(&Infos->st_mtime));
    
}


int main(int argc,char *argv[]){
    struct stat Infos;

    if (argc<2){
        perror("arguments");
    }

    if (argc==2){
        DIR* rep=opendir(argv[1]);

        if(rep==NULL){
            fprintf(stdout,"Erreur de ouverture");
        }
        

        struct dirent *element;
        while((element=readdir(rep))!=NULL){
            lstat(element->d_name,&Infos);
            printf("%-20s",element->d_name);
            lstat(element->d_name,&Infos);

        

            if(S_ISREG(Infos.st_mode)){
                    printf("fichier ordinaire");
            }

            if(S_ISDIR(Infos.st_mode)){
                printf("répertoire");
            }
    
            if (S_ISBLK(Infos.st_mode)){
                printf("périph. bloc");
            }

            if (S_ISLNK(Infos.st_mode)){
                printf("lien symbolique");
            }

            Afficheinode(&Infos);
            printf("\n");

        }
        closedir(rep);
        

    }


    for (int i=1;i<argc;i++){
        lstat(argv[i],&Infos);
        
        printf("%-20s",argv[i]);
        lstat(argv[i],&Infos);

        

        if(S_ISREG(Infos.st_mode)){
            printf("fichier ordinaire");
        }

        if(S_ISDIR(Infos.st_mode)){
            printf("répertoire");
        }
    
        if (S_ISBLK(Infos.st_mode)){
            printf("périph. bloc");
        }

        if (S_ISLNK(Infos.st_mode)){
            printf("lien symbolique");
        }

        Afficheinode(&Infos);
        printf("\n");

    }

    
    
    
    return 0;
}

