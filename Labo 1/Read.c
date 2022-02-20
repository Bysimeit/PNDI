#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
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
}
