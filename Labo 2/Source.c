#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define NB_FILE 24
#define NB_FILES_MAX 360

#define NB_FOLDER 15
#define LG_NAME 50

#define LG_VALUE 200
#define LG_PATH 50

bool startFile(char path[LG_PATH], char createFile[LG_NAME], int currentFile, bool firstLine);
double searchAcc(int numLine, char path[LG_PATH], int numCol);
int nbMouvement(char path[LG_PATH]);
double totalVacc(double x, double y, double z);
int searchGender(int numPerson);

void main(void) {
    char paths[NB_FOLDER][LG_NAME] = {
        "dws_1/sub_",
        "dws_2/sub_",
        "dws_11/sub_",
        "jog_9/sub_",
        "jog_16/sub_",
        "sit_5/sub_",
        "sit_13/sub_",
        "std_6/sub_",
        "std_14/sub_",
        "ups_3/sub_",
        "ups_4/sub_",
        "ups_12/sub_",
        "wlk_7/sub_",
        "wlk_8/sub_",
        "wlk_15/sub_"
    };

    int currentFile = 1;

    bool firstLine = false;
    for (int i = 0; i < NB_FOLDER; i++) {
        char path[LG_PATH];
        strcpy_s(path, LG_PATH, paths[i]);

        for (int j = 1; j <= NB_FILE - 2; j++) {
            char pathInter[LG_PATH];

            char charJ[3];
            sprintf(charJ, "%d", j);

            strcpy_s(pathInter, LG_PATH, path);
            strcat(pathInter, charJ);
            strcat(pathInter, ".csv");

            firstLine = startFile(pathInter, "trainSet.csv", currentFile, firstLine);
            currentFile++;
        }
    }
    puts("File trainSet.csv complete.");

    firstLine = false;
    for (int i = 1; i < NB_FOLDER; i++) {
        char path[LG_PATH];
        strcpy_s(path, LG_PATH, paths[i]);

        for (int j = 23; j <= NB_FILE; j++) {
            char pathInter[LG_PATH];

            char charJ[3];
            sprintf(charJ, "%d", j);

            strcpy_s(pathInter, LG_PATH, path);
            strcat(pathInter, charJ);
            strcat(pathInter, ".csv");

            firstLine = startFile(pathInter, "testSet.csv", currentFile, firstLine);
            currentFile++;
        }
    }
    puts("File testSet.csv complete.");
}

bool startFile(char path[LG_PATH], char createFile[LG_NAME], int currentFile, bool firstLine) {
    FILE* writeFile = fopen(createFile, "a+");

    if (writeFile == NULL) {
        perror("Unable to open the file");
        exit(1);
    }

    char line[200];
    int nbLine;

    int gender = searchGender(currentFile);

    if (!firstLine) {
        firstLine = true;
        fprintf(writeFile, "%s", "Mouvement,Genre,Index,Vacc\n");
    }
    nbLine = nbMouvement(path);

    for (int i = 0; i <= nbLine; i++) {
        double accX = searchAcc(i, path, 11);
        double accY = searchAcc(i, path, 12);
        double accZ = searchAcc(i, path, 13);
        
        double calcAcc = totalVacc(accX, accY, accZ);

        fprintf(writeFile, "%d,%d,%d,%f\n", i, gender, currentFile, calcAcc);
    }
    
    fclose(writeFile);

    return firstLine;
}

double searchAcc(int numLine, char path[LG_PATH], int numCol) {
    FILE* afile = fopen(path, "r");

    if (afile == NULL) {
        perror("Unable to open the file");
        exit(1);
    }

    char line[200];
    int indiceAcc = 0;
    double acc;
    bool firstLine = true;

    while (fgets(line, sizeof(line), afile)) {
        char* token;
        token = strtok(line, ",");
        int convert;
        if (!firstLine) {
            sscanf(token, "%d", &convert);
            if (convert == numLine) {
                while (token != NULL) {
                    indiceAcc++;
                    if (indiceAcc == numCol) {
                        sscanf(token, "%lf", &acc);
                    }
                    token = strtok(NULL, ",");
                }
                indiceAcc = 0;
            }
        }
        firstLine = false;
    }

    fclose(afile);

    return acc;
}

int nbMouvement(char path[LG_PATH]) {
    FILE* afile = fopen(path, "r");

    if (afile == NULL) {
        perror("Unable to open the file");
        exit(1);
    }

    char line[200];
    int nbMouvement;
    bool firstLine = true;

    while (fgets(line, sizeof(line), afile)) {
        char* token;
        token = strtok(line, ",");
        if (!firstLine) {
            sscanf(token, "%d", &nbMouvement);
        }
        firstLine = false;
    }

    fclose(afile);

    return nbMouvement;
}

double totalVacc(double x, double y, double z) {
    x = pow(x,2);
    y = pow(y,2);
    z = pow(z,2);
    
    double total = x + y + y;
    
    return sqrt(total);
}

int searchGender(int numPerson) {
    FILE* afile = fopen("data_subjects_info.csv", "r");

    if (afile == NULL) {
        perror("Unable to open the file");
        exit(1);
    }

    char line[200];
    int indiceGender = 0;
    int gender = 0;
    bool firstLine = true;

    while (fgets(line, sizeof(line), afile)) {
        char* token;
        token = strtok(line, ",");
        int convert;
        if (!firstLine) {
            sscanf(token, "%d", &convert);
            if (convert == numPerson) {
                while (token != NULL) {
                    indiceGender++;
                    if (indiceGender == 5) {
                        sscanf(token, "%d", &gender);
                    }
                    token = strtok(NULL, ",");
                }
                indiceGender = 0;
            }
        }
        firstLine = false;
    }
    fclose(afile);

    return gender;
}
