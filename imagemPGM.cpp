#include "imagemPGM.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <list>

using namespace std;

/* Leitura de arquivo PGM (ASCII): Construtor que aceita como par�metro um arquivo, ou seja, a imagem que ser� lida, e transfere os dados deste arquivo para o objeto da imagem */
imagemPGM::imagemPGM(FILE *arq)
{
    int i, j;
    rewind(arq);        //Reposicionando o ponteiro do arquivo, para garantir que ele est� no in�cio
    fscanf(arq, "%s", this->tipo);        //Seguindo a estrutura de uma imagem PGM, o primeiro item se refere ao tipo da imagem, dessa forma, coloca-se este valor no atributo tipo do objeto
    if(strcmp(this->tipo,"P2") != 0)        //Verificando se o tipo � v�lido, ou seja, se a imagem � realmente PGM
    {
        printf("\n      [ERRO] Arquivo n�o � PGM: [%s]", this->tipo);       //Se n�o for, exibe uma mensagem de erro
        fclose(arq);        //Fecha o arquivo
        return;     //E retorna
    }

    //Tratamento de coment�rio: � entendido que o coment�rio � o pr�ximo/segundo componente da imagem
    char ch[50];
    fgets(ch, 50, arq); //Lendo a quebra de linha da primeira linha
    fgets(ch, 50, arq); //Lendo o suposto coment�rio
    if(ch[0] != '#' || strcmp(ch, "") == 0)    //Se n�o tiver coment�rio
        fseek(arq, sizeof(char[3]), 0);     //Volta para o fim do tipo da imagem (string com 3 posi��es)

    fscanf(arq, "%d %d %d", &this->largura, &this->altura, &this->max);     //Os pr�ximos 3 componentes se referem aos atributos largura, altura e max, da imagem
    this->inicializaMatriz();   //Reservando espa�o de mem�ria para a matriz da imagem, como ser� visto, o tamanho � dado pela largura e altura da imagem
    for(i=0 ; i<this->altura ; i++)    //Iterando sobre as posi��es da imagem
    {
        for(j=0 ; j<this->largura ; j++)
            fscanf(arq, "%d", &(this->matriz[i][j])); //Passando os pixels da imagem/arquivo para a matriz do objeto da imagem
    }
    fclose(arq);    //Fechando o arquivo/imagem
}

/* Construtor que ser� utilizado no processo de instanciar um objeto para armazenar o resultado; nesse caso, n�o � necess�rio passar valores para a matriz, inicialmente */
imagemPGM::imagemPGM(int altura, int largura, int max, char* tipo)
{
    setAltura(altura); //Alterando a altura
    setLargura(largura); //Alterando a largura
    setMax(max); //Alterando o nivel maximo de intensidade
    setTipo(tipo); //Alterando o tipo
    inicializaMatriz(); //Alocando espa�o de mem�ria para a matriz
}

/* Desconstrutor */
imagemPGM::~imagemPGM()
{

}

/* M�todo para acessar um determinado pixel da imagem */
int imagemPGM::getPixel(int altura, int largura)
{
    return(this->matriz[altura][largura]); //Retorna este elemento na respectiva posi��o da matriz
}

/* M�todo para acessar a altura da imagem */
int imagemPGM::getAltura()
{
    return(this->altura); //Retornando o respectivo atributo
}

/* M�todo para acessar a largura da imagem */
int imagemPGM::getLargura()
{
    return(this->largura);  //Retornando o respectivo atributo
}

/* M�todo para acessar o valor de intensidade m�xima da imagem */
int imagemPGM::getMax()
{
    return(this->max);  //Retornando o respectivo atributo
}

/* M�todo para acessar o tipo da imagem */
char * imagemPGM::getTipo()
{
    return(this->tipo); //Retornando o respectivo atributo
}

/* M�todo para alterar um determinado pixel para um determinado valor na imagem */
void imagemPGM::setPixel(int altura, int largura, int valor)
{
    this->matriz[altura][largura] = valor; //Altera este elemento na respectiva posi��o da matriz
}

/* M�todo para alterar a altura da imagem */
void imagemPGM::setAltura(int altura)
{
    this->altura = altura;  //Alterando o respectivo atributo
}

/* M�todo para alterar a largura da imagem */
void imagemPGM::setLargura(int largura)
{
    this->largura = largura; //Alterando o respectivo atributo
}

/* M�todo para alterar o valor m�ximo de intensidade da imagem */
void imagemPGM::setMax(int max)
{
    this->max = max; //Alterando o respectivo atributo
}

/* M�todo para alterar o tipo da imagem */
void imagemPGM::setTipo(char * tipo)
{
    strcpy(this->tipo, tipo); //Alterando o respectivo atributo
}

/* M�todo para alocar espa�o de mem�ria dinamicamente para a matriz que representa a imagem */
void imagemPGM::inicializaMatriz()
{
    int i;
    this->matriz = (int **)calloc(this->altura, sizeof(int *)); //Alocando para as linhas (altura), al�m disso a fun��o calloc j� atribui 0 nessas posi��es
    for(i=0 ; i<this->altura ; i++)
        this->matriz[i] = (int*)calloc(this->largura, sizeof(int)); //Alocando para as colunas (largura), al�m disso a fun��o calloc j� atribui 0 nessas posi��es
}

/* M�todo que copia os atributos largura, altura, max e tipo de uma imagem para outra */
void imagemPGM::copia(imagemPGM *img)
{
    this->largura = img->getLargura();  //Copiando a largura
    this->max = img->getMax();  //Copiando o n�vel maximo de intensidade
    this->altura = img->getAltura();    //Copiando a altura
    strcpy(this->tipo, img->getTipo()); //Copiando o tipo
}

/* M�todo que copia a matriz de uma imagem para outra */
void imagemPGM::copiaMatriz(imagemPGM *img)
{
    int i, j;
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posi��es
    {
        for(j=0 ; j<this->largura ; j++)
            this->setPixel(i, j, img->getPixel(i, j)); //Atribuindo o respectivo valor de uma imagem para outra
    }
}

/* M�todo que copia todos os atributos de uma imagem para outra */
void imagemPGM::copiaImagem(imagemPGM *orig)
{
    this->copia(orig); //Utiliza o m�todo de c�pia dos atributos largura, altura, max e tipo
    this->copiaMatriz(orig); //Utiliza o m�todo de c�pia da matriz
}

/* Escrita de arquivo PGM (ASCII): M�todo que passa os atributos de um objeto para um arquivo */
void imagemPGM::escrever(FILE *arq)
{
    int i, j;
    rewind(arq);    //Reposicionando o ponteiro do arquivo, para garantir que ele est� no in�cio
    fprintf(arq, "%s\n", this->tipo); //Seguindo o formato de uma imagem PGM, o primeiro elemento deve ser o tipo, dessa forma, coloca-se este atributo no arquivo
    fprintf(arq, "%d %d\n", this->largura, this->altura); //Em seguida, coloca-se os atributos largura e altura
    fprintf(arq, "%d\n", this->max); //E, para finalizar o cabe�alho, coloca-se o n�vel m�ximo de intensidade
    for(i=0 ; i<this->altura ; i++)    //Iterando sobre as posi��es
    {
        for(j=0 ; j<this->largura ; j++)
            fprintf(arq, "%d ", this->matriz[i][j]);    //Passando o respectivo elemento da matriz para o arquivo
        fprintf(arq, "\n"); //Colocando um elemento de quebra de linha para que os elementos n�o fiquem em uma linha
    }
    fclose(arq);    //Fechando o arquivo
}

/* M�todo respons�vel por somar uma imagem com um valor */
void imagemPGM::somarValor(imagemPGM *resultado, int valorSomar)
{
    int i, j, novoValor;
    for(i=0 ; i<this->altura ; i++)     //Iterando sobre as posi��es da matriz
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

/* M�todo respons�vel por multiplicar uma imagem com um valor */
void imagemPGM::multiplicarValor(imagemPGM *resultado, int valorMultiplicar)
{
    int i, j, novoValor;
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posi��es da matriz
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

/* M�todo respons�vel por somar duas imagens */
void imagemPGM::somar(imagemPGM *img2, imagemPGM *resultado)
{
    int i, j, valorSoma;
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posi��es da matriz
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

/* M�todo respons�vel por subtrair duas imagens */
void imagemPGM::subtrair(imagemPGM *img2, imagemPGM *resultado)
{
    int i, j, valorSub;
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posi��es da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            valorSub = this->getPixel(i, j) - img2->getPixel(i, j); //Verificando o valor da subtra��o das duas imagens
            if(valorSub < 0)    //Verificando se o valor ficou negativo
                valorSub = 0;
            resultado->matriz[i][j] = valorSub; //Atribui o valor (dentro do intervalo de intensidade) para a imagem resultado
        }
    }
}

/* M�todo respons�vel por aplicar o fatiamento de n�veis de intensidade: no intervalo AB recebe um n�vel de cinza, fora recebe outro */
void imagemPGM::fatiamentoNI1(imagemPGM *resultado, int a, int b, int limiar1, int limiar2)
{
    int i, j;
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posi��es da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            if(this->matriz[i][j] >= a && this->matriz[i][j] <= b) //Verificando se o n�vel de intensidade da imagem encontra-se no intervalo
                resultado->matriz[i][j] = limiar1; //Se estiver, atribui para a imagem resultado o limiar para este intervalo
            else
                resultado->matriz[i][j] = limiar2; //Se n�o estiver, atribui para a imagem resultado o outro limiar
        }
    }
}

/* M�todo respons�vel por aplicar o fatiamento de n�veis de intensidade: no intervalo AB recebe um n�vel de cinza, fora n�o h� altera��o */
void imagemPGM::fatiamentoNI2(imagemPGM *resultado, int a, int b, int limiar1)
{
    int i, j;
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posi��es da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            if(this->matriz[i][j] >= a && this->matriz[i][j] <= b) //Verificando se o n�vel de intensidade da imagem encontra-se no intervalo
                resultado->matriz[i][j] = limiar1; //Se estiver, atribui para a imagem resultado o limiar para este intervalo
            else
                resultado->matriz[i][j] = this->matriz[i][j]; //Se n�o estiver, atribui para a imagem resultado o valor da imagem, pois n�o haver� altera��o
        }
    }
}

/* M�todo respons�vel por aplicar o filtro da m�dia. Considera o filtro apenas nos pixels internos (o filtro n�o fica fora da imagem) */
void imagemPGM::aplicarFiltroMedia(imagemPGM *resultado, int n)
{
    int i, j, a, b, s, t, auxS, auxT, valor;
    a = n/2; //Pegando a metade do tamanho do filtro e colocando em a
    b = n/2; //Pegando a metade do tamanho do filtro e colocando em b
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posi��es da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            valor = 0; //Inicializando o valor final para cada passagem do filtro
            for(s=0 ; s<n ; s++) //Iterando sobre as linhas do filtro
            {
                auxS = i - a; //auxS armazena o valor da subtra��o da metade do filtro (a) na linha atual da matriz
                if(auxS + s < 0 || auxS + s >= this->altura) //Verificando se este valor somado com a linha atual do filtro est� dentro da altura da imagem, ou seja, se n�o est� fora
                    continue;   //Se n�o estiver, n�o aplica-se o filtro, passa para a pr�xima linha do filtro (for s)
                for(t=0 ; t<n ; t++) //Iterando sobre as colunas do filtro
                {
                    auxT = j - b; //auxT armazena o valor da subtra��o da metade do filtro (b) na coluna atual da matriz
                    if(auxT + t < 0 || auxT + t >= this->largura) //Verificando se este valor somado com a coluna atual do filtro est� dentro da largura da imagem, ou seja, se n�o est� fora
                        continue; //Se n�o estiver, n�o aplica-se o filtro, passa para a pr�xima coluna do filtro (for t)
                    valor += this->matriz[auxS + s][auxT + t]; //Neste ponto, o filtro est� dentro da imagem, ent�o valor ir� armazenar os somat�rios dos pixels da imagem original que est�o dentro do filtro
                }
            }
            valor = valor / (n*n); //Depois de aplicar o filtro em uma posi��o da imagem, divide por n*n, j� que este � o filtro da m�dia
            if(valor < 0)   //Verificando se encontra-se dentro do intervalo permitido
                resultado->matriz[i][j] = 0;
            else if(valor > this->max)
                resultado->matriz[i][j] = this->max;
            else
                resultado->matriz[i][j] = valor;
        }
    }
}

/* M�todo respons�vel por aplicar o filtro da mediana. Considera o filtro apenas nos pixels internos (o filtro n�o fica fora da imagem) */
void imagemPGM::aplicarFiltroMediana(imagemPGM *resultado, int n)
{
    int i, j, a, b, s, t, auxS, auxT;
    list<int> lista;
    list<int>::iterator it;
    a = n/2; //Pegando a metade do tamanho do filtro e colocando em a
    b = n/2; //Pegando a metade do tamanho do filtro e colocando em b
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posi��es da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            lista.clear(); //Limpando a lista que ir� armazenar os elementos cujo filtro est� em cima
            for(s=0 ; s<n ; s++) //Iterando sobre as linhas do filtro
            {
                auxS = i - a; //auxS armazena o valor da subtra��o da metade do filtro (a) na linha atual da matriz
                if(auxS + s< 0 || auxS + s >= this->altura) //Verificando se este valor somado com a linha atual do filtro est� dentro da altura da imagem, ou seja, se n�o est� fora
                    continue; //Se n�o estiver, n�o aplica-se o filtro, passa para a pr�xima linha do filtro (for s)
                for(t=0 ; t<n ; t++) //Iterando sobre as colunas do filtro
                {
                    auxT = j - b; //auxT armazena o valor da subtra��o da metade do filtro (b) na coluna atual da matriz
                    if(auxT + t < 0 || auxT + t >= this->largura) //Verificando se este valor somado com a coluna atual do filtro est� dentro da largura da imagem, ou seja, se n�o est� fora
                        continue; //Se n�o estiver, n�o aplica-se o filtro, passa para a pr�xima coluna do filtro (for t)
                    //Neste ponto, o elemento do filtro est� dentro da imagem, ent�o insere ordenadamente na lista o respectivo elemento da atual itera��o em que o filtro encontra-se em cima
                    lista.insert(lower_bound(lista.begin(), lista.end(), this->matriz[auxS + s][auxT + t]), this->matriz[auxS + s][auxT + t]); //Encontra-se a respectiva posi��o do elemento para inserir ele ordenado na lista
                }
            }
            it = lista.begin(); //Um ponteiro para lista recebe seu inicio
            advance(it, lista.size()/2); //Colocando este ponteiro no meio da lista
            resultado->matriz[i][j] = *it; //Acessando o elemento do meio (mediano) e atribuindo esse valor (da mediana) na imagem resultado
        }
    }
}

/* M�todo respons�vel por aplicar o filtro laplaciano. Considera o filtro apenas nos pixels internos (o filtro n�o fica fora da imagem) */
void imagemPGM::aplicarFiltroLaplaciano(imagemPGM *resultado, int centro)
{
    int i, j, a, b, s, t, auxS, auxT, valor, *filtro, n = 3;

    //Inicialmente, � feito uma defini��o dos filtros laplacianos dispon�veis
    int FL4[9] = {0, -1, 0, -1, 4, -1, 0, -1, 0};
    int FL_4[9] = {0, 1, 0, 1, -4, 1, 0, 1, 0};
    int FL8[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
    int FL_8[9] = {1, 1, 1, 1, -8, 1, 1, 1, 1};

    if(centro == 4) //Em seguida, avalia-se o valor do centro e verifica-se qual filtro ser� utilizado
        filtro = FL4;
    else if(centro == -4)
        filtro = FL_4;
    else if(centro == 8)
        filtro = FL8;
    else if(centro == -8)
        filtro = FL_8;

    a = n/2; //Pegando a metade do tamanho do filtro e colocando em a
    b = n/2; //Pegando a metade do tamanho do filtro e colocando em b
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posi��es da matriz
    {
        for(j=0 ; j<this->largura ; j++)
        {
            valor = 0; //Inicializando o valor final para cada passagem do filtro
            for(s=0 ; s<n ; s++)    //Iterando sobre as linhas do filtro
            {
                auxS = i - a; //auxS armazena o valor da subtra��o da metade do filtro (a) na linha atual da matriz
                if(auxS + s< 0 || auxS + s >= this->altura) //Verificando se este valor somado com a linha atual do filtro est� dentro da altura da imagem, ou seja, se n�o est� fora
                    continue; //Se n�o estiver, n�o aplica-se o filtro, passa para a pr�xima linha do filtro (for s)
                for(t=0 ; t<n ; t++) //Iterando sobre as colunas do filtro
                {
                    auxT = j - b; //auxT armazena o valor da subtra��o da metade do filtro (b) na coluna atual da matriz
                    if(auxT + t < 0 || auxT + t >= this->largura) //Verificando se este valor somado com a coluna atual do filtro est� dentro da largura da imagem, ou seja, se n�o est� fora
                        continue; //Se n�o estiver, n�o aplica-se o filtro, passa para a pr�xima coluna do filtro (for t)
                    valor += (*(filtro + s*n +t))*this->matriz[auxS + s][auxT + t]; //Neste ponto, o filtro est� dentro da imagem, ent�o valor ir� armazenar os somat�rios dos pixels da imagem original que est�o dentro do filtro multiplicados pelos elementos das respectivas posi��es do filtro
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

/* M�todo respons�vel por capturar o histograma de uma imagem e retorn�-lo */
int * imagemPGM::getHistograma()
{
    int i, j, *histograma;
    histograma = (int*) calloc(this->max + 1, sizeof (int));    //Inicialmente aloca-se espa�o de mem�ria para o histograma (vetor cujas posi��es representam os n�veis de cinza)
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posi��es da matriz
    {
        for(j=0 ; j<this->largura ; j++)
            *(histograma + this->matriz[i][j]) += 1; //Incrementando a posi��o do histograma que representa o n�vel de cinza da matriz
    }
    return histograma; //Retornando o histograma
}

/* M�todo respons�vel por calcular a soma acumulada de um histrogama de uma imagem e retorn�-lo */
int * imagemPGM::getHistogramaAcum(int *histograma)
{
    int i;
    for(i=1 ; i<this->max + 1 ; i++) //Iterando sobre as posi��es do histograma
        *(histograma + i) += *(histograma + i - 1); //O valor da posi��o atual � somado com o valor da anterior, dessa forma, ser� feito uma soma acumulada dos valores
    return histograma; //Retorna o histograma com a soma acumulada dos valores
}

/* M�todo respons�vel por aplicar a equaliza��o de histograma global */
void imagemPGM::equalizarHistograma(imagemPGM *resultado)
{
    int i, j;
    int *histograma = this->getHistograma();     //Calculando o histograma da imagem
    histograma = this->getHistogramaAcum(histograma); //Aplicando a soma acumulada

    float fator = (float) this->max / (this->altura * this->largura); //fator armazena o valor da divis�o do n�vel m�ximo de intensiade com a dimens�o da imagem
    for(i=0 ; i<this->altura ; i++) //Iterando sobre as posi��es da matriz
    {
        for(j=0 ; j<this->largura ; j++)
            resultado->matriz[i][j] = fator * (*(histograma + this->getPixel(i, j))); //Fazendo a equaliza��o, ou seja, atribuindo � imagem resultado a multiplicando do respetivo valor do histograma com valores acumulados pelo fator
    }
}

/* M�todo respons�vel por compor os canais R, G e B (3 canais para colorida) */
void imagemPGM::carregaCanais(imagemPGM *g, imagemPGM *b, FILE *resultado)
{
    int i, j;
    rewind(resultado); //Reposicionando o ponteiro do arquivo, para garantir que ele est� no in�cio
    fprintf(resultado, "P3\n"); //Seguindo o formato de uma imagem PPM, o primeiro elemento deve ser o tipo, dessa forma, coloca-se este atributo no arquivo do canal vermelho
    fprintf(resultado, "%d %d\n", this->getLargura(), this->getAltura()); //Em seguida, coloca-se os atributos largura e altura
    fprintf(resultado, "%d\n", this->getMax()); //E, para finalizar o cabe�alho, coloca-se o n�vel m�ximo de intensidade
    for(i=0 ; i<this->getAltura() ; i++) //Iterando sobre as posi��es da matriz
    {
        for(j=0 ; j<this->getLargura() ; j++)
        {
            fprintf(resultado, "%d ", this->getPixel(i, j)); //Colocando no arquivo o elemento do canal vermelho (o objeto que chamou este m�todo)
            fprintf(resultado, "%d ", g->getPixel(i, j)); //Colocando no arquivo o elemento do canal verde
            fprintf(resultado, "%d ", b->getPixel(i, j)); //Colocando no arquivo o elemento do canal azul
        }
        fprintf(resultado, "\n"); //Colocando um elemento de quebra de linha
    }
}
