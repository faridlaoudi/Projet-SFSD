#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

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
    FILE *file = fopen(filename, "a+");
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
    FILE *file = fopen(filename, "r+");
    if (!file) {
        perror("Error opening file");
        return;
    }

    champ champ;
    printf("The Data of %s :\n",filename);
    while (fread(&champ, sizeof(champ), 1, file)) {
        if (champ.type == INT) {
            printf("Your ID: %d\n", champ.value.intValue);
        } else if (champ.type == FLOAT) {
            printf("Your result is : %f\n", champ.value.floatValue);
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
        printf("enregistrement %d, champ %d\n", *engCount + 1, j + 1);
        printf("Enter champ type (0 for ID, 1 for Result, 2 for name): ");
        scanf("%d", (int *)&((*engTableau)[*engCount].champs[j].type));

        if ((*engTableau)[*engCount].champs[j].type == INT) {
            printf("Enter your id: ");
            scanf("%d", &((*engTableau)[*engCount].champs[j].value.intValue));
        } else if ((*engTableau)[*engCount].champs[j].type == FLOAT) {
            printf("Enter Your resut: ");
            scanf("%f", &((*engTableau)[*engCount].champs[j].value.floatValue));
        } else if ((*engTableau)[*engCount].champs[j].type == STRING) {
            printf("Enter your name: ");
            scanf("%49s", (*engTableau)[*engCount].champs[j].value.stringValue);
        }
    }

    (*engCount)++;
}

void deleteStruct(eng **engTableau, int *engCount, int index) {
    free((*engTableau)[index].champs);

    for (int i = index; i < *engCount - 1; i++) {
        (*engTableau)[i] = (*engTableau)[i + 1];
    }

    *engTableau = realloc(*engTableau, (*engCount - 1) * sizeof(eng));
    (*engCount)--;
}

int main() {
    int engCount = 0, nbchamp = 0, deleteIndex, choice1, choice2, chfile;
    eng *engTableau = NULL;
    char filename[50];
    int fileOpened = 0;  //check if a file is opened
    do{
        do {
        chfile=0;
        printf("\nMenu:\n");
        printf("1. Open an existing file\n");
        printf("2. Create a new file\n");
        printf("3. Delete file\n");
        printf("4. Exit\n");
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
                    FILE *file = fopen(filename, "w+"); // Create an empty file
                    if (file == NULL) {
                        perror("Error creating file");
                    } else {
                        fclose(file);
                        printf("New file created.\n");
                    }
                    printf("Enter the number of fields in a record: ");
                    scanf("%d", &nbchamp);
                    fileOpened = 1;
                }
                break;

            case 3: // delete file
                printf("Enter the name of the file to delete: \n");
                    listFiles(".", ".txt");
                    scanf("%s", filename);
                    remove(filename);
                    fileOpened = 0;
                break;

            case 4: // Exit
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }
        }while(!fileOpened);
        do {
        printf("you are in %s \n1. Insert new enregistremetn\n2. View Data\n3. Delete an enregistrement\n4. Search\n5. Change the current file\n6. Exit\n",filename);
        printf("entre you choice :");
        scanf("%d", &choice2);

        switch (choice2) {
            case 1: // Insert new record
                if (!fileOpened) {
                    printf("No file is open. Please open or create a file first.\n");
                } else {
                    addStruct(&engTableau, &engCount, nbchamp);
                    writeFichier(engTableau, engCount, nbchamp, filename);
                }
                break;

            case 2: // View Data
                if (!fileOpened) {
                    printf("No file is open. Please open or create a file first.\n");
                } else {
                    readFromFile(filename);
                }
                break;

            case 3: // Delete a record
                if (!fileOpened) {
                    printf("No file is open. Please open or create a file first.\n");
                } else {
                     printf("Enter number of structure to delete: ");
                    scanf("%d", &deleteIndex);
                    if (deleteIndex >= 0 && deleteIndex < engCount) {
                    deleteStruct(&engTableau, &engCount, deleteIndex);
                    writeFichier(engTableau, engCount, nbchamp, filename);
                    } else {
                    printf("Invalid index\n");
                    }
                }
                break;

            case 4:
                //search with index or wiht our values
            
                break;
            case 5:
                //change file
                FILE *file = fopen(filename, "a+");
                fclose(file);
                fileOpened=0;
                chfile=1;
                break;
            case 6:
                printf("Good bye\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
            }
        }while (choice2 != 6 && choice2 !=5);
    }while(chfile==1 && choice2 != 6 && choice1 != 4);

    // Free memory and clean up
    for (int i = 0; i < engCount; i++) {
        free(engTableau[i].champs);
    }
    free(engTableau);

    return 0;
}
