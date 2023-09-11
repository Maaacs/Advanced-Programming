#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct zip_file_hdr{
    int assinatura;
    short versao;
    short flags;
    short metodo_compressao;
    short tempo_modificacao;
    short data_modificacao;
    int crc;
    int tamanho_comprimido;
    int tamanho_descomprimido;
    short comprimento_nome;
    short comprimento_campo_extra;
} __attribute__((packed));




int main(int argc, char *argv[]){
    if (argc < 2) {
        printf("Uso: %s <arquivo_entrada>\n", argv[0]);
        return 1;
    }

    FILE *arquivo_zip = fopen(argv[1], "rb");
    if (arquivo_zip == NULL){
        printf("ERRO! O arquivo não foi aberto!\n");
        return 1;
    }

    int numero_arquivos = 0;
    struct  zip_file_hdr *cabecalho_arquivo = malloc(sizeof(struct  zip_file_hdr));
    char *nome_arquivo = NULL;

    while (fread(cabecalho_arquivo, sizeof(struct  zip_file_hdr), 1, arquivo_zip) == 1){
        if (cabecalho_arquivo->assinatura != 0x04034b50){
            break;
        }

        nome_arquivo = malloc(sizeof(char) * cabecalho_arquivo->comprimento_nome + 1);

        if (nome_arquivo == NULL){
            printf("ERRO! Falha ao alocar memória para o nome do arquivo.\n");
            fclose(arquivo_zip);
            free(cabecalho_arquivo);
            return 1;
        }

        fread(nome_arquivo, cabecalho_arquivo->comprimento_nome, 1, arquivo_zip);
        nome_arquivo[cabecalho_arquivo->comprimento_nome] = '\0';

        printf("Arquivo %d:\n", numero_arquivos + 1);
        printf(" --> Nome do Arquivo: %s\n", nome_arquivo);
        printf(" --> Tamanho Compactado: %d\n", cabecalho_arquivo->tamanho_comprimido);
        printf(" --> Tamanho Descompactado: %d\n\n", cabecalho_arquivo->tamanho_descomprimido);

        free(nome_arquivo);

        fseek(arquivo_zip, cabecalho_arquivo->comprimento_campo_extra + cabecalho_arquivo->tamanho_comprimido, SEEK_CUR);
        numero_arquivos++;
    }

    fclose(arquivo_zip);
    free(cabecalho_arquivo);

    return 0;
}
