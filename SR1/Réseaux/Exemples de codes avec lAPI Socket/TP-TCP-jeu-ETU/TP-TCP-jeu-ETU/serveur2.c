/* =================================================================== */
// Progrmame Serveur qui calcule le résultat d'un coup joué à partir
// des coordonnées reçues de la part d'un client "joueur".
// Version CONCURRENTE : N clients/joueurs à la fois
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
/* FONCTION PRINCIPALE : SERVEUR CONCURRENT                            */
/* =================================================================== */
int main(int argc, char **argv) {

    setbuf(stdout, NULL);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1){
        perror("erreur socket");
        exit(1);
    }
    
    // configuration addresse serveur
    struct sockaddr_in local_addr, client_addr;
    bzero(&local_addr, sizeof(local_addr));
    local_addr.sin_port = htons(30000);
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    // bind + listen
    bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr));
    listen(sock, 5);
    
    printf("serveur concurrent en ecoute\n");
    
    socklen_t localAdressLongueur = sizeof(client_addr);
    
    while(1){
        int client_socket = accept(sock, (struct sockaddr*)&client_addr, &localAdressLongueur);
        if (client_socket < 0){
            perror("erreur accept");
            continue;
        }
        // on crée un procesus fils pour chaque client
        pid_t pid = fork();
        if (pid < 0){
            perror("erreur fork");
            close(client_socket);
            continue;
        }
        
        if (pid == 0){
            // processus fils
            // le fils na pas besoin du socket serveur
            close(sock);
            char buff[16];
            int lig, col, res;
            do {
                memset(buff, 0, sizeof(buff));
                recv(client_socket, buff, sizeof(buff), 0);
                
                sscanf(buff, "%d %d", &lig, &col);
                printf("fils %d - coup recu: %d %d\n", getpid(), lig, col);
                
                res = recherche_tresor(10, 4, 5, lig, col);
                printf("fils %d - res: %d\n", getpid(), res);
                
                sprintf(buff, "%d", res);
                send(client_socket, buff, strlen(buff)+1, 0);
                
            } while(res != 0);
            
            printf("fils %d - tresor trouve!\n", getpid());
            close(client_socket);
            exit(0);
            
        } else {
            // processus pere
            // le pere na pas besoin du socket client, cest le fils qui gere
            close(client_socket);
            // le pere repart attendre un autre client
        }
    }
    
    shutdown(sock, 2);
    close(sock);

    return 0;
} // end main
