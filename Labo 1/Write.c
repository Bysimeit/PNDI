#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int main() {
    char answer[10];

    FILE* afile = fopen("newdata_subjects_info.csv", "w");

    if (afile == NULL) {
        perror("Unable to open the file");
        exit(1);
    }

    char line[200];
    int numberLine = 1;
    do {
        printf("Ecrivez la %de ligne du fichier :\n", numberLine);
        gets_s(line, 200);
        fprintf(afile,"%s\n", line);
        printf("Avez-vous fini ? (Oui/Non) ");
        gets_s(answer, 10);
        numberLine++;
    } while (strcmp(answer, "Non") == 0);

    fclose(afile);
}