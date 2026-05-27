/*================================================*/
//Programme simulant un protocole de routage dynamique simplifié
// Ce programme code uniquement le comportement
// de récpeteur d'une annonce de routage
// émise depuis UN SEUL routeur voisin pour UN échange initial de routes
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

#define BUF_SIZE_IN 64 // we should receive less...
#define IPV4_ADR_STRLEN 16  // == INET_ADDRSTRLEN
#define LOCALHOST "127.0.0.1"
#define NO_BASE_PORT 17900  // base number for computing real port number


/* =================================================================== */
/* FONCTION PRINCIPALE : PEER PROCESSUS DE ROUTAGE ROLE RECEPTEUR ONLY */
/* =================================================================== */
int main(int argc, char **argv) {

  // Usage routPrec IDIP@ssRouter  MyNumberRouter NeigborNumberRouter
  // Example routPrec 10.1.1.1 1 2

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

  /* A COMPLETER PAR LES ETUDIANTS ...
  ************************************/
    // creation socket UDP pour la reception
  // ici on a besoin du bind contrairement a l'emetteur
  // car le voisin doit savoir sur quel port nous envoyer les messages
  int sock_rec = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock_rec < 0) {
      perror("erreur creation socket recepteur");
      exit(EXIT_FAILURE);
  }

  // calcul du port de reception a partir de notre numero
  // ex: si on est R2 -> port = 17900 + 2 = 17902
  int my_port = NO_BASE_PORT + atoi(argv[2]);
  printf("RECEP:en ecoute sur port %d\n", my_port);

  // configuration de notre adresse locale
  struct sockaddr_in adr_local;
  bzero(&adr_local, sizeof(adr_local));
  adr_local.sin_family = AF_INET;
  adr_local.sin_port = htons(my_port);
  adr_local.sin_addr.s_addr = htonl(INADDR_ANY); // on ecoute sur toutes les interfaces

  // bind obligatoire pour le recepteur
  // sans ca le voisin sait pas ou envoyer ses messages
  if (bind(sock_rec, (struct sockaddr*)&adr_local, sizeof(adr_local)) < 0) {
      perror("erreur bind");
      exit(EXIT_FAILURE);
  }

  // adresse de l'emetteur, sera remplie par recvfrom automatiquement
  struct sockaddr_in adr_voisin;
  socklen_t len_voisin = sizeof(adr_voisin);

  char buff[BUF_SIZE_IN];
  int nb_routes;

  // on recoit d'abord le nombre de routes qui vont arriver
  // protocole : l'emetteur envoie toujours le nb avant les routes
  memset(buff, 0, sizeof(buff));
  recvfrom(sock_rec, buff, sizeof(buff), 0,
           (struct sockaddr*)&adr_voisin, &len_voisin);
  sscanf(buff, "%d", &nb_routes);
  printf("RECEP:va recevoir %d routes\n", nb_routes);

  // reception des routes une par une
  for (int i = 0; i < nb_routes; i++) {
      memset(buff, 0, sizeof(buff));
      recvfrom(sock_rec, buff, sizeof(buff), 0,
               (struct sockaddr*)&adr_voisin, &len_voisin);
      printf("RECEP:route reçue -> %s\n", buff);

      // on ajoute la route seulement si elle est pas deja dans notre table
      // evite les doublons si on recoit deux fois la meme route
      if (!is_present_entry_table(&myRoutingTable, buff)) {
          add_entry_routing_table(&myRoutingTable, buff);
          printf("RECEP:route ajoutée a la table\n");
      } else {
          printf("RECEP:route deja presente, on skip\n");
      }
  }

  printf("RECEP:echange terminé\n");
  close(sock_rec);

  // Display new content of my routing table
  display_routing_table(&myRoutingTable,myId);
  exit(EXIT_SUCCESS);
 }
