#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


typedef struct ListaVizinhos{
    int id;
    struct ListaVizinhos *proximo;
}lista_vizinhos_t;

typedef struct No{
    int id;
    double x, y;
    bool FLAG_PACOTE_STATUS;
    lista_vizinhos_t **lista;
}no_t;

typedef no_t *grafo_t; // Vetor de nós

typedef struct Evento{
    double tempo;
    int alvo;
    int tipo;
}evento_t;

typedef struct ListaEventos{
    evento_t *evento;
    struct ListaEventos *proximo_lista_eventos;
}lista_eventos_t;



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


// Lê os nós do arquivo e preenhce o grafo
bool ler_nohs(FILE *arquivo, int num_nos, grafo_t grafo) {
    for (int i = 0; i < num_nos; i++) {
        no_t no_atual;
        int id;
        double pos_x, pos_y;
        if (fscanf(arquivo, "%d\t%lf\t%lf\n", &id, &pos_x, &pos_y) != 3) {
            return false;
        }
        no_atual.id = id;
        no_atual.x = pos_x;
        no_atual.y = pos_y;
        grafo[i] = no_atual;
        grafo[i].lista = NULL;
    }
    return true; 
}


// Imprime o grafo e suas listas
void grafo_imprimir(int tam, grafo_t grafo){
    for (int i = 0; i < tam; i++) {
        printf("NÓ %d:  ", grafo[i].id); // Imprime o ID do nó
        lista_vizinhos_imprimir(*grafo[i].lista);
    }
}


// Libera a memória para o grafo e listas
void free_grafo(int tam, grafo_t grafo) {
    for (int i = 0; i < tam; i++) {
        if (grafo[i].lista != NULL) {
            lista_vizinhos_t *atual = *grafo[i].lista;

            while (atual != NULL) {
                lista_vizinhos_t *prox = atual->proximo; // Armazena o próximo antes de liberar
                free(atual);
                atual = prox; // Avança para o próximo nó
            }

            free(grafo[i].lista);
            grafo[i].lista = NULL; // Limpa o ponteiro da lista
        }
    }
    free(grafo);
}



bool lista_eventos_adicionar_ordenado(evento_t *evento, lista_eventos_t **lista){
    
    lista_eventos_t *novo_evento = malloc(sizeof(lista_eventos_t));
    novo_evento->evento = evento;

    // Se a lista for NULL, aponta para o novo item
    if (*lista == NULL){
        if (novo_evento == NULL)
            return false;
        else{
            novo_evento->evento = evento;
            novo_evento->proximo_lista_eventos = NULL;
            *lista = novo_evento;
            return true;
        }
    }

    lista_eventos_t *atual_evento = *lista;
    if (atual_evento->proximo_lista_eventos == NULL){
        if (novo_evento == NULL || atual_evento == NULL){
            return false;
        }
         
        // Se o tempo do novo evento for menor do que o tempo do primeiro item da lista, adiciona o item no início da lista
        if (evento->tempo < atual_evento->evento->tempo){
            novo_evento->proximo_lista_eventos = atual_evento;
            *lista = novo_evento;
            atual_evento->proximo_lista_eventos = NULL;
            return true;
        }

        atual_evento->proximo_lista_eventos = novo_evento;
        return true;
    }

    // Percorre a lista para encontrar a posição adequada para inserir o novo evento (ordenadamente)
    while (atual_evento->proximo_lista_eventos != NULL && atual_evento->proximo_lista_eventos->evento->tempo < evento->tempo){
        atual_evento = atual_evento->proximo_lista_eventos;
    }

    // Adicione o novo evento como próximo do item_atual
    novo_evento->proximo_lista_eventos = atual_evento->proximo_lista_eventos;
    atual_evento->proximo_lista_eventos = novo_evento;

    return true;
}



void simulacao_iniciar(lista_eventos_t **lista, int tam, grafo_t grafo) {
    while (*lista != NULL) {
        lista_eventos_t *elemento_inicio_lista = *lista;
        *lista = elemento_inicio_lista->proximo_lista_eventos; // Pega o primeiro evento da lista de eventos
        elemento_inicio_lista->proximo_lista_eventos = NULL;

        double tempo = elemento_inicio_lista->evento->tempo;
        int alvo = elemento_inicio_lista->evento->alvo;

        printf("[%3.6f] Nó %d recebeu pacote\n", tempo, alvo); // Imprime a chegada do pacote no noh alvo

        // Verifica se o noh alvo ja recebeu um pacote
        if (!grafo[alvo].FLAG_PACOTE_STATUS) {
            lista_vizinhos_t *vizinho = *grafo[alvo].lista;
            while (vizinho != NULL) {
                printf("--> Repassando pacote para o nó %d ... \n", vizinho->id); // Repassa para os vizinhos do noh alvo
                evento_t *novo = (evento_t *)malloc(sizeof(evento_t)); // Cria um novo evento para o vizinho
                novo->alvo = vizinho->id;
                novo->tipo = 1;
                novo->tempo = tempo + (0.1 + (vizinho->id * 0.01));
                
       
                bool adicionar = lista_eventos_adicionar_ordenado(novo, lista);
                while (!adicionar) {
                    adicionar = lista_eventos_adicionar_ordenado(novo, lista);
                }
                vizinho = vizinho->proximo;
            }
            grafo[alvo].FLAG_PACOTE_STATUS = true;
        }
        free(elemento_inicio_lista->evento); // Libera o evento
        free(elemento_inicio_lista); // Libera elemento o elemento processado em cada iteracao
    }
}




int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        printf("Digite: <./executável> <arquivo_de_entrada>\n");
        return 1;
    }

    FILE* arquivo = fopen(argv[1], "r");
    

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de entrada\n");
    } else {
        int tam = 0;          
        double raio = 0.0;    

        if (fscanf(arquivo, "%d\t%lf\n", &tam, &raio) != 2) {
            printf("Erro ao ler parâmetros iniciais\n");
            return 1;
        }

        // Cria um grafo vazio com o numero de nohs especificado
        grafo_t GRAFO = grafo_criar(tam);

        if (GRAFO == NULL) {
            printf("ERRO ao alocar memória\n");
            return 1;
        }

        // Lê os nohs do arquivo e preenche o grafo
        if (!ler_nohs(arquivo, tam, GRAFO)) {
            printf("Erro ao ler nós do arquivo\n");
            return 1;
        }

        // Atualiza a lista de vizinhos de cada noh do grafo com base no raio de comunicaçao
        if (!grafo_atualizar_vizinhos(tam, raio, GRAFO)) {
            printf("Erro ao atualizar vizinhos\n");
            return 1;
        }

        // Cria um evento inicial e adiciona a lista de eventos
        evento_t* novo = (evento_t*)malloc(sizeof(evento_t));
        lista_eventos_t** lista_eventos = (lista_eventos_t**)malloc(sizeof(lista_eventos_t));
        *lista_eventos = NULL;

        if (novo == NULL || lista_eventos == NULL) {
            printf("Erro na alocação de memória para eventos\n");
            return 1;
        }

        // Valores do evento inicial
        novo->alvo = 0;
        novo->tipo = 1;
        novo->tempo = 1.0;

        // Adiciona o evento inicial à lista de eventos ordenadamente
        if (!lista_eventos_adicionar_ordenado(novo, lista_eventos)) {
            printf("Erro ao adicionar evento inicial\n");
            return 1;
        }

        simulacao_iniciar(lista_eventos, tam, GRAFO);

        free_grafo(tam, GRAFO);
        free(lista_eventos);
        fclose(arquivo);
    }

    return 0;
}
