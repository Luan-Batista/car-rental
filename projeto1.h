#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_PLACA 8
#define TAMANHO_MODELO 20
#define TAMANHO_MARCA 20
#define TAMANHO_CATEGORIA 15
#define TAMANHO_STATUS 16
#define qtdRecords 30

#define MAX 5
#define MIN 2


struct BTreeNode {
  char val[MAX + 1][8], count;
  struct BTreeNode *link[MAX + 1];
  int rrn;  // Novo campo para armazenar o RRN do nó
  int rrn_carro;
};

struct BTreeNode *root;
int current_rrn = 1;  // Variável global para controlar o RRN sequencial
struct BTreeNode *createNode(char *val, struct BTreeNode *child) {
  struct BTreeNode *newNode;
  newNode = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
  strcpy(newNode->val[1],val);
  newNode->count = 1;
  newNode->link[0] = root;
  newNode->link[1] = child;
  newNode->rrn = current_rrn++;  // Atribui o próximo RRN e incrementa
  newNode->rrn_carro = 0;
  return newNode;
}



struct BTreeNode *root = NULL;  // Ponteiro para a raiz

typedef struct {
char placa[TAMANHO_PLACA];
char modelo[TAMANHO_MODELO];
char marca[TAMANHO_MARCA];
int ano;
char categoria[TAMANHO_CATEGORIA];
int quilometragem;
char status[TAMANHO_STATUS];
} Veiculo;


Veiculo *readFile();
struct BTreeNode *carregarRaiz(const char *nomeArquivo);
void search(char *val, struct BTreeNode *myNode);
void insertNode(char *val, int pos, struct BTreeNode *node, struct BTreeNode *child);
void splitNode(char* val, char *pval, int pos, struct BTreeNode *node,struct BTreeNode *child, struct BTreeNode **newNode);
int setValue(char* val, char *pval,struct BTreeNode *node, struct BTreeNode **child);
void insert(char* val) ;
void traversal(struct BTreeNode *myNode,FILE *fp);
int verificaExistencia(const char *filename);
void salvarArvoreNoArquivo(struct BTreeNode *root);
void gravarNoArquivo(struct BTreeNode *node, FILE *fp);
Veiculo *carregarVeiculoPorRRN(int rrn);