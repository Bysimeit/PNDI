#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define LG_PATH 50
#define LG_LINE 10000
#define NB_COL 600
#define NB_MOUVEMENT 6

typedef struct data Data;

struct data {
    double moyenne[NB_COL];
    double ecartType[NB_COL];
    double moyenneGen;
};

void readTrainSet(char path[LG_PATH], Data data[NB_MOUVEMENT], int nbLines[NB_MOUVEMENT]);
void createFile(char path[LG_PATH], Data data[NB_MOUVEMENT], int nbLines[NB_MOUVEMENT]);

void main(void) {
    Data data[NB_MOUVEMENT];
    char path[LG_PATH] = "trainSet.csv";

    for (int iMouvement = 0; iMouvement < NB_MOUVEMENT; iMouvement++) {
        for (int iCol = 0; iCol < NB_COL; iCol++) {
            data[iMouvement].moyenne[iCol] = 0;
            data[iMouvement].ecartType[iCol] = 0;
        }
        data[iMouvement].moyenneGen = 0;
    }

    int nbLines[NB_MOUVEMENT];
    for (int iMouvement = 0; iMouvement < NB_MOUVEMENT; iMouvement++) {
        nbLines[iMouvement] = 0;
    }

    readTrainSet(path, data, nbLines);

    char createPath[LG_PATH] = "fiModel.csv";
    createFile(createPath, data, nbLines);
}

void readTrainSet(char path[LG_PATH], Data data[NB_MOUVEMENT], int nbLines[NB_MOUVEMENT]) {
    FILE* aFile = fopen(path, "r");

    if (aFile == NULL) {
        perror("Unable to open the file");
        exit(1);
    }

    char* token;
    char line[LG_LINE];

    double value;
    int compareMouvement;
    int iMouvement = 1;

    fgets(line, sizeof(line), aFile);
    while (fgets(line, sizeof(line), aFile)) {
        token = strtok(line, ",");
        int iCol = 0;
        while (token != NULL) {
            if (iCol == 0) {
                sscanf(token, "%d", &compareMouvement);
                if (compareMouvement != iMouvement) {
                    iMouvement = compareMouvement;
                }
                token = strtok(NULL, ",");
                iCol++;
            }
            if (iCol < 3) {
                token = strtok(NULL, ",");
                iCol++;
            } else {
                sscanf(token, "%lf", &value);
                data[iMouvement - 1].moyenne[iCol - 3] += value;
                data[iMouvement - 1].ecartType[iCol - 3] += pow(value, 2);
                token = strtok(NULL, ",");
                iCol++;
            }
        }
        nbLines[iMouvement - 1]++;
    }
    fclose(aFile);
}

void createFile(char path[LG_PATH], Data data[NB_MOUVEMENT], int nbLines[NB_MOUVEMENT]) {
    FILE* writeFile = fopen(path, "w");

    if (writeFile == NULL) {
        perror("Unable to open the file");
        exit(1);
    }

    fprintf(writeFile, "%s", "Mouvement");
    for (int i = 0; i < NB_COL; i++) {
        fprintf(writeFile, "%s", ",Vacc");
    }
    fprintf(writeFile, "%s", "\n");

    for (int iMouvement = 0; iMouvement < NB_MOUVEMENT; iMouvement++) {
        double totGen = 0;
        fprintf(writeFile, "%d", iMouvement + 1);
        for (int iCol = 0; iCol < NB_COL; iCol++) {
            fprintf(writeFile, ",%lf", data[iMouvement].moyenne[iCol] / nbLines[iMouvement]);
            totGen += data[iMouvement].moyenne[iCol];
        }
        fprintf(writeFile, "%s", "\n");

        fprintf(writeFile, "%d", iMouvement + 1);
        for (int iCol = 0; iCol < NB_COL; iCol++) {
            fprintf(writeFile, ",%lf", sqrt(data[iMouvement].ecartType[iCol] / nbLines[iMouvement] - pow(data[iMouvement].moyenne[iCol] / nbLines[iMouvement], 2)));
        }
        fprintf(writeFile, "%s", "\n");

        fprintf(writeFile, "%d", iMouvement + 1);
        fprintf(writeFile, ",%lf", totGen / (nbLines[iMouvement] * NB_COL));
        fprintf(writeFile, "%s", "\n");
    }
    fclose(writeFile);
}