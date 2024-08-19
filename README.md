# Biblia-escrita-em-pagina-de-memoria
### RESUMO:
Esse programa usa páginas de memória virtual do sistema operacional para gerenciar dados em arquivos e realizar operações de leitura e escrita diretamente na memória. O código utilizar a bíblia como exemplo, a escrevendo em um .txt a partir da memória virtual.

### Para testar rode o main.c com:
gcc -o main main.c
./main

# Conteúdo:
Main.c - Arquivo principal que faz a manipulação de memória e leitura e escrita dos arquivos.
BIBLIA_COMPLETA.txt - Biblia em espanhol no formato de texto
biblia.txt - arquivo criado com o conteúdo de BIBLIA_COMPLETA.txt

### Descrição:
O programa mapeia o arquivo BIBLIA_COMPLETA.txt para a memória usando mmap, acessando o conteúdo do arquivo diretamente na memória. Abre (ou cria, se não existir) o arquivo biblia.txt e o redimensiona para o tamanho do arquivo de origem usando ftruncate. Em seguida, o programa mapeia biblia.txt para a memória com permissões de leitura e escrita (PROT_READ | PROT_WRITE) usando mmap. O conteúdo do arquivo BIBLIA_COMPLETA.txt é copiado para o arquivo biblia.txt utilizando memcpy. Isso é possível porque o conteúdo do arquivo de origem está mapeado na memória e o arquivo de destino também está mapeado. Ao final, o programa desmapea ambos os arquivos da memória usando munmap e fecha os descritores de arquivo.

OBS:
- O programa sincroniza o mapeamento com o arquivo usando msync para garantir que todas as alterações sejam gravadas no arquivo.
- Calcula o número de páginas de memória utilizadas para armazenar o arquivo, arredondando para cima se necessário, e imprime essa informação.





