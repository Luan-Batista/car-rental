#include <stdio.h>
#include <stdlib.h>
#include "projeto1.h"


struct BTreeNode *createNode(char *val, int rrn_carro, struct BTreeNode *child) {
  struct BTreeNode *newNode;
  newNode = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
  strcpy(newNode->val.chave[1],val);
  newNode->count = 1;
  newNode->link[0] = root;
  newNode->link[1] = child;
  newNode->rrn = current_rrn++; 
  newNode->val.rrn_carro = rrn_carro;
  return newNode;
}

struct BTreeNode *carregarRaiz(const char *nomeArquivo) {
    FILE *fp = fopen(nomeArquivo, "r+");
    if (fp == NULL) {
        perror("Erro ao abrir o arquivo btree_M.idx");
        return NULL;
    }

    struct BTreeNode *root = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
    if (root == NULL) {
        perror("Erro ao alocar memória para a raiz");
        fclose(fp);
        return NULL;
    }

    fseek(fp, 0, SEEK_SET);
    fscanf(fp, "%d|%c|", &root->rrn, &root->count);
    for (int i = 0; i < root->count; i++) {  
        fscanf(fp, "%7s|", root->val.chave[i]);
    }

    for (int i = 0; i <= root->count; i++) {
        root->link[i] = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));  
        if (root->link[i] == NULL) {
            fprintf(stderr, "Erro ao alocar memória para o filho %d\n", i);
            for (int j = 0; j < i; j++) {
                free(root->link[j]); 
            }
            free(root);
            fclose(fp);
            return NULL;
        }

        
        if (fscanf(fp, "%d", &root->link[i]->rrn) != 1) {
            fprintf(stderr, "Erro ao ler o rrn_page do filho %d\n", i);
            for (int j = 0; j <= i; j++) {
                free(root->link[j]);
            }
            free(root);
            fclose(fp);
            return NULL;
        }
    }

    fclose(fp);
    return root;
}


Veiculo *readFile(){
FILE *fp = fopen("veiculos.dat", "rb");
    if (fp == NULL) {
    perror("Erro ao abrir o arquivo veiculos.dat");
    }


    size_t recordSize = sizeof(Veiculo);
    int numRecords = qtdRecords;  

  
    Veiculo *veiculo = (Veiculo*)malloc(numRecords * sizeof(Veiculo));
    if (veiculo == NULL) {
        perror("Erro ao alocar memória");
        fclose(fp);
        return NULL;
    }


    size_t recordsRead = fread(veiculo, recordSize, numRecords, fp);

    if (recordsRead < numRecords) { 
        printf("Erro ao ler o registro\n");
        fclose(fp);
        free(veiculo);
        return NULL;
    }


    fclose(fp);

    return veiculo;

}

void search(char *val, struct BTreeNode *myNode) {
    int pos;
    if (!myNode) {
        printf("Veículo com placa %s não encontrado.\n", val);
        return;
    }

   
    if (strcmp(val, myNode->val.chave[1]) < 0) {
        pos = 0;
    } else {
        for (pos = myNode->count; (strcmp(val, myNode->val.chave[pos]) < 0 && pos > 1); pos--);
        if (strcmp(val, myNode->val.chave[pos]) == 0) {
            int rrn_carro = myNode->val.rrn_carro;  
            printf("%d",rrn_carro);
            printf("Placa %s encontrada. RRN do Veículo: %d\n", val, rrn_carro);

           
            Veiculo *veiculo = carregarVeiculoPorRRN(rrn_carro);
            if (veiculo) {
                printf("Informações do veículo:\n");
                printf("Placa: %s\n", veiculo->placa);
                printf("Modelo: %s\n", veiculo->modelo);
                printf("Marca: %s\n", veiculo->marca);
                printf("Ano: %d\n", veiculo->ano);
                printf("Categoria: %s\n", veiculo->categoria);
                printf("Quilometragem: %d\n", veiculo->quilometragem);
                printf("Disponibilidade: %s\n", veiculo->status);
                free(veiculo);
            }
            return;
        }
    }


    search(val, myNode->link[pos]);
}



void insertNode(char *val, int rrn_carro, int pos, struct BTreeNode *node, struct BTreeNode *child) {
    int j = node->count;
    while (j > pos) {
        strcpy(node->val.chave[j + 1], node->val.chave[j]);
        node->link[j + 1] = node->link[j];
        j--;
    }
    strcpy(node->val.chave[j + 1], val);
    node->val.rrn_carro = rrn_carro; 
    node->link[j + 1] = child;
    node->count++;
}



void splitNode(char *val, int rrn_carro, char *pval, int *p_rrn_carro, int pos,
               struct BTreeNode *node, struct BTreeNode *child, struct BTreeNode **newNode) {
    int median, j;

    if (pos > MIN)
        median = MIN + 1;
    else
        median = MIN;

    *newNode = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
    (*newNode)->rrn = current_rrn++; 
    j = median + 1;
    while (j <= MAX) {
        strcpy((*newNode)->val.chave[j - median], node->val.chave[j]);
        (*newNode)->link[j - median] = node->link[j];
        j++;
    }
    node->count = median;
    (*newNode)->count = MAX - median;

    if (pos <= MIN) {
        insertNode(val, rrn_carro, pos, node, child);
    } else {
        insertNode(val, rrn_carro, pos - median, *newNode, child);
    }
    strcpy(pval, node->val.chave[node->count]);
    *p_rrn_carro = node->val.rrn_carro;  
    (*newNode)->link[0] = node->link[node->count];
    node->count--;
}





int setValue(char *val, int rrn_carro, char *pval, int *p_rrn_carro,
             struct BTreeNode *node, struct BTreeNode **child) {
    int pos;
    if (!node) {
        strcpy(pval, val);
        *p_rrn_carro = rrn_carro;  
        *child = NULL;
        return 1;
    }

    if (strcmp(val, node->val.chave[1]) < 0) {
        pos = 0;
    } else {
        for (pos = node->count;
             (strcmp(val, node->val.chave[pos]) < 0 && pos > 1); pos--);
        if (strcmp(val, node->val.chave[pos]) == 0) {
            printf("Chaves duplicadas não são permitidas\n");
            return 0;
        }
    }

    if (setValue(val, rrn_carro, pval, p_rrn_carro, node->link[pos], child)) {
        if (node->count < MAX) {
            insertNode(pval, *p_rrn_carro, pos, node, *child);
        } else {
            splitNode(pval, rrn_carro, pval, p_rrn_carro, pos, node, *child, child);
            return 1;
        }
    }
    return 0;
}


void gravarNoArquivo(struct BTreeNode *node, FILE *fp) {
    if (!node) return;


    fprintf(fp, "%d|", node->rrn);


    fprintf(fp, "%d|", node->count);


    for (int i = 1; i <= node->count; i++) {
        fprintf(fp, "%s", node->val.chave[i]);
        if (i < node->count) fprintf(fp, ",");  
    }
    fprintf(fp, "|");

    for (int i = 0; i <= node->count; i++) {
        if (node->link[i]) {
            fprintf(fp, "%d ", node->link[i]->rrn);
        } else {
            fprintf(fp, "-1 ");  
        }
    }
    fprintf(fp, "\n");

    
    for (int i = 0; i <= node->count; i++) {
        if (node->link[i]) {
            gravarNoArquivo(node->link[i], fp);
        }
    }
}

void salvarArvoreNoArquivo(struct BTreeNode *root) {
    FILE *fp = fopen("btree_M.idx", "w");
    if (!fp) {
        perror("Erro ao abrir o arquivo para gravação");
        return;
    }
    gravarNoArquivo(root, fp);
    fclose(fp);
}


void insert(char *val, int rrn_carro) {
    int flag;
    struct BTreeNode *child;
    char pval[8];
    
    int p_rrn_carro = 0;
    flag = setValue(val, rrn_carro, pval, &p_rrn_carro, root, &child);
    if (flag) {
        root = createNode(pval, rrn_carro, child);
    }


    salvarArvoreNoArquivo(root);
}



void traversal(struct BTreeNode *myNode, FILE *fp) {
    if (!myNode) return;


    fprintf(fp, "%d|", myNode->rrn);


    fprintf(fp, "%d|", myNode->count);

    for (int i = 1; i <= myNode->count; i++) {
        fprintf(fp, "%s", myNode->val.chave[i]);
        if (i < myNode->count) fprintf(fp, ","); 
    }
    fprintf(fp, "|");


    for (int i = 0; i <= myNode->count; i++) {
        if (myNode->link[i]) fprintf(fp, "%d ", myNode->link[i]->rrn);
    }
    fprintf(fp, "\n");


    for (int i = 0; i <= myNode->count; i++) {
        if (myNode->link[i]) traversal(myNode->link[i], fp);
    }
}

int verificaExistencia(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp != NULL) {
        fclose(fp);
        return 1;  
    }
    return 0;  
}

Veiculo *carregarVeiculoPorRRN(int rrn) {
    FILE *fp = fopen("veiculos.dat", "rb");
    if (!fp) {
        perror("Erro ao abrir o arquivo de veículos");
        return NULL;
    }

    Veiculo *veiculo = (Veiculo *)malloc(sizeof(Veiculo));
    if (!veiculo) {
        perror("Erro ao alocar memória para o veículo");
        fclose(fp);
        return NULL;
    }

    long offset = (rrn-1) * sizeof(Veiculo);
    fseek(fp, offset, SEEK_SET);

    if (fread(veiculo, sizeof(Veiculo), 1, fp) != 1) {
        perror("Erro ao ler o veículo do arquivo");
        free(veiculo);
        fclose(fp);
        return NULL;
    }

    fclose(fp);
    return veiculo;
}

void exibirMenu(){
    printf("\nMenu:\n");
    printf("1. Inserir um veículo\n");
    printf("2. Buscar Veículo\n");
    printf("0. Sair\n");
}