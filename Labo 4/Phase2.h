#ifndef Phase2_H
#define Phase2_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>
#define NB_CLASSES 6
#define NB_MOVMENT 6
#define LG_PATH 25
#define NB_COL 600
#define LG_LINE 10000
#define NB_DEPLACEMENT 60
#define NB_INDICATOR 3

#endif

typedef struct classe Class;
struct classe {
	int classNumber;
	int nbGoodClass;
	int nbOccurences;
};


void displayResultsByClass(int realClasses[], int estimatedClasses[], int nbTests);
void initiationClasses(Class classes[]);
void compareRealEstimated(int realClasses[], int estimatedClasses[], int nbTests, Class classes[]);
void displayAccuracy(int realClasses[], int estimatedClasses[], int nbTests);
void displayConfusionMatrix(int realClasses[], int estimatedClasses[], int nbTests);
void createConfusionMatrix(int realClasses[], int estimatedClasses[], int nbTests, int confusionMatrix[][NB_CLASSES]);


/*
void main() {
	int nbTest = 8;
	int realClasses[8] = { 5,2,5,3,1,3,2,4 };
	int estimatedClasses[8] = { 5,5,1,2,1,3,2,4 };

	displayResultsByClass(realClasses, estimatedClasses, nbTest);
	displayAccuracy(realClasses, estimatedClasses, nbTest);
	displayConfusionMatrix(realClasses, estimatedClasses, nbTest);
}
*/

void displayResultsByClass(int realClasses[], int estimatedClasses[], int nbTests) {
	double percentage;
	Class classes[NB_CLASSES];
	initiationClasses(classes);
	compareRealEstimated(realClasses, estimatedClasses, nbTests, classes);

	puts("Numero Classe\t|Bien classes\t|Total\t|Pourcentage");
	puts("________________|_______________|_______|__________");

	for (int iClass = 0; iClass < NB_CLASSES; iClass++) {
		percentage = (double)classes[iClass].nbGoodClass / classes[iClass].nbOccurences * 100;
		printf("%d\t\t|", classes[iClass].classNumber);
		printf("%d\t\t|", classes[iClass].nbGoodClass);
		printf("%d\t|", classes[iClass].nbOccurences);
		printf("%.2f%%\n", percentage);
	}
}

void initiationClasses(Class classes[]) {
	for (int iClass = 0; iClass < NB_CLASSES; iClass++) {
		classes[iClass].classNumber = iClass + 1;
		classes[iClass].nbGoodClass = 0;
		classes[iClass].nbOccurences = 0;
	}
}

void compareRealEstimated(int realClasses[], int estimatedClasses[], int nbTests, Class classes[]) {
	int classIndex;
	for (int iTest = 0; iTest < nbTests; iTest++) {
		classIndex = realClasses[iTest] - 1;
		classes[classIndex].nbOccurences++;
		if (realClasses[iTest] == estimatedClasses[iTest]) {
			classes[classIndex].nbGoodClass++;
		}
	}
}

void displayAccuracy(int realClasses[], int estimatedClasses[], int nbTests) {
	int totalGoodClasses = 0;
	double accuracy;
	for (int iTest = 0; iTest < nbTests; iTest++) {
		if (realClasses[iTest] == estimatedClasses[iTest]) {
			totalGoodClasses++;
		}
	}

	accuracy = (double)totalGoodClasses / nbTests * 100;
	printf("L'accuracy est de %.2f%%\n", accuracy);
}

void displayConfusionMatrix(int realClasses[], int estimatedClasses[], int nbTests) {
	int confusionMatrix[NB_CLASSES][NB_CLASSES];
	createConfusionMatrix(realClasses, estimatedClasses, nbTests, confusionMatrix);

	for (int iLine = 0; iLine < NB_CLASSES; iLine++) {
		for (int iRow = 0; iRow < NB_CLASSES; iRow++) {
			printf("%d\t", confusionMatrix[iLine][iRow]);
		}
		printf("\n");
	}
}

void createConfusionMatrix(int realClasses[], int estimatedClasses[], int nbTests, int confusionMatrix[][NB_CLASSES]) {
	for (int iLine = 0; iLine < NB_CLASSES; iLine++) {
		for (int iRow = 0; iRow < NB_CLASSES; iRow++) {
			confusionMatrix[iLine][iRow] = 0;
		}
	}
	for (int i = 0; i < nbTests; i++)
	{
		confusionMatrix[estimatedClasses[i] - 1][realClasses[i] - 1]++;
	}
}