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

// Adiciona items na lista
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
