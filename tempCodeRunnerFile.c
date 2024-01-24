
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "functions.h"

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
         printf("--- you are in %s \n1. Insert new enregistrement\n2. View Data\n3. Delete an enregistrement\n4. Search\n5. Display header information\n6. Back\n0. Exit\n",filename);
         printf("entre you choice :");
         scanf("%d", &choice2);

            switch (choice2) {
                case 1: // Insert new record
                if (!fileOpened) {
                    printf("No file is open. Please open or create a file first.\n");
                } else {
                    printf("Enter the key (entier) of the new record: ");
                    scanf("%d", &E.cle);
                    printf("Enter the information of the new record: ");
                    scanf("%s", &E.info);
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
                    printf("Enter the key of the record to delete: ");
                    scanf("%d", &deleteKey);
                    Suppression_phisique_TOVC(filepointer,deleteKey);
                    printf("Done\n");
                }
                break;
                case 4:
                    int i,j,trouv;
                    printf("Enter the key to search for: ");
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
    //need to fix the delete function

    return 0;
}