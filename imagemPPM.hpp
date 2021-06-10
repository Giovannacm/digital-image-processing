#ifndef IMAGEMPPM_HPP
#define IMAGEMPPM_HPP

#include <stdio.h>

class imagemPPM
{
    private:
        int ***matriz;       //Matriz que será alocada dinamicamente após a definição do valor de altura e largura (e 3 canais)
        int largura, altura; //Largura (coluna) e altura (linha) da imagem (RESOLUÇÃO ESPACIAL)
        int max;             //Valor de intensidade máxima (0: preto, max: branco) (PROFUNDIDADE EM BITS)
        char tipo[3];        //Tipo da imagem (PGM ou PPM)

    public:
        imagemPPM(FILE *arq);   //Leitura de arquivo PGM (ASCII): Construtor que aceita como parâmetro um arquivo, ou seja, a imagem que será lida, e transfere os dados deste arquivo para o objeto da imagem
        imagemPPM(int altura, int largura, int max, char* tipo); //Construtor que será utilizado no processo de instanciar um objeto para armazenar o resultado; nesse caso, não é necessário passar valores para a matriz, inicialmente
        virtual ~imagemPPM();   //Desconstrutor
        int getPixel(int altura, int largura, int canal); //Método para acessar um determinado pixel da imagem
        int getAltura();        //Método para acessar a altura da imagem
        int getLargura();       //Método para acessar a largura da imagem
        int getMax();           //Método para acessar o valor de intensidade máxima da imagem
        char * getTipo();       //Método para acessar o tipo da imagem
        void setPixel(int altura, int largura, int canal, int valor); //Método para alterar um determinado pixel para um determinado valor na imagem
        void setAltura(int altura);     //Método para alterar a altura da imagem
        void setLargura(int largura);   //Método para alterar a largura da imagem
        void setMax(int max);           //Método para alterar o valor máximo de intensidade da imagem
        void setTipo(char * tipo);      //Método para alterar o tipo da imagem
        void inicializaMatriz();        //Método para alocar espaço de memória dinamicamente para a matriz que representa a imagem
        void copia(imagemPPM *img);     //Método que copia os atributos largura, altura, max e tipo de uma imagem para outra
        void copiaMatriz(imagemPPM *img);   //Método que copia a matriz de uma imagem para outra
        void copiaImagem(imagemPPM *orig);  //Método que copia todos os atributos de uma imagem para outra
        void escrever(FILE *arq);   //Escrita de arquivo PGM (ASCII): Método que passa os atributos de um objeto para um arquivo
        void somarValor(imagemPPM *resultado, int valorSomar);  //Método responsável por somar uma imagem com um valor
        void multiplicarValor(imagemPPM *resultado, int valorMultiplicar); //Método responsável por multiplicar uma imagem com um valor
        void somar(imagemPPM *img2, imagemPPM *resultado);   //Método responsável por somar duas imagens (this + img2)
        void subtrair(imagemPPM *img2, imagemPPM *resultado);//Método responsável por subtrair duas imagens (this - img2)
        void fatiamentoNI1(imagemPPM *resultado, int a, int b, int limiar1, int limiar2); //Método responsável por aplicar o fatiamento de níveis de intensidade: no intervalo AB recebe um nível de cinza, fora recebe outro
        void fatiamentoNI2(imagemPPM *resultado, int a, int b, int limiar1); //Método responsável por aplicar o fatiamento de níveis de intensidade: no intervalo AB recebe um nível de cinza, fora não há alteração
        void aplicarFiltroMedia(imagemPPM *resultado, int n);           //Método responsável por aplicar o filtro da média. Considera o filtro apenas nos pixels internos (o filtro não fica fora da imagem)
        void aplicarFiltroMediana(imagemPPM *resultado, int n);         //Método responsável por aplicar o filtro da mediana. Considera o filtro apenas nos pixels internos (o filtro não fica fora da imagem)
        void aplicarFiltroLaplaciano(imagemPPM *resultado, int centro); //Método responsável por aplicar o filtro laplaciano. Considera o filtro apenas nos pixels internos (o filtro não fica fora da imagem)
        int * getHistograma();                          //Método responsável por capturar o histograma de uma imagem e retorná-lo
        int * getHistogramaAcum(int *histograma);       //Método responsável por calcular a soma acumulada de um histrogama de uma imagem e retorná-lo
        void equalizarHistograma(imagemPPM *resultado); //Método responsável por aplicar a equalização de histograma global
        void salvaCanais(FILE *r, FILE *g, FILE *b);    //Método responsável por extrair os canais R, G e B (colorida para 3 canais)
};

#endif /* IMAGEMPPM */

