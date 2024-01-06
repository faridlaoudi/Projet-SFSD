//structure de fichier tovc
#include <cstdio>
#ifndef code2.h
#define code2.h
//le bloc
#define TAILLE 512
#define Buffer 1024
typedef struct 
{
    //+1 pour le caractere de fin de chaine '/0'//
 char tab[TAILLE+1];
} Bloc,Buffer;
//structure des enregistrement 
 typedef struct
    {
        //num de bloc dernier pour linsertion
        int numeroDernierBloc;
        //position libre
        int posLibre;
        // nombre d'eng
        int nbEnreg;
        //tracker les caractere inserer pour verifier l'espace
        int nbCharIns;
        //tracker la suppression
        int nbCharSupp;
    } Entete;
     
    // des constantes pour les numeros des entetes du fichier TOVC
    #define ENTETE_NUMERO_DERNIER_BLOC 1
    #define ENTETE_POS_LIBRE 2
    #define ENTETE_NBR_ENREG 3
    #define ENTETE_NBR_CHAR_INS 4
    #define ENTETE_NBR_CHAR_SUP 5

// entete de fichier 
    typedef struct
    {
        Entete entete;
        FILE *fichier;
    } TOVC;
    // les fonctions de la machine abstraites pour un fichier TOVC: [entete, aff_entete, lire_dir, ecrire_dir, ouvrir, fermer, alloc_bloc]
    int entete(TOVC *f, int caracteristiqueNum);
    void afficher_ent(TOVC *f, int caracteristiqueNum, int valeur);
    void lire_fich(TOVC *f, int blocNum, Buffer *buf);
    void ecrire_fich(TOVC *f, int blocNum, Buffer *buf);
    void ouvrir(TOVC *f, char nomf[], char mode);
    void fermer(TOVC *f);
    int alloc_bloc(TOVC *f);

    void lire_chaineCar(TOVC *f, Buffer *buf, int *i, int *j, int taille, char *chaine[]); // lire une chaine de caracteres a partir du fichier
    void ecrire_chaineCar(TOVC *f, Buffer *buf, int *i, int *j, int taille, char chaine[]); // ecrire une chaine de caracteres dans du fichier
    void inserer_chaineCar(char nomf[], int i, int j, int taille, char chaine[]); // inserer une chaine dans un fichier 
    void afficher_entete(TOVC *f); // afficher l'entete d'un fichier
    void afficher_bloc(TOVC *f, int i); // afficher le contenu d'un bloc dans le fichier
    void afficher_fichier(TOVC *f); // afficher le contenu de tous le fichier
#endif 
