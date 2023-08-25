#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char *argv[]){
    int tamanhoMaximo = atoi(argv[1]);                 // Converte para int
    int tamanhoPalavra = strlen(argv[2]);    

    for (int x = 1; x <= tamanhoMaximo; x++){      // Iteracao da menor combinação possível ate o tamanhoMaximo das combinações     
        int numero_total_possibilidades = pow(tamanhoPalavra, x);   // Numero total de possibilidades eh = n^k (tamanhoPalavra^tamanhoMaximo) (exponenciação)
        for (int i = 0; i < numero_total_possibilidades; i++){        // Itera até numero de possibilidades
            int indice_atual = i;                      

            for (int j = 0; j < x; j++){      
                printf("%c", argv[2][indice_atual % tamanhoPalavra]);  // Imprime o caractere da no idice da posição atual
                indice_atual = indice_atual / tamanhoPalavra;         // Proxima posicao          
            }

            printf("\n");                    
        }
    }

    printf("\n");                             

    return 0;
}
