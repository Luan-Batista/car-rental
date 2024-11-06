# Instruções de Execução do Programa

Este programa implementa uma árvore B para gerenciar registros de veículos em um sistema de locadora de carros. A árvore B é usada para indexação eficiente dos veículos, permitindo busca, inserção e manutenção de registros.

## Requisitos

- **Sistema Operacional:** Linux ou Windows (via WSL)
- **Compilador:** GCC (ou outro compilador C)
- **Arquivos de Entrada:**
  - `veiculos.dat`: Arquivo binário contendo os dados dos veículos.
  - `btree_M.idx`: Arquivo de índice onde a árvore B será armazenada.
  
## Estrutura dos Arquivos

- **veiculos.dat:** Contém registros de veículos com os seguintes campos:
  - Placa (string)
  - Modelo (string)
  - Marca (string)
  - Ano (inteiro)
  - Categoria (string)
  - Quilometragem (inteiro)
  - Status de Disponibilidade (string)

- **btree_M.idx:** Arquivo de índice binário que armazena a árvore B com chaves e links para os nós filhos.

## Como Compilar o Programa

1. Abra o terminal.
2. Navegue até o diretório onde os arquivos do programa estão armazenados.
3. Compile o código utilizando o compilador GCC com o seguinte comando:

   ```bash
   gcc -o programa programa.c
