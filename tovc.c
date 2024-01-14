#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <C:\Users\DELL\Desktop\nvsfsd\Projet-SFSD\tovc.h>
TOVC* creation(char* file_name, int nb) {
    TOVC* fichier = open(file_name, 'N');
      // Open a new file
    if (fichier == NULL) {
        printf("Error opening the file.\n");
        return NULL;
    }
    eng record;
    for (int i = 1; i <= nb; i++) {
        // Get user input for the record data
        printf("Enter data for record %d:\n", i);
        printf("Key: ");
        scanf("%s", record.cle);
        printf("Variable Field: ");
        scanf("%s", record.champVariable);

        // Write the record to the file
        int blockNum = allocBloc(fichier);
        writeDir(fichier, blockNum, *(BUFFER*)&record);
    }

    // Close the file
    fermer(fichier);

    // Reopen the file in read mode
    fichier = open(file_name, 'A');

    return fichier;
}
void change_entete(TOVC* Pointeur , int i,int valeur){
      if (i==1) (Pointeur->entete).numeroDernierBloc=valeur;
        else if (i==2) (Pointeur->entete).posLibre=valeur;
        else if (i==3) (Pointeur->entete).nbCharIns=valeur;
        else (Pointeur->entete).nbCharSupp=valeur;
}
 
int Entete(TOVC* Pointeur , int i){
        if (i==1) return ((Pointeur->entete).numeroDernierBloc);
        if (i==2) return ((Pointeur->entete).posLibre);
        if (i==3) return ((Pointeur->entete).nbCharIns);
        return ((Pointeur->entete).nbCharSupp);
}
TOVC* open(char* file_name, char modee) {
    TOVC *Pointeur = malloc(sizeof(TOVC));
TOVC* open(char* file_name, char modee) {
    TOVC *Pointeur = malloc(sizeof(TOVC));
    if (Pointeur == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    if ((modee == 'A') || (modee =='a')) 
        Pointeur->fichier = fopen(file_name, "rb+");

    // Check if the file is opened successfully
    if (Pointeur->fichier == NULL) {
        // The file doesn't exist, create a new one
        Pointeur->fichier = fopen(file_name, "wb+");
        
        // Initialize the header for a new file
        (Pointeur->entete).numeroDernierBloc = 0;
        (Pointeur->entete).posLibre = 0;
        (Pointeur->entete).nbCharIns = 0;
        (Pointeur->entete).nbCharSupp = 0;
        
        // Write the header to the file
        fwrite(&(Pointeur->entete), sizeof(ENTETE), 1, Pointeur->fichier);
    } else {
        // The file already exists, read the existing header
        fread(&(Pointeur->entete), sizeof(ENTETE), 1, Pointeur->fichier);
    }

    return Pointeur;
}
void fermer(TOVC* Pointeur) {
    fseek(Pointeur->fichier ,0,0);
    fwrite(&(Pointeur->entete), sizeof(ENTETE), 1, Pointeur->fichier);
    fclose(Pointeur->fichier);
    free(Pointeur);
}
void writeDir(TOVC *Pointeur, int i, Tbloc buff) {
    // Transmission des informations du bloc depuis un Tbloc
    Pointeur->tab_buffer[i] = buff;
    // Mise à jour de l'en-tête 
    change_entete(Pointeur, 0, i);
    change_entete(Pointeur, 1, i * sizeof(Tbloc));  // Fixed-size block
    // Positionnement du curseur au bon emplacement dans le fichier
    if (fseek(Pointeur->fichier, sizeof(ENTETE) + (i - 1) * sizeof(Tbloc), SEEK_SET) != 0) {
        fprintf(stderr, "Error in fseek.\n");
        return;
    }
    // Écriture du bloc depuis le Tbloc dans le fichier
    size_t bytesWritten = fwrite(&buff, sizeof(Tbloc), 1, Pointeur->fichier);
    if (bytesWritten != 1) {
        fprintf(stderr, "Error in fwrite.\n");
        return;
    }
}
void readDir(TOVC *Pointeur, int i, Tbloc* buf) {
    if (i <= Entete(Pointeur, 1)) {
        nbLecturesDir++;
        // Utilisation d'un Tbloc temporaire
        Tbloc tempTbloc;
        // Positionnement du curseur au bon emplacement dans le fichier
        if (fseek(Pointeur->fichier, sizeof(ENTETE) + (i - 1) * sizeof(Tbloc), SEEK_SET) != 0) {
            fprintf(stderr, "Error in fseek.\n");
            return;
        }
        // Lecture du bloc depuis le fichier vers le Tbloc temporaire
        size_t bytesRead = fread(&tempTbloc, sizeof(Tbloc), 1, Pointeur->fichier);
        if (bytesRead != 1) {
            fprintf(stderr, "Error in fread.\n");
            return;
        }
        // Copie du contenu du Tbloc temporaire dans le Tbloc de sortie
        *buf = tempTbloc;
    }
}
int allocBloc(TOVC *Pointeur)        /*allocation d'un bloc*/
{
       change_entete(Pointeur, 1, Entete(Pointeur, 1)+1);
        return Entete(Pointeur,1);
}

int main() {
    TOVC* fichier;
    char file_name[20], mode[20];
    int nb = 0;

    printf("Donnez le nom du fichier : \n");
    scanf(" %s", file_name);
    printf("Ancien ou Nouveau ? (a/n) : ");
    scanf(" %s", mode);

    if (mode[0] == 'a' || mode[0] == 'A') {
        fichier = open(file_name, 'A');
    } else {
        printf("Donner le nombre d'enregistrement que vous voulez ins%crer dans votre fichier SVP !!\n", 130);
        scanf("%d", &nb);
        fichier = creation(file_name, nb);
    }

    return 0;
}

