#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NOME 51 // 50 caracteres no vetor com último nulo \0

typedef struct pessoa {
    char nome[MAX_NOME];
    long long int cpf;
    int idade;
} pessoa_t;

typedef struct lista_pessoas {
    pessoa_t pessoa;
    struct lista_pessoas *proximo;
} lista_pessoas_t;

typedef lista_pessoas_t** tabela_hash_t; // vetor de ponteiros para a LSE

int tabela_hash_tam; // Tamanho da Tabela Hash



// Adiciona uma pessoa a uma lista encadeada 
bool lista_pessoas_adicionar(pessoa_t *pessoa, lista_pessoas_t **lista) {
    lista_pessoas_t *novo = (lista_pessoas_t*)malloc(sizeof(lista_pessoas_t));
    if (novo == NULL) {
        return false;
    }
    novo->pessoa = *pessoa;
    novo->proximo = *lista;
    *lista = novo;
    return true;
}

// Imprime as pessoas em uma lista encadeada. 
void lista_pessoas_listar(lista_pessoas_t *lista) {
    while (lista != NULL) {
        printf("- %s %lld %d\n", lista->pessoa.nome, lista->pessoa.cpf, lista->pessoa.idade);
        lista = lista->proximo;
    }
}

// Aloca memória para um vetor de ponteiros para LSE. Cada ponteiro de lista (elemento do vetor) apontar para nulo.
tabela_hash_t tabela_hash_pessoas_criar() {
    tabela_hash_t tabela = (tabela_hash_t)malloc(sizeof(lista_pessoas_t*) * tabela_hash_tam);
    for (int i = 0; i < tabela_hash_tam; i++) {
        tabela[i] = NULL;
    }
    return tabela;
}

// Calcula a função Hash de cada pessoa (resto da divisão do CPF pelo tamanho da Tabela Hash).
int tabela_hash_pessoas_funcao(pessoa_t *pessoa) {
    return (int)(pessoa->cpf % tabela_hash_tam);
}

// Adiciona uma pessoa à tabela hash.
bool tabela_hash_pessoas_adicionar(pessoa_t *pessoa, tabela_hash_t tabela_hash) {
    int posicao = tabela_hash_pessoas_funcao(pessoa);
    return lista_pessoas_adicionar(pessoa, &tabela_hash[posicao]);
}

// Lista as pessoas da tabela hash.
void tabela_hash_pessoas_listar(tabela_hash_t tabela_hash) {
    int encontrou_pessoas = 0;
    for (int i = 0; i < tabela_hash_tam; i++) {
        if(tabela_hash[i]){
            printf("POSIÇÃO %d DA TABELA HASH:\n", i);
            lista_pessoas_listar(tabela_hash[i]);
            encontrou_pessoas = 1;
        }
    }
    if(!encontrou_pessoas){ // Verifica se o arquivo contém os dados,.
        printf("A tabela está vazia.\n");
    }
}

int main(int argc, char **argv) {
    // Verifica se o número de argumentos passados é menor que o necessário
    if (argc < 3) {
        printf("Digite: %s <tamanho da tabela hash> <arquivo txt>\n", argv[0]);
        return 1;
    }

    // Apenas para ler o tamanho da tabela hash
    sscanf(argv[1], "%d", &tabela_hash_tam);
    tabela_hash_t tabela_hash = tabela_hash_pessoas_criar();

    // Abre o arquivo
    FILE *arq_externo = fopen(argv[2], "r");
    // Verifica se o arquivo existe
    if (arq_externo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    pessoa_t pessoa;
    // Lê string de max 50 caracteres até encontrar o tab "\t"
    while (fscanf(arq_externo, "%50[^\t]\t%lld\t%d\n", pessoa.nome, &pessoa.cpf, &pessoa.idade) == 3) {
        tabela_hash_pessoas_adicionar(&pessoa, tabela_hash);
    }
    fclose(arq_externo);

    
    tabela_hash_pessoas_listar(tabela_hash);

    // Libera a memória alocada
    for (int i = 0; i < tabela_hash_tam; i++) {
        lista_pessoas_t *lista = tabela_hash[i];
        while (lista != NULL) {
            lista_pessoas_t *proximo = lista->proximo;
            free(lista);
            lista = proximo;
        }
    }
    free(tabela_hash);

    return 0;
}
