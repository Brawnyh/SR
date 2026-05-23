/* =================================================================== */
// Progrmame Serveur qui calcule le résultat d'un coup joué à partir
// des coordonnées reçues de la part d'un client "joueur".
// Version ITERATIVE : 1 seul client/joueur à la fois
/* =================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "tresor.h"

/* =================================================================== */
/* FONCTION PRINCIPALE : SERVEUR ITERATIF                              */
/* =================================================================== */
int main(int argc, char **argv) {
    setbuf(stdout, NULL); //remplace flush stdout,pr pouvoir voir les prints en serveur
    //création socket
    int sock=socket(AF_INET,SOCK_STREAM,0);
    if (sock==-1){
        perror("erreur socket");
        exit(1);
    }
    //On associe une addresse socket/numero service
    struct sockaddr_in local_addr,client_addr;
    bzero(&local_addr,sizeof(local_addr));
    local_addr.sin_port = htons(30000);
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //binding
    bind(sock,(struct sockaddr *)&local_addr,sizeof(local_addr));
    //on met la socket a l'ecoute des demandes
    listen(sock,5);

    //creation socket client dediée a cette connexion
    socklen_t localAdressLongueur=sizeof(client_addr);
    while(1){
        int client_socket=accept(sock,(struct sockaddr*)&client_addr,&localAdressLongueur);
        char buff[16];
        int lig,col,res;
        do{
            memset(buff,0,sizeof(buff));
            recv(client_socket,buff,16,0);
        
            sscanf(buff,"%d %d",&lig,&col);
            printf("cout reçu: %d %d",lig,col);
            res=recherche_tresor(10,4,5,lig,col); //taille tableau,x_tresor,y_tresor
    
            printf("trouvé: %d",res);
            sprintf(buff, "%d", res);
            send(client_socket, buff, strlen(buff)+1, 0);
            

    }while(res!=0);
    close(client_socket);

    }

    


    //fermeture
    shutdown(sock,2);
    close(sock);

    return 0;
} // end main
