#include "projeto1.h"
int nextRRN = 0;

// Função para criar um novo nó
Node *createNode() {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Erro ao alocar nó");
        exit(1);
    }

    for (int i = 0; i < MAX_KEYS; i++) {
        strcpy(newNode->key[i], "");
    }
    for (int i = 0; i < ordem; i++) {
        newNode->children[i] = NULL;  // Inicializar filhos com NULL
    }
    newNode->keyCount = 0;
    newNode->isLeaf = 1;
    newNode->rrn_page = nextRRN++; // Atribuir o RRN atual e incrementar

    return newNode;
}

void print_node(FILE *fp, Node *node) {
    if (node == NULL) return; // Evita impressão se o nó for NULL
    
    fprintf(fp, "RRN da página: %d\n", node->rrn_page);
    fprintf(fp, "Chaves no nó: [");
    for (int i = 0; i < node->keyCount; i++) {
        fprintf(fp, "%s", node->key[i]);
        if (i < node->keyCount - 1) {
            fprintf(fp, ", ");
        }
    }
    fprintf(fp, "]\n");

    fprintf(fp, "Filhos: ");
    for (int i = 0; i <= node->keyCount; i++) {
        if (node->children[i] != NULL) {
            fprintf(fp, "%d ", node->children[i]->rrn_page);
        }
    }
    fprintf(fp, "\n\n");
}

// Função recursiva que percorre toda a árvore
void printAllNodes(FILE *fp, Node *node) {
    if (node != NULL) {
        print_node(fp, node);  // Imprime o nó atual
        for (int i = 0; i <= node->keyCount; i++) {
            printAllNodes(fp, node->children[i]);  // Imprime os filhos
        }
    }
}

// Função principal que abre o arquivo e chama a função recursiva
void printBTreeToFile(Node *root) {
    FILE *fp = fopen("btree.idx", "w");  // Abre o arquivo no modo "w" para escrita
    if (fp == NULL) {
        perror("Erro ao abrir o arquivo btree.idx para escrita");
        return;
    }

    printAllNodes(fp, root);  // Chama a função para imprimir todos os nós, começando pela raiz
    fclose(fp);  // Fecha o arquivo após terminar a impressão
}





// Função para dividir um nó quando está cheio
// Função para dividir um nó quando está cheio
void splitChild(Node *parent, int index, Node *fullChild) {
    Node *newNode = createNode();
    newNode->isLeaf = fullChild->isLeaf;

    // Metade superior das chaves do nó cheio
    int midIndex = fullChild->keyCount / 2;  // Índice do meio

    // Copia as chaves superiores para o novo nó
    for (int i = midIndex + 1; i < fullChild->keyCount; i++) {
        strcpy(newNode->key[newNode->keyCount++], fullChild->key[i]);
    }

    // Remove a chave do meio do nó cheio
    fullChild->keyCount = midIndex; // Agora o nó cheio contém apenas as chaves inferiores

    // Atualiza o nó pai

    for (int i = parent->keyCount; i >= index + 1; i--) {
        parent->children[i + 1] = parent->children[i];
    }
    parent->children[index + 1] = newNode;

    for (int i = parent->keyCount - 1; i >= index; i--) {
        strcpy(parent->key[i + 1], parent->key[i]);
    }
    strcpy(parent->key[index], fullChild->key[midIndex]); // Promove a chave do meio
    parent->keyCount++;
}


// Função para inserir uma chave em um nó não cheio
void insertNonFull(Node *node, char *key) {
    int i = node->keyCount - 1;

    if (node->isLeaf) {
        // Encontra a posição de inserção e move as chaves para abrir espaço
        while (i >= 0 && strcmp(key, node->key[i]) < 0) {
            strcpy(node->key[i + 1], node->key[i]);
            i--;
        }
        strcpy(node->key[i + 1], key);
        node->keyCount++;
    } else {
        // Encontra o filho onde a chave deve ser inserida
        while (i >= 0 && strcmp(key, node->key[i]) < 0) {
            i--;
        }
        i++;

        // Se o filho estiver cheio, divida-o
        if (node->children[i]->keyCount == MAX_KEYS) {
            splitChild(node, i, node->children[i]);
            if (strcmp(key, node->key[i]) > 0) {
                i++;
            }
        }
        insertNonFull(node->children[i], key);
    }
}

// Função principal de inserção
void insertBTree(BTree *tree, char *key) {
    Node *r = tree->root;
    if (r->keyCount == MAX_KEYS) {  // Se o nó raiz estiver cheio
        Node *newRoot = createNode();
        newRoot->isLeaf = 0;
        newRoot->children[0] = r;  // A raiz antiga se torna o primeiro filho
        splitChild(newRoot, 0, r);  // Dividir a raiz
        insertNonFull(newRoot, key); // Inserir a nova chave
        tree->root = newRoot; // Atualizar a raiz da árvore
    } else {
        insertNonFull(r, key); // Inserir a chave no nó raiz
    }
}

// void printBTree(Node *node, int level) {
//     if (node != NULL) {
//         printf("Nível %d: ", level);
//         print_node(node);

//         for (int i = 0; i <= node->keyCount; i++) {
//             printBTree(node->children[i], level + 1);
//         }
//     }
// }

BTree *createBTree() {
    BTree *newTree = (BTree *)malloc(sizeof(BTree));
    newTree->root = createNode();
    return newTree;
}



VEHICLE* readFile(FILE *fp) {
    fp = fopen("veiculos.dat", "rb");
    if (fp == NULL) {
        perror("Erro ao abrir o arquivo veiculos.dat");
        return NULL;
    }

    size_t recordSize = sizeof(VEHICLE);
    int numRecords = qtdRecords;  // Número de registros a serem lidos

    // Alocar memória para múltiplos registros de VEHICLE
    VEHICLE *vehicle = (VEHICLE*)malloc(numRecords * sizeof(VEHICLE));
    if (vehicle == NULL) {
        perror("Erro ao alocar memória");
        fclose(fp);
        return NULL;
    }

    // Ler registros do arquivo
    size_t recordsRead = fread(vehicle, recordSize, numRecords, fp);
    if (recordsRead < numRecords) {
        printf("Erro ao ler o registro\n");
        fclose(fp);
        free(vehicle);
        return NULL;
    }

    // Fechar o arquivo após a leitura
    fclose(fp);

    return vehicle;
}
