#include <stdio.h>
#include <stdlib.h>
#define ordem 5
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

typedef struct no{
    char keys[ordem - 1];
    int  rrn[ordem - 1];
    struct no *children[ordem + 1];
    int keyCount;
    struct no *parent;
}Node;

typedef struct BTree{
    Node *root;
}BTree;


int readFile(FILE *fp);

