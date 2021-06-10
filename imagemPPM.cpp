#include "imagemPPM.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <list>

using namespace std;

/* Leitura de arquivo PPM (ASCII): Construtor que aceita como parâmetro um arquivo, ou seja, a imagem que será lida, e transfere os dados deste arquivo para o objeto da imagem */
imagemPPM::imagemPPM(FILE *arq)
{
    int i, j, k;
    rewind(arq);        //Reposicionando o ponteiro do arquivo, para garantir que ele está no início
    fscanf(arq, "%s", this->tipo);        //Seguindo a estrutura de uma imagem PPM, o primeiro item se refere ao tipo da imagem, dessa forma, coloca-se este valor no atributo tipo do objeto
    if(strcmp(this->tipo,"P3") != 0)        //Verificando se o tipo é válido, ou seja, se a imagem é realmente PPM
    {
        printf("\n      [ERRO] Arquivo não é PPM: [%s]", this->tipo);       //Se não for, exibe uma mensagem de erro
        fclose(arq);        //Fecha o arquivo
        return;     //E retorna
    }

    //Tratamento de comentário: é entendido que o comentário é o próximo/segundo componente da imagem
    char ch[50];
    fgets(ch, 50, arq); //Lendo a quebra de linha da primeira linha
    fgets(ch, 50, arq); //Lendo o suposto comentário
    if(ch[0] != '#' || strcmp(ch, "") == 0)    //Se não tiver comentário
        fseek(arq, sizeof(char[3]), 0);     //Volta para o fim do tipo da imagem (string com 3 posições)

    fscanf(arq, "%d %d %d", &this->largura, &this->altura, &this->max);     //Os próximos 3 componentes se referem aos atributos largura, altura e max, da imagem
    this->inicializaMatriz();   //Reservando espaço de memória para a matriz da imagem, como será visto, o tamanho é dado pela largura, altura da imagem e canais (3)
    for(i=0 ; i<this->altura ; i++)    //Iterando sobre as posições da imagem
    {
        for(j=0 ; j<this->largura ; j++)
        {
            for(k=0 ; k<3 ; k++)        //Iterando sobre os canais da imagem
                fscanf(arq, "%d", &(this->matriz[i][j][k]));    //Passando os pixels da imagem/arquivo para a matriz do objeto da imagem
        }
    }
    fclose(arq);     //Fechando o arquivo/imagem
}

/* Construtor que será utilizado no processo de instanciar um objeto para armazenar o resultado; nesse caso, não é necessário passar valores para a matriz, inicialmente */
imagemPPM::imagemPPM(int altura, int largura, int max, char* tipo)
{
    setAltura(altura); //Alterando a altura
    setLargura(largura); //Alterando a largura
    setMax(max); //Alterando o nivel maximo de intensidade
    setTipo(tipo); //Alterando o tipo
    inicializaMatriz(); //Alocando espaço de memória para a matriz
}

/* Desconstrutor */
imagemPPM::~imagemPPM()
{

}

/* Método para acessar um determinado pixel em um canal da imagem */
int imagemPPM::getPixel(int altura, int largura, int canal)
{
    return(this->matriz[altura][largura][canal]); //Retorna este elemento na respectiva posição da matriz
}

/* Método para acessar a altura da imagem */
int imagemPPM::getAltura()
{
    return(this->altura); //Retornando o respectivo atributo
}

/* Método para acessar a largura da imagem */
int imagemPPM::getLargura()
{
    return(this->largura); //Retornando o respectivo atributo
}

/* Método para acessar o valor de intensidade máxima da imagem */
int imagemPPM::getMax()
{
    return(this->max); //Retornando o respectivo atributo
}

/* Método para acessar o tipo da imagem */
char * imagemPPM::getTipo()
{
    return(this->tipo); //Retornando o respectivo atributo
}

/* Método para alterar um determinado pixel para um determinado valor em um canal na imagem */
void imagemPPM::setPixel(int altura, int largura, int canal, int valor)
{
    this->matriz[altura][largura][canal] = valor; //Altera este elemento na respectiva posição da matriz
}

/* Método para alterar a altura da imagem */
void imagemPPM::setAltura(int altura)
{
    this->altura = altura; //Alterando o respectivo atributo
}

/* Método para alterar a largura da imagem */
void imagemPPM::setLargura(int largura)
{
    this->largura = largura; //Alterando o respectivo atributo
}

/* Método para alterar o valor máximo de intensidade da imagem */
void imagemPPM::setMax(int max)
{
    this->max = max; //Alterando o respectivo atributo
}

/* Método para alterar o tipo da imagem */
void imagemPPM::setTipo(char * tipo)
{
    strcpy(this->tipo, tipo); //Alterando o respectivo atributo
}

/* Método para alocar espaço de memória dinamicamente para a matriz que representa a imagem */
void imagemPPM::inicializaMatriz()
{
    int i, j;
    this->matriz = (int ***)calloc(this->altura, sizeof(int *)); //Alocando para as linhas (altura), além disso a função calloc já atribui 0 nessas posições
    for(i=0 ; i<this->altura ; i++ )
    {
        this->matriz[i] = (int **)calloc(this->largura, sizeof(int)); //Alocando para as colunas (largura), além disso a função calloc já atribui 0 nessas posições
        for(j=0 ; j<this->largura ; j++)
            this->matriz[i][j] = (int *)calloc(3, sizeof(int)); //Alocando para os canais (3), além disso a função calloc já atribui 0 nessas posições
    }
}

/* Método que copia os atributos largura, altura, max e tipo de uma imagem para outra */
void imagemPPM::copia(imagemPPM *img)
{
    this->largura = img->getLargura();  //Copiando a largura
    this->max = img->getMax();  //Copiando o nível maximo de intensidade
    this->altura = img->getAltura();    //Copiando a altura
    strcpy(this->tipo, img->getTipo());     //Copiando o tipo
}

/* Método que copia a matriz de uma imagem para outra */
void imagemPPM::copiaMatriz(imagemPPM *img)
{
    int i, j, k;
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posições
    {
        for(j=0 ; j<this->largura ; j++)
        {
            for(k=0 ; k<3 ; k++)  //Iterando sobre os canais
                this->setPixel(i, j, k, img->getPixel(i, j, k)); //Atribuindo o respectivo valor de uma imagem para outra
        }
    }
}

/* Método que copia todos os atributos de uma imagem para outra */
void imagemPPM::copiaImagem(imagemPPM *orig)
{
    this->copia(orig); //Utiliza o método de cópia dos atributos largura, altura, max e tipo
    this->copiaMatriz(orig); //Utiliza o método de cópia da matriz
}

/* Escrita de arquivo PPM (ASCII): Método que passa os atributos de um objeto para um arquivo */
void imagemPPM::escrever(FILE *arq)
{
    int i, j, k;
    rewind(arq);    //Reposicionando o ponteiro do arquivo, para garantir que ele está no início
    fprintf(arq, "%s\n", this->tipo);  //Seguindo o formato de uma imagem PPM, o primeiro elemento deve ser o tipo, dessa forma, coloca-se este atributo no arquivo
    fprintf(arq, "%d %d\n", this->largura, this->altura); //Em seguida, coloca-se os atributos largura e altura
    fprintf(arq, "%d\n", this->max); //E, para finalizar o cabeçalho, coloca-se o nível máximo de intensidade
    for(i=0 ; i<this->altura ; i++)    //Iterando sobre as posições
    {
        for(j=0 ; j<this->largura ; j++)
        {
            for(k=0 ; k<3 ; k++)    //Iterando sobre os canais
                fprintf(arq, "%d ", this->matriz[i][j][k]);     //Passando o respectivo elemento da matriz para o arquivo
        }
        fprintf(arq, "\n"); //Colocando um elemento de quebra de linha para que os elementos não fiquem em uma linha
    }
    fclose(arq);    //Fechando o arquivo
}

/* Método responsável por somar uma imagem com um valor */
void imagemPPM::somarValor(imagemPPM *resultado, int valorSomar)
{
    int i, j, k, novoValor;
    for(i=0 ; i<this->altura ; i++)     //Iterando sobre as posições da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            for(k=0 ; k<3 ; k++)    //Iterando sobre os canais
            {
                novoValor = this->matriz[i][j][k] + valorSomar;     //Verificando o valor da imagem com o valor a ser somado
                if(novoValor > this->max) //Verificando se encontra-se fora do intervalo
                    novoValor = this->max;
                else if(novoValor < 0)
                    novoValor = 0;
                resultado->matriz[i][j][k] = novoValor; //Atribui o valor (dentro do intervalo de intensidade) para a imagem resultado
            }
        }
    }
}

/* Método responsável por multiplicar uma imagem com um valor */
void imagemPPM::multiplicarValor(imagemPPM *resultado, int valorMultiplicar)
{
    int i, j, k, novoValor;
    for(i=0 ; i<this->altura ; i++)     //Iterando sobre as posições da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            for(k=0 ; k<3 ; k++)    //Iterando sobre os canais
            {
                novoValor = this->matriz[i][j][k] * valorMultiplicar;   //Verificando o valor da imagem com o valor a ser multiplicado
                if(novoValor > this->max) //Verificando se encontra-se fora do intervalo
                    novoValor = this->max;
                else if(novoValor < 0)
                    novoValor = 0;
                resultado->matriz[i][j][k] = novoValor; //Atribui o valor (dentro do intervalo de intensidade) para a imagem resultado
            }
        }
    }
}

/* Método responsável por somar duas imagens */
void imagemPPM::somar(imagemPPM *img2, imagemPPM *resultado)
{
    int i, j, k, valorSoma;
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posições da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            for(k=0 ; k<3 ; k++)  //Iterando sobre os canais
            {
                valorSoma = this->getPixel(i, j, k) + img2->getPixel(i, j, k);  //Verificando o valor da soma das duas imagens
                if(valorSoma > this->max) //Verificando se ultrapassou o valor permitido
                    valorSoma = this->max;
                resultado->matriz[i][j][k] = valorSoma; //Atribui o valor (dentro do intervalo de intensidade) para a imagem resultado
            }
        }
    }
}

/* Método responsável por subtrair duas imagens */
void imagemPPM::subtrair(imagemPPM *img2, imagemPPM *resultado)
{
    int i, j, k, valorSub;
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posições da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            for(k=0 ; k<3 ; k++)  //Iterando sobre os canais
            {
                valorSub = this->getPixel(i, j, k) - img2->getPixel(i, j, k); //Verificando o valor da subtração das duas imagens
                if(valorSub < 0) //Verificando se o valor ficou negativo
                    valorSub = 0;
                resultado->matriz[i][j][k] = valorSub; //Atribui o valor (dentro do intervalo de intensidade) para a imagem resultado
            }
        }
    }
}

/* Fatiamento de níveis de intensidade: no intervalo AB recebe um nível de cinza, fora recebe outro */
void imagemPPM::fatiamentoNI1(imagemPPM *resultado, int a, int b, int limiar1, int limiar2)
{
    int i, j, k;
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posições da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            for(k=0 ; k<3 ; k++)  //Iterando sobre os canais
            {
                if(this->matriz[i][j][k] >= a && this->matriz[i][j][k] <= b) //Verificando se o nível de intensidade da imagem encontra-se no intervalo
                    resultado->matriz[i][j][k] = limiar1;   //Se estiver, atribui para a imagem resultado o limiar para este intervalo
                else
                    resultado->matriz[i][j][k] = limiar2;   //Se não estiver, atribui para a imagem resultado o outro limiar
            }
        }
    }
}

/* Fatiamento de níveis de intensidade: no intervalo AB recebe um nível de cinza, fora não há alteração */
void imagemPPM::fatiamentoNI2(imagemPPM *resultado, int a, int b, int limiar1)
{
    int i, j, k;
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posições da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            for(k=0 ; k<3 ; k++) //Iterando sobre os canais
            {
                if(this->matriz[i][j][k] >= a && this->matriz[i][j][k] <= b) //Verificando se o nível de intensidade da imagem encontra-se no intervalo
                    resultado->matriz[i][j][k] = limiar1; //Se estiver, atribui para a imagem resultado o limiar para este intervalo
                else
                    resultado->matriz[i][j][k] = this->matriz[i][j][k]; //Se não estiver, atribui para a imagem resultado o valor da imagem, pois não haverá alteração
            }
        }
    }
}

/* Método responsável por aplicar o filtro da média. Considera o filtro apenas nos pixels internos (o filtro não fica fora da imagem) */
void imagemPPM::aplicarFiltroMedia(imagemPPM *resultado, int n)
{
    int i, j, k, a, b, s, t, auxS, auxT, valor;
    a = n/2; //Pegando a metade do tamanho do filtro e colocando em a
    b = n/2; //Pegando a metade do tamanho do filtro e colocando em b
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posições da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            for(k=0 ; k<3 ; k++) //Iterando sobre os canais
            {
                valor = 0; //Inicializando o valor final para cada passagem do filtro
                for(s=0 ; s<n ; s++) //Iterando sobre as posições do filtro
                {
                    auxS = i - a; //auxS armazena o valor da subtração da metade do filtro (a) na linha atual da matriz
                    if(auxS + s < 0 || auxS + s >= this->altura) //Verificando se este valor somado com a linha atual do filtro está dentro da altura da imagem, ou seja, se não está fora
                        continue; //Se não estiver, não aplica-se o filtro, passa para a próxima linha do filtro (for s)
                    for(t=0 ; t<n ; t++)
                    {
                        auxT = j - b; //auxT armazena o valor da subtração da metade do filtro (b) na coluna atual da matriz
                        if(auxT + t < 0 || auxT + t >= this->largura) //Verificando se este valor somado com a coluna atual do filtro está dentro da largura da imagem, ou seja, se não está fora
                            continue; //Se não estiver, não aplica-se o filtro, passa para a próxima coluna do filtro (for t)
                        valor += this->matriz[auxS + s][auxT + t][k]; //Neste ponto, o filtro está dentro da imagem, então valor irá armazenar os somatórios dos pixels da imagem original que estão dentro do filtro
                    }
                }
                valor = valor / (n*n); //Depois de aplicar o filtro em uma posição da imagem, divide por n*n, já que este é o filtro da média
                if(valor < 0) //Verificando se encontra-se dentro do intervalo permitido
                    resultado->matriz[i][j][k] = 0;
                else if(valor > this->max)
                    resultado->matriz[i][j][k] = this->max;
                else
                    resultado->matriz[i][j][k] = valor;
            }
        }
    }
}

/* Método responsável por aplicar o filtro da mediana. Considera o filtro apenas nos pixels internos (o filtro não fica fora da imagem) */
void imagemPPM::aplicarFiltroMediana(imagemPPM *resultado, int n)
{
    int i, j, k, a, b, s, t, auxS, auxT;
    list<int> lista;
    list<int>::iterator it;
    a = n/2; //Pegando a metade do tamanho do filtro e colocando em a
    b = n/2; //Pegando a metade do tamanho do filtro e colocando em b
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posições da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            for(k=0 ; k<3 ; k++) //Iterando sobre os canais
            {
                lista.clear(); //Limpando a lista que irá armazenar os elementos cujo filtro está em cima
                for(s=0 ; s<n ; s++) //Iterando sobre as linhas do filtro
                {
                    auxS = i - a; //auxS armazena o valor da subtração da metade do filtro (a) na linha atual da matriz
                    if(auxS + s< 0 || auxS + s >= this->altura) //Verificando se este valor somado com a linha atual do filtro está dentro da altura da imagem, ou seja, se não está fora
                        continue; //Se não estiver, não aplica-se o filtro, passa para a próxima linha do filtro (for s)
                    for(t=0 ; t<n ; t++) //Iterando sobre as colunas do filtro
                    {
                        auxT = j - b; //auxT armazena o valor da subtração da metade do filtro (b) na coluna atual da matriz
                        if(auxT + t < 0 || auxT + t >= this->largura) //Verificando se este valor somado com a coluna atual do filtro está dentro da largura da imagem, ou seja, se não está fora
                            continue; //Se não estiver, não aplica-se o filtro, passa para a próxima coluna do filtro (for t)
                        //Neste ponto, o elemento do filtro está dentro da imagem, então insere ordenadamente na lista o respectivo elemento da atual iteração em que o filtro encontra-se em cima
                        lista.insert(lower_bound(lista.begin(), lista.end(), this->matriz[auxS + s][auxT + t][k]), this->matriz[auxS + s][auxT + t][k]); //Encontra-se a respectiva posição do elemento para inserir ele ordenado na lista
                    }
                }
                it = lista.begin(); //Um ponteiro para lista recebe seu inicio
                advance(it, lista.size()/2); //Colocando este ponteiro no meio da lista
                resultado->matriz[i][j][k] = *it; //Acessando o elemento do meio (mediano) e atribuindo esse valor (da mediana) na imagem resultado
            }
        }
    }
}

/* Método responsável por aplicar o filtro laplaciano. Considera o filtro apenas nos pixels internos (o filtro não fica fora da imagem) */
void imagemPPM::aplicarFiltroLaplaciano(imagemPPM *resultado, int centro)
{
    int i, j, k, a, b, s, t, auxS, auxT, valor, n = 3, *filtro;

    //Inicialmente, é feito uma definição dos filtros laplacianos disponíveis
    int FL4[9] = {0, -1, 0, -1, 4, -1, 0, -1, 0};
    int FL_4[9] = {0, 1, 0, 1, -4, 1, 0, 1, 0};
    int FL8[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
    int FL_8[9] = {1, 1, 1, 1, -8, 1, 1, 1, 1};

    if(centro == 4) //Em seguida, avalia-se o valor do centro e verifica-se qual filtro será utilizado
        filtro = FL4;
    else if(centro == -4)
        filtro = FL_4;
    else if(centro == 8)
        filtro = FL8;
    else if(centro == -8)
        filtro = FL_8;

    a = n/2; //Pegando a metade do tamanho do filtro e colocando em a
    b = n/2; //Pegando a metade do tamanho do filtro e colocando em b
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posições da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            for(k=0 ; k<3 ; k++) //Iterando sobre os canais
            {
                valor = 0; //Inicializando o valor final para cada passagem do filtro
                for(s=0 ; s<n ; s++) //Iterando sobre as posições da matriz
                {
                    auxS = i - a; //auxS armazena o valor da subtração da metade do filtro (a) na linha atual da matriz
                    if(auxS + s< 0 || auxS + s >= this->altura) //Verificando se este valor somado com a linha atual do filtro está dentro da altura da imagem, ou seja, se não está fora
                        continue; //Se não estiver, não aplica-se o filtro, passa para a próxima linha do filtro (for s)
                    for(t=0 ; t<n ; t++) //Iterando sobre as colunas do filtro
                    {
                        auxT = j - b; //auxT armazena o valor da subtração da metade do filtro (b) na coluna atual da matriz
                        if(auxT + t < 0 || auxT + t >= this->largura) //Verificando se este valor somado com a coluna atual do filtro está dentro da largura da imagem, ou seja, se não está fora
                            continue; //Se não estiver, não aplica-se o filtro, passa para a próxima coluna do filtro (for t)
                        valor += *(filtro + s*n +t)*this->matriz[auxS + s][auxT + t][k]; //Neste ponto, o filtro está dentro da imagem, então valor irá armazenar os somatórios dos pixels da imagem original que estão dentro do filtro multiplicados pelos elementos das respectivas posições do filtro
                    }
                }
                if(valor < 0) //Verificando se encontra-se dentro do intervalo permitido
                    resultado->matriz[i][j][k] = 0;
                else if(valor > this->max)
                    resultado->matriz[i][j][k] = this->max;
                else
                    resultado->matriz[i][j][k] = valor;
            }
        }
    }
}

/* Método responsável por capturar o histograma de uma imagem e retorná-lo */
int * imagemPPM::getHistograma()
{
    int i, j, *histograma;
    histograma = (int*) calloc((this->max + 1)*3, sizeof (int)); //Inicialmente aloca-se espaço de memória para o histograma (vetor cujas posições representam os níveis de cinza), além disso, como a imagem possui 3 canais multiplicamos sua dimensão po 3 para armazenar sequencialmente os valores dos canais RGB
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posições da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            *(histograma + this->matriz[i][j][0]) += 1;          //Da posição [0, 255] armazena o canal vermelho: incrementando a posição do histograma que representa o nível de cinza da matriz no canal vermelho (0)
            *(histograma + this->matriz[i][j][1] + 256) += 1;    //Da posição [256, 511] armazena o canal verde: incrementando a posição do histograma que representa o nível de cinza da matriz no canal verde (1)
            *(histograma + this->matriz[i][j][2] + 512) += 1;    //Da posição [512, 767] armazena o canal azul: incrementando a posição do histograma que representa o nível de cinza da matriz no canal azul (2)
        }
    }
    return histograma; //Retornando o histograma
}

/* Método responsável por calcular a soma acumulada de um histrogama de uma imagem e retorná-lo */
int * imagemPPM::getHistogramaAcum(int *histograma)
{
    int i, k;
    for(k=0 ; k<3 ; k++) //Iterando sobre os canais
    {
        for(i=1 ; i<this->max + 1 ; i++) //Iterando sobre as posições do histograma, no respectivo canal
            *(histograma + (256 * k) + i) += *(histograma + (256 * k) + i - 1); //O valor da posição atual é somado com o valor da anterior, dessa forma, será feito uma soma acumulada dos valores
    }
    return histograma; //Retorna o histograma com a soma acumulada dos valores
}

/* Método responsável por aplicar a equalização de histograma global */
void imagemPPM::equalizarHistograma(imagemPPM *resultado)
{
    int i, j, k;
    int *histograma = this->getHistograma();     //Calculando o histograma da imagem
    histograma = this->getHistogramaAcum(histograma); //Aplicando a soma acumulada

    float fator = (float) this->max / (this->altura * this->largura); //fator armazena o valor da divisão do nível máximo de intensiade com a dimensão da imagem
    for(k=0 ; k<3 ; k++) //Iterando sobre os canais
    {
        for(i=0 ; i<this->altura ; i++) //Iterando sobre as posições da matriz
        {
            for(j=0 ; j<this->largura ; j++)
                resultado->matriz[i][j][k] = fator * (*(histograma + k*256 + this->getPixel(i, j, k))); //Fazendo a equalização, ou seja, atribuindo à imagem resultado a multiplicando do respetivo valor do histograma (no respectivo canal) com valores acumulados pelo fator

        }
    }
}

/* Método responsável por extrair os canais R, G e B (colorida para 3 canais) */
void imagemPPM::salvaCanais(FILE *r, FILE *g, FILE *b)
{
    int i, j;
    rewind(r); //Reposicionando o ponteiro do arquivo, para garantir que ele está no início
    fprintf(r, "P2\n"); //Seguindo o formato de uma imagem PGM, o primeiro elemento deve ser o tipo, dessa forma, coloca-se este atributo no arquivo do canal vermelho
    fprintf(r, "%d %d\n", this->largura, this->altura); //Em seguida, coloca-se os atributos largura e altura
    fprintf(r, "%d\n", this->max); //E, para finalizar o cabeçalho, coloca-se o nível máximo de intensidade
    rewind(g); //Reposicionando o ponteiro do arquivo, para garantir que ele está no início
    fprintf(g, "P2\n"); //Seguindo o formato de uma imagem PGM, o primeiro elemento deve ser o tipo, dessa forma, coloca-se este atributo no arquivo do canal verde
    fprintf(g, "%d %d\n", this->largura, this->altura); //Em seguida, coloca-se os atributos largura e altura
    fprintf(g, "%d\n", this->max); //E, para finalizar o cabeçalho, coloca-se o nível máximo de intensidade
    rewind(b); //Reposicionando o ponteiro do arquivo, para garantir que ele está no início
    fprintf(b, "P2\n"); //Seguindo o formato de uma imagem PGM, o primeiro elemento deve ser o tipo, dessa forma, coloca-se este atributo no arquivo do canal azul
    fprintf(b, "%d %d\n", this->largura, this->altura); //Em seguida, coloca-se os atributos largura e altura
    fprintf(b, "%d\n", this->max); //E, para finalizar o cabeçalho, coloca-se o nível máximo de intensidade
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posições da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            fprintf(r, "%d ", this->matriz[i][j][0]); //Colocando no arquivo o elemento do canal vermelho
            fprintf(g, "%d ", this->matriz[i][j][1]); //Colocando no arquivo o elemento do canal verde
            fprintf(b, "%d ", this->matriz[i][j][2]); //Colocando no arquivo o elemento do canal azul
        }
        fprintf(r, "\n"); //Colocando um elemento de quebra de linha
        fprintf(g, "\n"); //Colocando um elemento de quebra de linha
        fprintf(b, "\n"); //Colocando um elemento de quebra de linha
    }
    fclose(r); //Fechando o arquivo do canal vermelho
    fclose(g); //Fechando o arquivo do canal verde
    fclose(b); //Fechando o arquivo do canal azul
}
