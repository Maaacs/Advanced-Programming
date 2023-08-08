#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


// Implemente uma estrutura chamada evento_t para armazenar um evento.
typedef struct {
  double tempo;
  int alvo;
  int tipo;
} evento_t;

// Implemente uma estrutura chamada lista_eventos_t para a lista encadeada.
typedef struct lista_eventos{
  evento_t cargautil;
  struct lista_eventos* proximo; 
}lista_eventos_t;

// struct auxiliare
typedef struct lista TListaSE;
struct lista{
  lista_eventos_t* inicio;
  //int tamanho; //flag para futuras implementações
};


TListaSE* criaLista(){
  TListaSE* lista = malloc(sizeof(TListaSE));
  lista->inicio = NULL;
//   lista->tamanho = 0;
  return lista;
}

lista_eventos_t* criarElementoLSE(evento_t carga){
  lista_eventos_t* elem = malloc(sizeof(lista_eventos_t));
  elem->cargautil = carga;
  elem->proximo=NULL;

  return elem;
}

// Implemente uma função para adicionar um evento no início da lista. Esta função deverá ter a seguinte interface:
// bool lista_eventos_adicionar_inicio(evento_t *evento, lista_eventos_t **lista);

bool lista_eventos_adicionar_inicio(TListaSE* evento, evento_t lista){
  lista_eventos_t *novo = criarElementoLSE(lista);

//   evento->tamanho++;
  if (evento->inicio == NULL){
    evento->inicio = novo;
  }else{
    novo->proximo = evento->inicio;
    evento->inicio = novo;
  }
  return true;
}


// Implemente também uma função para listar os eventos de uma lista. Esta função deverá ter a seguinte interface:

void imprimirInfo(evento_t info){
  printf("%lf %d %d\n", info.tempo, info.alvo, info.tipo);
//   printf("Vou imprimir a informação\n");
}

void lista_eventos_listar(TListaSE *lista){
  lista_eventos_t* cam = lista->inicio;
  printf("inicio da impressão\n");
  while(cam!=NULL){
    //printf("%p ", cam);
    imprimirInfo(cam->cargautil);
    cam = cam->proximo;
  }
  printf("\n");
}


// Implemente uma função para adicionar um evento no final da lista. Esta função deverá ter a seguinte interface:
// bool lista_eventos_adicionar_fim(evento_t *evento, lista_eventos_t **lista);

bool lista_eventos_adicionar_fim(TListaSE* evento, evento_t lista){
  lista_eventos_t *novo = criarElementoLSE(lista);
//   evento->tamanho++;
  if (evento->inicio == NULL){
    evento->inicio = novo;
  }else{
    lista_eventos_t *cam = evento->inicio;
    while (cam->proximo!=NULL){
      cam = cam->proximo;
    }
    cam->proximo = novo;
  }
  return true;
}


bool lista_eventos_adicionar_ordenado(evento_t *evento, lista_eventos_t **lista) {
  lista_eventos_t *novo = criarElementoLSE(*evento);

  // Se a lista for NULL, faça-a apontar para o novo item.
  if (*lista == NULL) {
    *lista = novo;
    return false;
  }

  // Se o tempo do novo evento for menor do que o tempo do primeiro item da lista, adicione o item no início da lista.
  if (evento->tempo < (*lista)->cargautil.tempo) {
    novo->proximo = *lista;
    *lista = novo;
    return false;
  }

  // Percorra a lista para encontrar a posição adequada para inserir o novo evento.
  lista_eventos_t *item_atual = *lista;
  while (item_atual->proximo != NULL && item_atual->proximo->cargautil.tempo < evento->tempo) {
    item_atual = item_atual->proximo;
  }

  // Adicione o novo evento como próximo do item_atual.
  novo->proximo = item_atual->proximo;
  item_atual->proximo = novo;

  return true;
}


int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Uso: %s nome_do_arquivo\n", argv[0]);
    return 1;
  }

  FILE* arquivo = fopen(argv[1], "r");
  if (!arquivo) {
    printf("Erro ao abrir o arquivo.\n");
    return 1;
  }

  TListaSE* lista_eventos = criaLista();
  double tempo;
  int alvo, tipo;

//  continua lendo as linhas do arquivo enquanto todas as três atribuições forem bem sucedidas
  while (fscanf(arquivo, "%lf\t%d\t%d\n", &tempo, &alvo, &tipo) == 3) {
    evento_t novo_evento;
    novo_evento.tempo = tempo;
    novo_evento.alvo = alvo;
    novo_evento.tipo = tipo;

    //lista_eventos_adicionar_inicio(lista_eventos, novo_evento);
    //lista_eventos_adicionar_fim(lista_eventos, novo_evento);
    lista_eventos_adicionar_ordenado(&novo_evento, &(lista_eventos->inicio));

  }

  fclose(arquivo);

  lista_eventos_listar(lista_eventos);

  return 0;
}

