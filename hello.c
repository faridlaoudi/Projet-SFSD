#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE* file;
    file = fopen("creation","w+");
    fprintf(file, "Some text");
    fclose(file);

   return 0;
}
