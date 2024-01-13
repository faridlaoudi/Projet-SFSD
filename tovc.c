#include <stdio.h>
#include <stdlib.h>
#include <tovc.h>

TOVC* open(char* file_name , char modee) {
    TOVC *pointeur = malloc(sizeof(TOVC));

    if ((modee == 'A') || (modee =='a')) 
        pointeur->fichier = fopen(file_name, "r+");
    else 
        pointeur->fichier = fopen(file_name, "w+");

    if ((modee == 'A') || (modee =='a')) {
        fread(&(pointeur->entete), sizeof(ENTETE), 1, pointeur->fichier);
    } else {
        (pointeur->entete).numeroDernierBloc = 0;
        (pointeur->entete).posLibre = 0;
        (pointeur->entete).nbCharIns = 0;
        (pointeur->entete).nbCharSupp = 0;
        fread(&(pointeur->entete), sizeof(ENTETE), 1, pointeur->fichier);
    }
    return pointeur;
}

void fermer(TOVC* pointeur) {
    fseek(pointeur->fichier, 0, 0);
    fwrite(&(pointeur->entete), sizeof(ENTETE), 1, pointeur->fichier);
    fclose(pointeur->fichier);
    free(pointeur);
}
