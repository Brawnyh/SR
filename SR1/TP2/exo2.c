#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void lire_nieme(char *nom, int n) {
    FILE *f = fopen(nom, "rb");
    if (f == NULL) {
        perror("Erreur ouverture fichier");
        exit(1);
    }

    if (n <= 0) {
        perror("n positif");
        fclose(f);
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    long taille = ftell(f);
    int nb_entiers = taille / sizeof(int);

    if (n > nb_entiers) {
        printf("n depasse la taille du fichier\n");
        fclose(f);
        exit(1);
    }

    fseek(f, (n-1) * sizeof(int), SEEK_SET);

    int valeur;
    fread(&valeur, sizeof(int), 1, f);

    printf("Valeur lue: %d\n", valeur);

    fclose(f);
}

void ecrire_nieme(char *nom, int n, int valeur) {
    FILE *f = fopen(nom, "rb+");
    if (f == NULL) {
        perror("Erreur ouverture fichier");
        exit(1);
    }

    if (n <= 0) {
        perror("n positif");
        fclose(f);
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    long taille = ftell(f); //pos actuelle
    int nb_entiers = taille / sizeof(int);

    if (n > nb_entiers) {
        perror("depasement taille fich");
        fclose(f);
        exit(1);
    }

    fseek(f, (n-1) * sizeof(int), SEEK_SET);

    fwrite(&valeur, sizeof(int), 1, f);

    printf("Valeur ecrite: %d\n", valeur);

    fclose(f);
}

int main(int argc, char *argv[]) {

    if (argc != 3 && argc != 4) {
        perror("arguments");
        return 1;
    }
    int n = atoi(argv[1]);
    if (argc == 3) {
        lire_nieme(argv[2], n);
    } else {
        int valeur = atoi(argv[2]);
        ecrire_nieme(argv[3], n, valeur);
    }
    return 0;
}