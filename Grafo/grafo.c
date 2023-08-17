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


// Insere os itens pra cada elemento do grafo
bool grafo_atualizar_vizinhos(int tam, double raio_comunicacao, grafo_t grafo) {
    for (int i = 0; i < tam; i++) { // Percorre todos os nós do grafo
        for (int j = 0; j < tam; j++) { // Compara o nó atual (i) com todos os outros nós (j)
            if (i != j) {
                // Calcula a distância euclidiana entre os nós 
                double distancia = sqrt(pow(grafo[i].x - grafo[j].x, 2) + pow(grafo[i].y - grafo[j].y, 2));
                if (distancia < raio_comunicacao) {
                    // Verifica se ainda não existe uma lista de adjacências para o nó j
                    if (grafo[j].lista == NULL) {
                        lista_vizinhos_t **lista_vizinhos_j = (lista_vizinhos_t **)malloc(sizeof(lista_vizinhos_t **));
                        if (lista_vizinhos_j == NULL) {
                            return false; 
                        }
                        grafo[j].lista = lista_vizinhos_j; // Vincula a lista criada para um determinado nó
                    }
                    int id = grafo[i].id; // Obtém o ID do nó i
                    bool inseriu = lista_vizinhos_adicionar(id, grafo[j].lista); // Adiciona o ID do nó i à lista de vizinhos do nó j
                    // Enquanto não inserir, tenta novamente
                    while (!inseriu) {
                        inseriu = lista_vizinhos_adicionar(id, grafo[j].lista);
                    }
                }
            }
        }
    }

    return true;
}

void grafo_imprimir(int tam, grafo_t grafo){
    for (int i = 0; i < tam; i++) {
        printf("NO %d:  ", grafo[i].id);
        lista_vizinhos_imprimir(*grafo[i].lista);
    }
}
