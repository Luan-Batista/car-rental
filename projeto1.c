#include "projeto1.h"

int readFile(FILE *fp){

    fp = fopen("veiculos.dat", "rb");
    if(fp == NULL){
        perror("Erro ao abrir o arquivo veiculos.dat");
        return 1;
    }

    size_t recordSize = sizeof(VEHICLE);

    VEHICLE vehicle;
    size_t recordsRead = fread(&vehicle, recordSize,1, fp);
    if(recordsRead != 1){
        printf("Erro ao ler o registro");
        fclose(fp);
        return 1;       
    }
    // Imprimir os dados do veículo
    printf("Registro de RNN 0\n");
    printf("Placa: %s\n", vehicle.licensePlate);
    printf("Modelo: %s\n", vehicle.model);
    printf("Marca: %s\n", vehicle.brand);
    printf("Ano: %d\n", vehicle.year);
    printf("Categoria: %s\n", vehicle.category);
    printf("Quilometragem: %d\n", vehicle.km);
    printf("Status: %s\n", vehicle.status);
    printf("---------------------------\n");

    fclose(fp);
    return 0;
}