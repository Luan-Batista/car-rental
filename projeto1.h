#include <stdio.h>
#include <stdlib.h>

#define licensePlateSize 8
#define modelSize 20
#define brandSize 20
#define categorySize 15
#define statusSize 16

typedef struct vehicle{
    char licensePlate[licensePlateSize];
    char model[modelSize];
    char brand[brandSize];
    int year;
    char category[categorySize];
    int km;
    char status[statusSize];
}VEHICLE;

int readFile(FILE *fp);