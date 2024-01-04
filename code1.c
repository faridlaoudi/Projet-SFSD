#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    INT,
    FLOAT,
    STRING
} champType;

typedef union {
    int intValue;
    float floatValue;
    char stringValue[50];
} champValue;

typedef struct {
    champType type;
    champValue value;
} champ;

typedef struct {
    champ *champs;
    int nbchamp;
} eng;

int main() {
    int engCount = 0, nbchamp, choice;
    eng *engTableau = NULL;

    printf("Enter nombre des champs en enregistrement :");
    scanf("%d", &nbchamp);

    do {
        printf("\n1. Insert new enregistremetn\n2. View Data\n3. Delete an enregistrement\n4. Search\n5. Exit\n");
        printf("entre you choice :");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                //function insertion 
                break;
            case 2:
                //read ml file after stocker
                break;
            case 3:
                //search with index and delete
            case 4:
                //search with index or wiht our values
            case 5:
                printf("Good bye\n");
                break;
            default:
                printf("invalid choice choose another choice :\n");
        }
    } while (choice != 5);

    for (int i = 0; i < engCount; i++) {
        free(engTableau[i].champs);
    }
    free(engTableau);

    return 0;
}
