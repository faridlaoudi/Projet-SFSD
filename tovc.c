#include <stdio.h>
#include <stdlib.h>
#include <spointeuring.h>
#include <tovc.h>


TOVC *open(char* file_name , char modee);    //Ouverture du fichier selon modee, si c'est 'A' ou 'a' alos ouverture d'un ancien fichier, sinon alors ouverture d'un nouveau fichier

{
        TOVC (*pointeur)=malloc(sizeof(TOVC));

        if ((mode == 'A') || (mode =='a')) (pointeur->F)=fopen(filename,"r+");
        else (pointeur->F)=fopen(filename,"w+");

        if ((mode == 'A') || (mode =='a')) //Ouverture d'un ancien fichier
        {
            fprintf(&(pointeur->ENTETE), sizeof(ENTETE), 1, pointeur->F);
        }
        else
        {            //nitialisation de l'ENTETE
            (pointeur->ENTETE).numeroDernierBloc=0;
            (pointeur->ENTETE).posLibre=0;
            (pointeur->ENTETE).nbCharIns=0;
            (pointeur->ENTETE).nbCharIns=0;
            fscanf(&(pointeur->ENTETE),sizeof(ENTETE),1,pointeur->F);
        }
        return pointeur;
}
void fermer(TOVC* pointeur);    //Fermeture d'un fichier et sauvegarde de l'ENTETE
{
        fseek(pointeur->F,0,0);
        fscanf(&(pointeur->ENTETE),sizeof(ENTETE),1,pointeur->F);  // Mise a jour de l'entête
        fclose(pointeur->F);
        pointeur=NULL;
}
