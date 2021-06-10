#include "imagemPGM.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <list>

using namespace std;

/* Leitura de arquivo PGM (ASCII): Construtor que aceita como parâmetro um arquivo, ou seja, a imagem que será lida, e transfere os dados deste arquivo para o objeto da imagem */
imagemPGM::imagemPGM(FILE *arq)
{
    int i, j;
    rewind(arq);        //Reposicionando o ponteiro do arquivo, para garantir que ele está no início
    fscanf(arq, "%s", this->tipo);        //Seguindo a estrutura de uma imagem PGM, o primeiro item se refere ao tipo da imagem, dessa forma, coloca-se este valor no atributo tipo do objeto
    if(strcmp(this->tipo,"P2") != 0)        //Verificando se o tipo é válido, ou seja, se a imagem é realmente PGM
    {
        printf("\n      [ERRO] Arquivo não é PGM: [%s]", this->tipo);       //Se não for, exibe uma mensagem de erro
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
    this->inicializaMatriz();   //Reservando espaço de memória para a matriz da imagem, como será visto, o tamanho é dado pela largura e altura da imagem
    for(i=0 ; i<this->altura ; i++)    //Iterando sobre as posições da imagem
    {
        for(j=0 ; j<this->largura ; j++)
            fscanf(arq, "%d", &(this->matriz[i][j])); //Passando os pixels da imagem/arquivo para a matriz do objeto da imagem
    }
    fclose(arq);    //Fechando o arquivo/imagem
}

/* Construtor que será utilizado no processo de instanciar um objeto para armazenar o resultado; nesse caso, não é necessário passar valores para a matriz, inicialmente */
imagemPGM::imagemPGM(int altura, int largura, int max, char* tipo)
{
    setAltura(altura); //Alterando a altura
    setLargura(largura); //Alterando a largura
    setMax(max); //Alterando o nivel maximo de intensidade
    setTipo(tipo); //Alterando o tipo
    inicializaMatriz(); //Alocando espaço de memória para a matriz
}

/* Desconstrutor */
imagemPGM::~imagemPGM()
{

}

/* Método para acessar um determinado pixel da imagem */
int imagemPGM::getPixel(int altura, int largura)
{
    return(this->matriz[altura][largura]); //Retorna este elemento na respectiva posição da matriz
}

/* Método para acessar a altura da imagem */
int imagemPGM::getAltura()
{
    return(this->altura); //Retornando o respectivo atributo
}

/* Método para acessar a largura da imagem */
int imagemPGM::getLargura()
{
    return(this->largura);  //Retornando o respectivo atributo
}

/* Método para acessar o valor de intensidade máxima da imagem */
int imagemPGM::getMax()
{
    return(this->max);  //Retornando o respectivo atributo
}

/* Método para acessar o tipo da imagem */
char * imagemPGM::getTipo()
{
    return(this->tipo); //Retornando o respectivo atributo
}

/* Método para alterar um determinado pixel para um determinado valor na imagem */
void imagemPGM::setPixel(int altura, int largura, int valor)
{
    this->matriz[altura][largura] = valor; //Altera este elemento na respectiva posição da matriz
}

/* Método para alterar a altura da imagem */
void imagemPGM::setAltura(int altura)
{
    this->altura = altura;  //Alterando o respectivo atributo
}

/* Método para alterar a largura da imagem */
void imagemPGM::setLargura(int largura)
{
    this->largura = largura; //Alterando o respectivo atributo
}

/* Método para alterar o valor máximo de intensidade da imagem */
void imagemPGM::setMax(int max)
{
    this->max = max; //Alterando o respectivo atributo
}

/* Método para alterar o tipo da imagem */
void imagemPGM::setTipo(char * tipo)
{
    strcpy(this->tipo, tipo); //Alterando o respectivo atributo
}

/* Método para alocar espaço de memória dinamicamente para a matriz que representa a imagem */
void imagemPGM::inicializaMatriz()
{
    int i;
    this->matriz = (int **)calloc(this->altura, sizeof(int *)); //Alocando para as linhas (altura), além disso a função calloc já atribui 0 nessas posições
    for(i=0 ; i<this->altura ; i++)
        this->matriz[i] = (int*)calloc(this->largura, sizeof(int)); //Alocando para as colunas (largura), além disso a função calloc já atribui 0 nessas posições
}

/* Método que copia os atributos largura, altura, max e tipo de uma imagem para outra */
void imagemPGM::copia(imagemPGM *img)
{
    this->largura = img->getLargura();  //Copiando a largura
    this->max = img->getMax();  //Copiando o nível maximo de intensidade
    this->altura = img->getAltura();    //Copiando a altura
    strcpy(this->tipo, img->getTipo()); //Copiando o tipo
}

/* Método que copia a matriz de uma imagem para outra */
void imagemPGM::copiaMatriz(imagemPGM *img)
{
    int i, j;
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posições
    {
        for(j=0 ; j<this->largura ; j++)
            this->setPixel(i, j, img->getPixel(i, j)); //Atribuindo o respectivo valor de uma imagem para outra
    }
}

/* Método que copia todos os atributos de uma imagem para outra */
void imagemPGM::copiaImagem(imagemPGM *orig)
{
    this->copia(orig); //Utiliza o método de cópia dos atributos largura, altura, max e tipo
    this->copiaMatriz(orig); //Utiliza o método de cópia da matriz
}

/* Escrita de arquivo PGM (ASCII): Método que passa os atributos de um objeto para um arquivo */
void imagemPGM::escrever(FILE *arq)
{
    int i, j;
    rewind(arq);    //Reposicionando o ponteiro do arquivo, para garantir que ele está no início
    fprintf(arq, "%s\n", this->tipo); //Seguindo o formato de uma imagem PGM, o primeiro elemento deve ser o tipo, dessa forma, coloca-se este atributo no arquivo
    fprintf(arq, "%d %d\n", this->largura, this->altura); //Em seguida, coloca-se os atributos largura e altura
    fprintf(arq, "%d\n", this->max); //E, para finalizar o cabeçalho, coloca-se o nível máximo de intensidade
    for(i=0 ; i<this->altura ; i++)    //Iterando sobre as posições
    {
        for(j=0 ; j<this->largura ; j++)
            fprintf(arq, "%d ", this->matriz[i][j]);    //Passando o respectivo elemento da matriz para o arquivo
        fprintf(arq, "\n"); //Colocando um elemento de quebra de linha para que os elementos não fiquem em uma linha
    }
    fclose(arq);    //Fechando o arquivo
}

/* Método responsável por somar uma imagem com um valor */
void imagemPGM::somarValor(imagemPGM *resultado, int valorSomar)
{
    int i, j, novoValor;
    for(i=0 ; i<this->altura ; i++)     //Iterando sobre as posições da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            novoValor = this->matriz[i][j] + valorSomar;     //Verificando o valor da imagem com o valor a ser somado
            if(novoValor > this->max)  //Verificando se encontra-se fora do intervalo
                novoValor = this->max;
            else if(novoValor < 0)
                novoValor = 0;
            resultado->matriz[i][j] = novoValor; //Atribui o valor (dentro do intervalo de intensidade) para a imagem resultado
        }
    }
}

/* Método responsável por multiplicar uma imagem com um valor */
void imagemPGM::multiplicarValor(imagemPGM *resultado, int valorMultiplicar)
{
    int i, j, novoValor;
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posições da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            novoValor = this->matriz[i][j] * valorMultiplicar;  //Verificando o valor da imagem com o valor a ser multiplicado
            if(novoValor > this->max) //Verificando se encontra-se fora do intervalo
                novoValor = this->max;
            else if(novoValor < 0)
                novoValor = 0;
            resultado->matriz[i][j] = novoValor; //Atribui o valor (dentro do intervalo de intensidade) para a imagem resultado
        }
    }
}

/* Método responsável por somar duas imagens */
void imagemPGM::somar(imagemPGM *img2, imagemPGM *resultado)
{
    int i, j, valorSoma;
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posições da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            valorSoma = this->getPixel(i, j) + img2->getPixel(i, j);  //Verificando o valor da soma das duas imagens
            if(valorSoma > this->max) //Verificando se ultrapassou o valor permitido
                valorSoma = this->max;
            resultado->matriz[i][j] = valorSoma; //Atribui o valor (dentro do intervalo de intensidade) para a imagem resultado
        }
    }
}

/* Método responsável por subtrair duas imagens */
void imagemPGM::subtrair(imagemPGM *img2, imagemPGM *resultado)
{
    int i, j, valorSub;
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posições da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            valorSub = this->getPixel(i, j) - img2->getPixel(i, j); //Verificando o valor da subtração das duas imagens
            if(valorSub < 0)    //Verificando se o valor ficou negativo
                valorSub = 0;
            resultado->matriz[i][j] = valorSub; //Atribui o valor (dentro do intervalo de intensidade) para a imagem resultado
        }
    }
}

/* Método responsável por aplicar o fatiamento de níveis de intensidade: no intervalo AB recebe um nível de cinza, fora recebe outro */
void imagemPGM::fatiamentoNI1(imagemPGM *resultado, int a, int b, int limiar1, int limiar2)
{
    int i, j;
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posições da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            if(this->matriz[i][j] >= a && this->matriz[i][j] <= b) //Verificando se o nível de intensidade da imagem encontra-se no intervalo
                resultado->matriz[i][j] = limiar1; //Se estiver, atribui para a imagem resultado o limiar para este intervalo
            else
                resultado->matriz[i][j] = limiar2; //Se não estiver, atribui para a imagem resultado o outro limiar
        }
    }
}

/* Método responsável por aplicar o fatiamento de níveis de intensidade: no intervalo AB recebe um nível de cinza, fora não há alteração */
void imagemPGM::fatiamentoNI2(imagemPGM *resultado, int a, int b, int limiar1)
{
    int i, j;
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posições da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            if(this->matriz[i][j] >= a && this->matriz[i][j] <= b) //Verificando se o nível de intensidade da imagem encontra-se no intervalo
                resultado->matriz[i][j] = limiar1; //Se estiver, atribui para a imagem resultado o limiar para este intervalo
            else
                resultado->matriz[i][j] = this->matriz[i][j]; //Se não estiver, atribui para a imagem resultado o valor da imagem, pois não haverá alteração
        }
    }
}

/* Método responsável por aplicar o filtro da média. Considera o filtro apenas nos pixels internos (o filtro não fica fora da imagem) */
void imagemPGM::aplicarFiltroMedia(imagemPGM *resultado, int n)
{
    int i, j, a, b, s, t, auxS, auxT, valor;
    a = n/2; //Pegando a metade do tamanho do filtro e colocando em a
    b = n/2; //Pegando a metade do tamanho do filtro e colocando em b
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posições da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            valor = 0; //Inicializando o valor final para cada passagem do filtro
            for(s=0 ; s<n ; s++) //Iterando sobre as linhas do filtro
            {
                auxS = i - a; //auxS armazena o valor da subtração da metade do filtro (a) na linha atual da matriz
                if(auxS + s < 0 || auxS + s >= this->altura) //Verificando se este valor somado com a linha atual do filtro está dentro da altura da imagem, ou seja, se não está fora
                    continue;   //Se não estiver, não aplica-se o filtro, passa para a próxima linha do filtro (for s)
                for(t=0 ; t<n ; t++) //Iterando sobre as colunas do filtro
                {
                    auxT = j - b; //auxT armazena o valor da subtração da metade do filtro (b) na coluna atual da matriz
                    if(auxT + t < 0 || auxT + t >= this->largura) //Verificando se este valor somado com a coluna atual do filtro está dentro da largura da imagem, ou seja, se não está fora
                        continue; //Se não estiver, não aplica-se o filtro, passa para a próxima coluna do filtro (for t)
                    valor += this->matriz[auxS + s][auxT + t]; //Neste ponto, o filtro está dentro da imagem, então valor irá armazenar os somatórios dos pixels da imagem original que estão dentro do filtro
                }
            }
            valor = valor / (n*n); //Depois de aplicar o filtro em uma posição da imagem, divide por n*n, já que este é o filtro da média
            if(valor < 0)   //Verificando se encontra-se dentro do intervalo permitido
                resultado->matriz[i][j] = 0;
            else if(valor > this->max)
                resultado->matriz[i][j] = this->max;
            else
                resultado->matriz[i][j] = valor;
        }
    }
}

/* Método responsável por aplicar o filtro da mediana. Considera o filtro apenas nos pixels internos (o filtro não fica fora da imagem) */
void imagemPGM::aplicarFiltroMediana(imagemPGM *resultado, int n)
{
    int i, j, a, b, s, t, auxS, auxT;
    list<int> lista;
    list<int>::iterator it;
    a = n/2; //Pegando a metade do tamanho do filtro e colocando em a
    b = n/2; //Pegando a metade do tamanho do filtro e colocando em b
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posições da matriz
    {
        for(j=0 ; j<this->largura ; j++)
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
                    lista.insert(lower_bound(lista.begin(), lista.end(), this->matriz[auxS + s][auxT + t]), this->matriz[auxS + s][auxT + t]); //Encontra-se a respectiva posição do elemento para inserir ele ordenado na lista
                }
            }
            it = lista.begin(); //Um ponteiro para lista recebe seu inicio
            advance(it, lista.size()/2); //Colocando este ponteiro no meio da lista
            resultado->matriz[i][j] = *it; //Acessando o elemento do meio (mediano) e atribuindo esse valor (da mediana) na imagem resultado
        }
    }
}

/* Método responsável por aplicar o filtro laplaciano. Considera o filtro apenas nos pixels internos (o filtro não fica fora da imagem) */
void imagemPGM::aplicarFiltroLaplaciano(imagemPGM *resultado, int centro)
{
    int i, j, a, b, s, t, auxS, auxT, valor, *filtro, n = 3;

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
            valor = 0; //Inicializando o valor final para cada passagem do filtro
            for(s=0 ; s<n ; s++)    //Iterando sobre as linhas do filtro
            {
                auxS = i - a; //auxS armazena o valor da subtração da metade do filtro (a) na linha atual da matriz
                if(auxS + s< 0 || auxS + s >= this->altura) //Verificando se este valor somado com a linha atual do filtro está dentro da altura da imagem, ou seja, se não está fora
                    continue; //Se não estiver, não aplica-se o filtro, passa para a próxima linha do filtro (for s)
                for(t=0 ; t<n ; t++) //Iterando sobre as colunas do filtro
                {
                    auxT = j - b; //auxT armazena o valor da subtração da metade do filtro (b) na coluna atual da matriz
                    if(auxT + t < 0 || auxT + t >= this->largura) //Verificando se este valor somado com a coluna atual do filtro está dentro da largura da imagem, ou seja, se não está fora
                        continue; //Se não estiver, não aplica-se o filtro, passa para a próxima coluna do filtro (for t)
                    valor += (*(filtro + s*n +t))*this->matriz[auxS + s][auxT + t]; //Neste ponto, o filtro está dentro da imagem, então valor irá armazenar os somatórios dos pixels da imagem original que estão dentro do filtro multiplicados pelos elementos das respectivas posições do filtro
                }
            }
            if(valor < 0) //Verificando se encontra-se dentro do intervalo permitido
                resultado->matriz[i][j] = 0;
            else if(valor > this->max)
                resultado->matriz[i][j] = this->max;
            else
                resultado->matriz[i][j] = valor;
        }
    }
}

/* Método responsável por capturar o histograma de uma imagem e retorná-lo */
int * imagemPGM::getHistograma()
{
    int i, j, *histograma;
    histograma = (int*) calloc(this->max + 1, sizeof (int));    //Inicialmente aloca-se espaço de memória para o histograma (vetor cujas posições representam os níveis de cinza)
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posições da matriz
    {
        for(j=0 ; j<this->largura ; j++)
            *(histograma + this->matriz[i][j]) += 1; //Incrementando a posição do histograma que representa o nível de cinza da matriz
    }
    return histograma; //Retornando o histograma
}

/* Método responsável por calcular a soma acumulada de um histrogama de uma imagem e retorná-lo */
int * imagemPGM::getHistogramaAcum(int *histograma)
{
    int i;
    for(i=1 ; i<this->max + 1 ; i++) //Iterando sobre as posições do histograma
        *(histograma + i) += *(histograma + i - 1); //O valor da posição atual é somado com o valor da anterior, dessa forma, será feito uma soma acumulada dos valores
    return histograma; //Retorna o histograma com a soma acumulada dos valores
}

/* Método responsável por aplicar a equalização de histograma global */
void imagemPGM::equalizarHistograma(imagemPGM *resultado)
{
    int i, j;
    int *histograma = this->getHistograma();     //Calculando o histograma da imagem
    histograma = this->getHistogramaAcum(histograma); //Aplicando a soma acumulada

    float fator = (float) this->max / (this->altura * this->largura); //fator armazena o valor da divisão do nível máximo de intensiade com a dimensão da imagem
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posições da matriz
    {
        for(j=0 ; j<this->largura ; j++)
            resultado->matriz[i][j] = fator * (*(histograma + this->getPixel(i, j))); //Fazendo a equalização, ou seja, atribuindo à imagem resultado a multiplicando do respetivo valor do histograma com valores acumulados pelo fator
    }
}

/* Método responsável por compor os canais R, G e B (3 canais para colorida) */
void imagemPGM::carregaCanais(imagemPGM *g, imagemPGM *b, FILE *resultado)
{
    int i, j;
    rewind(resultado); //Reposicionando o ponteiro do arquivo, para garantir que ele está no início
    fprintf(resultado, "P3\n"); //Seguindo o formato de uma imagem PPM, o primeiro elemento deve ser o tipo, dessa forma, coloca-se este atributo no arquivo do canal vermelho
    fprintf(resultado, "%d %d\n", this->getLargura(), this->getAltura()); //Em seguida, coloca-se os atributos largura e altura
    fprintf(resultado, "%d\n", this->getMax()); //E, para finalizar o cabeçalho, coloca-se o nível máximo de intensidade
    for(i=0 ; i<this->getAltura() ; i++) //Iterando sobre as posições da matriz
    {
        for(j=0 ; j<this->getLargura() ; j++)
        {
            fprintf(resultado, "%d ", this->getPixel(i, j)); //Colocando no arquivo o elemento do canal vermelho (o objeto que chamou este método)
            fprintf(resultado, "%d ", g->getPixel(i, j)); //Colocando no arquivo o elemento do canal verde
            fprintf(resultado, "%d ", b->getPixel(i, j)); //Colocando no arquivo o elemento do canal azul
        }
        fprintf(resultado, "\n"); //Colocando um elemento de quebra de linha
    }
}
