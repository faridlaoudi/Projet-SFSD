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
    int engCount = 0, nbchamp, choice, deleteIndex, searchIndex, searchValue;
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
                addStruct(&engTableau, &engCount, nbchamp);
                writeFichier(engTableau, engCount, nbchamp, "donner.txt");
                break;
            case 2:
                //read ml file after stocker
                readFromFile("donner.txt");
                break;
            case 3:
                //search with index and delete
                printf("Enter number of structure to delete: ");
                scanf("%d", &deleteIndex);
                if (deleteIndex >= 0 && deleteIndex < engCount) {
                    deleteStruct(&engTableau, &engCount, deleteIndex);
                    writeFichier(engTableau, engCount, nbchamp, "donner.txt");
                } else {
                    printf("Invalid index\n");
                }
                break;
            case 4:
                //search with index or wiht our values
                break;
            case 5:
                printf("Good bye\n");
                break;
            default:
                printf("invalid choice choose another one :\n");
        }
    } while (choice != 5);

    for (int i = 0; i < engCount; i++) {
        free(engTableau[i].champs);
    }
    free(engTableau);

    return 0;
}