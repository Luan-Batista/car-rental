#include <stdio.h>
#include <stdlib.h>
#include "projeto1.c"

int main() {
    int ch;
    int op;
    FILE *fp = NULL;
    int existe = verificaExistencia("btree_M.idx");
    Veiculo *veiculo = readFile();

    // Exibir todos os registros de veículos
    for (int i = 0; i < qtdRecords; i++) {
        printf("Registro de RRN %d\n", i);
        printf("Placa: %s\n", veiculo[i].placa);
        printf("Modelo: %s\n", veiculo[i].modelo);
        printf("Marca: %s\n", veiculo[i].marca);
        printf("Ano: %d\n", veiculo[i].ano);
        printf("Categoria: %s\n", veiculo[i].categoria);
        printf("Quilometragem: %d\n", veiculo[i].quilometragem);
        printf("Status: %s\n", veiculo[i].status);
        printf("---------------------------\n");
    }

    if (existe == 1) {
        // Carregar a raiz se o arquivo existir
        root = carregarRaiz("btree_M.idx");
    } else {
        // Criar um novo arquivo e inicializar a árvore se o arquivo não existir
        fp = fopen("btree_M.idx", "w");
        if (fp == NULL) {
            perror("Erro ao criar o arquivo de índice");
            return 1;
        }

        // Inserir todos os veículos na árvore
        for (int i = 0; i < qtdRecords; i++) {
            insert(veiculo[i].placa);
          
        }

        // Gravar a estrutura inicial da árvore no arquivo e fechar
        // traversal(root, fp);
        fclose(fp);
    }

    // Menu de operações para o usuário
    do {
        printf("\nMenu:\n");
        printf("1. Inserir um veículo\n");
        printf("2. Remover um veículo\n");
        printf("4. Buscar Veículo\n");
        printf("5. Listar todos os veículos\n");
        printf("0. Sair\n");
        scanf("%d", &op);

        switch (op) {
            case 1:
                // Inserir um novo veículo
                    insert("HHC8765");
                   
                break;

            case 2:
                // Lógica para remover um veículo (implementar se necessário)
                break;

            case 4:
                // Lógica para buscar um veículo
                printf("Digite a placa para buscar: ");
                char placa[8];
                scanf("%s", placa);
                search(placa, root);
                break;

            case 5:
                // Exibir todos os veículos e salvar a árvore no arquivo
                fp = fopen("btree_M.idx", "w");
                if (fp != NULL) {
                    traversal(root, fp);
                    fclose(fp);
                } else {
                    perror("Erro ao abrir o arquivo para gravação");
                }
                break;
        }
    } while (op != 0);

    // Liberar a memória dos veículos
    free(veiculo);

    return 0;
}
