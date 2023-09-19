#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#define PNG_SIGNATURE_LENGTH 8
#define PNG_CHUNK_CRC_LENGTH 4

struct chunk{
    int length;
    char type[4];
};

struct png_chunk_hdr{
    int width;
    int height;
    char bit_depth;
    char colour_type;
    char compression_method;
    char filter_method;
    char interlace_method;
} 
__attribute__((packed));

int main(int argc, char *argv[]){

    if (argc < 2){
        printf("Digite: %s <nome do arquivo>\n", argv[0]);
        return 1;
    }

    FILE *png_file = fopen(argv[1], "rb");
    if (png_file == NULL){
        perror("Erro ao abrir o arquivo");
        return 1;
    }

     // Memoria para chunk
    struct chunk *chunk_ = malloc(sizeof(struct chunk));
    if (chunk_ == NULL){
        perror("Erro ao alocar memória para o chunk");
        fclose(png_file);
        return 1;
    }

    // Memória para cabeçalho
    struct png_chunk_hdr *png_chunk = malloc(sizeof(struct png_chunk_hdr));
    if (png_chunk == NULL){
        perror("Erro ao alocar memória para o png_chunk");
        free(chunk_);
        fclose(png_file);
        return 1;
    }

    // Pula os primeiros 8 bytes do arquivo PNG que contem a assinatura PNG
    fseek(png_file, PNG_SIGNATURE_LENGTH, SEEK_CUR);

    int i = 1;
    for (; !feof(png_file); i++){

        // Lê o cabeçalho do chunk atual
        fread(chunk_, sizeof(struct chunk), 1, png_file);
        printf("Lendo chunk: %d\n", i); // chunk atual
        printf("\t--> Tamanho: %d\n", ntohl(chunk_->length));
        printf("\t--> Tipo: %s\n", chunk_->type);

        // Se o tipo do chunk for IHDR -> lê o cabeçalho do chunk IHDR
        if (!strcmp(chunk_->type, "IHDR")){ 
            fread(png_chunk, sizeof(struct png_chunk_hdr), 1, png_file);
            printf("\t\t--> Largura: %d\n", ntohl(png_chunk->width));
            printf("\t\t--> Altura: %d\n", ntohl(png_chunk->height));
            // Pula os 4 bytes do CRC do chunk IHDR
            fseek(png_file, PNG_CHUNK_CRC_LENGTH, SEEK_CUR);
        }
        else if (!strcmp(chunk_->type, "IEND")){
            break;
        }

        else{
            fseek(png_file, ntohl(chunk_->length) + PNG_CHUNK_CRC_LENGTH, SEEK_CUR);
        }
    }
    
    free(chunk_);
    free(png_chunk);
    fclose(png_file);
    return 0;
}
