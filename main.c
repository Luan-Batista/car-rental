#include "projeto1.c"

int main(){
    FILE *fp = fopen("veiculos.dat","r+");
    readFile(fp);
}