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


lista_eventos_t* criarEventoLSE(evento_t *carga){
    lista_eventos_t* novo_evento = malloc(sizeof(lista_eventos_t));
    novo_evento->evento = carga;
    novo_evento->proximo = NULL;
    return novo_evento;
}

bool lista_eventos_adicionar_inicio(evento_t *evento, lista_eventos_t **lista) {
    lista_eventos_t *novo_evento = criarEventoLSE(evento);
    if(!novo_evento){
      return false; // Falha na alocação de memória para o novo evento
    }
    
    if (!lista){ // Se lista null
         *lista = novo_evento; // Adiciona direto no início
    }else{
        novo_evento->proximo = *lista; // Move para trás o evento 
        *lista = novo_evento; // Adiciona no início o novo evento
    }
    return true;
}


bool lista_eventos_adicionar_fim(evento_t *evento, lista_eventos_t **lista) {
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


bool lista_eventos_adicionar_ordenado(evento_t *evento, lista_eventos_t **lista) {
  lista_eventos_t *novo = criarEventoLSE(evento);

  // Se a lista for NULL, aponta para o novo item
  if (*lista == NULL) {
    *lista = novo;
    return false;
  }

  // Se o tempo do novo evento for menor do que o tempo do primeiro item da lista, adiciona o item no início da lista
  if (evento->tempo < (*lista)->evento->tempo) {
    novo->proximo = *lista;
    *lista = novo;
    return false;
  }

  // Percorre a lista para encontrar a posição adequada para inserir o novo evento (ordenadamente)
  lista_eventos_t *item_atual = *lista;
  while (item_atual->proximo != NULL && item_atual->proximo->evento->tempo < evento->tempo) {
    item_atual = item_atual->proximo;
  }

  // Adicione o novo evento como próximo do item_atual
  novo->proximo = item_atual->proximo;
  item_atual->proximo = novo;

  return true;
}


void imprimirInfo(evento_t *info){
  printf("Tempo: %3.6f Alvo: %d Tipo: %d\n",info->tempo, info->alvo, info->tipo);
}


void lista_eventos_listar(lista_eventos_t *lista){
  lista_eventos_t *info = lista;

  printf("\n[RESULTADOS]\n");
  while(info!=NULL){ 
    //printf("%p ", info); // Imprime o endereço
    imprimirInfo(info->evento);
    info = info->proximo;
  }
  printf("\n");
}

// Seleciona uma das 3 opções de inserção na lista encadeada
bool seleciona_funcao_adicionar(evento_t *evento, lista_eventos_t **lista, int opcao) {
    switch (opcao) {
        case 1:
            return lista_eventos_adicionar_inicio(evento, lista);
        case 2:
            return lista_eventos_adicionar_fim(evento, lista);
        case 3:
            return lista_eventos_adicionar_ordenado(evento, lista);
        default:
            printf("Opção inválida. Não foi possível adicionar o evento.\n");
            return false;
    }
}


int main(int argc, char* argv[]) {
    // Verifica se o número de argumentos passados é diferente de 2
    if (argc != 2) {
        printf("Digite: %s nome_do_arquivo.txt\n", argv[0]);
        return 1;
    }

    FILE* arquivo = fopen(argv[1], "r");
  
    // Verifica se o arquivo existe
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    lista_eventos_t *lista_eventos = NULL;
    double tempo;
    int alvo, tipo, opcao;

    // Exibe as opções de inserção para o usuário
    printf("Qual tipo de inserção deseja usar?\n");
    printf("Digite 1 para lista_eventos_adicionar_inicio\n");
    printf("Digite 2 para lista_eventos_adicionar_fim\n");
    printf("Digite 3 para lista_eventos_adicionar_ordenado\n");
    // Lê a opção
    scanf("%d", &opcao);

    // Verifica se a opção é válida antes de prosseguir
    if (opcao < 1 || opcao > 3) {
        printf("Opção inválida. Encerrando o programa.\n");
        return 1;
    }

    // Continua lendo as linhas do arquivo e adicionando os eventos na lista usando a seleciona_funcao_adicionar
    while (fscanf(arquivo, "%lf\t%d\t%d\n", &tempo, &alvo, &tipo) == 3) {
        evento_t *novo_evento = (evento_t *)malloc(sizeof(evento_t));
        novo_evento->tempo = tempo;
        novo_evento->alvo = alvo;
        novo_evento->tipo = tipo;
        seleciona_funcao_adicionar(novo_evento, &lista_eventos, opcao);
    }

    fclose(arquivo);

    // Verifica se os dados foram lidos e carregados corretamente
    if (!lista_eventos) {
      printf("A lista está vazia.\n");
    } else {
      lista_eventos_listar(lista_eventos);
    }

    return 0;
}

