#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>
#include <stdbool.h>

#include "tabrout.h"

#define BUF_SIZE 64
#define IPV4_ADR_STRLEN 16
#define LOCALHOST "127.0.0.1"
#define NO_BASE_PORT 17900

int main(int argc, char **argv) {

  char idInitConfigFile[20];
  char myId[32];
  routing_table_t myRoutingTable;

  /* Building ID Router from command args */
  sprintf(myId, "R%s %s", argv[2], argv[1]);
  printf("ROUTEUR : %s\n", myId);

  /* Building Config File ID from command args */
  sprintf(idInitConfigFile, "R%sCfg", argv[2]);
  strcat(idInitConfigFile, ".txt");

  /* Loading My Routing Table from Initial Config file */
  init_routing_table(&myRoutingTable, idInitConfigFile);
  printf("ROUTEUR : %d entrées initialement chargées \n", myRoutingTable.nb_entry);
  display_routing_table(&myRoutingTable, myId);

  // on fork pour faire les deux roles en paralele
  pid_t pid = fork();

  if (pid < 0) {
      perror("erreur fork");
      exit(EXIT_FAILURE);
  }
  if (pid == 0) {
      // calcul port de reception -> NO_BASE_PORT + mon numero
      int my_port = NO_BASE_PORT + atoi(argv[2]);
      printf("RECEPTEUR (fils %d) : ecoute sur port %d\n", getpid(), my_port);
      // creation socket UDP reception
      int sock_rec = socket(AF_INET, SOCK_DGRAM, 0);
      if (sock_rec < 0) {
          perror("erreur socket recepteur");
          exit(EXIT_FAILURE);
      }

      // config adresse locale pour le bind
      struct sockaddr_in adr_local;
      bzero(&adr_local, sizeof(adr_local));
      adr_local.sin_family = AF_INET;
      adr_local.sin_port = htons(my_port);
      adr_local.sin_addr.s_addr = htonl(INADDR_ANY);
      // bind obligatoire pour le recepteur
      if (bind(sock_rec, (struct sockaddr*)&adr_local, sizeof(adr_local)) < 0) {
          perror("erreur bind recepteur");
          exit(EXIT_FAILURE);
      }

      // adresse voisin remplie automatiquement par recvfrom
      struct sockaddr_in adr_voisin;
      socklen_t len_voisin = sizeof(adr_voisin);

      char buff[BUF_SIZE];
      int nb_routes;

      // on recoit d'abord le nombre de routes annoncées
      memset(buff, 0, sizeof(buff));
      recvfrom(sock_rec, buff, sizeof(buff), 0,
               (struct sockaddr*)&adr_voisin, &len_voisin);
      sscanf(buff, "%d", &nb_routes);
      printf("RECEP:va recevoir %d routes du voisin\n", nb_routes);

      // reception des routes une par une
      for (int i = 0; i < nb_routes; i++) {
          memset(buff, 0, sizeof(buff));
          recvfrom(sock_rec, buff, sizeof(buff), 0,
                   (struct sockaddr*)&adr_voisin, &len_voisin);
          printf("RECEP:route reçue -> %s\n", buff);

          // ajout seulement si pas deja presente dans la table
          if (!is_present_entry_table(&myRoutingTable, buff)) {
              add_entry_routing_table(&myRoutingTable, buff);
              printf("RECEP:route ajoutée\n");
          } else {
              printf("RECEP:route deja connue, on skip\n");
          }
      }

      printf("RECEP:reception terminée\n");
      close(sock_rec);
      display_routing_table(&myRoutingTable, myId);
      exit(EXIT_SUCCESS);
  }
  else {

      // petit sleep pour laisser le temps au fils (recepteur)
      // de faire son bind avant qu'on envoie quoi que ce soit
      sleep(1);

      // calcul port du voisin
      int neighbor_port = NO_BASE_PORT + atoi(argv[3]);
      printf("EMET (pere %d) : envoi vers port %d\n", getpid(), neighbor_port);

      // creation socket UDP emission
      // pas de bind ici, le systeme attribue un port automatiquement
      int sock_em = socket(AF_INET, SOCK_DGRAM, 0);
      if (sock_em < 0) {
          perror("erreur socket emetteur");
          exit(EXIT_FAILURE);
      }

      // config adresse du voisin destinataire
      struct sockaddr_in adr_voisin;
      bzero(&adr_voisin, sizeof(adr_voisin));
      adr_voisin.sin_family = AF_INET;
      adr_voisin.sin_port = htons(neighbor_port);
      inet_pton(AF_INET, LOCALHOST, &adr_voisin.sin_addr);
      char buff[BUF_SIZE];
      // on annonce d'abord le nb de routes qui vont suivre
      sprintf(buff, "%d", myRoutingTable.nb_entry);
      sendto(sock_em, buff, strlen(buff)+1, 0,
             (struct sockaddr*)&adr_voisin, sizeof(adr_voisin));
      printf("EMET:annonce %d routes\n", myRoutingTable.nb_entry);
      // envoi des routes une par une
      for (int i = 0; i < myRoutingTable.nb_entry; i++) {
          memset(buff, 0, sizeof(buff));
          strcpy(buff, myRoutingTable.tab_entry[i]);
          sendto(sock_em, buff, strlen(buff)+1, 0,
                 (struct sockaddr*)&adr_voisin, sizeof(adr_voisin));
          printf("EMET:route envoyée -> %s\n", buff);
      }

      printf("EMET:emission terminée\n");
      close(sock_em);

      // on attend que le fils (recepteur) termine
      wait(NULL);
      exit(EXIT_SUCCESS);
  }
}