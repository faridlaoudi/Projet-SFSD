#include <stdio.h>
#include <stdlib.h>

int main() {
    int eng,champ;
    FILE* file;
    file = fopen("creation","w+");
    fprintf(file, "Some text");
    
    printf("entrer le nombre des enregistrement\n");
    do{
        scanf("%d",&eng);
    }while(eng<1);
    printf("entrer le nombre des champs\n");
    do{
        scanf("%d",&champ);
    }while(champ<1);
    printf("nb enreg :%d\n",eng);
    printf("nb des champs :%d\n",champ);
    fclose(file);

   return 0;
}
