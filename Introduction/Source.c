#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

void main(void) {
    char answer[10];

    printf("Lire ou ecrire ? ");
    gets_s(answer, 10);

    if (strcmp(answer, "ecrire") == 0) {
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
    } else {
        FILE* afile = fopen("data_subjects_info.csv", "r");
    
        if (afile == NULL) {
            perror("Unable to open the file");
            exit(1);
        }

        char line[200];

        while (fgets(line, sizeof(line), afile)) {
            char* token;
            token = strtok(line, ",");
            while (token != NULL) {
                printf("%-10s", token);
                token = strtok(NULL, ",");
            }
            printf("\n");
        }

        fclose(afile);
    }
}
