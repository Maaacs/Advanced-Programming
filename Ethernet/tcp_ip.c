#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <netinet/in.h>

#define TAM_MAC 6 
#define TAM_IP 4 
#define ETHERNET_HEADER_SIZE sizeof(ethernet_header_t) // tamanho da struct
#define IP_HEADER_SIZE sizeof(ip_header_t)
#define TCP_HEADER_SIZE sizeof(tcp_header_t)

// Cabeçalho Ethernet
typedef struct {
    uint8_t daddr[TAM_MAC]; // Endereço de destino
    uint8_t saddr[TAM_MAC]; // Endereço de origem
    uint16_t protocol; 
} ethernet_header_t;

// Cabeçalho IP
typedef struct {
    uint8_t hdr_len : 4, version : 4; 
    uint8_t tos; // Tipo de serviço
    uint16_t tot_len; // Tamanho do pacote
    uint16_t id;
    uint16_t frag_off; // Deslocamento do fragmento
    uint8_t ttl; 
    uint8_t protocol;
    uint16_t check;
    uint8_t saddr[TAM_IP];
    uint8_t daddr[TAM_IP];
} ip_header_t;

// Cabeçalho TCP
typedef struct {
    uint16_t sport; // Porta de origem
    uint16_t dport; // Porta de destino
    uint32_t seq;
    uint32_t ack_seq; // Número de reconhecimento
    uint8_t reservado : 4, hdr_len : 4; 
    uint8_t flags;
    uint16_t window;
    uint16_t check;
    uint16_t urg_ptr;
} tcp_header_t;


void print_mac(uint8_t mac[TAM_MAC]) {
    for (int i = 0; i < TAM_MAC; i++) {
        printf("%.2x", mac[i]);  // hexadecimal
        if (i != TAM_MAC - 1)
            printf(":");
    }
    printf("\n");
}


void print_ip(uint8_t ip[TAM_IP]) {
    for (int i = 0; i < TAM_IP; i++) {
        printf("%d", ip[i]); // decimal
        if (i != TAM_IP - 1)
            printf(".");
    }
    printf("\n");
}

// Lê o cabeçalho Ethernet do arquivo
bool read_ethernet_header(FILE *file, ethernet_header_t *ethernet_header) {
    return fread(ethernet_header, ETHERNET_HEADER_SIZE, 1, file) == 1;
}

// Lê o cabeçalho IP do arquivo
bool read_ip_header(FILE *file, ip_header_t *ip_header) {
    return fread(ip_header, IP_HEADER_SIZE, 1, file) == 1;
}

// Lê o cabeçalho TCP do arquivo
bool read_tcp_header(FILE *file, tcp_header_t *tcp_header) {
    return fread(tcp_header, TCP_HEADER_SIZE, 1, file) == 1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Insira: %s <nome_do_arquivo>\n", argv[0]);
        return 1;
    }

    FILE *http_file = fopen(argv[1], "rb");
    if (http_file == NULL) {
        printf("ERRO! Não foi possível abrir o arquivo.\n");
        return 1;
    }

    ethernet_header_t ethernet_header;
    ip_header_t ip_header;
    tcp_header_t tcp_header;

    if (!read_ethernet_header(http_file, &ethernet_header)) {
        printf("Erro ao ler o cabeçalho Ethernet.\n");
        fclose(http_file);
        return 1;
    }

    printf("Lendo Ethernet ..\n");
    printf("--> MAC de Origem:   ");
    print_mac(ethernet_header.saddr);
    printf("--> MAC de Destino:   ");
    print_mac(ethernet_header.daddr);

    if (!read_ip_header(http_file, &ip_header)) {
        printf("Erro ao ler o cabeçalho IP.\n");
        fclose(http_file);
        return 1;
    }

    printf("Lendo IP ..\n");
    printf("--> Versão do IP:   %d\n", ip_header.version);
    printf("--> Tamanho do cabeçalho:   %d  bytes\n", ip_header.hdr_len * 4);
    printf("--> Tamanho do pacote:   %d  bytes \n", ntohs(ip_header.tot_len));
    printf("--> Endereço IP de Origem:   ");
    print_ip(ip_header.saddr);
    printf("--> Endereço IP de Destino:   ");
    print_ip(ip_header.daddr);

    // Lê os bytes restantes do cabeçalho IP
    fseek(http_file, ip_header.hdr_len * 4 - IP_HEADER_SIZE, SEEK_CUR);

    if (!read_tcp_header(http_file, &tcp_header)) {
        printf("Erro ao ler o cabeçalho TCP.\n");
        fclose(http_file);
        return 1;
    }

    printf("Lendo TCP ..\n");
    printf("--> Porta de Origem:  %d\n", ntohs(tcp_header.sport));
    printf("--> Porta de Destino:   %d\n", ntohs(tcp_header.dport));
    printf("--> Tamanho do cabeçalho:   %d   bytes\n", tcp_header.hdr_len * 4);

    // Lê os bytes restantes do cabeçalho TCP
    fseek(http_file, tcp_header.hdr_len * 4 - TCP_HEADER_SIZE, SEEK_CUR);

    // Calcula o tamanho dos dados HTTP
    int tam_dados = ntohs(ip_header.tot_len) - (ip_header.hdr_len * 4) - (tcp_header.hdr_len * 4);

    printf("Lendo Dados (HTTP) ..\n");
    printf("--> Tamanho dos dados: %d bytes\n", tam_dados);
    printf("--> Dados:\n");

    char cha;
    int i = 0;
    do {
        cha = fgetc(http_file);
        i++;
        printf("%c", cha);
    } while (i < tam_dados);

    fclose(http_file);

    return 0;
}
