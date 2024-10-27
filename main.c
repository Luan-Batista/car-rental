// int main(){
//     // FILE *fp = NULL; 
//     Node *root = createNode();
//     insertBTree(&root, "abc123");
//     insertBTree(&root, "def456");
//     insertBTree(&root, "ghi789");
//     insertBTree(&root, "jkl012");
//     insertBTree(&root, "mno345");

//     print_node(root);
//     // VEHICLE *vehicle = malloc(10*sizeof(VEHICLE));
//     // vehicle = readFile(fp);
//     // int op;
// }

#include "projeto1.h"
int main() {
    // Criar a árvore B
    FILE *fp = NULL;
    FILE *fp2 = NULL;
    fp2 = fopen("btree.idx","w");
    BTree *tree = createBTree();
    VEHICLE *vehicle = readFile(fp);
    

    // Inserir algumas chaves

    for(int i=0;i<qtdRecords;i++)
    {
        insertBTree(tree,vehicle[i].licensePlate);
    }

    for(int i=0;i<5;i++)
    {
        printf("Registro de RNN %d\n", i);
        printf("Placa: %s\n", vehicle[i].licensePlate);
        printf("Modelo: %s\n", vehicle[i].model);
        printf("Marca: %s\n", vehicle[i].brand);
        printf("Ano: %d\n", vehicle[i].year);
        printf("Categoria: %s\n", vehicle[i].category);
        printf("Quilometragem: %d\n", vehicle[i].km);
        printf("Status: %s\n", vehicle[i].status);
        printf("---------------------------\n");
    }

    
    printAllNodes(fp2,tree->root);
    // Imprimir a árvore B
    // printf("Árvore B:\n");
    // printBTree(tree->root, 0);

    return 0;
}



    // char keys[ordem - 1];
    // int  rrn[ordem - 1];
    // struct no *children[ordem + 1];
    // int keyCount;
    // struct no *parent;
    // int isLeaf;


