#ifndef IMAGEMPPM_HPP
#define IMAGEMPPM_HPP

#include <stdio.h>

class imagemPPM
{
    private:
        int ***matriz;       //Matriz que ser� alocada dinamicamente ap�s a defini��o do valor de altura e largura (e 3 canais)
        int largura, altura; //Largura (coluna) e altura (linha) da imagem (RESOLU��O ESPACIAL)
        int max;             //Valor de intensidade m�xima (0: preto, max: branco) (PROFUNDIDADE EM BITS)
        char tipo[3];        //Tipo da imagem (PGM ou PPM)

    public:
        imagemPPM(FILE *arq);   //Leitura de arquivo PGM (ASCII): Construtor que aceita como par�metro um arquivo, ou seja, a imagem que ser� lida, e transfere os dados deste arquivo para o objeto da imagem
        imagemPPM(int altura, int largura, int max, char* tipo); //Construtor que ser� utilizado no processo de instanciar um objeto para armazenar o resultado; nesse caso, n�o � necess�rio passar valores para a matriz, inicialmente
        virtual ~imagemPPM();   //Desconstrutor
        int getPixel(int altura, int largura, int canal); //M�todo para acessar um determinado pixel da imagem
        int getAltura();        //M�todo para acessar a altura da imagem
        int getLargura();       //M�todo para acessar a largura da imagem
        int getMax();           //M�todo para acessar o valor de intensidade m�xima da imagem
        char * getTipo();       //M�todo para acessar o tipo da imagem
        void setPixel(int altura, int largura, int canal, int valor); //M�todo para alterar um determinado pixel para um determinado valor na imagem
        void setAltura(int altura);     //M�todo para alterar a altura da imagem
        void setLargura(int largura);   //M�todo para alterar a largura da imagem
        void setMax(int max);           //M�todo para alterar o valor m�ximo de intensidade da imagem
        void setTipo(char * tipo);      //M�todo para alterar o tipo da imagem
        void inicializaMatriz();        //M�todo para alocar espa�o de mem�ria dinamicamente para a matriz que representa a imagem
        void copia(imagemPPM *img);     //M�todo que copia os atributos largura, altura, max e tipo de uma imagem para outra
        void copiaMatriz(imagemPPM *img);   //M�todo que copia a matriz de uma imagem para outra
        void copiaImagem(imagemPPM *orig);  //M�todo que copia todos os atributos de uma imagem para outra
        void escrever(FILE *arq);   //Escrita de arquivo PGM (ASCII): M�todo que passa os atributos de um objeto para um arquivo
        void somarValor(imagemPPM *resultado, int valorSomar);  //M�todo respons�vel por somar uma imagem com um valor
        void multiplicarValor(imagemPPM *resultado, int valorMultiplicar); //M�todo respons�vel por multiplicar uma imagem com um valor
        void somar(imagemPPM *img2, imagemPPM *resultado);   //M�todo respons�vel por somar duas imagens (this + img2)
        void subtrair(imagemPPM *img2, imagemPPM *resultado);//M�todo respons�vel por subtrair duas imagens (this - img2)
        void fatiamentoNI1(imagemPPM *resultado, int a, int b, int limiar1, int limiar2); //M�todo respons�vel por aplicar o fatiamento de n�veis de intensidade: no intervalo AB recebe um n�vel de cinza, fora recebe outro
        void fatiamentoNI2(imagemPPM *resultado, int a, int b, int limiar1); //M�todo respons�vel por aplicar o fatiamento de n�veis de intensidade: no intervalo AB recebe um n�vel de cinza, fora n�o h� altera��o
        void aplicarFiltroMedia(imagemPPM *resultado, int n);           //M�todo respons�vel por aplicar o filtro da m�dia. Considera o filtro apenas nos pixels internos (o filtro n�o fica fora da imagem)
        void aplicarFiltroMediana(imagemPPM *resultado, int n);         //M�todo respons�vel por aplicar o filtro da mediana. Considera o filtro apenas nos pixels internos (o filtro n�o fica fora da imagem)
        void aplicarFiltroLaplaciano(imagemPPM *resultado, int centro); //M�todo respons�vel por aplicar o filtro laplaciano. Considera o filtro apenas nos pixels internos (o filtro n�o fica fora da imagem)
        int * getHistograma();                          //M�todo respons�vel por capturar o histograma de uma imagem e retorn�-lo
        int * getHistogramaAcum(int *histograma);       //M�todo respons�vel por calcular a soma acumulada de um histrogama de uma imagem e retorn�-lo
        void equalizarHistograma(imagemPPM *resultado); //M�todo respons�vel por aplicar a equaliza��o de histograma global
        void salvaCanais(FILE *r, FILE *g, FILE *b);    //M�todo respons�vel por extrair os canais R, G e B (colorida para 3 canais)
};

#endif /* IMAGEMPPM */

