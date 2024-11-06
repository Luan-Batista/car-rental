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
    // for (int i = 0; i < qtdRecords; i++) {
    //     printf("Registro de RRN %d\n", i);
    //     printf("Placa: %s\n", veiculo[i].placa);
    //     printf("Modelo: %s\n", veiculo[i].modelo);
    //     printf("Marca: %s\n", veiculo[i].marca);
    //     printf("Ano: %d\n", veiculo[i].ano);
    //     printf("Categoria: %s\n", veiculo[i].categoria);
    //     printf("Quilometragem: %d\n", veiculo[i].quilometragem);
    //     printf("Status: %s\n", veiculo[i].status);
    //     printf("---------------------------\n");
    // }

    if (existe == 1) {
        // Carregar a raiz se o arquivo existir
        root = carregarRaiz("btree_M.idx");
        if (root != NULL) {
            printf("Raiz carregada com sucesso!\n");
        } else {
            printf("Falha ao carregar a raiz!\n");
        }
    } else {
    
        fp = fopen("btree_M.idx", "w");
        if (fp == NULL) {
            perror("Erro ao criar o arquivo de índice");
            return 1;
        }
        
        for (int i = 0; i < qtdRecords; i++) {
            insert(veiculo[i].placa,i);
        }

        fclose(fp);
    }

    do {
        exibirMenu();
        scanf("%d", &op);
        switch (op) {
            case 1:
                qtdRecords++;
                Veiculo *novoVeiculo = readFile();
                insert(novoVeiculo[qtdRecords-1].placa,qtdRecords);
                printf("chave: %s inserida",novoVeiculo[qtdRecords-1].placa);

                break;
            case 2:
                printf("Digite a placa para buscar: ");
                char placa[8];
                scanf("%s", placa);
                search(placa, root);
                break;
        }
    } while (op != 0);

    free(veiculo);
    free(root);
    return 0;
}
