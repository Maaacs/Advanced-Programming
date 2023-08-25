# Strings e Quebra senha

As saídas dos testes realizados estão disponíveis no diretório Testes.

----

Para o primeiro programa [strings .c](./strings.c)

Execute o com -lm para incluir as libs matemáticas `sqrt` e `pow` na compilação.
```
gcc strings.c -o executavel -lm 
```

```
./executavel 3 abc
```

----

Para o segundo programa [quebraSenha.c](./quebraSenha.c)

Execute o programa com -lm e -lcrypt para incluir as libs na compilação 
```
gcc quebraSenha.c -o quebra_senha -lcrypt -lm
```

```
./quebra_senha
```


[Contribution guidelines for this project](./quebraSenha.c)