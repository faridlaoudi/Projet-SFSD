#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "code2.h"
//main//

int entete(TOVC *f, int choixutilisateur)
{
    switch(choixutilisateur)
    {
        case ENTETE_NUMERO_DERNIER_BLOC:
            return f->entete.numeroDernierBloc;
            break;
        case ENTETE_POS_LIBRE:
            return f->entete.posLibre;
            break;
        case ENTETE_NBR_ENREG:
            return f->entete.nbEnreg;
            break;
        case ENTETE_NBR_CHAR_INS:
            return f->entete.nbCharIns;
            break;
        case ENTETE_NBR_CHAR_SUP:
            return f->entete.nbCharSupp;
            break;
        default:
            printf("[ERROR] caracteristique n'existe pas !");
            return -999;
            break;
    }
}
void aff_entete(TOVC *f, int choixutilisateur, int valeur)
{
    switch(choixutilisateur)
    {
        case ENTETE_NUMERO_DERNIER_BLOC:
            f->entete.numeroDernierBloc = valeur;
            break;
        case ENTETE_POS_LIBRE:
            f->entete.posLibre = valeur;
            break;
        case ENTETE_NBR_ENREG:
            f->entete.nbrEnreg = valeur;
            break;
        case ENTETE_NBR_CHAR_INS:
            f->entete.nbrCharIns = valeur;
            break;
        case ENTETE_NBR_CHAR_SUP:
            f->entete.nbrCharSupp = valeur;
            break;
        default:
            printf("[ERROR]  n'existe pas !");
            break;
    }
}
void lire_fich(TOVC *f, int blocNum, Buffer *buf)
{
    fseek(f->fichier, sizeof(Entete) + (blocNum-1)*sizeof(Bloc), SEEK_SET);
    fread(buf, sizeof(Bloc), 1, f->fichier);
}
void ecrire_fich(TOVC *f, int blocNum, Buffer *buf)
{
    fseek(f->fichier, sizeof(Entete) + (blocNum-1)*sizeof(Bloc), SEEK_SET);
    fwrite(buf, sizeof(Bloc), 1, f->fichier);
}
//ouverture
void ouvrir(TOVC *f, char nomf[], char mode)
{
    if ((mode == 'N') || (mode == 'n')) // ouverture d'un nouveau fichier
    {
        f->fichier = fopen(nomf, "wb+");
        if (f->fichier == NULL)
        {
            printf("[ERROR] erreur d'ouverture de fichier !");
            return;
        }

        // initialisations de l'entete du nouveau fichier
        aff_entete(f, ENTETE_NUMERO_DERNIER_BLOC, 0); // le numero du dernier bloc = 0
        aff_entete(f, ENTETE_POS_LIBRE, 0); // la position libre dans le dernier bloc = 0
        aff_entete(f, ENTETE_NBR_ENREG, 0); // le nombre des enregistrements dans le fichier = 0
        aff_entete(f, ENTETE_NBR_CHAR_INS, 0); // le nombre des caracteres inseres = 0
        aff_entete(f, ENTETE_NBR_CHAR_SUP, 0); // le nombre des caracteres suppremes = 0
        fwrite(&(f->entete), sizeof(Entete), 1, f->fichier);
        rewind(f->fichier);
    }
    else if ((mode == 'A') || (mode == 'a')) // ouverture d'un ancien fichier
    {
        f->fichier = fopen(nomf, "rb+");
        if (f->fichier == NULL)
        {
            printf("[ERROR] erreur d'ouverture de fichier !");
            return;
        }

        // lire l'entete de l'ancien fichier
        fread(&(f->entete), sizeof(Entete), 1, f->fichier);
        rewind(f->fichier);
    }
    else
    {
        printf("[ERROR] mode n'existe pas !");
        f->fichier = NULL;
        return;
    }
}
void fermer(TOVC *f)
{
    rewind(f->fichier);
    fwrite(&(f->entete), sizeof(Entete), 1, f->fichier); // sauvegarder l'entete dans le fichier (bloc 0)
    fclose(f->fichier);
}
int alloc_bloc(TOVC *f)
{
    int numeroDernierBloc = entete(f, ENTETE_NUMERO_DERNIER_BLOC) + 1;
    aff_entete(f, ENTETE_NUMERO_DERNIER_BLOC, numeroDernierBloc);
    return numeroDernierBloc;
}
//comme fgets()
void lire_chaine(TOVC *f, Buffer *buf, int *i, int *j, int taille, char *chaine[])
{
    *chaine = calloc(taille + 1, sizeof(char));

    for (int k = 0; k < taille; k++)
    {
        if (*j >= TAILLE )
        {
            (*i)++;
            *j = 0;
            lire_dir(f, *i, buf);
        }
        (*chaine)[k] = buf->tab[*j];
        (*j)++;
    }

    (*chaine)[taille] = '\0';
}
void ecrire_chaine(TOVC *f, Buffer *buf, int *i, int *j, int taille, char chaine[])
{
    for (int k = 0; k < taille; k++)
    {
        if (*j >= TAILLE)
        {
            buf->tab[TAILLE] = '\0';
            ecrire_dir(f, *i, buf);
            (*i)++;
            *j = 0;

            if ((*i) <= entete(f, ENTETE_NUMERO_DERNIER_BLOC))
                lire_dir(f, *i, buf);
        }

        buf->tab[*j] = chaine[k];
        (*j)++;
    }
}

void inserer_chaineCarchar (nomf[], int i, int j, int taille, char chaine[])
{
    TOVC f;
    Buffer buf;
    int k = 0, indice;
    char temp;

    ouvrir(&f, nomf, 'A');
    lire_dir(&f, i, &buf); // la premiere lecture

    // copier avec deplacement jusqu'a l'avant dernier bloc des caracteres de la chaine de caracteres
    while (i < entete(&f, ENTETE_NUMERO_DERNIER_BLOC))
    {
        if (j >= MAX_CHARS)
        {
            buf.tab[MAX_CHARS] = '\0';
            ecrire_dir(&f, i, &buf);
            i++;
            j = 0;
            lire_dir(&f, i, &buf);

            if (i == entete(&f, ENTETE_NUMERO_DERNIER_BLOC))
                break;
        }

        temp = buf.tab[j];
        buf.tab[j] = chaine[k];
        chaine[k] = temp;

        k = (k + 1) % taille;
        j++;
    }

    // decalages dans le dernier bloc
    while (j < entete(&f, ENTETE_POS_LIBRE))
    {
        temp = buf.tab[j];
        buf.tab[j] = chaine[k];
        chaine[k] = temp;

        k = (k + 1) % taille;
        j++;
    }

    // continuer les decalages avec ajout des nouveau bloc si neccessaire
    indice = (k - 1 + taille) % taille; // sauvegarder l'indice d'arret
    while (k != indice)
    {
        if (j >= TAILLE)
        {
            buf.tab[TAILLE] = '\0';
            ecrire_dir(&f, i, &buf);
            i++;
            j = 0;
        }

        buf.tab[j] = chaine[k];
        k = (k + 1) % taille;
        j++;
    }

    if (j >= TAILLE)
    {
        buf.tab[TAILLE] = '\0';
        ecrire_dir(&f, i, &buf);
        i++;
        j = 0;
    }

    buf.tab[j] = chaine[k]; // ecriture du dernier caractere
    j++;
    buf.tab[j] = '\0';
    ecrire_dir(&f, i, &buf); // derniere ecriture

    // mise a jour de l'entete du fichier
    aff_entete(&f, ENTETE_NUMERO_DERNIER_BLOC, i);
    aff_entete(&f, ENTETE_POS_LIBRE, j);
    aff_entete(&f, ENTETE_NBR_ENREG, entete(&f, ENTETE_NBR_ENREG) + 1);
    aff_entete(&f, ENTETE_NBR_CHAR_INS, entete(&f, ENTETE_NBR_CHAR_INS) + taille);

    fermer(&f);
}
void afficher_entete(TOVC *f)
{
    int h, w;
    
    int xLen = strlen("+--------------------------------------------------------------------+");
   
    printf("+-----------------------------\\ ENTETE /-----------------------------+\n");

    
    printf("|");
 
    printf("|\tLe numero du dernier bloc  \t\t\t%d\n", entete(f, ENTETE_NUMERO_DERNIER_BLOC));
   
    printf("|");
    printf("|\tLa position libre dans le dernier bloc  \t%d\n", entete(f, ENTETE_POS_LIBRE));
   
    printf("|");
    
    printf("|\tLe nombre des enregistrement  \t\t\t%d\n", entete(f, ENTETE_NBR_ENREG));
    printf("|");
    printf("|\tLe nombre des caracteres insere  \t\t%d\n", entete(f, ENTETE_NBR_CHAR_INS));
    printf("|");
    printf("|\tLe nombre des caracteres supprime  \t\t%d\n", entete(f, ENTETE_NBR_CHAR_SUP));
    printf("+--------------------------------------------------------------------+\n");
}
void afficher_fichier(TOVC *f)
{
    afficher_entete(f);
    printf("\n");
    for (int i = 1; i <= entete(f, ENTETE_NUMERO_DERNIER_BLOC); i++)
    {
        afficher_bloc(f, i);
        printf("\n");
    }
}