#ifndef TOVC_H
#define TOVC_H
//taille maximal du bloc 
#define MAX_taille 512
#define MAX_chauv 10
#define SEP_ENR '|' // Séparateur denregistrements
#define SEP_CHAMP '$' // Séparateur des champs
#define TAILLE_CLE 10
#define TAILLE_MAX_CHAMP_VARIABLE 10
#define NB_BUFFERS 10
// declaration du bloc
int nbLecturesDir=0;
typedef struct {
    char donnees[MAX_taille + 1];
} Tbloc;
// un buffer//
typedef Tbloc BUFFER;
//structure dentete 
typedef struct {
        //num de bloc dernier pour linsertion
        int numeroDernierBloc;
        //position libre
        int posLibre;
        // nombre d'eng
        int nbEnreg;
        //tracker les enregistrement inserer pour verifier l'espace libre
        int nbCharIns;
        //tracker la suppression
        int nbCharSupp;
}ENTETE;
// structure de fichier tovc
typedef struct 
{    char cle[TAILLE_CLE]; // Clé de l'enregistrement
     char champVariable[TAILLE_MAX_CHAMP_VARIABLE]; //cahmps variable
}eng;
//fichier //
typedef struct
{
    ENTETE entete;
    FILE *fichier;
    Tbloc tab_buffer[NB_BUFFERS];
}TOVC;
//fonction pour louverture en mode qlq//
TOVC* open(char* nom_fichier , char mode);
// close//
void fermer(TOVC *pointeur);
//retourne les parametre de lentete 
int Entete(TOVC* pointeur , int i);
//modifier les parametre de lentete
void change_entete(TOVC* pointeur , int i,int valeur);
void readDir(TOVC *Pointeur, int i, BUFFER* buff);      // pour la lecture d'un bloc
void writeDir(TOVC *Pointeur, int i, BUFFER buff);     //pour l'écriture d'un bloc
//allocation
int allocBloc(TOVC *Pointeur); 
TOVC *creation(char *file_name, int nb) ;
#endif 