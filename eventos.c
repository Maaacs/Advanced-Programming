#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


// Implemente uma estrutura chamada evento_t para armazenar um evento. Esta estrutura deverá ter os campos tempo (double), alvo (int) e tipo (int).
typedef struct {
  double tempo;
  int alvo;
  int tipo;
} evento_t;

// Implemente uma estrutura chamada lista_eventos_t para a lista encadeada. Esta estrutura deverá ter um
// campo para apontar para um evento (evento_t*) e um campo para apontar para o proximo evento
// (lista_eventos_t*). Uma lista nova/vazia irá apontar para NULL, portanto não há necessidade de se
// implementar uma função que crie uma nova lista. 


// structs auxiliares
typedef struct lista_eventos{
  evento_t cargautil;
  struct lista_eventos* proximo; // sucessor
}lista_eventos_t;

typedef struct lista TListaSE;
struct lista{
  lista_eventos_t* inicio;
  int tamanho;
};


TListaSE* criaLista(){
  TListaSE* lista = malloc(sizeof(TListaSE));

  lista->inicio = NULL;
  lista->tamanho = 0;

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

void lista_eventos_adicionar_inicio(TListaSE* evento, evento_t lista){
  lista_eventos_t *novo = criarElementoLSE(lista);

  evento->tamanho++;
  if (evento->inicio == NULL){
    evento->inicio = novo;
  }else{
    novo->proximo = evento->inicio;
    evento->inicio = novo;
  }
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

void lista_eventos_adicionar_fim(TListaSE* evento, evento_t lista){
  lista_eventos_t *novo = criarElementoLSE(lista);
  evento->tamanho++;
  if (evento->inicio == NULL){
    evento->inicio = novo;
  }else{
    lista_eventos_t *cam = evento->inicio;
    while (cam->proximo!=NULL){
      cam = cam->proximo;
    }
    cam->proximo = novo;
  }
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

    lista_eventos_adicionar_inicio(lista_eventos, novo_evento);
  }

  fclose(arquivo);

  lista_eventos_listar(lista_eventos);

  return 0;
}

