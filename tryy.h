#ifndef TOOVC_H
#define TOOVC_H
#define Taille_Bloc 50
#define Max_enreg 20
#define b 3

typedef struct Tbloc
{
	char chaine[Taille_Bloc+1];
}	Tbloc ;

typedef struct Tbloc Buffer;

typedef char semi_enreg[Taille_Bloc+1];

typedef struct Enreg
{
    // taille de la longueur = 3
    int cle; //cle < = 999
    int sup; // booleen
    char info[Taille_Bloc-6];
}   Enreg;

typedef struct Entete
{
    int adr_dernier_bloc; //numero d'ordre
    int nbr_enreg;
    int indice_libre;
    int nb_sup;
}Entete;

typedef struct TOVC
{
    FILE *F;
    Entete entete;
}	TOVC;

TOVC *ouvrir(char *filename,char mod); // mod = 'A' ancien (rb+) || mod = 'N' nouveau (wb+)
void fermer(TOVC * filepointer);
int entete(TOVC *filepointer,int i);
void aff_entete(TOVC *filepointer,int i,int val);
void liredir(TOVC *filepointer,int i,Buffer *buf);
void ecriredir(TOVC *filepointer,int i,Buffer buf);
int alloc_bloc(TOVC *filepointer);
void NumtoS(int Num,int max,char* S);
void sub_string(char *S,int i,int k, char *R);
void Sup_inter_string(char *S,int i,int k);
void EnregtoSemi(Enreg E,semi_enreg SE);
void SemitoEnreg(semi_enreg SE,Enreg *E)
void recupsemi_enreg(TOVC *filepointer,semi_enreg SE,int *i,int *j) 
void affich_TOVC(TOVC * filepointer);
void Recherche_TOVC(TOVC *filepointer,int cle,int *i,int *j,int *trouv);
void insertion_pos_rec(TOVC *filepointer,int *i,int *j,semi_enreg SE);
TOVC* creation(char* file_name, int nb);

#endif