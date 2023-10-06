# Split video

Podemos dividir os vídeos em quantas partes quisermos, desde que o tamanho máximo de cada parte seja menor que o tamanho do arquivo de entrada. O número de partes depende do tamanho máximo que passamos como argumento na linha de comando. Por exemplo, se passarmos 4 MB como tamanho máximo, o programa vai dividir o vídeo em partes de até 4 MB cada.  Sendo assim, podemos rodar com:

``./mpg_split_size leon.mpg 4``