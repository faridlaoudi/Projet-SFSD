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

void writeFichier(eng *engTableau, int engCount, int nbchamp, const char* filename) {
    FILE *file = fopen(filename, "w+");
    if (!file) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < engCount; i++) {
        for (int j = 0; j < nbchamp; j++) {
            fwrite(&engTableau[i].champs[j], sizeof(champ), 1, file);
        }
    }

    fclose(file);
}

void readFromFile(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    champ champ;
    while (fread(&champ, sizeof(champ), 1, file)) {
        if (champ.type == INT) {
            printf("Your ID: %d\n", champ.value.intValue);
        } else if (champ.type == FLOAT) {
            printf("float is : %f\n", champ.value.floatValue);
        } else if (champ.type == STRING) {
            printf("Your name is : %s\n", champ.value.stringValue);
        }
    }

    fclose(file);
}

void addStruct(eng **engTableau, int *engCount, int nbchamp) {
    *engTableau = realloc(*engTableau, (*engCount + 1) * sizeof(eng));
    (*engTableau)[*engCount].champs = (champ *)malloc(nbchamp * sizeof(champ));
    (*engTableau)[*engCount].nbchamp = nbchamp;

    for (int j = 0; j < nbchamp; j++) {
        printf("Structure %d, champ %d\n", *engCount + 1, j + 1);
        printf("Enter champ type (0 for ID, 1 for la note, 2 for name): ");
        scanf("%d", (int *)&((*engTableau)[*engCount].champs[j].type));

        if ((*engTableau)[*engCount].champs[j].type == INT) {
            printf("Enter your id: ");
            scanf("%d", &((*engTableau)[*engCount].champs[j].value.intValue));
        } else if ((*engTableau)[*engCount].champs[j].type == FLOAT) {
            printf("Enter the note num: ");
            scanf("%f", &((*engTableau)[*engCount].champs[j].value.floatValue));
        } else if ((*engTableau)[*engCount].champs[j].type == STRING) {
            printf("Enter your name: ");
            scanf("%49s", (*engTableau)[*engCount].champs[j].value.stringValue);
        }
    }

    (*engCount)++;
}
<<<<<<< HEAD
void deleteStruct(eng **engTableau, int *engCount, int index) {
    free((*engTableau)[index].champs);

    for (int i = index; i < *engCount - 1; i++) {
        (*engTableau)[i] = (*engTableau)[i + 1];
    }

    *engTableau = realloc(*engTableau, (*engCount - 1) * sizeof(eng));
    (*engCount)--;
}
=======
>>>>>>> main

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