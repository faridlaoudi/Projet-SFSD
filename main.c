
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define Taille_Bloc 30
#define Max_enreg 20
#define b 3


int hasExtension(const char *fileName, const char *extension) {
    const char *dot = strrchr(fileName, '.');
    if (!dot || dot == fileName) return 0;
    return strcmp(dot, extension) == 0;
}

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
    int sup; // bool�en
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

//---------------------------------------------------------------------
//-----DEFENITION DU MODELE--------------------------------------------
//---------------------------------------------------------------------

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

void fermer(TOVC * pF)
{

    fseek(pF->F,0,0);
    fwrite(&(pF->entete),sizeof(Entete),1,pF->F);
    fclose(pF->F);

}

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

void recupsemi_enreg(TOVC *pF,semi_enreg SE,int *i,int *j) //a modifier ! comme hidouci
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


void affich_TOVC(TOVC * pF)
{
    int i=1,i1=1,j=0,j1=0;
    Enreg E;
    semi_enreg SE;
    printf("ENTETE : %d\t%d\t%d\t%d\n",entete(pF,1),entete(pF,2),entete(pF,3),entete(pF,4));
    while (i<=entete(pF,1))
    {
        recupsemi_enreg(pF,SE,&i1,&j1);
        SemitoEnreg(SE,&E);
        printf("%d|%d|%s",E.cle,E.sup,E.info);
        if (i==i1) printf(" Dans le Bloc %d\n",i);
        else printf(" commence du bloc %d et chevauche le bloc %d\n",i,i1);
        if (j1==Taille_Bloc) {i1++;j1=0;}
        i=i1;j=j1;
        if ((i==entete(pF,1)) && j==entete(pF,3)) break;
    }
}



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

void Suppression_phisique_TOVC(TOVC *pF,int cle)
{
    int i,i1,j,j1,k,trouv;
    semi_enreg inter1,inter2;
    semi_enreg SE;
    int stop=0;
    Buffer Buf;
    Recherche_TOVC(pF,cle,&i,&j,&trouv);
    if (trouv)
    {
        i1=i;
        j1=j;
        recupsemi_enreg(pF,SE,&i1,&j1);
        while (!stop)
        {

            recupsemi_enreg(pF,SE,&i1,&j1);
            sub_string(SE,0,Taille_Bloc-j,inter1);
            sub_string(SE,strlen(inter1),strlen(SE),inter2);
            liredir(pF,i,&Buf);
            Buf.chaine[j]='\0';
            sprintf(Buf.chaine,"%s%s",Buf.chaine,inter1);
            j+=strlen(inter1);
            ecriredir(pF,i,Buf);
            if (strlen(inter2)!=0)
            {
                i=i1;
                j=strlen(inter2);
                liredir(pF,i,&Buf);
                for (k=0;k<j;k++) Buf.chaine[k]=inter2[k];
                ecriredir(pF,i,Buf);
            }
            stop=(i1==entete(pF,1)) && (j1==entete(pF,3));
        }
        printf("17\n");
        aff_entete(pF,1,i);
        printf("18\n");
        aff_entete(pF,3,j);
        printf("19\n");
        aff_entete(pF,2,entete(pF,2)-1);
        printf("20\n");

    }
}


int main()
{
    char filename[20];
    int choice1,choice2,chfile,searchKey, found,deleteKey;
    Enreg E;
    E.sup=0;
    int fileOpened = 0;
    do{
        do {
        chfile=0;
        printf("\nMenu:\n");
        printf("1. Open an existing file\n");
        printf("2. Create a new file\n");
        printf("3. Delete file\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice1);

            switch (choice1) {
                case 1: // Open existing file
                    if (fileOpened) {
                    printf("A file is already open. Please close it first.\n");
                    } else {
                    printf("Enter the name of the file to open: \n");
                    listFiles(".", ".txt");
                    scanf("%s", filename);
                    fileOpened = 1;
                    }
                break;
                case 2: // Create new file
                    if (fileOpened) {
                    printf("A file is already open. Please close it first.\n");
                    } else {
                    printf("Enter the name for new file (add .txt): ");
                    scanf("%s", filename);
                    TOVC *filepointer = ouvrir(filename,'N');
                    fileOpened = 1;
                    if (filepointer == NULL) {
                        perror("Error creating file");
                    } else {
                        fclose(filepointer->F);
                        printf("New file created.\n");
                    }
                }
                break;
                case 3: // delete file
                    printf("Enter the name of the file to delete: \n");
                    listFiles(".", ".txt");
                    scanf("%s", filename);
                    remove(filename);
                    fileOpened = 0;
                break;
                case 0: // Exit
                    printf("Exiting...\n");
                break;
                default:
                printf("Invalid choice. Please try again.\n");
            }
        }while(choice1 != 0 && !fileOpened);
        TOVC *filepointer = ouvrir(filename, 'A');
        do {
         printf("you are in %s \n1. Insert new enregistremetn\n2. View Data\n3. Delete an enregistrement\n4. Search\n5. Display header information\n6. Back\n0. Exit\n",filename);
         printf("entre you choice :");
         scanf("%d", &choice2);

            switch (choice2) {
                case 1: // Insert new record
                if (!fileOpened) {
                    printf("No file is open. Please open or create a file first.\n");
                } else {
                    do {
                    printf("Enter the key (entier) of the new record: ");
                    scanf("%d", &E.cle);
                    // Clear the input buffer
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF) { }
                    }while (E.cle<0);

                    printf("Enter the information of the new record: \n");
                    fgets(E.info, sizeof(E.info), stdin); // Replace 'sizeof(E.info)' with the actual size of E.info array
                    int l;
                    while ((l = getchar()) != '\n' && l != EOF) { }
                    insertion_TOVC(filepointer, E);
                    printf("insertion 1 terminee\n");
                }
                break;
                case 2: // View Data
                if (!fileOpened) {
                    printf("No file is open. Please open or create a file first.\n");
                } else {
                    affich_TOVC(filepointer);
                }
                break;
                case 3: // Delete a record
                if (!fileOpened) {
                    printf("No file is open. Please open or create a file first.\n");
                } else {
                    printf("Enter the key of the record to delete: \n");
                    scanf("%d", &deleteKey);
                    Suppression_phisique_TOVC(filepointer,deleteKey);
                    printf("Done\n");
                }
                break;
                case 4:
                    int i,j,trouv;
                    printf("Enter the key to search for: \n");
                    scanf("%d", &searchKey);
                    Recherche_TOVC(filepointer,searchKey,&i,&j,&trouv);
                    if (trouv){
                        printf("Record with key %d found at block %d, position %d.\n", searchKey, i, j);
                    }else{
                        printf("Record with key %d not found.\n", searchKey);
                    } 
                break;
                case 5:
                    printf("Header Information:\n");
                    printf("Number of blocks: %d\n", entete(filepointer, 1));
                    printf("Number of records: %d\n", entete(filepointer, 2));
                    printf("Free index: %d\n", entete(filepointer, 3));
                    printf("Number of deleted records: %d\n", entete(filepointer, 4)); 
                break;
                case 6:
                //change file
                FILE *file = fopen(filename, "a+");
                fclose(file);
                fileOpened=0;
                chfile=1;
                break;
                case 0:
                printf("Good bye\n");
                break;
                
                default:
                printf("Invalid choice. Please try again.\n");
            }
        }while (choice2 !=0 && choice2 !=6);
    }while(choice1 != 0 && choice2 !=0 );

    //i will add function to free the storage
    //fix the exit option
    //fix the data save

    return 0;
}