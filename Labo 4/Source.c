#define _CRT_SECURE_NO_WARNINGS

#include "Phase2.h"

typedef struct model Model;
struct model {
    int type;
    double averages[NB_COL];
    double stds[NB_COL];
    double globalAvg;
};

void readModel(Model models[NB_MOVMENT], char path[LG_PATH]);
int decomposition(char line[], double data[]);
double calculDistanceEuclidienneAverage(double data[], Model models[], int iMovment);
double calculDistanceEuclidienneStds(double data[], Model models[], int iMovment);
double compareLowValue(double result, int indicators[NB_INDICATOR], int cell, int iMovment, double minStd);

void main(void) {
    Model models[NB_MOVMENT];
    char pathModel[LG_PATH] = "fiModel.csv";

    readModel(models, pathModel);

    FILE* fiSet = fopen("testSet.csv", "r");

    if (fiSet == NULL) {
        perror("Unable to open the file !");
        exit(0);
    }
    else {
        int nbLine = 0;
        char line[LG_LINE];
        double data[NB_COL + 3];
        int realClasses[NB_DEPLACEMENT];
        int estimatedClasses[NB_DEPLACEMENT];
        double resultAverage = 0;
        double resultStds = 0;
        int indicators[NB_INDICATOR];
        fgets(line, sizeof(line), fiSet);
        while (fgets(line, sizeof(line), fiSet)) {
            int numMovment = decomposition(line, data);
            realClasses[nbLine] = numMovment;

            double moyenneGen = 0;
            for (int i = 3; i < NB_COL; i++) {
                moyenneGen += data[i];
            }
            moyenneGen = moyenneGen / NB_COL;

            double minStd = DBL_MAX;
            double minAverage = DBL_MAX;
            double minMoyenneGen = DBL_MAX;
            for (int iMovment = 0; iMovment < NB_MOVMENT; iMovment++) {
                resultAverage = calculDistanceEuclidienneAverage(data, models, iMovment);
                minAverage = compareLowValue(resultAverage, indicators, 0, iMovment, minAverage);
                resultStds = calculDistanceEuclidienneStds(data, models, iMovment);
                minStd = compareLowValue(resultStds, indicators, 1, iMovment, minStd);

                double relayMoyenne;
                if (models[iMovment].globalAvg > moyenneGen) {
                    relayMoyenne = models[iMovment].globalAvg - moyenneGen;
                } else {
                    relayMoyenne = moyenneGen - models[iMovment].globalAvg;
                }

                if (relayMoyenne < minMoyenneGen) {
                    minMoyenneGen = relayMoyenne;
                    indicators[2] = iMovment + 1;
                }
            }

            if (indicators[0] == indicators[1] || indicators[0] == indicators[2]) {
                estimatedClasses[nbLine] = indicators[0];
            }
            else {
                if (indicators[1] == indicators[2]) {
                    estimatedClasses[nbLine] = indicators[1];
                }
                else {
                    estimatedClasses[nbLine] = indicators[0];
                }
            }
            nbLine++;
        }

        displayResultsByClass(realClasses, estimatedClasses, NB_DEPLACEMENT);
        displayAccuracy(realClasses, estimatedClasses, NB_DEPLACEMENT);
        displayConfusionMatrix(realClasses, estimatedClasses, NB_DEPLACEMENT);
    }
}

void readModel(Model models[NB_MOVMENT], char path[LG_PATH]) {
    FILE* fiRead = fopen(path, "r");

    if (fiRead == NULL) {
        perror("Unable to open the file !");
        exit(0);
    }
    else {
        char line[LG_LINE];
        double data[NB_COL + 1];

        fgets(line, sizeof(line), fiRead);
        while (fgets(line, sizeof(line), fiRead)) {
            char* token = strtok(line, ",");
            int i = 0;
            while (token != NULL) {
                sscanf(token, "%lf", &data[i]);
                i++;
                token = strtok(NULL, ",");
            }
            int movment = (int)data[0];
            models[movment - 1].type = movment;

            for (i = 1; i < NB_COL; i++) {
                models[movment - 1].averages[i - 1] = data[i];
            }

            fgets(line, sizeof(line), fiRead);
            token = strtok(line, ",");
            i = 0;
            while (token != NULL) {
                sscanf(token, "%lf", &data[i]);
                i++;
                token = strtok(NULL, ",");
            }

            movment = (int)data[0];
            for (i = 1; i < NB_COL; i++) {
                models[movment - 1].stds[i - 1] = data[i];
            }

            fgets(line, sizeof(line), fiRead);
            token = strtok(line, ",");
            i = 0;
            while (token != NULL) {
                sscanf(token, "%lf", &data[i]);
                i++;
                token = strtok(NULL, ",");
            }

            movment = (int)data[0];
            models[movment - 1].globalAvg = data[1];
        }
    }
}

int decomposition(char line[], double data[])
{
    int i = 0;
    char* token = NULL;
    char* nextToken = NULL;
    char s[2] = ",";
    int mvt;
    token = strtok_s(line, s, &nextToken);
    mvt = atoi(line);
    token = strtok_s(NULL, s, &nextToken);
    while (token != NULL)
    {
        data[i] = atof(token);
        i++;
        token = strtok_s(NULL, s, &nextToken);
    }
    return mvt;
}

double calculDistanceEuclidienneAverage(double data[], Model models[], int iMovment) {
    double result = 0;
    int iAverage = 0;
    for (int i = 3; i < 603; i++) {
        result += pow(data[i] - models[iMovment].averages[iAverage], 2);
        iAverage++;
    }

    return sqrt(result);
}

double calculDistanceEuclidienneStds(double data[], Model models[], int iMovment) {
    double std[NB_COL];
    int iStd = 0;
    for (int i = 3; i < NB_COL + 3; i++) {
        std[iStd] = sqrt(pow((data[i] - models[iMovment].averages[iStd]), 2));
        iStd++;
    }

    double result = 0;
    for (int i = 0; i < NB_COL - 1; i++) {
        result += pow(std[i] - models[iMovment].stds[i], 2);
    }
    result = sqrt(result);
    return result;
}

double compareLowValue(double result, int indicators[NB_INDICATOR], int cell, int iMovment, double minValue) {
    if (result < minValue) {
        minValue = result;
        indicators[cell] = iMovment + 1;
    }

    return minValue;
}