#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){

    if (argc != 3){
        printf("Uso: %s <arquivo de entrada> <tamanho máximo em MB>\n", argv[0]);
        return 1;
    }


    FILE *mpg_file = fopen(argv[1], "rb");
    if (mpg_file == NULL){
        printf("ERRO! O arquivo não foi aberto!\n");
        return 2;
    }

    // Converte o tamanho maximo em MB para bytes
    int max_size = atoi(argv[2]) * 1024 * 1024;

    unsigned char *buffer = malloc(max_size);
    if (buffer == NULL){
        printf("ERRO! Não foi possível alocar memória para o buffer!\n");
        return 3;
    }


    char output_name[100];
    sprintf(output_name, "video_parte_1.mpg");

    FILE *output_file = fopen(output_name, "wb");
    if (output_file == NULL){
        printf("ERRO! Não foi possível abrir o arquivo de saída %s!\n", output_name);
        return 4;
    }
    
    printf("Criando arquivo %s ..\n", output_name);


    int output_size = 0;
    int buffer_size = 0;
    unsigned char byte[4];
    int output_count = 1;

    // Enquanto não chegar ao final do arquivo de entrada
    while (fread(byte, 4, 1, mpg_file) == 1){
        // Se os quatro bytes lidos não forem o código de Sequence
        if (memcmp("\x00\x00\x01\xB3", byte, 4) != 0){
            buffer[buffer_size] = byte[0];
            buffer_size++;
            fseek(mpg_file, -3, SEEK_CUR);
        }
        else {
            if (output_size + buffer_size > max_size){
                fclose(output_file);
                output_count++;
                sprintf(output_name, "video_parte_%d.mpg", output_count);
                output_file = fopen(output_name, "wb");
                if (output_file == NULL){
                    printf("ERRO! Não foi possível abrir o arquivo de saída %s!\n", output_name);
                    return 5;
                }
                fwrite(buffer, buffer_size, 1, output_file);
                printf("Criando arquivo %s ..\n", output_name);
                output_size = buffer_size;
            }
            else {
                fwrite(buffer, buffer_size, 1, output_file);
                output_size += buffer_size;
            }
            memcpy(buffer, byte, 4);
            buffer_size = 4;
        }
    }

    fclose(output_file);
    free(buffer);
    fclose(mpg_file);

    return 0;
}
