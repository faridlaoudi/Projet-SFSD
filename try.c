
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void fermer(TOVC * filepointer)
{

    fseek(filepointer->F,0,0);
    fwrite(&(filepointer->entete),sizeof(Entete),1,filepointer->F);
    fclose(filepointer->F);

}

int entete(TOVC *filepointer,int i)
{
    if (i==1) return ((filepointer->entete).adr_dernier_bloc);
    if (i==2) return ((filepointer->entete).nbr_enreg);
    if (i==4) return ((filepointer->entete).nb_sup);
    return ((filepointer->entete).indice_libre);
}

void aff_entete(TOVC *filepointer,int i,int val)
{
    if (i==1) (filepointer->entete).adr_dernier_bloc=val;
    else if (i==2) (filepointer->entete).nbr_enreg=val;
    else if (i==4) (filepointer->entete).nb_sup=val;
    else (filepointer->entete).indice_libre=val;
}

void liredir(TOVC *filepointer,int i,Buffer *buf)
{
    if (i<=entete(filepointer,1))
    {
        fseek(filepointer->F,sizeof(Entete)+(i-1)*sizeof(Tbloc),SEEK_SET);
        fread(buf,sizeof(Tbloc),1,filepointer->F);
    }
}

void ecriredir(TOVC *filepointer,int i,Buffer buf)
{
    if (i<=entete(filepointer,1))
    {
        fseek(filepointer->F,sizeof(Entete)+(i-1)*sizeof(Tbloc),SEEK_SET);
        fwrite(&buf,sizeof(Tbloc),1,filepointer->F);
    }
}

int alloc_bloc(TOVC *filepointer)
{
    aff_entete(filepointer,1,entete(filepointer,1)+1);
    return entete(filepointer,1);
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

void recupsemi_enreg(TOVC *filepointer,semi_enreg SE,int *i,int *j) //a modifier ! comme hidouci
{
    int taille;
    char inter[Taille_Bloc+1],inter2[Taille_Bloc+1];
    Buffer buf;
    liredir(filepointer,*i,&buf);
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
        liredir(filepointer,*i,&buf);
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
        liredir(filepointer,*i,&buf);
        sub_string(buf.chaine,*j,taille+4-strlen(inter),inter2);
        sprintf(inter,"%s%s",inter,inter2);
        sprintf(SE,"%s%s",SE,inter);
        (*j)+=strlen(inter2);
    }
}


void affich_TOVC(TOVC * filepointer)
{
    int i=1,i1=1,j=0,j1=0;
    Enreg E;
    semi_enreg SE;
    printf("ENTETE : %d\t%d\t%d\t%d\n",entete(filepointer,1),entete(filepointer,2),entete(filepointer,3),entete(filepointer,4));
    while (i<=entete(filepointer,1))
    {
        recupsemi_enreg(filepointer,SE,&i1,&j1);
        SemitoEnreg(SE,&E);
        printf("%d|%d|%s",E.cle,E.sup,E.info);
        if (i==i1) printf(" Dans le Bloc %d\n",i);
        else printf(" commence du bloc %d et chevauche le bloc %d\n",i,i1);
        if (j1==Taille_Bloc) {i1++;j1=0;}
        i=i1;j=j1;
        if ((i==entete(filepointer,1)) && j==entete(filepointer,3)) break;
    }
}



void Recherche_TOVC(TOVC *filepointer,int cle,int *i,int *j,int *trouv)
{
    (*trouv)=0;
    (*i)=1;
    (*j)=0;
    int i1=1;
    int j1=0;
    semi_enreg SE;
    Enreg E;
    while (!(*trouv) && (*i)<=entete(filepointer,1))
    {
        recupsemi_enreg(filepointer,SE,&i1,&j1);
        SemitoEnreg(SE,&E);
        if (!E.sup && E.cle>=cle) break;
        (*i)=i1;
        (*j)=j1;
        if (((*i)==entete(filepointer,1)) && (*j)==entete(filepointer,3)) break;
    }
    if ((entete(filepointer,1)>0) && ((!E.sup) && (E.cle==cle))) (*trouv)=1;
}

void insertion_pos_rec(TOVC *filepointer,int *i,int *j,semi_enreg SE)
{

    //SI Semi enregistrement
    Buffer buf;
    semi_enreg inter1,inter2;
    semi_enreg SI;
    if ((*i)<=entete(filepointer,1))
    {
        liredir(filepointer,(*i),&buf);
        sub_string(SE,0,Taille_Bloc-(*j),inter1);
        sub_string(SE,strlen(inter1),strlen(SE),inter2);
        sub_string(buf.chaine,(*j),Taille_Bloc,SI);
        buf.chaine[*j]='\0';
        sprintf(buf.chaine,"%s%s",buf.chaine,inter1);
        (*j)+=strlen(inter1);
        ecriredir(filepointer,(*i),buf);
        if ((*i)==entete(filepointer,1)) aff_entete(filepointer,3,*j);
        if (strlen(inter2)!=0)
        {
            (*i)+=1;
            (*j)=0;
            insertion_pos_rec(filepointer,i,j,inter2);
        }
        if (strlen(SI)!=0) insertion_pos_rec(filepointer,i,j,SI);
    }
    else
    {
        alloc_bloc(filepointer);
        sprintf(buf.chaine,"%s",SE);
        aff_entete(filepointer,3,strlen(SE));
        (*j)=strlen(SE);
        ecriredir(filepointer,(*i),buf);
    }
}

void insertion_TOVC(TOVC *filepointer,Enreg E)
{
    int i,j,trouv;
    Buffer Buf;
    semi_enreg SE;
    Recherche_TOVC(filepointer,E.cle,&i,&j,&trouv);
    if (!trouv)
    {

        EnregtoSemi(E,SE);
        insertion_pos_rec(filepointer,&i,&j,SE);
        aff_entete(filepointer,2,entete(filepointer,2)+1);
    }


}
void suppression_TOVC(TOVC *filepointer,int cle)
{
    int i,j,trouv;
    Buffer Buf;
    Recherche_TOVC(filepointer,cle,&i,&j,&trouv);
    if (trouv)
    {
        if ((j+6)<Taille_Bloc)
        {
            liredir(filepointer,i,&Buf);
            Buf.chaine[j+6]='1';
            ecriredir(filepointer,i,Buf);
        }
        else
        {
            liredir(filepointer,i+1,&Buf);
            Buf.chaine[(j+6)%Taille_Bloc]='1';
            ecriredir(filepointer,i+1,Buf);
        }
        aff_entete(filepointer,2,entete(filepointer,2)-1);
        aff_entete(filepointer,4,entete(filepointer,4)+1);
    }
}

void Suppression_phisique_TOVC(TOVC *filepointer,int cle)
{
    int i,i1,j,j1,k,trouv;
    semi_enreg inter1,inter2;
    semi_enreg SE;
    int stop=0;
    Buffer Buf;
    Recherche_TOVC(filepointer,cle,&i,&j,&trouv);
    if (trouv)
    {
        i1=i;
        j1=j;
        recupsemi_enreg(filepointer,SE,&i1,&j1);
        while (!stop)
        {
            recupsemi_enreg(filepointer,SE,&i1,&j1);
            sub_string(SE,0,Taille_Bloc-j,inter1);
            sub_string(SE,strlen(inter1),strlen(SE),inter2);
            liredir(filepointer,i,&Buf);
            Buf.chaine[j]='\0';
            sprintf(Buf.chaine,"%s%s",Buf.chaine,inter1);
            j+=strlen(inter1);
            ecriredir(filepointer,i,Buf);
            if (strlen(inter2)!=0)
            {
                i=i1;
                j=strlen(inter2);
                liredir(filepointer,i,&Buf);
                for (k=0;k<j;k++) Buf.chaine[k]=inter2[k];
                ecriredir(filepointer,i,Buf);
            }
            stop=(i1==entete(filepointer,1)) && (j1==entete(filepointer,3));
        }
        aff_entete(filepointer,1,i);
        aff_entete(filepointer,3,j);
        aff_entete(filepointer,2,entete(filepointer,2)-1);

    }
}TOVC* creation(char* file_name, int nb) {
    // Open a new file for writing
    TOVC* filepointer = ouvrir(file_name, 'N');
    if (filepointer == NULL) {
        printf("Error opening the file for writing.\n");
        return NULL;
    }

    Enreg record;
    record.cle=0;
    for (int i = 1; i <= nb; i++) {
        // Get user input for the record data
        printf("Enterer le cle de %d enrefistrement:\n", i);
        scanf("%d",&record.cle);
        printf("les donnees: ");
        scanf("%s", &record.info);

        // Write the record to the file
        int blockNum = alloc_bloc(filepointer);
        if (blockNum == -1) {
            // Handle allocation failure
            printf("Error allocating a new block.\n");
            fermer(filepointer);
            return NULL;
        }

        // Use writeDir function for writing
        ecriredir(filepointer, blockNum, *(Buffer*)&record);
    }

    // Close the file after writing records
    fermer(filepointer);

    // Reopen the file in read mode
    filepointer = ouvrir(file_name, 'A');
    if (filepointer == NULL) {
        // Handle reopening failure
        printf("Error reopening the file for reading.\n");
        return NULL;
    }

    return filepointer;
}

int main()
{
    char filename[20];
    int nbenrg;

    printf("Enter file name: ");
    scanf("%s", filename);

    printf("Enter number of records: ");
    scanf("%d", &nbenrg);

    TOVC *filepointer = creation(filename, nbenrg);

    int choice;
    do
    {
        printf("\nMenu:\n");
        printf("1. Insert a new record\n");
        printf("2. Delete an existing record\n");
        printf("3. Search based on key\n");
        printf("4. Display file contents\n");
        printf("5. Display header information\n");
        printf("0. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            Enreg newRecord;
            newRecord.cle = 0;
            newRecord.sup = 0;

            printf("Enter the key of the new record: ");
            scanf("%d", &newRecord.cle);

            printf("Enter the information of the new record: ");
            scanf("%s", newRecord.info);

            insertion_TOVC(filepointer, newRecord);
            break;
        }
        case 2:
        {
            int deleteKey;
            printf("Enter the key of the record to delete: ");
            scanf("%d", &deleteKey);

            suppression_TOVC(filepointer, deleteKey);
            break;
        }
        case 3:
        {
            int searchKey, i, j, found;
            printf("Enter the key to search for: ");
            scanf("%d", &searchKey);

            Recherche_TOVC(filepointer, searchKey, &i, &j, &found);

            if (found)
                printf("Record with key %d found at block %d, position %d.\n", searchKey, i, j);
            else
                printf("Record with key %d not found.\n", searchKey);

            break;
        }
        case 4:
            affich_TOVC(filepointer);
            break;
        case 5:
            printf("Header Information:\n");
            printf("Number of blocks: %d\n", entete(filepointer, 1));
            printf("Number of records: %d\n", entete(filepointer, 2));
            printf("Free index: %d\n", entete(filepointer, 3));
            printf("Number of deleted records: %d\n", entete(filepointer, 4));
            break;
        case 0:
            // Exit the program
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }

    } while (choice != 0);

    // Close the file before exiting
    fermer(filepointer);

    return 0;
}