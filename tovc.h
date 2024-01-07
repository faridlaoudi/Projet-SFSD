#ifndef TOVC.h
#define TOVC.h
//taille maximal du bloc 
#define MAX_taille 512
//taille de buffer
#define BUFFER 1024
// un buffer //
#define SEP_ENR'|' // Séparateur denregistrements
#define SEP_CHAMP '$' // Séparateur des champs
// declaration du bloc
typedef struct 
{  
    //+1 pour le caractere de fin de chaine '/0'//
 char donnees[MAX_taille+1]
 //svt est le numero du bloc suivant on aura besoin pour linsertion et la suppression logique et meme nrmm pour garder lordre 
 int svt;
 int espaceLibre;//pour la gestion de chevauchement
}Tbloc;
//structure dentete 
typedef struct {
    //identifiant unique pour chaque enregistrement
  int id;
  //nb deng effective 
  int nb_eng;
}ENTETE;
// structure de fichier tovc
typedef struct 
{
     char cle[TAILLE_CLE]; // Clé de l'enregistrement
     char champVariable[TAILLE_MAX_CHAMP_VARIABLE]; //cahmps variable
}eng;

typedef struct
{
    ENTETE entete;
    FILE *fichier;
}TOVC;
