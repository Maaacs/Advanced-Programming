#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 3
#define MPEG_FILE_ARG 1
#define SEQ_HEADER 0xB3 // Valor padrão pro cabeçalho da sequência
#define GOP 0xB8 // Valor padrão pro Group of Pictures
#define PICTURE 0x00 // Valor padrão para uma imagem 


// Struct pra rmazenaa as informações do cabeçalho da sequência de um MPEG
typedef struct {
    unsigned int width;
    unsigned int height;
    float frame_rate;
} SeqHeader;


// Imprime o código de byte do GOP quando encontrado
void handle_gop() {
    printf("--> Codigo: %.2x -- Group of Pictures \n", GOP);
}


// Imprime o código de byte do slice quando encontrado
void handle_slice(unsigned char byte) {
    printf("--> Codigo: %.2x -- Slice\n", byte);
}


// Função para lidar com o cabeçalho da sequência. Lê os próximos 4 bytes do arquivo e calcula width, height e FPS
void handle_seq_header(FILE *mpg_file) {
    unsigned char byte1 = fgetc(mpg_file);
    unsigned char byte2 = fgetc(mpg_file);
    unsigned char byte3 = fgetc(mpg_file);
    unsigned char byte4 = fgetc(mpg_file);
    unsigned int largura = byte1 * 16 + (byte2 >> 4); // Calcula largura
    unsigned int altura = (byte2 & 0x0F) * 256 + byte3; // calcula altura
    unsigned int frame_rate = byte4 & 0x0f; // Calcula fps mascaranco os 4 primeiros bits. Exemplo -> 0x12 que é 00010010,  vira 00000010 (0x02)
    float fr = 0.0;

    printf("--> Código: %.2x ", SEQ_HEADER);
    printf("--  Sequence Header  ");
    printf("--  Width = %d  ", largura);
    printf("--  Height = %d  ", altura);

    // Mapeia o código do fps que veio em bits para o fps real. Portanto, 0x02 do exemplo acima será 2m, ou seja, 24.000
    switch(frame_rate) {
        case 1:
            fr = 23.976;
            break;
        case 2:
            fr = 24.000;
            break;
        case 3:
            fr = 25.000;
            break;
        case 4:
            fr = 29.970;
            break;
        case 5:
            fr = 30.000;
            break;
        case 6:
            fr = 50.000;
            break;
        case 7:
            fr = 59.940;
            break;
        case 8:
            fr = 30.000;
            break;
        default:
            printf("Taxa de quadros desconhecida.\n");
            return;
    }

    printf("--  Frame rate = %.3lffps\n", fr);
}


//  Lê os próximos 2 bytes do arquivo e usa esses bytes para determinar o tipo de quadro da imagem
void handle_picture(FILE *mpg_file) {
    unsigned char byte1 = fgetc(mpg_file);
    unsigned char byte2 = fgetc(mpg_file);

    unsigned char tipo = ((byte2 << 2) & 0xFF) >> 5;
    printf("--> Código: %.2x ", PICTURE);
    printf("-- Picture -- Tipo = ");

    if (tipo == 1){
        printf("I\n");
    }
    else if (tipo == 2){
        printf("P\n");
    }
    else if (tipo == 3){
        printf("B\n"); 
    }
    else {
        printf("Tipo desconhecido\n");
    }
}



// Função principal pra decodificar o arquivo MPEG
void decode_mpeg(char *file_name) {
    int i = 1;
    unsigned char buffer[BUFFER_SIZE];

    FILE *mpg_file = fopen(file_name, "rb");
    if (mpg_file == NULL){
        printf("ERRO! O arquivo não pôde ser aberto!\n");
        return;
    }

    for (; !feof(mpg_file); i++){ // Continua a leitura do arquivo até o fim
        fread(buffer, BUFFER_SIZE, 1, mpg_file);

        if (buffer[0] == '\x00' && buffer[1] == '\x00' && buffer[2] == '\x01'){
            unsigned char byte[1];
            fread(byte, 1, 1, mpg_file);

            // Lida com os diferentes tipos de bytes do arquivo com as devidas funções
            switch (byte[0]){
                case SEQ_HEADER:
                    handle_seq_header(mpg_file);
                    break;
                case GOP:
                    handle_gop();
                    break;
                case PICTURE:
                    handle_picture(mpg_file);
                    break;
                default:
                    if (byte[0] >= 0x01 && byte[0] <= 0xAF) {
                        handle_slice(byte[0]);
                    } else {
                        printf("--> Código: %.2x -- Tipo de stream não implementado\n", byte[0]);
                    }
                    break;
            }
        }
        // Se os 3 bytes lidos não forem o início de um código de início de sequência, volta 2 bytes
        else{
            fseek(mpg_file, -2, SEEK_CUR);
        }
    }
}


int main(int argc, char *argv[]){
    if (argc < 2) {
        printf("ERRO! Nenhum arquivo foi fornecido!\n");
        printf("Digite: %s <nome do arquivo>\n", argv[0]);
        return 0;
    }

    decode_mpeg(argv[MPEG_FILE_ARG]);

    return 0;
}
