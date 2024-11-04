#include <stdio.h>
#include <stdlib.h>
#include "projeto1.h"



// Função para carregar a raiz da Árvore B a partir de um arquivo
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

    // Ler a raiz do arquivo (assumindo que a primeira linha contém a raiz)
    fseek(fp, 0, SEEK_SET);
    fscanf(fp, "%d|%c|", &root->rrn, &root->count);
    for (int i = 0; i < root->count; i++) {  
        fscanf(fp, "%7s|", root->val[i]);
    }

    for (int i = 0; i <= root->count; i++) {
        root->link[i] = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));  
        if (root->link[i] == NULL) {
            fprintf(stderr, "Erro ao alocar memória para o filho %d\n", i);
            for (int j = 0; j < i; j++) {
                free(root->link[j]);  // Liberar memória já alocada
            }
            free(root);
            fclose(fp);
            return NULL;
        }

        // Ler o rrn_page para cada filho
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
    int numRecords = qtdRecords;  // Número de registros a serem lidos

    // Alocar memória para múltiplos registros de VEHICLE
    Veiculo *veiculo = (Veiculo*)malloc(numRecords * sizeof(Veiculo));
    if (veiculo == NULL) {
        perror("Erro ao alocar memória");
        fclose(fp);
        return NULL;
    }

    // Ler registros do arquivo
    size_t recordsRead = fread(veiculo, recordSize, numRecords, fp);

    if (recordsRead < numRecords) { 
        printf("Erro ao ler o registro\n");
        fclose(fp);
        free(veiculo);
        return NULL;
    }

    // Fechar o arquivo após a leitura
    fclose(fp);

    return veiculo;

}

void search(char *val, struct BTreeNode *myNode) {
    int pos;
    if (!myNode) {
        printf("Veículo com placa %s não encontrado.\n", val);
        return;
    }

    // Procura pela chave na Árvore B
    if (strcmp(val, myNode->val[1]) < 0) {
        pos = 0;
    } else {
        for (pos = myNode->count; (strcmp(val, myNode->val[pos]) < 0 && pos > 1); pos--)
            ;
        if (strcmp(val, myNode->val[pos]) == 0) {
            int rrn = myNode->rrn;  // Obtém o RRN associado
          
            printf("Placa %s encontrada. RRN: %d\n", val, rrn);

            // Carrega o veículo pelo RRN
            Veiculo *veiculo = carregarVeiculoPorRRN(rrn);
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

    // Continua a busca recursivamente nos filhos
    search(val, myNode->link[pos]);
}
// Insert node
void insertNode(char *val, int pos, struct BTreeNode *node,
        struct BTreeNode *child) {
  int j = node->count;
  while (j > pos) {
    strcpy(node->val[j + 1],node->val[j]);
    node->link[j + 1] = node->link[j];
    j--;
  }
  strcpy(node->val[j + 1],val);
  node->link[j + 1] = child;
  node->count++;
}

// Split node
void splitNode(char* val, char *pval, int pos, struct BTreeNode *node,
         struct BTreeNode *child, struct BTreeNode **newNode) {
  int median, j;

  if (pos > MIN)
    median = MIN + 1;
  else
    median = MIN;

  *newNode = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
  (*newNode)->rrn = current_rrn++;  // Atribui o próximo RRN ao novo nó
  j = median + 1;
  while (j <= MAX) {
    strcpy((*newNode)->val[j - median],node->val[j]);
    (*newNode)->link[j - median] = node->link[j];
    j++;
  }
  node->count = median;
  (*newNode)->count = MAX - median;

  if (pos <= MIN) {
    insertNode(val, pos, node, child);
  } else {
    insertNode(val, pos - median, *newNode, child);
  }
  strcpy(pval, node->val[node->count]);
  (*newNode)->link[0] = node->link[node->count];
  node->count--;
}

// Set the value
int setValue(char* val, char *pval,
           struct BTreeNode *node, struct BTreeNode **child) {
  int pos;
  if (!node) {
    strcpy(pval, val);
    *child = NULL;
    return 1;
  }

  if (strcmp(val,node->val[1])<0) {
    pos = 0;
  } else {
    for (pos = node->count;
       (strcmp(val, node->val[pos]) < 0 && pos > 1); pos--);
    if (strcmp(val,node->val[pos])==0) {
      printf("Duplicates are not permitted\n");
      return 0;
    }
  }
  if (setValue(val, pval, node->link[pos], child)) {
    if (node->count < MAX) {
      insertNode(pval, pos, node, *child);
    } else {
      splitNode(pval, pval, pos, node, *child, child);
      return 1;
    }
  }
  return 0;
}
void gravarNoArquivo(struct BTreeNode *node, FILE *fp) {
    if (!node) return;

    // Escreve o RRN do nó
    fprintf(fp, "%d|", node->rrn);

    // Escreve o número de chaves
    fprintf(fp, "%d|", node->count);

    // Escreve as chaves no nó
    for (int i = 1; i <= node->count; i++) {
        fprintf(fp, "%s", node->val[i]);
        if (i < node->count) fprintf(fp, ",");  // Separador de chaves
    }
    fprintf(fp, "|");

    // Escreve os RRNs dos filhos
    for (int i = 0; i <= node->count; i++) {
        if (node->link[i]) {
            fprintf(fp, "%d ", node->link[i]->rrn);
        } else {
            fprintf(fp, "-1 ");  // Indica que não há filho
        }
    }
    fprintf(fp, "\n");

    // Grava recursivamente os nós filhos
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

// Insert the value
void insert(char* val) {
    int flag;
    struct BTreeNode *child;
    char pval[8];

    flag = setValue(val, pval, root, &child);
    if (flag) {
        root = createNode(pval, child);
    }

    // Salva a árvore no arquivo após a inserção
    salvarArvoreNoArquivo(root);
}

// Traversal function modified to show RRN|key_count|keys|children format
void traversal(struct BTreeNode *myNode, FILE *fp) {
    if (!myNode) return;

    // Print the RRN of the node
    fprintf(fp, "%d|", myNode->rrn);

    // Print the number of keys in the node
    fprintf(fp, "%d|", myNode->count);

    // Print all keys in the node
    for (int i = 1; i <= myNode->count; i++) {
        fprintf(fp, "%s", myNode->val[i]);
        if (i < myNode->count) fprintf(fp, ",");  // comma separate keys
    }
    fprintf(fp, "|");

    // Print children RRN
    for (int i = 0; i <= myNode->count; i++) {
        if (myNode->link[i]) fprintf(fp, "%d ", myNode->link[i]->rrn);
    }
    fprintf(fp, "\n");

    // Recursive traversal for each child node
    for (int i = 0; i <= myNode->count; i++) {
        if (myNode->link[i]) traversal(myNode->link[i], fp);
    }
}

int verificaExistencia(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp != NULL) {
        fclose(fp);
        return 1;  // Arquivo existe
    }
    return 0;  // Arquivo não existe
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

    // Calcula o offset com base no tamanho do registro e no RRN
    long offset = rrn * sizeof(Veiculo);
    fseek(fp, offset, SEEK_SET);

    // Lê o registro do veículo
    if (fread(veiculo, sizeof(Veiculo), 1, fp) != 1) {
        perror("Erro ao ler o veículo do arquivo");
        free(veiculo);
        fclose(fp);
        return NULL;
    }

    fclose(fp);
    return veiculo;
}