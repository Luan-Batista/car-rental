#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ordem 5
#define MAX_KEYS (ordem - 1)
#define licensePlateSize 8
#define modelSize 20
#define brandSize 20
#define categorySize 15
#define statusSize 16
#define qtdRecords 14
// Variável global para controle do próximo RRN

typedef struct vehicle {
    char licensePlate[licensePlateSize];
    char model[modelSize];
    char brand[brandSize];
    int year;
    char category[categorySize];
    int km;
    char status[statusSize];
} VEHICLE;

typedef struct no Node;

struct no {
   char key[MAX_KEYS][licensePlateSize];
   Node* children[ordem];
   int rrn_page; // Adicionei essa variável para os nós, mas verifique se ela é necessária
   int keyCount;
   int isLeaf;
};

typedef struct {
    Node *root;
} BTree;

// Funções
VEHICLE *readFile(FILE *fp);
Node *createNode();
BTree* createBTree();
void print_node(FILE *fp, Node *node);
void insertNonFull(Node *node, char *key);
void splitChild(Node *parentNode, int index, Node *fullChild);
void insertBTree(BTree *tree, char *key);
void printBTree(Node *node, int level);
void printAllNodes(FILE *fp, Node *node);
void printBTreeToFile(Node *node);
int buscarCarroNaArvore(BTree *tree);
int verificaExistencia(const char *filename);
// Node *carregaRaizBTree(const char *filename);