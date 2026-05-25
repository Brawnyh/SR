/*=============================================================*/
//Programme simulant un protocole de routage dynamique simplifié
// Ce programme code uniquement le comportement
// d'émetteur d'une annonce de routage
// vers UN SEUL routeur voisin pour UN échange initial de routes
// T. Desprats - Novembre 2022

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h> // struct sockaddr_in
#include <time.h>
#include <signal.h>
#include <stdbool.h>

#include "tabrout.h"

#define BUF_SIZE_OUT 64 // we should send less...
#define IPV4_ADR_STRLEN 16  // == INET_ADDRSTRLEN
#define LOCALHOST "127.0.0.1"
#define NO_BASE_PORT 17900  // base number for computing real port number


/* =================================================================== */
/* FONCTION PRINCIPALE : PEER PROCESSUS DE ROUTAGE ROLE EMETTEUR ONLY  */
/* =================================================================== */

int main(int argc, char **argv) {

  // Usage routPem IDIP@ssRouter  MyNumberRouter NeigborNumberRouter
  // Example routPem 10.1.1.1 1 2

  char idInitConfigFile [20]; //Id of the configuration file of the router
  char myId [32]; // String array representing the whole id of the Router
  routing_table_t myRoutingTable; //Routing TABLE


  /* Building ID Router from command args */
  sprintf(myId,"R%s %s",argv[2],argv[1]);
  printf("ROUTEUR : %s\n",myId );
  //printf("construction id fichier\n");
  /* Building Config File ID from command args */
  sprintf(idInitConfigFile,"R%sCfg",argv[2]);
  strcat(idInitConfigFile,".txt");
  //printf("\n Nom fichier Configuration : %s",idInitConfigFile);
  /* Loading My Routing Table from Initial Config file */
  init_routing_table(&myRoutingTable, idInitConfigFile);
  printf("ROUTEUR : %d entrées initialement chargées \n",myRoutingTable.nb_entry);
  display_routing_table(&myRoutingTable,myId);

  /* A COMPLETER PAR LES ETUDIANTS ....*/

  // creation de la socket UDP pour l'emission
  // pas besoin de bind ici, le systeme choisit le port tout seul
  int sock_em = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock_em < 0) {
      perror("erreur creation socket emetteur");
      exit(EXIT_FAILURE);
  }

  // on calcule le port du voisin a partir de son numero
  // ex: voisin 2 -> port 17902
  int neighbor_port = NO_BASE_PORT + atoi(argv[3]);
  int neighbor_num = atoi(argv[3]);

  // remplissage de l'adresse du voisin (destinataire)
  // on utilise localhost pour les tests sur la meme machine
  struct sockaddr_in adr_voisin;
  bzero(&adr_voisin, sizeof(adr_voisin));
  adr_voisin.sin_family = AF_INET;
  adr_voisin.sin_port = htons(neighbor_port);
  inet_pton(AF_INET, LOCALHOST, &adr_voisin.sin_addr);

  printf("EMETTEUR : envoi vers voisin R%d port %d\n", neighbor_num, neighbor_port);

  char buff[BUF_SIZE_OUT];

  // protocole : on envoie d'abord le nombre d'entrees qui vont suivre
  // comme ca le recepteur sait combien de messages il doit recevoir
  sprintf(buff, "%d", myRoutingTable.nb_entry);
  sendto(sock_em, buff, strlen(buff)+1, 0,
         (struct sockaddr*)&adr_voisin, sizeof(adr_voisin));
  printf("EMETTEUR : annonce du nb d'entrees -> %d\n", myRoutingTable.nb_entry);

  // maintenant on envoie chaque entree de la table un par un
  for (int i = 0; i < myRoutingTable.nb_entry; i++) {
      memset(buff, 0, sizeof(buff));
      // on copie l'entree dans le buffer avant envoi
      strcpy(buff, myRoutingTable.tab_entry[i]);
      sendto(sock_em, buff, strlen(buff)+1, 0,
             (struct sockaddr*)&adr_voisin, sizeof(adr_voisin));
      printf("EMETTEUR : route envoyée -> %s\n", buff);
  }

  printf("EMETTEUR : toutes les routes ont été envoyées\n");

  // fermeture socket, plus besoin
  close(sock_em);



  exit(EXIT_SUCCESS);
 }
