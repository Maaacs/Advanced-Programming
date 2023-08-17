#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct ListaVizinhos{
    int id;
    struct ListaVizinhos *proximo;
}lista_vizinhos_t;

typedef struct No{
    int id;
    double x, y;
    lista_vizinhos_t **lista;
}no_t;

typedef no_t *grafo_t; // Vetor de nós

// Adiciona itens na lista
bool lista_vizinhos_adicionar(int vizinho, lista_vizinhos_t **lista){
    lista_vizinhos_t *novo_vizinho = malloc(sizeof(lista_vizinhos_t));
    if (!novo_vizinho){
        return false;
    }
    novo_vizinho->id = vizinho;
    novo_vizinho->proximo = *lista;
    *lista = novo_vizinho;
    return true;
}

// Imprime os itens da lista
void lista_vizinhos_imprimir(lista_vizinhos_t *lista){
    while (lista != NULL) {
        printf("%d ", lista->id);
        lista = lista->proximo;
    }
    printf("\n");
}


// Cria um grafo vazio com o tamanho especificado
grafo_t grafo_criar(int tam){
    grafo_t grafo = (grafo_t)malloc(tam * sizeof(no_t));

    if (!grafo){
        printf("Erro ao alocar memória para o grafo");
        return NULL;
    }
    for (int i = 0; i < tam; i++) {
        grafo[i].id = i;
        grafo[i].lista = NULL;
    }
    return grafo;
}
