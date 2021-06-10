# digital-image-processing
Trabalho desenvolvido durante a disciplina de Processamento Digital de Imagens - Unesp. 

O objetivo deste trabalho consiste no desenvolvimento de um sistema de manipulação de imagens PGM (em tons de cinza) e PPM (colorida). Para isso, foi utilizada a lingagem C/C++ e a IDE Code Blocks para a manipulação do projeto.

Ao executar a aplicação é solicitado ao usuário a operação que o mesmo deseja fazer: ler e salvar uma imagem, soma e subtração entre duas imagens, soma e multiplicação de um valor, fatiamento de níveis de intensidade, equalização de histograma e aplicação de filtros (média, mediana e laplaciano), etc.

A aplicação permite, ainda, além da execução iterativa das operações sobre uma imagem, a leitura de um arquivo texto com as operações a serem feitas, automatizando o processo. Para essa funcionalidade (operação 1), é solicitado um arquivo de texto que descreve as operações de acordo com a seguinte sintaxe:

● Leitura de uma imagem PGM (extensão pgm) ou PPM (extensão ppm): leitura nome_arquivo.extensao

● Somar a imagem com um valor: somar-valor valor

● Multiplicar a imagem com um valor: multiplicar-valor valor

● Somar a imagem com outra imagem (mesma extensão): somar-imagem nome_arquivo2.extensao

● Subtrair da imagem outra imagem (mesma extensão): subtrair-imagem nome_arquivo2.extensao

● Fatiamento de níveis de intensidade:

  ○ pixels no intervalo [A,B] recebem valor1 e fora recebem valor2: fatiamento1 A B valor1 valor2

  ○ pixels no intervalo [A,B] recebem valor1 e fora não ocorre alterações: fatiamento2 A B valor1

● Aplicar filtro na imagem:

  ○ filtro da média de dimensão n por n: filtro-media n
  
  ○ filtro da mediana de dimensão n por n: filtro-mediana n
  
  ○ filtro laplaciano 3x3 com o elemento central x: filtro-laplaciano x

● Equalizar histograma: equalizar-histograma

● Extrair canais RGB (apenas para imagem PPM) e salvar em 3 arquivos (PGM): extrair-canais nome_arquivoR.pgm nome_arquivoG.pgm nome_arquivoB.pgm

● Escrita de uma imagem resultado PGM (extensão pgm) ou PPM (extensão ppm): escrita nome_arq_result.extensao

● Compor canais RGB (3 arquivos PGM, ou seja, os canais, irão compor 1 PPM): compor-canais nome_arquivoR.pgm nome_arquivoG.pgm nome_arquivoB.pgm nome_arquivoRGB.ppm
