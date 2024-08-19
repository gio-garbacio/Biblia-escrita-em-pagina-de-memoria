/*Este código agora mapeia BIBLIA_COMPLETA.txt para a memória, copia seu conteúdo para biblia.txt, e depois imprime o número de páginas de memória usadas e o tamanho do arquivo.*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    // Abrir o arquivo de origem (BIBLIA_COMPLETA.txt)
    int arquivo_origem = open("./BIBLIA_COMPLETA.txt", O_RDONLY);
    if (arquivo_origem == -1) {
        perror("Erro ao abrir o arquivo de origem");
        exit(EXIT_FAILURE);
    }

    // Obter o tamanho do arquivo de origem
    struct stat info_origem;
    if (fstat(arquivo_origem, &info_origem) == -1) {
        perror("Erro ao obter tamanho do arquivo de origem");
        close(arquivo_origem);
        exit(EXIT_FAILURE);
    }

    // Obter o tamanho da página do sistema
    long tamanho_pagina = sysconf(_SC_PAGESIZE);
    if (tamanho_pagina == -1) {
        perror("Erro ao obter tamanho da página");
        close(arquivo_origem);
        exit(EXIT_FAILURE);
    }

    // Mapear o arquivo de origem para a memória
    void *mapa_memoria_origem = mmap(NULL, info_origem.st_size, PROT_READ, MAP_PRIVATE, arquivo_origem, 0);
    if (mapa_memoria_origem == MAP_FAILED) {
        perror("Erro ao mapear o arquivo de origem na memória");
        close(arquivo_origem);
        exit(EXIT_FAILURE);
    }

    // Abrir o arquivo de destino (biblia.txt) para escrita
    int arquivo_destino = open("./biblia.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (arquivo_destino == -1) {
        perror("Erro ao abrir o arquivo de destino");
        munmap(mapa_memoria_origem, info_origem.st_size);
        close(arquivo_origem);
        exit(EXIT_FAILURE);
    }

    // Redimensionar o arquivo de destino para o tamanho do arquivo de origem
    if (ftruncate(arquivo_destino, info_origem.st_size) == -1) {
        perror("Erro ao redimensionar o arquivo de destino");
        munmap(mapa_memoria_origem, info_origem.st_size);
        close(arquivo_origem);
        close(arquivo_destino);
        exit(EXIT_FAILURE);
    }

    // Mapear o arquivo de destino para a memória
    void *mapa_memoria_destino = mmap(NULL, info_origem.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, arquivo_destino, 0);
    if (mapa_memoria_destino == MAP_FAILED) {
        perror("Erro ao mapear o arquivo de destino na memória");
        munmap(mapa_memoria_origem, info_origem.st_size);
        close(arquivo_origem);
        close(arquivo_destino);
        exit(EXIT_FAILURE);
    }

    // Copiar o conteúdo do arquivo de origem para o arquivo de destino
    memcpy(mapa_memoria_destino, mapa_memoria_origem, info_origem.st_size);

    // Sincronizar o mapeamento com o arquivo (opcional, mas recomendado)
    if (msync(mapa_memoria_destino, info_origem.st_size, MS_SYNC) == -1) {
        perror("Erro ao sincronizar o mapeamento do arquivo de destino");
    }

    // Calcular o número de páginas utilizadas
    long paginas_utilizadas = (info_origem.st_size + tamanho_pagina - 1) / tamanho_pagina;  // Arredondar para cima
    printf("Número de páginas de memória utilizadas: %ld\n", paginas_utilizadas);
    printf("Tamanho do arquivo: %lld bytes\n", (long long)info_origem.st_size);

    // Desmapear o arquivo de origem da memória
    if (munmap(mapa_memoria_origem, info_origem.st_size) == -1) {
        perror("Erro ao desmapear o arquivo de origem");
    }

    // Desmapear o arquivo de destino da memória
    if (munmap(mapa_memoria_destino, info_origem.st_size) == -1) {
        perror("Erro ao desmapear o arquivo de destino");
    }

    // Fechar os descritores de arquivo
    close(arquivo_origem);
    close(arquivo_destino);

    return 0;
}
