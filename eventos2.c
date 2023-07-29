#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct {
  double tempo;
  int alvo;
  int tipo;
} evento_t;

typedef struct lista_eventos_t{
  evento_t *evento;
  struct lista_eventos_t *proximo; 
}lista_eventos_t;


lista_eventos_t* criarLSE(){
    lista_eventos_t* lista_eventos = malloc(sizeof(lista_eventos_t));
    lista_eventos->evento = NULL;
    lista_eventos->proximo = NULL;
    return lista_eventos;
}

lista_eventos_t* criarEventoLSE(evento_t *carga){
    lista_eventos_t* novo_evento = malloc(sizeof(lista_eventos_t));
    novo_evento->evento = carga;
    novo_evento->proximo = NULL;

    return novo_evento;
}

bool lista_eventos_adicionar_inicio(evento_t *evento, lista_eventos_t **lista) {
  if (!lista) {
    return false; // Retorna falso se o evento ou a lista forem nulos
  }

  lista_eventos_t *novo_evento = criarEventoLSE(evento);
  if (!novo_evento) {
    return false; // Falha na alocação de memória para o novo evento
  }

  novo_evento->proximo = *lista;
  *lista = novo_evento;

  return true;
}
// bool lista_eventos_adicionar_inicio(evento_t *evento, lista_eventos_t **lista) {

//     if (!lista) {
//         return false; // Retorna falso se o evento ou a lista forem nulos
//     }

//     lista_eventos_t *novo_evento = criarEventoLSE(evento);
//     if (evento == NULL){
//          *lista = novo_evento;
//     }else{
//         novo_evento->proximo = *lista;
//         *lista = novo_evento;
//     }
//     return true;
// }

// bool lista_eventos_adicionar_fim(evento_t *evento, lista_eventos_t **lista) {
//     if (!lista) {
//         return false; // Retorna falso se o evento ou a lista forem nulos
//     }

//     lista_eventos_t *novo_evento = criarEventoLSE(evento);
//     if (!novo_evento) {
//         printf("max");
//         return false; // Falha na alocação de memória para o novo evento
//     }
    
//     if(*lista == NULL){
//         *lista = novo_evento;
//     }else{
//         // Encontra o último item da lista
//         lista_eventos_t *ultimo_evento = *lista;
//         while (ultimo_evento->proximo != NULL) {
//         // printf("não é null");
//         ultimo_evento = ultimo_evento->proximo;
//     }
//         // Adiciona o novo evento no final da lista
//         ultimo_evento->proximo = novo_evento;
//     }
//     return true;
// }


bool lista_eventos_adicionar_fim(evento_t *evento, lista_eventos_t **lista) {
    if (!lista) {
        return false; // Retorna falso se a lista for nula
    }

    lista_eventos_t *novo_evento = criarEventoLSE(evento);
    if (!novo_evento) {
        return false; // Falha na alocação de memória para o novo evento
    }

    // Se a lista estiver vazia, o novo evento será o primeiro (início da lista)
    if (*lista == NULL) {
        *lista = novo_evento;
    } else {
        // Encontra o último nó da lista
        lista_eventos_t *ultimo = *lista;
        while (ultimo->proximo != NULL) {
            ultimo = ultimo->proximo;
        }

        // Adiciona o novo evento como próximo do último nó
        ultimo->proximo = novo_evento;
    }

    return true;
}



void imprimirInfo(evento_t *info){
  printf("%lf %d %d\n", info->tempo, info->alvo, info->tipo);
}


void lista_eventos_listar(lista_eventos_t *lista){
  lista_eventos_t *info = lista;
  printf("inicio da impressão\n");
  while(info!=NULL){ // aqui tbm gerou problema de Segmentation fault
    //printf("%p ", info);
    imprimirInfo(info->evento);
    info = info->proximo;
  }
  printf("\n");
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
  lista_eventos_t *lista_eventos = NULL;
  // lista_eventos_t *lista_eventos = criarLSE(); o problema de Segmentation fault (core dumped) estava aqui
  double tempo;
  int alvo, tipo;

//  continua lendo as linhas do arquivo enquanto todas as três atribuições forem bem sucedidas
  while (fscanf(arquivo, "%lf\t%d\t%d\n", &tempo, &alvo, &tipo) == 3) {
    // Cria um novo evento com os valores lidos
    evento_t *novo_evento = (evento_t *)malloc(sizeof(evento_t));

    novo_evento->tempo = tempo;
    novo_evento->alvo = alvo;
    novo_evento->tipo = tipo;



    lista_eventos_adicionar_inicio(novo_evento, &lista_eventos);
    lista_eventos_adicionar_fim(novo_evento, &lista_eventos);
    //lista_eventos_adicionar_ordenado(&novo_evento, &(lista_eventos->inicio));

  }

  fclose(arquivo);

    if (!lista_eventos) {
        printf("A lista está vazia.\n");
    } else {
        lista_eventos_listar(lista_eventos);
    }

  return 0;
}

