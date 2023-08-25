#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <crypt.h>
#include <time.h>

// Sabendo que uma senha tem, no máximo, 4 caracteres e contém apenas letras minúsculas
char *senhaCriptografada = "$6$7yCakIXevncmT6se$m002Lkf2BK6Qgyhc.c/PxMTvcmBAXYtIo"
                          "kUKvWwvB5H5zCt5HhhPOlV8ygebOcSsgNqG74whVwN.8UF9WaGfs/";

// Sabendo que uma senha tem, no máximo, 4 caracteres e contém apenas números
// char *senhaCriptografada = "$6$LrSF5BAseToYYHJ0$SYY1avj8FRoRGpn.1kPXuZ6Xn5WTl2kL3"
//                            "hxc3yMWdDUyz4c/Ac3Av3WO8Q9LciP8o4c9WaeLcgxIXWaHpJMFb.";

// Sabendo que uma senha tem, no máximo, 3 caracteres e contém apenas letras minúsculas
// char *senhaCriptografada = "$6$rMAk28dVkWjpYoA3$SkWbPYqEB8O/10ryvvjm1qN9BOrkeBOXp"
//                            "JScVSGDL5L88OIs0UCBuP.pnd9TQ6SBx60dLKwR9WAzfnLtvjGvj.";

// Sabendo que uma senha tem, no máximo, 5 caracteres e contém apenas números
// char *senhaCriptografada = "$6$l2xE4w9twgjtnZBz$9YK9krslZFraLffy5VNiahAfT.xZNvB54"
//                            "j91DMCMIoVFvj335ZKxb11qgVMn.KzU2GqVPPyS2FTBqPSciYq761";


int verificaSenha(char* senhaTeste) {
    char *senhaTesteCriptografada = crypt(senhaTeste, senhaCriptografada);
    return strcmp(senhaTesteCriptografada, senhaCriptografada);
}

double calcularTempo(clock_t inicio, clock_t fim) {
    double tempo_execucao = (fim - inicio) / CLOCKS_PER_SEC;
    printf("\nTempo de execução: ~%f segundos\n", tempo_execucao);
}

int main(int argc, char *argv[]) {
    clock_t inicio = clock();
    int tamanhoMaximo = 4;
    char caracteres[] = "abc";
    // char caracteres[] = "abcdefghijklmnopqrstuvwxyz";
    // char caracteres[] = "1234567890";
    int tamanhoPalavra = strlen(caracteres);

    for (int x = 1; x <= tamanhoMaximo; x++) {
        int numero_total_possibilidades = pow(tamanhoPalavra, x);
        for (int i = 0; i < numero_total_possibilidades; i++) {
            int indice_atual = i;

            char senhaTeste[x + 1];

            for (int j = 0; j < x; j++) {
                senhaTeste[j] = caracteres[indice_atual % tamanhoPalavra];
                printf("%c", senhaTeste[j]);
                indice_atual = indice_atual / tamanhoPalavra; 
            }

            senhaTeste[x] = '\0';

            if (verificaSenha(senhaTeste)) {
                printf("--> Não\n");
            }
            if (!verificaSenha(senhaTeste)) {
                printf("--> Sim! Senha encontrada!\n");
                clock_t fim = clock();
                double tempo_execucao = calcularTempo(inicio, fim);
                return 0;
            }
        }
    }
    clock_t fim = clock();
    double tempo_execucao = calcularTempo(inicio, fim);
    return 0;
}
