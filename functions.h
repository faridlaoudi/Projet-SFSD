#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define Taille_Bloc 30
#define Max_enreg 20
#define b 3

//Vérifie si un fichier a une certaine extension
int hasExtension(const char *fileName, const char *extension) {
    const char *dot = strrchr(fileName, '.');
    if (!dot || dot == fileName) return 0;
    return strcmp(dot, extension) == 0;
}
//Affiche la liste des fichiers avec une extension spécifique dans un répertoire donné.
void listFiles(const char *path, const char *extension) {
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    printf("avialable files :\n");
    while ((entry = readdir(dir)) != NULL) {
        if (hasExtension(entry->d_name, extension)) {
            printf("%s\n", entry->d_name);
        }
    }

    closedir(dir);
}
//Structure représentant un bloc de données, avec un tableau de caractères
typedef struct Tbloc
{
	char chaine[Taille_Bloc+1];
}	Tbloc ;
// buffer de type tbloc
typedef struct Tbloc Buffer;
//un tableau de caractères représentant un enregistrement partiel
typedef char semi_enreg[Taille_Bloc+1];
//Structure représentant un enregistrement complet avec une clé (cle), une indication de suppression logique (sup), et des informations (info).
typedef struct Enreg
{
    // taille de la longueur = 3
    int cle; //cle < = 999
    int sup; // bool�en
    char info[Taille_Bloc-6];
}   Enreg;
//Structure représentant l'en-tête du fichier avec des informations telles que l'adresse du dernier bloc, le nombre d'enregistrements, l'indice libre et le nombre d'enregistrements supprimés.
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
// ouverture de fichier sans dependance de GTK
TOVC *ouvrir(char *filename,char mod) // mod = 'A' ancien (rb+) || mod = 'N' nouveau (wb+)
{
    TOVC *I = malloc(sizeof(TOVC));
    char s[3];
    if ((mod == 'A') || (mod =='a')) sprintf(s,"rb+");
    else sprintf(s,"wb+");
    I->F=fopen(filename,s);
    if ((mod == 'A') || (mod =='a'))
    {
        fread(&(I->entete),sizeof(Entete),1,I->F);
    }
    else
    {
        (I->entete).adr_dernier_bloc=0;
        (I->entete).nbr_enreg=0;
        (I->entete).indice_libre=0;
        (I->entete).nb_sup=0;
        fwrite(&(I->entete),sizeof(Entete),1,I->F);
    }
    return I;
}
// fermer un fichier TOVC en sauvegardant les informations d'en-tête dans le fichier et en fermant le pointeur de fichier associé
void fermer(TOVC * pF)
{

    fseek(pF->F,0,0);
    fwrite(&(pF->entete),sizeof(Entete),1,pF->F);
    fclose(pF->F);

}
//une fonction d'accès aux différentes informations d'en-tête stockées
int entete(TOVC *pF,int i)
{
    if (i==1) return ((pF->entete).adr_dernier_bloc);
    if (i==2) return ((pF->entete).nbr_enreg);
    if (i==4) return ((pF->entete).nb_sup);
    return ((pF->entete).indice_libre);
}

void aff_entete(TOVC *pF,int i,int val)
{
    if (i==1) (pF->entete).adr_dernier_bloc=val;
    else if (i==2) (pF->entete).nbr_enreg=val;
    else if (i==4) (pF->entete).nb_sup=val;
    else (pF->entete).indice_libre=val;
}

void liredir(TOVC *pF,int i,Buffer *buf)
{
    if (i<=entete(pF,1))
    {
        fseek(pF->F,sizeof(Entete)+(i-1)*sizeof(Tbloc),SEEK_SET);
        fread(buf,sizeof(Tbloc),1,pF->F);
    }
}

void ecriredir(TOVC *pF,int i,Buffer buf)
{
    if (i<=entete(pF,1))
    {
        fseek(pF->F,sizeof(Entete)+(i-1)*sizeof(Tbloc),SEEK_SET);
        fwrite(&buf,sizeof(Tbloc),1,pF->F);
    }
}

int alloc_bloc(TOVC *pF)
{
    aff_entete(pF,1,entete(pF,1)+1);
    return entete(pF,1);
}

void NumtoS(int Num,int max,char* S)
{
    int p10=1;
    int i=0;
    for (i=1;i<=max;i++) p10*=10;
    Num%=p10;
    char inter[max];
    sprintf(S,"");
    sprintf(inter,"%d",Num);
    int j=max-strlen(inter);
    while (j>0)
    {
        sprintf(S,"%s0",S);
        j--;
    }
    sprintf(S,"%s%s",S,inter);
}

void sub_string(char *S,int i,int k, char *R)
{
    if (i<0) i=0;
    sprintf(R,"");
    while (i<strlen(S) && k>0)
    {
        sprintf(R,"%s%c",R,S[i]);
        i++;
        k--;
    }
}

void Sup_inter_string(char *S,int i,int k)
{
    //adnane => Ane avec i=1 k =3
    char inter1[Taille_Bloc+1];
    char inter2[Taille_Bloc+1];
    sub_string(S,0,i,inter1);
    sub_string(S,i+k,strlen(S),inter2);
    sprintf(S,"%s%s",inter1,inter2);
}

void EnregtoSemi(Enreg E,semi_enreg SE)
{
    int taille=strlen(E.info);
    char inter[3];
    NumtoS(taille,3,inter);
    sprintf(SE,"%s",inter);
    NumtoS(E.cle,3,inter);
    sprintf(SE,"%s%s",SE,inter);
    if (E.sup) sprintf(SE,"%s1%s",SE,E.info); else sprintf(SE,"%s0%s",SE,E.info);
}

void SemitoEnreg(semi_enreg SE,Enreg *E)
{
    char inter[Taille_Bloc+1];
    sprintf(inter,"");
    sub_string(SE,3,3,inter);
    E->cle=atoi(inter);
    sprintf(inter,"%c",SE[6]);
    E->sup=atoi(inter);
    sub_string(SE,7,strlen(SE)-7,E->info);
}
/*La fonction construit le semi-enregistrement en ajoutant la taille convertie en une chaîne de caractères.
Elle extrait ensuite une sous-chaîne de taille+4 caractères (inter) à partir de l'indice *j.
Si la longueur de la sous-chaîne est égale à la taille+4, elle est ajoutée au semi-enregistrement, et *j est incrémenté de taille+4.
Sinon, la lecture continue sur le bloc suivant, et la partie manquante est lue à partir de l'indice 0 du nouveau bloc*/
void recupsemi_enreg(TOVC *pF,semi_enreg SE,int *i,int *j) 
{
    int taille;
    char inter[Taille_Bloc+1],inter2[Taille_Bloc+1];
    Buffer buf;
    liredir(pF,*i,&buf);
    sub_string(buf.chaine,*j,3,inter);
    if (strlen(inter)==3)
    {
        taille=atoi(inter);
        (*j)+=3;
    }
    else
    {
        (*i)++;
        (*j)=0;
        liredir(pF,*i,&buf);
        sub_string(buf.chaine,*j,3-strlen(inter),inter2);
        sprintf(inter,"%s%s",inter,inter2);
        taille=atoi(inter);
        (*j)+=strlen(inter2);
    }
    sprintf(SE,"%s",inter);
    sub_string(buf.chaine,*j,taille+4,inter);
    if (strlen(inter)==taille+4)
    {
        sprintf(SE,"%s%s",SE,inter);
        (*j)+=taille+4;
    }
    else
    {
        (*i)++;
        (*j)=0;
        liredir(pF,*i,&buf);
        sub_string(buf.chaine,*j,taille+4-strlen(inter),inter2);
        sprintf(inter,"%s%s",inter,inter2);
        sprintf(SE,"%s%s",SE,inter);
        (*j)+=strlen(inter2);
    }
}

//La fonction vise à afficher une représentation détaillée des enregistrements stockés dans le fichier, y compris leur clé, leur statut de suppression et des informations supplémentaires. Elle gère les cas où un enregistrement s'étend sur plusieurs blocs
void affich_TOVC(TOVC * pF)
{
    int i=1,i1=1,j=0,j1=0;
    Enreg E;
    semi_enreg SE;
    printf("-------------------------------   ENTETE : %d   %d   %d   %d   -------------------------------\n",entete(pF,1),entete(pF,2),entete(pF,3),entete(pF,4));
    while (i<=entete(pF,1))
    {
        recupsemi_enreg(pF,SE,&i1,&j1);
        SemitoEnreg(SE,&E);
        printf("-------------------------------   %d|%d|%s",E.cle,E.sup,E.info);
        if (i==i1) printf(" Dans le Bloc %d   -------------------------------\n",i);
        else printf(" commence du bloc %d et chevauche le bloc %d   -------------------------------\n",i,i1);
        if (j1==Taille_Bloc) {i1++;j1=0;}
        i=i1;j=j1;
        if ((i==entete(pF,1)) && j==entete(pF,3)) break;
    }
}


//La fonction a pour objectif de déterminer la position d'un enregistrement avec une clé spécifique dans le fichier. Si l'enregistrement est trouvé, les variables i et j sont mises à jour pour indiquer la position de l'enregistrement dans le fichier, et la variable trouv est définie à 1.
void Recherche_TOVC(TOVC *pF,int cle,int *i,int *j,int *trouv)
{
    (*trouv)=0;
    (*i)=1;
    (*j)=0;
    int i1=1;
    int j1=0;
    semi_enreg SE;
    Enreg E;
    while (!(*trouv) && (*i)<=entete(pF,1))
    {
        recupsemi_enreg(pF,SE,&i1,&j1);
        SemitoEnreg(SE,&E);
        if (!E.sup && E.cle>=cle) break;
        (*i)=i1;
        (*j)=j1;
        if (((*i)==entete(pF,1)) && (*j)==entete(pF,3)) break;
    }
    if ((entete(pF,1)>0) && ((!E.sup) && (E.cle==cle))) (*trouv)=1;
}
//Cette fonction semble être conçue pour insérer un semi-enregistrement dans un fichier tout en gérant les cas où l'insertion chevauche plusieurs blocs. La récursivité est utilisée pour gérer les parties du semi-enregistrement qui débordent dans des blocs suivants. La fonction prend en compte la mise à jour de l'entête du dernier bloc si l'insertion se fait dans le dernier bloc existant
void insertion_pos_rec(TOVC *pF,int *i,int *j,semi_enreg SE)
{

    //SI Semi enregistrement
    Buffer buf;
    semi_enreg inter1,inter2;
    semi_enreg SI;
    if ((*i)<=entete(pF,1))
    {
        liredir(pF,(*i),&buf);
        sub_string(SE,0,Taille_Bloc-(*j),inter1);
        sub_string(SE,strlen(inter1),strlen(SE),inter2);
        sub_string(buf.chaine,(*j),Taille_Bloc,SI);
        buf.chaine[*j]='\0';
        sprintf(buf.chaine,"%s%s",buf.chaine,inter1);
        (*j)+=strlen(inter1);
        ecriredir(pF,(*i),buf);
        if ((*i)==entete(pF,1)) aff_entete(pF,3,*j);
        if (strlen(inter2)!=0)
        {
            (*i)+=1;
            (*j)=0;
            insertion_pos_rec(pF,i,j,inter2);
        }
        if (strlen(SI)!=0) insertion_pos_rec(pF,i,j,SI);
    }
    else
    {
        alloc_bloc(pF);
        sprintf(buf.chaine,"%s",SE);
        aff_entete(pF,3,strlen(SE));
        (*j)=strlen(SE);
        ecriredir(pF,(*i),buf);
    }
}
//Cette fonction sert à insérer un nouvel enregistrement dans le fichier géré par TOVC. Elle utilise la recherche pour déterminer où insérer le nouvel enregistrement en fonction de sa clé. Si l'enregistrement avec la même clé existe déjà, l'insertion est évitée
void insertion_TOVC(TOVC *pF,Enreg E)
{
    int i,j,trouv;
    Buffer Buf;
    semi_enreg SE;
    Recherche_TOVC(pF,E.cle,&i,&j,&trouv);
    if (!trouv)
    {

        EnregtoSemi(E,SE);
        insertion_pos_rec(pF,&i,&j,SE);
        aff_entete(pF,2,entete(pF,2)+1);
    }


}


//Cette fonction sert à marquer un enregistrement comme supprimé dans un fichier géré par TOVC. Elle utilise la recherche pour localiser l'enregistrement avec la clé spécifiée, puis met à jour le marqueur de suppression et les statistiques d'entête.
void suppression_TOVC(TOVC *pF,int cle)
{
    int i,j,trouv;
    Buffer Buf;
    Recherche_TOVC(pF,cle,&i,&j,&trouv);
    if (trouv)
    {
        if ((j+6)<Taille_Bloc)
        {
            liredir(pF,i,&Buf);
            Buf.chaine[j+6]='1';
            ecriredir(pF,i,Buf);
        }
        else
        {
            liredir(pF,i+1,&Buf);
            Buf.chaine[(j+6)%Taille_Bloc]='1';
            ecriredir(pF,i+1,Buf);
        }
        aff_entete(pF,2,entete(pF,2)-1);
        aff_entete(pF,4,entete(pF,4)+1);
    }
}

void Suppression_phisique_TOVC(TOVC *pF, int cle) {
    int i, i1, j, j1, k, trouv;
    semi_enreg inter1, inter2;
    semi_enreg SE;
    int stop = 0;
    Buffer Buf;
    Recherche_TOVC(pF, cle, &i, &j, &trouv);
    if (trouv) {
        i1 = i;
        j1 = j;
        recupsemi_enreg(pF, SE, &i1, &j1);

        while (!stop) {
            recupsemi_enreg(pF, SE, &i1, &j1);
            sub_string(SE, 0, Taille_Bloc - j, inter1);
            sub_string(SE, strlen(inter1), strlen(SE), inter2);
            liredir(pF, i, &Buf);
            Buf.chaine[j] = '\0';
            sprintf(Buf.chaine, "%s%s", Buf.chaine, inter1);
            j += strlen(inter1);
            ecriredir(pF, i, Buf);
            if (strlen(inter2) != 0) {
                i = i1;
                j = 0;  // Reset j to 0 as we move to the next block
                liredir(pF, i, &Buf);
                strcpy(Buf.chaine, inter2);  // Copy inter2 to the beginning of Buf.chaine
                ecriredir(pF, i, Buf);
            }

            // Update stop condition to check if i1 and j1 reached the end
            stop = (i1 > entete(pF, 1)) || ((i1 == entete(pF, 1)) && (j1 >= entete(pF, 3)));
        }
        aff_entete(pF,1,i);
        aff_entete(pF,3,j);
        aff_entete(pF,2,entete(pF,2)-1);
    }
}




#endif