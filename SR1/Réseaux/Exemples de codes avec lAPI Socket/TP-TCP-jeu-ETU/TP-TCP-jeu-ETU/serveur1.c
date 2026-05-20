/* =================================================================== */
// Progrmame Serveur qui calcule le résultat d'un coup joué à partir
// des coordonnées reçues de la part d'un client "joueur".
// Version ITERATIVE : 1 seul client/joueur à la fois
/* =================================================================== */

#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <tresor.h>

/* =================================================================== */
/* FONCTION PRINCIPALE : SERVEUR ITERATIF                              */
/* =================================================================== */
int main(int argc, char **argv) {
    //création socket
    int sock=socket(AF_INET,SOCK_STREAM,0);
    if (sock==-1){
        perror("erreur socket");
        exit(0);
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
    int localAdressLongueur=sizeof(client_addr);
    while(1){
        int client_socket=accept(socket,(struct sockaddr*)&client_addr,&localAdressLongueur);
        char buff[16];
        recv(socket,buff,16,0);
        int res;
        sscanf(buff,"%d",res);
    }


    return 0;
} // end main
