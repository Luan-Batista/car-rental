#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_PLACA 8
#define TAMANHO_MODELO 20
#define TAMANHO_MARCA 20
#define TAMANHO_CATEGORIA 15
#define TAMANHO_STATUS 16
#define MAX 5
#define MIN 2
int qtdRecords = 30;

typedef struct chave{
  char chave[MAX+1][8];
  int rrn_carro;
}Tchave;

struct BTreeNode{
  Tchave val;
  char count;
  struct BTreeNode *link[MAX + 1];
  int rrn;  // Novo campo para armazenar o RRN do nó
};

struct BTreeNode *root;
int current_rrn = 0;  // Variável global para controlar o RRN sequencial




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
void insertNode(char *val, int rrn_carro, int pos, struct BTreeNode *node, struct BTreeNode *child);
void splitNode(char *val, int rrn_carro, char *pval, int *p_rrn_carro, int pos,struct BTreeNode *node, struct BTreeNode *child, struct BTreeNode **newNode);
int setValue(char *val, int rrn_carro, char *pval, int *p_rrn_carro,struct BTreeNode *node, struct BTreeNode **child);
void insert(char* val,int rrn_carro);
void traversal(struct BTreeNode *myNode,FILE *fp);
int verificaExistencia(const char *filename);
void salvarArvoreNoArquivo(struct BTreeNode *root);
void gravarNoArquivo(struct BTreeNode *node, FILE *fp);
Veiculo *carregarVeiculoPorRRN(int rrn);
void exibirMenu();
struct BTreeNode *createNode(char *val, int rrn_carro, struct BTreeNode *child);