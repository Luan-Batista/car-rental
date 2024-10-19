#include "projeto1.h"


int readFile(FILE *fp){
    fp = fopen("veiculos.dat","rb");
    if(fp == NULL){
        perror("Erro ao abrir o arquivo veiculos.dat");
    }

    size_t recordSize = sizeof(VEHICLE);

    VEHICLE *vehicle;
    vehicle = (VEHICLE*)malloc((10*sizeof(VEHICLE)));
    if (vehicle == NULL) {
        perror("Erro ao alocar memória");
        fclose(fp);
        return -1; // Retorna erro
    }
    size_t recordsRead = fread(&vehicle, recordSize,10, fp);
    if(recordsRead < 1){
        printf("Erro ao ler o registro");
        free(vehicle);
        fclose(fp);
        return -1;
    }

    for(int i=0;i<recordsRead;i++)
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
    // Imprimir os dados do veículo
    
    free(vehicle);
    fclose(fp);
    return 0;
}