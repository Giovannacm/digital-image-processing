#include <iostream>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#include "imagemPGM.hpp"
#include "imagemPPM.hpp"

using namespace std;

/* Fun��o para ler um arquivo com a sequ�ncia de processamentos */
void lerSequencia(FILE *arquivo)
{
    FILE *arqImg = NULL, *arqImg2 = NULL, *arqImgResultado = NULL, *FR = NULL, *FG = NULL, *FB = NULL;
    imagemPGM *imgPGM = NULL, *imgPGM2 = NULL, *imgPGMResultado = NULL, *imgR = NULL, *imgG = NULL, *imgB = NULL;
    imagemPPM *imgPPM = NULL, *imgPPM2 = NULL,*imgPPMResultado = NULL;
    char linha[100], *palavra, *nomeArquivo, *nomeArquivoEsq, *nomeArquivo2, *nomeArquivoR, *nomeArquivoG, *nomeArquivoB, nomeArquivoRGB[50], *extensao = NULL, copiaNomeArquivo[50], copiaNomeArquivo2[50];
    int valor, valor2, a, b, n, centro, tipo;
    while(!feof(arquivo))   //Enquanto n�o acabar o arquivo
    {
        fgets(linha, 100, arquivo); //� feito uma leitura das linhas
        linha[strlen(linha) - 1] == '\n' ? linha[strlen(linha) - 1] = ' ' : linha[strlen(linha)] = ' '; //Eliminando o operador de quebra de lina (se esta n�o for a �ltima)
        palavra = strtok(linha, " "); //Por meio da fun��o strtok, s�o lidas todas as palavras da linha. Esta fun��o � respons�vel por retornar tokens seprarados por um determinado caractere (nesse caso, o espa�o)

        while(palavra != NULL) //Enquando n�o acabar de dividir a linha em tokens, � feito uma compara��o de cada palavra retornada para verificar qual funcionalidade ser� feita
        {
            if(strcmp(palavra,"leitura") == 0) //Se for para ler uma imagem
            {
                nomeArquivo = strtok(NULL, "."); //� capturado o pr�ximo token da linha at� o ".", para que seja obtido o nome do arquivo
                strcpy(copiaNomeArquivo, nomeArquivo); //� feito uma c�pia deste nome do arquivo para eventuais consultas

                extensao = strtok(NULL, " "); //� capturado o pr�ximo token da linha at� o pr�ximo espa�o para que seja obtido a extens�o do arquivo (pgm ou ppm)
                if(strcmp(extensao,"pgm") == 0) //Se a extens�o for pgm
                    tipo = 2;   //Atribui 2 � uma vari�vel tipo para que sejam separados os processamentos dos respetivos tipos de imagem
                else if(strcmp(extensao,"ppm") == 0) //Se for ppm
                    tipo = 3; //Atribui 3 � vari�vel tipo

                if(tipo == 2) //Se a imagem lida � do tipo pgm
                {
                    if((arqImg = fopen(strcat(nomeArquivo, ".pgm"), "r+"))==NULL) //� aberto o arquivo descrito pelo usu�rio
                    {
                        cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << nomeArquivo;
                        return;
                    }
                    imgPGM = new imagemPGM(arqImg); //E � instanciado um objeto para a imagem de entrada, onde os dados desse arquivo s�o passados para o objeto
                    imgPGMResultado = new imagemPGM(imgPGM->getAltura(), imgPGM->getLargura(), imgPGM->getMax(), imgPGM->getTipo()); //Al�m disso, � instanciado um objeto para armazenar o resultado do pr�ximo processamento, seus atributos s�o dados pelos atributos da imagem aberta (s� n�o � copiada a matriz)

                    cout << "    [IMAGEM PGM CARREGADA COM SUCESSO]\n";
                }
                else if(tipo == 3) //Se a imagem lida � do tipo ppm
                {
                    if((arqImg = fopen(strcat(nomeArquivo, ".ppm"), "r+"))==NULL) //� aberto o arquivo descrito pelo usu�rio
                    {
                        cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << nomeArquivo;
                        return;
                    }
                    imgPPM = new imagemPPM(arqImg); //E � instanciado um objeto para a imagem de entrada, onde os dados desse arquivo s�o passados para o objeto
                    imgPPMResultado = new imagemPPM(imgPPM->getAltura(), imgPPM->getLargura(), imgPPM->getMax(), imgPPM->getTipo()); //Al�m disso, � instanciado um objeto para armazenar o resultado do pr�ximo processamento, seus atributos s�o dados pelos atributos da imagem aberta (s� n�o � copiada a matriz)

                    cout << "    [IMAGEM PPM CARREGADA COM SUCESSO]\n";
                }
                else //Em caso de erro no tipo da imagem
                {
                    cout << "\n    [ERRO AO LER A IMAGEM]: " << nomeArquivo << "." << extensao;
                    return;
                }
            }
            else if(strcmp(palavra,"escrita") == 0) //Se for para escrever o resultado
            {
                nomeArquivoEsq = strtok(NULL, " "); //� capturado o nome do arquivo (espera-se que esse arquivo tenha a respectiva extens�o do resultado)

                if((arqImgResultado = fopen(nomeArquivoEsq, "w+"))==NULL) //Criando um arquivo com a extens�o definida pelo usu�rio
                {
                    cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << nomeArquivoEsq;
                    return;
                }
                if(tipo == 2) //Se a imagem de entrada for pgm
                    imgPGM->escrever(arqImgResultado); //� aplicado o respectivo m�todo de escrita na imagem com o arquivo resultado
                else if(tipo == 3) //Se a imagem de entrada for ppm
                    imgPPM->escrever(arqImgResultado);
                cout << "    [PROCESSO escrever FINALIZADO COM SUCESSO]\n";
            }
            else if(strcmp(palavra,"somar-valor") == 0) //Se for para somar um valor � imagem
            {
                valor = atoi(strtok(NULL, " ")); //Captura-se o valor

                if(tipo == 2) //E aplica o respectivo m�todo na imagem de acordo com seu tipo
                    imgPGM->somarValor(imgPGMResultado, valor);
                else if(tipo == 3)
                    imgPPM->somarValor(imgPPMResultado, valor);

                cout << "    [PROCESSO somarValor FINALIZADO COM SUCESSO]\n";
            }
            else if(strcmp(palavra,"multiplicar-valor") == 0) //Se for para multiplicar um valor � imagem
            {
                valor = atoi(strtok(NULL, " ")); //Captura-se o valor

                if(tipo == 2) //E aplica o respectivo m�todo na imagem de acordo com seu tipo
                    imgPGM->multiplicarValor(imgPGMResultado, valor);
                else if(tipo == 3)
                    imgPPM->multiplicarValor(imgPPMResultado, valor);

                cout << "    [PROCESSO multiplicarValor FINALIZADO COM SUCESSO]\n";
            }
            else if(strcmp(palavra,"somar-imagem") == 0) //Se for para somar outra imagem � imagem atual
            {
                nomeArquivo2 = strtok(NULL, " "); //� caputrado o nome do segundo arquivo

                if((arqImg2 = fopen(nomeArquivo2, "r+"))==NULL) //� aberto o outro arquivo
                {
                    cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << nomeArquivo2;
                    return;
                }

                if(tipo == 2) //Se a imagem aberta anteriormente for pgm
                {
                    imgPGM2 = new imagemPGM(arqImg2); //Instancia um novo objeto PGM para a imagem 2 de acordo com o arquivo aberto

                    if(imgPGM->getAltura() != imgPGM2->getAltura() && imgPGM->getLargura() != imgPGM2->getLargura()) //Verifica-se as dimens�es das duas imagens
                    {
                        cout << "\n    [ERRO]: AS DUAS IMAGENS DEVEM TER AS MESMAS DIMENS�ES!\n";
                        return;
                    }

                    imgPGM->somar(imgPGM2, imgPGMResultado); //Aplica o m�todo de somar duas imagens
                }
                else if(tipo == 3) //Se a imagem aberta anteriormente for ppm
                {
                    imgPPM2 = new imagemPPM(arqImg2); //Instancia um novo objeto PPM para a imagem 2 de acordo com o arquivo aberto

                    if(imgPPM->getAltura() != imgPPM2->getAltura() && imgPPM->getLargura() != imgPPM2->getLargura()) //Verifica-se as dimens�es das duas imagens
                    {
                        cout << "\n    [ERRO]: AS DUAS IMAGENS DEVEM TER AS MESMAS DIMENS�ES!\n";
                        return;
                    }

                    imgPPM->somar(imgPPM2, imgPPMResultado); //Aplica o m�todo de somar duas imagens
                }

                cout << "    [PROCESSO somar FINALIZADO COM SUCESSO]\n";
            }
            else if(strcmp(palavra,"subtrair-imagem") == 0) //Se for para subtrair outra imagem � imagem atual
            {
                nomeArquivo2 = strtok(NULL, " "); //� caputrado o nome do segundo arquivo

                if((arqImg2 = fopen(nomeArquivo2, "r+"))==NULL) //� aberto o outro arquivo
                {
                    cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << nomeArquivo2;
                    return;
                }

                if(tipo == 2) //Se a imagem aberta anteriormente for pgm
                {
                    imgPGM2 = new imagemPGM(arqImg2); //Instancia um novo objeto PGM para a imagem 2 de acordo com o arquivo aberto

                    if(imgPGM->getAltura() != imgPGM2->getAltura() && imgPGM->getLargura() != imgPGM2->getLargura()) //Verifica-se as dimens�es das duas imagens
                    {
                        cout << "\n    [ERRO]: AS DUAS IMAGENS DEVEM TER AS MESMAS DIMENS�ES!\n";
                        return;
                    }

                    imgPGM->subtrair(imgPGM2, imgPGMResultado); //Aplica o m�todo de subtrair duas imagens
                }
                else if(tipo == 3) //Se a imagem aberta anteriormente for ppm
                {
                    imgPPM2 = new imagemPPM(arqImg2); //Instancia um novo objeto PPM para a imagem 2 de acordo com o arquivo aberto

                    if(imgPPM->getAltura() != imgPPM2->getAltura() && imgPPM->getLargura() != imgPPM2->getLargura()) //Verifica-se as dimens�es das duas imagens
                    {
                        cout << "\n    [ERRO]: AS DUAS IMAGENS DEVEM TER AS MESMAS DIMENS�ES!\n";
                        return;
                    }

                    imgPPM->subtrair(imgPPM2, imgPPMResultado); //Aplica o m�todo de subtrair duas imagens
                }

                cout << "    [PROCESSO subtrair FINALIZADO COM SUCESSO]\n";
            }
            else if(strcmp(palavra,"fatiamento1") == 0) //Se for para aplicar o primeiro tipo de fatiamento
            {
                a = atoi(strtok(NULL, " ")); //� capturado o limite inferior do intervalo
                b = atoi(strtok(NULL, " ")); //E o limite superior
                valor = atoi(strtok(NULL, " ")); //Al�m disso, o primeiro valor (para o intervalo [a,b])
                valor2 = atoi(strtok(NULL, " ")); //E o segundo valor para os n�veis restantes

                if(tipo == 2) //Se a imagem de entrada for pgm
                    imgPGM->fatiamentoNI1(imgPGMResultado, a, b, valor, valor2); //Chama o respetivo m�todo para esse tipo de objeto
                else if(tipo == 3) //Se for ppm
                    imgPPM->fatiamentoNI1(imgPPMResultado, a, b, valor, valor2); //Chama o respetivo m�todo para esse tipo de objeto

                cout << "    [PROCESSO fatiamentoNI1 FINALIZADO COM SUCESSO]\n";
            }
            else if(strcmp(palavra,"fatiamento2") == 0) //Se for para aplicar o segundo tipo de fatiamento
            {
                a = atoi(strtok(NULL, " ")); //� capturado o limite inferior do intervalo
                b = atoi(strtok(NULL, " ")); //E o limite superior
                valor = atoi(strtok(NULL, " ")); //Al�m do primeiro valor (para o intervalo [a,b])

                if(tipo == 2) //Se a imagem de entrada for pgm
                    imgPGM->fatiamentoNI2(imgPGMResultado, a, b, valor); //Chama o respetivo m�todo para esse tipo de objeto
                else if(tipo == 3) //Se for ppm
                    imgPPM->fatiamentoNI2(imgPPMResultado, a, b, valor); //Chama o respetivo m�todo para esse tipo de objeto

                cout << "    [PROCESSO fatiamentoNI2 FINALIZADO COM SUCESSO]\n";
            }
            else if(strcmp(palavra,"filtro-media") == 0) //Se for para aplicar o filtro da m�dia
            {
                n = atoi(strtok(NULL, " ")); //� capturado o tamanho do filtro

                if(tipo == 2) //Se a imagem de entrada for pgm
                    imgPGM->aplicarFiltroMedia(imgPGMResultado, n); //Chama o respetivo m�todo para esse tipo de objeto
                else if(tipo == 3) //Se for ppm
                    imgPPM->aplicarFiltroMedia(imgPPMResultado, n); //Chama o respetivo m�todo para esse tipo de objeto

                cout << "    [PROCESSO aplicarFiltroMedia FINALIZADO COM SUCESSO]\n";
            }
            else if(strcmp(palavra,"filtro-mediana") == 0) //Se for para aplicar o filtro da mediana
            {
                n = atoi(strtok(NULL, " ")); //� capturado o tamanho do filtro

                if(tipo == 2) //Se a imagem de entrada for pgm
                    imgPGM->aplicarFiltroMediana(imgPGMResultado, n); //Chama o respetivo m�todo para esse tipo de objeto
                else if(tipo == 3) //Se for ppm
                    imgPPM->aplicarFiltroMediana(imgPPMResultado, n); //Chama o respetivo m�todo para esse tipo de objeto

                cout << "    [PROCESSO aplicarFiltroMediana FINALIZADO COM SUCESSO]\n";
            }
            else if(strcmp(palavra,"filtro-laplaciano") == 0) //Se for para aplicar o filtro laplaciano
            {
                centro = atoi(strtok(NULL, " ")); //� capturado o elemento central do filtro

                if(tipo == 2) //Se a imagem de entrada for pgm
                    imgPGM->aplicarFiltroLaplaciano(imgPGMResultado, centro); //Chama o respetivo m�todo para esse tipo de objeto
                else if(tipo == 3) //Se for ppm
                    imgPPM->aplicarFiltroLaplaciano(imgPPMResultado, centro); //Chama o respetivo m�todo para esse tipo de objeto

                cout << "    [PROCESSO aplicarFiltroLaplaciano FINALIZADO COM SUCESSO]\n";
            }
            else if(strcmp(palavra,"equalizar-histograma") == 0) //Se for para equalizar o histograma
            {
                if(tipo == 2) //Verifica-se o tipo da imagem de entrada (pgm ou ppm). Nesse caso, se for pgm
                    imgPGM->equalizarHistograma(imgPGMResultado); //Chama o respetivo m�todo para esse tipo de objeto
                else if(tipo == 3) //Se ela for ppm
                    imgPPM->equalizarHistograma(imgPPMResultado); //Chama o respetivo m�todo para esse tipo de objeto

                cout << "    [PROCESSO equalizarHistograma FINALIZADO COM SUCESSO]\n";
            }
            else if(strcmp(palavra,"extrair-canais") == 0) //Se for para extrair canais
            {
                if(tipo == 3) //A imagem de entrada deve ser ppm
                {
                    nomeArquivoR = strtok(NULL, " "); //� feito a leitura do nome do arquivo que ir� armazenar o canal R
                    nomeArquivoG = strtok(NULL, " "); //� feito a leitura do nome do arquivo que ir� armazenar o canal G
                    nomeArquivoB = strtok(NULL, " "); //� feito a leitura do nome do arquivo que ir� armazenar o canal B

                    if((FR = fopen(nomeArquivoR, "w+"))==NULL) //Cria-se o arquivo para o canal R de acordo com o nome do arquivo
                    {
                        cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << copiaNomeArquivo2;
                        return;
                    }

                    if((FG = fopen(nomeArquivoG, "w+"))==NULL) //Cria-se o arquivo para o canal G de acordo com o nome do arquivo
                    {
                        cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << copiaNomeArquivo2;
                        return;
                    }

                    if((FB = fopen(nomeArquivoB, "w+"))==NULL) //Cria-se o arquivo para o canal B de acordo com o nome do arquivo
                    {
                        cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << copiaNomeArquivo2;
                        return;
                    }

                    imgPPM->salvaCanais(FR, FG, FB); //Chama o m�todo de salvar canais para a imagem
                    cout << "    [PROCESSO salvaCanais FINALIZADO COM SUCESSO]\n";
                }
            }
            else if(strcmp(palavra,"compor-canais") == 0) //Se for para compor canais
            {
                nomeArquivoR = strtok(NULL, " "); //� feito a leitura do nome do arquivo do canal R
                nomeArquivoG = strtok(NULL, " "); //� feito a leitura do nome do arquivo do canal G
                nomeArquivoB = strtok(NULL, " "); //� feito a leitura do nome do arquivo do canal B
                nomeArquivo = strtok(NULL, " "); //� feito a leitura do nome do arquivo que ir� armazenar o resultado da composi��o

                if((FR = fopen(nomeArquivoR, "r+"))==NULL) //Abre o arquivo do canal R
                {
                    cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << nomeArquivoR;
                    return;
                }
                imgR = new imagemPGM(FR); //Passa os dados desse arquivo para um objeto que ir� representar a imagem do canal vermelho

                if((FG = fopen(nomeArquivoG, "r+"))==NULL) //Abre o arquivo do canal G
                {
                    cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << nomeArquivoG;
                    return;
                }
                imgG = new imagemPGM(FG); //Passa os dados desse arquivo para um objeto que ir� representar a imagem do canal verde

                if((FB = fopen(nomeArquivoB, "r+"))==NULL) //Abre o arquivo do canal B
                {
                    cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << nomeArquivoB;
                    return;
                }
                imgB = new imagemPGM(FB); //Passa os dados desse arquivo para um objeto que ir� representar a imagem do canal azul

                if((arqImg = fopen(nomeArquivo, "w+"))==NULL) //� criado um arquivo PPM para armazenar o resultado
                {
                    cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << nomeArquivoRGB;
                    return;
                }
                imgR->carregaCanais(imgG, imgB, arqImg); //� chamado o m�todo de carregar canais para algum dos objetos
                cout << "    [PROCESSO carregaCanais FINALIZADO COM SUCESSO]\n";

                //Como dito nas instru��es de uso, o usu�rio pode escolher a opera��o de compor-canais para criar uma imagem PPM e depois aplicar opera��es sobre essa imagem PPM
                imgPPM = new imagemPPM(arqImg); //Dessa forma, instancia-se um objeto PPM de acordo com o arquivo criado no m�todo de carregar canais, transferindo os dados deste para o objeto
                imgPPMResultado = new imagemPPM(imgPPM->getAltura(), imgPPM->getLargura(), imgPPM->getMax(), imgPPM->getTipo()); //Al�m disso, � instanciado um objeto para armazenar o resultado do pr�ximo processamento, seus atributos s�o dados pelos atributos da imagem aberta (s� n�o � copiada a matriz)
                tipo = 3; //Como ela � PPM, tipo recebe 3
                cout << "    [IMAGEM PPM CARREGADA COM SUCESSO]\n";
            }

            //Nesse ponto algum processamento foi feito. Se imagem foi carregada nessa �ltima opera��o, palavra=leitura.
            //Por�m, se a imagem j� foi aberta em algum processamento anterior, e a opera��o atual foi algum outro processamento (imgPGMResultado != NULL), os dados da imagem resultado s�o transferidos para a imagem atual de modo que ela armazene o processamento (imagem intermedi�ria)
            // e na pr�xima opera��o (se houver) o processamento � aplicado nesta imagem intermedi�ria (imagemPGM/PPM) e o resultado � salvo na imagem resultado.

            if(imgPGMResultado != NULL && tipo == 2 && (strcmp(palavra,"leitura") != 0)) //Se j� foi feito algum processamento (n�o de leitura) e a imagem aberta inicialmente foi PGM
                imgPGM->copiaImagem(imgPGMResultado); //transfere os dados, por meio do m�todo de c�pia de atributos, do resultado atual para a imagem intermedi�ria, de modo que na pr�xima itera��o o processamento seja feito nesse objeto e imgPGMResultado armazene o resultado a opera��o feita na proxima iteracao

            if(imgPPMResultado != NULL && tipo == 3 && (strcmp(palavra,"extrair-canais") != 0) && (strcmp(palavra,"compor-canais") != 0) && (strcmp(palavra,"leitura") != 0)) //Se j� foi feito algum processamento (n�o de leitura) e a imagem aberta inicialmente foi PPM, e essa �ltima opera��o n�o foi de extra��o de canais, j� que n�o foi feita nenhuma altera��o na imagem e nem de composi��o, pois j� foi salva a imagem original
                imgPPM->copiaImagem(imgPPMResultado); //transfere os dados, por meio do m�todo de c�pia de atributos, do resultado atual para a imagem intermedi�ria, de modo que na pr�xima itera��o o processamento seja feito nesse objeto e imgPPMResultado armazene o resultado a opera��o feita na proxima iteracao

            palavra = strtok(NULL, " "); //Capturando a pr�xima palavra, ou tokens separados por espa�o
        }
    }
}

/* Fun��o para exibir o menu principal e retorna a opera��o escolhida pelo usu�rio. Os nomes das imagens s�o passados como par�metro para que seja exibido qual imagem est� aberta */
int menuPrincipal(char *nomePGM, char *nomePPM)
{
    int opcao;
    cout << "    --------------------------------------------------------------------------\n";
    cout << "    ------ PROCESSAMENTO DIGITAL DE IMAGENS - GIOVANNA CARREIRA MARINHO ------\n";
    cout << "    ------------------- TRABALHO: Processamento Sequencial -------------------\n";
    cout << "    --------------------------------------------------------------------------\n";
    cout << "    [0]   SAIR                                                     [99]  AJUDA\n";
    cout << "    [1]   LER ARQUIVO COM A SEQU�NCIA DE PROCESSAMENTOS \n";
    cout << "    --------------------------------------------------------------------------\n";
    cout << "    [2]   LER IMAGEM PGM (tons de cinza)           ABERTA -> " << nomePGM << "\n"; //Mostra qual imagem PGM est� aberta
    cout << "          [3]   SOMAR VALOR \n";
    cout << "          [4]   MULTIPLICAR VALOR \n";
    cout << "          [5]   SOMAR COM OUTRA IMAGEM PGM \n";
    cout << "          [6]   SUBTRAIR OUTRA IMAGEM PGM \n";
    cout << "          [7]   FATIAMENTO DE N�VEIS DE INTENSIDADE (1) NA IMAGEM PGM \n";
    cout << "          [8]   FATIAMENTO DE N�VEIS DE INTENSIDADE (2) NA IMAGEM PGM \n";
    cout << "          [9]   APLICAR FILTRO NA IMAGEM PGM \n";
    cout << "          [10]  EQUALIZAR HISTOGRAMA DA IMAGEM PGM\n";
    cout << "    [11]  ESCREVER/SALVAR IMAGEM PGM \n";
    cout << "    --------------------------------------------------------------------------\n";
    cout << "    [12]  LER IMAGEM PPM (colorida)                ABERTA -> " << nomePPM << "\n"; //Mostra qual imagem PPM est� aberta
    cout << "          [13]  SOMAR VALOR \n";
    cout << "          [14]  MULTIPLICAR VALOR \n";
    cout << "          [15]  SOMAR COM OUTRA IMAGEM PPM \n";
    cout << "          [16]  SUBTRAIR OUTRA IMAGEM PPM \n";
    cout << "          [17]  FATIAMENTO DE N�VEIS DE INTENSIDADE (1) NA IMAGEM PPM \n";
    cout << "          [18]  FATIAMENTO DE N�VEIS DE INTENSIDADE (2) NA IMAGEM PPM \n";
    cout << "          [19]  APLICAR FILTRO NA IMAGEM PPM \n";
    cout << "          [20]  EQUALIZAR HISTOGRAMA DA IMAGEM PPM\n";
    cout << "          [21]  SALVAR CANAIS DA IMAGEM PPM (1 arquivo -> 3 arquivos) \n";
    cout << "    [22]  ESCREVER/SALVAR IMAGEM PPM \n";
    cout << "    [23]  CARREGAR CANAIS PARA UMA IMAGEM PPM (3 arquivos -> 1 arquivo) \n";
    cout << "    --------------------------------------------------------------------------\n";
    cout << "    [Digite a opera��o desejada]: ";
    cin >> opcao;   //Lendo a opera��o solicitada pelo usu�rio
    return(opcao);  //Retornando o valor inteiro escolhido
}

/* Fun��o para exibir instru��es de uso para o usu�rio */
void ajuda()
{
    cout << "\n  [AJUDA - Instru��es de uso da aplica��o] \n";
    cout << "  O processamento sequ�ncial pode ser feito de duas formas: \n";

    cout << "\n  -> A sequ�ncia � descrita em arquivo: \n";
    cout << "     Para uso dessa opera��o � necess�rio escolher a opera��o 1. Um arquivo texto (.txt) ser� lido e a sequ�ncia de \n";
    cout << "    opera��es descrita nele ser� aplicada. A seguir, ser�o apresentadas as sintaxes de cada opera��o: \n";
    cout << "     - Leitura de uma imagem PGM (extens�o .pgm) ou PPM (extens�o .ppm): leitura nome_arquivo.extensao \n";
    cout << "       - Somar a imagem com um valor: somar-valor valor \n";
    cout << "       - Multiplicar a imagem com um valor: multiplicar-valor valor \n";
    cout << "       - Somar a imagem com outra imagem (mesma extens�o): somar-imagem nome_arquivo2.extensao \n";
    cout << "       - Subtrair da imagem outra imagem (mesma extens�o): subtrair-imagem nome_arquivo2.extensao \n";
    cout << "       - Fatiamento de n�veis de intensidade: \n";
    cout << "         - pixels no intervalo [A,B] recebem valor1 e fora recebem valor2: fatiamento1 A B valor1 valor2 \n";
    cout << "         - pixels no intervalo [A,B] recebem valor1 e fora n�o ocorre altera��es: fatiamento2 A B valor1 \n";
    cout << "       - Aplicar filtro na imagem: \n";
    cout << "         - filtro da m�dia de dimens�o n por n: filtro-media n \n";
    cout << "         - filtro da mediana de dimens�o n por n: filtro-mediana n \n";
    cout << "         - filtro laplaciano 3x3 com o elemento central x: filtro-laplaciano x \n";
    cout << "       - Equalizar histograma: equalizar-histograma \n";
    cout << "       - Extrair canais RGB (apenas para imagem PPM) de uma imagem PPM e salvar cada canal em uma imagem PGM \n";
    cout << "                   extrair-canais nome_arquivoR.pgm nome_arquivoG.pgm nome_arquivoB.pgm\n";
    cout << "     - Escrita de uma imagem resultado PGM (extens�o .pgm) ou PPM (extens�o .ppm): escrita nome_arq_result.extensao\n";
    cout << "     - Compor canais RGB (3 PGM -> 1 PPM) de uma imagem PPM a partir de 3 arquivos PGM e salvar\n";
    cout << "                   compor-canais nome_arquivoR.pgm nome_arquivoG.pgm nome_arquivoB.pgm nome_arquivoRGB.ppm\n";
    cout << "     � necess�rio ter uma instru��o de abertura de imagem para que sejam aplicados os processamentos. Ou seja, antes\n";
    cout << "    de descrever as opera��es no arquivo, a primeira instru��o deve ser de leitura da imagem. Ou, de acordo com a\n";
    cout << "    necessidade, o usu�rio pode compor/criar uma imagem PPM (instru��o compor-canais) e, em seguida, colocar instru��es\n";
    cout << "    para serem aplicadas nesta imagem colorida criada. \n";
    cout << "     No final de cada processamento, � necess�rio um comando de escrita para que o resultado atual seja salvo no \n";
    cout << "    diret�rio da aplica��o. \n";
    cout << "     � importante destacar que se o usu�rio executou apenas a opera��o de extrair-canais sobre uma imagem PPM, o \n";
    cout << "    processo de escrita ser� desnecess�rio, uma vez que a imagem aberta anteriormente n�o foi alterada, apenas \n";
    cout << "    seus canais foram salvos. Al�m disso, n�o se faz necess�ria a instru��o de escrita ap�s a opera��o de \n";
    cout << "    compor-canais, pois ao fazer essa opera��o a imagem PPM j� � criada e salva no diret�rio da aplica��o \n";
    cout << "    e pode ser utilizada para aplicar outros processamentos em seguida. \n";
    cout << "     Comandos n�o dispon�veis na aplica��o, n�o ser�o aplicados. Dessa forma, espera-se que n�o h� erros de sintaxe. \n";
    cout << "    Al�m disso, como pode ser observado, ao passar o nome de uma imagem, � necess�rio dizer sua extens�o. \n";

    cout << "\n  -> Interativamente: o usu�rio escolhe os processamentos passo-a-passo \n";
    cout << "     Neste caso, o usu�rio deve abrir uma imagem (op��o 2 para PGM e 12 para PPM) antes de aplicar a sequ�ncia. \n";
    cout << "     As opera��es dispon�veis para cada tipo de imagem encontram-se abaixo de cada op��o de leitura da mesma, ou \n";
    cout << "    seja, op��es 3 a 10 s�o para imagens PGM e 13 a 21 para imagens PPM. Sendo assim, uma vez aberto a imagem, os \n";
    cout << "    respectivos processamentos podem ser aplicados sequ�ncialmente, um ap�s o outro. A qualquer momento o resultado \n";
    cout << "    atual pode ser salvo por meio da op��o 11 (para imagem PGM) ou 22 (para imagem PPM). \n";
    cout << "     Para uso da opera��o descrita pela op��o 23, n�o � necess�ria a abertura pr�via de uma imagem, basta que o \n";
    cout << "    usu�rio escolha a op��o diretamente. Em seguida, como uma nova imagem foi composta, ele pode aplicar opera��es \n";
    cout << "    (13 a 21) sobre esta imagem RGB/PGM. \n";
}

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "Portuguese");
    FILE *arqPGM = NULL, *arqPGMResultado = NULL, *arqPGM2 = NULL, *arqPPM = NULL, *arqPPMResultado = NULL, *arqPPM2 = NULL, *R = NULL, *G = NULL, *B = NULL, *ArqSequencia = NULL;
    imagemPGM *imgPGM = NULL, *imgPGMResultado = NULL, *imgPGM2 = NULL, *imgR = NULL, *imgG = NULL, *imgB = NULL;
    imagemPPM *imgPPM = NULL, *imgPPMResultado = NULL, *imgPPM2 = NULL;
    char aux[50], nomeArquivo[50], nomeArquivo2[50], nomeArquivoRes[50], copiaNomePGM[50] = "nenhuma", copiaNomePPM[50] = "nenhuma", nomeArquivoSeq[50], tipo;
    int operacao, limiar, limiar2, a, b, n, valor, centro, erro;

    do
    {
        operacao = menuPrincipal(copiaNomePGM, copiaNomePPM); //Lendo a opera��o solicitada pelo usu�rio
        erro = 0;
        system("cls"); //Limpando a tela
        switch(operacao) //Verificando qual opera��o foi solicitada
        {
            case 0:
                cout << "\n    [VOC� ESCOLHEU A OP��O: SAIR]\n";

                break;

            case 1:
                cout << "\n    [LER ARQUIVO COM A SEQU�NCIA DE PROCESSAMENTOS]\n";

                cout << "    [NOME DO ARQUIVO COM A SEQU�NCIA]: ";
                cin >> nomeArquivoSeq;

                if((ArqSequencia = fopen(strcat(nomeArquivoSeq, ".txt"), "r+"))==NULL) //Abrindo o arquivo com a sequ�ncia de processamentos
                {
                    cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << nomeArquivoSeq;
                    break;
                }

                lerSequencia(ArqSequencia); //Chamando a fun��o para ler o arquivo
                fclose(ArqSequencia); //Fechando o arquivo

                cout << "    [PROCESSO lerSequencia FINALIZADO COM SUCESSO]\n";

                break;

            case 2:
                cout << "\n    [LER IMAGEM PGM (tons de cinza)]\n";

                cout << "    [NOME DO ARQUIVO .pgm]: ";
                cin >> nomeArquivo;
                strcpy(copiaNomePGM, nomeArquivo); //C�pia do nome do arquivo para eventuais consultas

                if((arqPGM = fopen(strcat(nomeArquivo, ".pgm"), "r+"))==NULL) //Abrindo o arquivo indicado pelo usu�rio
                {
                    cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << nomeArquivo;
                    break;
                }

                imgPGM = new imagemPGM(arqPGM); //Instanciando um objeto do tipo imagem PGM, nesse caso, os dados dados s�o transferidos do arquivo para o objeto
                imgPGMResultado = new imagemPGM(imgPGM->getAltura(), imgPGM->getLargura(), imgPGM->getMax(), imgPGM->getTipo()); //Al�m disso, � instanciado um objeto para armazenar o resultado do pr�ximo processamento, seus atributos s�o dados pelos atributos da imagem aberta (s� n�o � copiada a matriz)
                cout << "    [IMAGEM PGM CARREGADA COM SUCESSO]\n";

                break;

            case 3:
                cout << "\n    [SOMAR VALOR]\n";

                if(arqPGM == NULL || imgPGM == NULL) //Verificando se j� existe uma imagem aberta antes dessa opera��o
                {
                    cout << "    [ERRO]: � NECESS�RIO CARREGAR UMA IMAGEM PARA FAZER ESSA OPERA��O!\n";
                    break;
                }

                cout << "    [VALOR PARA SOMAR]: ";
                cin >> valor;

                imgPGM->somarValor(imgPGMResultado, valor); //Aplicando o m�todo para a imagem e armazenando o resultado final na imagem resultado
                cout << "    [PROCESSO somarValor FINALIZADO COM SUCESSO]\n";

                break;

            case 4:
                cout << "\n    [MULTIPLICAR VALOR]\n";

                if(arqPGM == NULL || imgPGM == NULL) //Verificando se j� existe uma imagem aberta antes dessa opera��o
                {
                    cout << "    [ERRO]: � NECESS�RIO CARREGAR UMA IMAGEM PARA FAZER ESSA OPERA��O!\n";
                    break;
                }

                cout << "    [VALOR PARA MULTIPLICAR]: ";
                cin >> valor;

                imgPGM->multiplicarValor(imgPGMResultado, valor); //Aplicando o m�todo para a imagem e armazenando o resultado final na imagem resultado

                cout << "    [PROCESSO multiplicarValor FINALIZADO COM SUCESSO]\n";

                break;

            case 5:
                cout << "\n    [SOMAR COM OUTRA IMAGEM PGM]\n";

                if(arqPGM == NULL || imgPGM == NULL) //Verificando se j� existe uma imagem aberta antes dessa opera��o
                {
                    cout << "    [ERRO]: � NECESS�RIO CARREGAR UMA IMAGEM PARA FAZER ESSA OPERA��O!\n";
                    break;
                }

                cout << "    [NOME DO ARQUIVO .pgm DA SEGUNDA IMAGEM]: ";
                cin >> nomeArquivo2;

                if((arqPGM2 = fopen(strcat(nomeArquivo2, ".pgm"), "r+"))==NULL) //Abrindo o arquivo pgm da segunda imagem
                {
                    cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << nomeArquivo2;
                    erro = 1;
                    break;
                }
                imgPGM2 = new imagemPGM(arqPGM2);   //Criando um objeto para a segunda imagem (passando os dados do arquivo anterior neste objeto)
                cout << "    [IMAGEM 2 PGM CARREGADA COM SUCESSO]\n";

                if(imgPGM->getLargura() != imgPGM2->getLargura() && imgPGM->getAltura() != imgPGM2->getAltura()) //Verificando as dimens�es das duas imagens
                {
                    cout << "\n    [ERRO]: AS DUAS IMAGENS DEVEM TER AS MESMAS DIMENS�ES!\n";
                    erro = 1;
                    break;
                }

                imgPGM->somar(imgPGM2, imgPGMResultado); //Aplicando o m�todo para a imagem e armazenando o resultado final na imagem resultado

                cout << "    [PROCESSO somar FINALIZADO COM SUCESSO]\n";

                break;

            case 6:
                cout << "\n    [SUBTRAIR OUTRA IMAGEM PGM]\n";

                if(arqPGM == NULL || imgPGM == NULL) //Verificando se j� existe uma imagem aberta antes dessa opera��o
                {
                    cout << "    [ERRO]: � NECESS�RIO CARREGAR UMA IMAGEM PARA FAZER ESSA OPERA��O!\n";
                    erro = 1;
                    break;
                }

                cout << "    [NOME DO ARQUIVO .pgm DA SEGUNDA IMAGEM]: ";
                cin >> nomeArquivo2;

                if((arqPGM2 = fopen(strcat(nomeArquivo2, ".pgm"), "r+"))==NULL) //Abrindo o arquivo pgm da segunda imagem
                {
                    cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << nomeArquivo2;
                    erro = 1;
                    break;
                }
                imgPGM2 = new imagemPGM(arqPGM2); //Criando um objeto para a segunda imagem (passando os dados do arquivo anterior neste objeto)
                cout << "    [IMAGEM 2 PGM CARREGADA COM SUCESSO]\n";

                if(imgPGM->getLargura() != imgPGM2->getLargura() && imgPGM->getAltura() != imgPGM2->getAltura()) //Verificando as dimens�es das duas imagens
                {
                    cout << "\n    [ERRO]: AS DUAS IMAGENS DEVEM TER AS MESMAS DIMENS�ES!\n";
                    erro = 1;
                    break;
                }

                imgPGM->subtrair(imgPGM2, imgPGMResultado); //Aplicando o m�todo para a imagem e armazenando o resultado final na imagem resultado

                cout << "    [PROCESSO subtrair FINALIZADO COM SUCESSO]\n";

                break;

            case 7:
                cout << "\n    [FATIAMENTO DE N�VEIS DE INTENSIDADE (1) NA IMAGEM PGM]\n";
                cout << "    O intervalo [A, B] recebe um n�vel de cinza e os pixels fora recebem outro\n";

                if(arqPGM == NULL || imgPGM == NULL) //Verificando se j� existe uma imagem aberta antes dessa opera��o
                {
                    cout << "    [ERRO]: � NECESS�RIO CARREGAR UMA IMAGEM PARA FAZER ESSA OPERA��O!\n";
                    break;
                }

                cout << "    [LIMITE INFERIOR DO INTERVALO (A)]: ";
                cin >> a;

                cout << "    [LIMITE SUPERIOR DO INTERVALO (B)]: ";
                cin >> b;

                cout << "    [LIMIAR PARA O INTERVALO [A,B]]: ";
                cin >> limiar;

                cout << "    [LIMIAR PARA OS N�VEIS DE INTENSIDADE RESTANTES]: ";
                cin >> limiar2;

                imgPGM->fatiamentoNI1(imgPGMResultado, a, b, limiar, limiar2); //Aplicando o m�todo para a imagem e armazenando o resultado final na imagem resultado

                cout << "    [PROCESSO fatiamentoNI1 FINALIZADO COM SUCESSO]\n";

                break;

            case 8:
                cout << "\n    [FATIAMENTO DE N�VEIS DE INTENSIDADE (2) NA IMAGEM PGM]\n";
                cout << "    O intervalo [A, B] recebe um n�vel de cinza e os pixels fora n�o possuem altera��es\n";

                if(arqPGM == NULL || imgPGM == NULL) //Verificando se j� existe uma imagem aberta antes dessa opera��o
                {
                    cout << "    [ERRO]: � NECESS�RIO CARREGAR UMA IMAGEM PARA FAZER ESSA OPERA��O!\n";
                    break;
                }

                cout << "    [LIMITE INFERIOR DO INTERVALO (A)]: ";
                cin >> a;

                cout << "    [LIMITE SUPERIOR DO INTERVALO (B)]: ";
                cin >> b;

                cout << "    [LIMIAR PARA O INTERVALO [A,B]]: ";
                cin >> limiar;

                imgPGM->fatiamentoNI2(imgPGMResultado, a, b, limiar); //Aplicando o m�todo para a imagem e armazenando o resultado final na imagem resultado

                cout << "    [PROCESSO fatiamentoNI2 FINALIZADO COM SUCESSO]\n";

                break;

            case 9:
                cout << "\n    [APLICAR FILTRO NA IMAGEM PGM]\n";

                if(arqPGM == NULL || imgPGM == NULL) //Verificando se j� existe uma imagem aberta antes dessa opera��o
                {
                    cout << "    [ERRO]: � NECESS�RIO CARREGAR UMA IMAGEM PARA FAZER ESSA OPERA��O!\n";
                    break;
                }

                cout << "    [A/a] FILTRO DA M�DIA \n    [M/m] FILTRO DA MEDIANA \n    [L/l] FILTRO LAPLACIANO \n";
                cout << "    [FILTRO ESCOLHIDO]: ";
                cin >> tipo;

                if(tipo=='A' || tipo=='a') //Se for o filtro da m�dia
                {
                    cout << "    [DIMENS�O DO FILTRO]: ";
                    cin >> n;
                    imgPGM->aplicarFiltroMedia(imgPGMResultado, n); //Aplica o respectivo m�todo para a imagem e armazenando o resultado final na imagem resultado
                    cout << "    [PROCESSO aplicarFiltroMedia FINALIZADO COM SUCESSO]\n";
                }
                else if(tipo=='M' || tipo=='m') //Se for o filtro da mediana
                {
                    cout << "    [DIMENS�O DO FILTRO]: ";
                    cin >> n;
                    imgPGM->aplicarFiltroMediana(imgPGMResultado, n); //Aplica o respectivo m�todo para a imagem e armazenando o resultado final na imagem resultado
                    cout << "    [PROCESSO aplicarFiltroMediana FINALIZADO COM SUCESSO]\n";
                }
                else if(tipo=='L' || tipo=='l') //Se for o filtro laplaciano
                {
                    cout << "    [ELEMENTO CENTRAL DO FILTRO]: ";
                    cin >> centro;
                    imgPGM->aplicarFiltroLaplaciano(imgPGMResultado, centro); //Aplica o respectivo m�todo para a imagem e armazenando o resultado final na imagem resultado
                    cout << "    [PROCESSO aplicarFiltroLaplaciano FINALIZADO COM SUCESSO]\n";
                }
                else //Erro no tipo do filtro
                {
                    cout << "    [ERRO]: TIPO DE FILTRO INV�LIDO!\n";
                    erro = 1;
                    break;
                }

                break;

            case 10:
                cout << "\n    [EQUALIZAR HISTOGRAMA DA IMAGEM PGM]\n";

                if(arqPGM == NULL || imgPGM == NULL) //Verificando se j� existe uma imagem aberta antes dessa opera��o
                {
                    cout << "    [ERRO]: � NECESS�RIO CARREGAR UMA IMAGEM PARA FAZER ESSA OPERA��O!\n";
                    break;
                }

                imgPGM->equalizarHistograma(imgPGMResultado); //Aplicando o m�todo para a imagem e armazenando o resultado final na imagem resultado

                cout << "    [PROCESSO equalizarHistograma FINALIZADO COM SUCESSO]\n";

                break;

            case 11:
                cout << "\n    [ESCREVER IMAGEM PGM (tons de cinza)]\n";

                if(arqPGM == NULL || imgPGM == NULL) //Verificando se j� existe uma imagem aberta antes dessa opera��o
                {
                    cout << "    [ERRO]: � NECESS�RIO CARREGAR UMA IMAGEM PARA FAZER ESSA OPERA��O!\n";
                    break;
                }

                cout << "    [NOME DO ARQUIVO .pgm QUE IR� ARMAZENAR O RESULTADO ATUAL]: ";
                cin >> nomeArquivoRes;

                if((arqPGMResultado = fopen(strcat(nomeArquivoRes, ".pgm"), "w+"))==NULL) //Criando o arquivo para armazenar o resultado
                {
                    cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << nomeArquivoRes;
                    erro = 1;
                    break;
                }
                imgPGM->escrever(arqPGMResultado); //Aplicando o m�todo para a imagem com o arquivo pgm do resultado (ou seja, passando os atributos desse objeto para o arquivo)

                cout << "    [PROCESSO escrever FINALIZADO COM SUCESSO]\n";

                break;

            case 12:
                cout << "\n    [LER IMAGEM PPM (colorida)]\n";

                cout << "    [NOME DO ARQUIVO .ppm]: ";
                cin >> nomeArquivo;
                strcpy(copiaNomePPM, nomeArquivo); //C�pia do nome do arquivo para eventuais consultas

                if((arqPPM = fopen(strcat(nomeArquivo, ".ppm"), "r+"))==NULL) //Abrindo o arquivo indicado pelo usu�rio
                {
                    cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << nomeArquivo;
                    break;
                }
                imgPPM = new imagemPPM(arqPPM); //Instanciando um objeto do tipo imagem PPM, nesse caso, os dados dados s�o transferidos do arquivo para o objeto
                imgPPMResultado = new imagemPPM(imgPPM->getAltura(), imgPPM->getLargura(), imgPPM->getMax(), imgPPM->getTipo()); //Al�m disso, � instanciado um objeto para armazenar o resultado do pr�ximo processamento, seus atributos s�o dados pelos atributos da imagem aberta (s� n�o � copiada a matriz)

                cout << "    [IMAGEM PPM CARREGADA COM SUCESSO]\n";

                break;

            case 13:
                cout << "\n    [SOMAR VALOR]\n";

                if(arqPPM == NULL || imgPPM == NULL) //Verificando se j� existe uma imagem aberta antes dessa opera��o
                {
                    cout << "    [ERRO]: � NECESS�RIO CARREGAR UMA IMAGEM PARA FAZER ESSA OPERA��O!\n";
                    break;
                }

                cout << "    [VALOR PARA SOMAR]: ";
                cin >> valor;

                imgPPM->somarValor(imgPPMResultado, valor); //Aplicando o m�todo para a imagem e armazenando o resultado final na imagem resultado

                cout << "    [PROCESSO somarValor FINALIZADO COM SUCESSO]\n";

                break;

            case 14:
                cout << "\n    [MULTIPLICAR VALOR]\n";

                if(arqPPM == NULL || imgPPM == NULL) //Verificando se j� existe uma imagem aberta antes dessa opera��o
                {
                    cout << "    [ERRO]: � NECESS�RIO CARREGAR UMA IMAGEM PARA FAZER ESSA OPERA��O!\n";
                    break;
                }

                cout << "    [VALOR PARA MULTIPLICAR]: ";
                cin >> valor;

                imgPPM->multiplicarValor(imgPPMResultado, valor); //Aplicando o m�todo para a imagem e armazenando o resultado final na imagem resultado

                cout << "    [PROCESSO multiplicarValor FINALIZADO COM SUCESSO]\n";

                break;

            case 15:
                cout << "\n    [SOMAR COM OUTRA IMAGEM PPM]\n";

                if(arqPPM == NULL || imgPPM == NULL) //Verificando se j� existe uma imagem aberta antes dessa opera��o
                {
                    cout << "    [ERRO]: � NECESS�RIO CARREGAR UMA IMAGEM PARA FAZER ESSA OPERA��O!\n";
                    break;
                }

                cout << "    [NOME DO ARQUIVO .ppm DA SEGUNDA IMAGEM]: ";
                cin >> nomeArquivo2;

                if((arqPPM2 = fopen(strcat(nomeArquivo2, ".ppm"), "r+"))==NULL) //Abrindo o arquivo ppm da segunda imagem
                {
                    cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << nomeArquivo2;
                    erro = 1;
                    break;
                }
                imgPPM2 = new imagemPPM(arqPPM2); //Criando um objeto para a segunda imagem (passando os dados do arquivo anterior neste objeto)
                cout << "    [IMAGEM 2 PPM CARREGADA COM SUCESSO]\n";

                if(imgPPM->getLargura() != imgPPM2->getLargura() && imgPPM->getAltura() != imgPPM2->getAltura()) //Verificando as dimens�es das duas imagens
                {
                    cout << "\n    [ERRO]: AS DUAS IMAGENS DEVEM TER AS MESMAS DIMENS�ES\n";
                    erro = 1;
                    break;
                }

                imgPPM->somar(imgPPM2, imgPPMResultado); //Aplicando o m�todo para a imagem e armazenando o resultado final na imagem resultado

                cout << "    [PROCESSO somar FINALIZADO COM SUCESSO]\n";

                break;

            case 16:
                cout << "\n    [SUBTRAIR OUTRA IMAGEM PPM]\n";

                if(arqPPM == NULL || imgPPM == NULL) //Verificando se j� existe uma imagem aberta antes dessa opera��o
                {
                    cout << "    [ERRO]: � NECESS�RIO CARREGAR UMA IMAGEM PARA FAZER ESSA OPERA��O!\n";
                    break;
                }

                cout << "    [NOME DO ARQUIVO .ppm DA SEGUNDA IMAGEM]: ";
                cin >> nomeArquivo2;

                if((arqPPM2 = fopen(strcat(nomeArquivo2, ".ppm"), "r+"))==NULL) //Abrindo o arquivo ppm da segunda imagem
                {
                    cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << nomeArquivo2;
                    erro = 1;
                    break;
                }
                imgPPM2 = new imagemPPM(arqPPM2); //Criando um objeto para a segunda imagem (passando os dados do arquivo anterior neste objeto)
                cout << "    [IMAGEM 2 PPM CARREGADA COM SUCESSO]\n";

                if(imgPPM->getLargura() != imgPPM2->getLargura() && imgPPM->getAltura() != imgPPM2->getAltura()) //Verificando as dimens�es das duas imagens
                {
                    cout << "\n    [ERRO]: AS DUAS IMAGENS DEVEM TER AS MESMAS DIMENS�ES\n";
                    erro = 1;
                    break;
                }

                imgPPM->subtrair(imgPPM2, imgPPMResultado); //Aplicando o m�todo para a imagem e armazenando o resultado final na imagem resultado

                cout << "    [PROCESSO subtrair FINALIZADO COM SUCESSO]\n";

                break;

            case 17:
                cout << "\n    [FATIAMENTO DE N�VEIS DE INTENSIDADE (1) NA IMAGEM PPM]\n";
                cout << "    O intervalo [A, B] recebe um n�vel de cinza e os pixels fora recebem outro\n";

                if(arqPPM == NULL || imgPPM == NULL) //Verificando se j� existe uma imagem aberta antes dessa opera��o
                {
                    cout << "    [ERRO]: � NECESS�RIO CARREGAR UMA IMAGEM PARA FAZER ESSA OPERA��O!\n";
                    break;
                }

                cout << "    [LIMITE INFERIOR DO INTERVALO (A)]: ";
                cin >> a;

                cout << "    [LIMITE SUPERIOR DO INTERVALO (B)]: ";
                cin >> b;

                cout << "    [LIMIAR PARA O INTERVALO [A,B]]: ";
                cin >> limiar;

                cout << "    [LIMIAR PARA OS N�VEIS DE INTENSIDADE RESTANTES]: ";
                cin >> limiar2;

                imgPPM->fatiamentoNI1(imgPPMResultado, a, b, limiar, limiar2); //Aplicando o m�todo para a imagem e armazenando o resultado final na imagem resultado

                cout << "    [PROCESSO fatiamentoNI1 FINALIZADO COM SUCESSO]\n";

                break;

            case 18:
                cout << "\n    FATIAMENTO DE N�VEIS DE INTENSIDADE (2) NA IMAGEM PPM]\n";
                cout << "    O intervalo [A, B] recebe um n�vel de cinza e os pixels fora n�o possuem altera��es\n";

                if(arqPPM == NULL || imgPPM == NULL) //Verificando se j� existe uma imagem aberta antes dessa opera��o
                {
                    cout << "    [ERRO]: � NECESS�RIO CARREGAR UMA IMAGEM PARA FAZER ESSA OPERA��O!\n";
                    break;
                }

                cout << "    [LIMITE INFERIOR DO INTERVALO (A)]: ";
                cin >> a;

                cout << "    [LIMITE SUPERIOR DO INTERVALO (B)]: ";
                cin >> b;

                cout << "    [LIMIAR PARA O INTERVALO [A,B]]: ";
                cin >> limiar;

                imgPPM->fatiamentoNI2(imgPPMResultado, a, b, limiar); //Aplicando o m�todo para a imagem e armazenando o resultado final na imagem resultado

                cout << "    [PROCESSO fatiamentoNI2 FINALIZADO COM SUCESSO]\n";

                break;

            case 19:
                cout << "\n    [APLICAR FILTRO NA IMAGEM PPM]\n";

                if(arqPPM == NULL || imgPPM == NULL) //Verificando se j� existe uma imagem aberta antes dessa opera��o
                {
                    cout << "    [ERRO]: � NECESS�RIO CARREGAR UMA IMAGEM PARA FAZER ESSA OPERA��O!\n";
                    break;
                }

                cout << "    [A/a] FILTRO DA M�DIA \n    [M/m] FILTRO DA MEDIANA \n    [L/l] FILTRO LAPLACIANO \n";
                cout << "    [FILTRO ESCOLHIDO]: ";
                cin >> tipo;

                if(tipo=='A' || tipo=='a') //Se for o filtro da m�dia
                {
                    cout << "    [DIMENS�O DO FILTRO]: ";
                    cin >> n;
                    imgPPM->aplicarFiltroMedia(imgPPMResultado, n); //Aplica o respectivo m�todo para a imagem e armazenando o resultado final na imagem resultado
                    cout << "    [PROCESSO aplicarFiltroMedia FINALIZADO COM SUCESSO]\n";
                }
                else if(tipo=='M' || tipo=='m') //Se for o filtro da mediana
                {
                    cout << "    [DIMENS�O DO FILTRO]: ";
                    cin >> n;
                    imgPPM->aplicarFiltroMediana(imgPPMResultado, n); //Aplica o respectivo m�todo para a imagem e armazenando o resultado final na imagem resultado
                    cout << "    [PROCESSO aplicarFiltroMediana FINALIZADO COM SUCESSO]\n";
                }
                else if(tipo=='L' || tipo=='l') //Se for o filtro laplaciano
                {
                    cout << "    [ELEMENTO CENTRAL DO FILTRO]: ";
                    cin >> centro;
                    imgPPM->aplicarFiltroLaplaciano(imgPPMResultado, centro); //Aplica o respectivo m�todo para a imagem e armazenando o resultado final na imagem resultado
                    cout << "    [PROCESSO aplicarFiltroLaplaciano FINALIZADO COM SUCESSO]\n";
                }
                else //Erro no tipo do filtro
                {
                    cout << "    [ERRO]: TIPO DE FILTRO INV�LIDO!\n";
                    erro = 1;
                    break;
                }

                break;

            case 20:
                cout << "\n    [EQUALIZAR HISTOGRAMA DA IMAGEM PPM]\n";

                if(arqPPM == NULL || imgPPM == NULL) //Verificando se j� existe uma imagem aberta antes dessa opera��o
                {
                    cout << "    [ERRO]: � NECESS�RIO CARREGAR UMA IMAGEM PARA FAZER ESSA OPERA��O!\n";
                    break;
                }

                imgPPM->equalizarHistograma(imgPPMResultado); //Aplicando o m�todo para a imagem e armazenando o resultado final na imagem resultado

                cout << "    [PROCESSO equalizarHistograma FINALIZADO COM SUCESSO]\n";

                break;

            case 21:
                cout << "\n    [SALVAR CANAIS DA IMAGEM PPM (1 arquivo -> 3 arquivos)]\n";

                if(arqPPM == NULL || imgPPM == NULL) //Verificando se j� existe uma imagem aberta antes dessa opera��o
                {
                    cout << "    [ERRO]: � NECESS�RIO CARREGAR UMA IMAGEM PARA FAZER ESSA OPERA��O!\n";
                    break;
                }

                cout << "    [NOME DO ARQUIVO .pgm QUE IR� ARMAZENAR O CANAL VERMELHO]: ";
                cin >> aux;
                if((R = fopen(strcat(aux, ".pgm"), "w+"))==NULL) //Criano um arquivo com o pr�-fixo _R para armazenar o canal vermelho
                {
                    cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << aux;
                    erro = 1;
                    break;
                }

                cout << "    [NOME DO ARQUIVO .pgm QUE IR� ARMAZENAR O CANAL VERDE]: ";
                cin >> aux;
                if((G = fopen(strcat(aux, ".pgm"), "w+"))==NULL) //Criano um arquivo com o pr�-fixo _G para armazenar o canal verde
                {
                    cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << aux;
                    erro = 1;
                    break;
                }

                cout << "    [NOME DO ARQUIVO .pgm QUE IR� ARMAZENAR O CANAL AZUL]: ";
                cin >> aux;
                if((B = fopen(strcat(aux, ".pgm"), "w+"))==NULL) //Criano um arquivo com o pr�-fixo _B para armazenar o canal azul
                {
                    cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << aux;
                    erro = 1;
                    break;
                }

                imgPPM->salvaCanais(R, G, B); //Aplicando o m�todo para a imagem e armazenando os resultados nas respectivas imagens
                cout << "    [PROCESSO salvaCanais FINALIZADO COM SUCESSO]\n";
                break;

            case 22:
                cout << "\n    [ESCREVER IMAGEM PPM (colorida)]\n";

                if(arqPPM == NULL || imgPPM == NULL) //Verificando se j� existe uma imagem aberta antes dessa opera��o
                {
                    cout << "    [ERRO]: � NECESS�RIO CARREGAR UMA IMAGEM PARA FAZER ESSA OPERA��O!\n";
                    break;
                }

                cout << "    [NOME DO ARQUIVO .ppm QUE IR� ARMAZENAR O RESULTADO ATUAL]: ";
                cin >> nomeArquivoRes;

                if((arqPPMResultado = fopen(strcat(nomeArquivoRes, ".ppm"), "w+"))==NULL) //Criando o arquivo para armazenar o resultado
                {
                    cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << nomeArquivoRes;
                    erro = 1;
                    break;
                }
                imgPPM->escrever(arqPPMResultado); //Aplicando o m�todo para a imagem com o arquivo ppm do resultado (ou seja, passando os atributos desse objeto para o arquivo)

                cout << "    [PROCESSO escrever FINALIZADO COM SUCESSO]\n";

                break;

            case 23:
                cout << "\n    [CARREGAR CANAIS PARA UMA IMAGEM PPM (3 arquivos -> 1 arquivo)]\n";

                cout << "    [NOME DO ARQUIVO .pgm DO CANAL VERMELHO]: ";
                cin >> nomeArquivo;
                if((R = fopen(strcat(nomeArquivo, ".pgm"), "r+"))==NULL)
                {
                    cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: "<< nomeArquivo;
                    erro = 1;
                    break;
                }
                imgR = new imagemPGM(R);

                cout << "    [NOME DO ARQUIVO .pgm DO CANAL VERDE]: ";
                cin >> nomeArquivo;
                if((G = fopen(strcat(nomeArquivo, ".pgm"), "r+"))==NULL)
                {
                    cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << nomeArquivo;
                    erro = 1;
                    break;
                }
                imgG = new imagemPGM(G);

                cout << "    [NOME DO ARQUIVO .pgm DO CANAL AZUL]: ";
                cin >> nomeArquivo;
                if((B = fopen(strcat(nomeArquivo, ".pgm"), "r+"))==NULL)
                {
                    cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << nomeArquivo;
                    erro = 1;
                    break;
                }
                imgB = new imagemPGM(B);

                cout << "    [NOME DO ARQUIVO .ppm QUE IR� SALVAR A IMAGEM RESULTADO]: ";
                cin >> nomeArquivo;
                strcpy(copiaNomePPM, nomeArquivo); //C�pia do nome do arquivo para eventuais consultas

                if((arqPPM = fopen(strcat(nomeArquivo, ".ppm"), "w+"))==NULL)
                {
                    cout << "\n    [ERRO DE ABERTURA DO ARQUIVO]: " << aux;
                    erro = 1;
                    break;
                }

                imgR->carregaCanais(imgG, imgB, arqPPM);
                cout << "    [PROCESSO carregaCanais FINALIZADO COM SUCESSO]\n";

                imgPPM = new imagemPPM(arqPPM); //Instanciando um objeto do tipo imagem PPM, nesse caso, os dados dados s�o transferidos do arquivo para o objeto
                imgPPMResultado = new imagemPPM(imgPPM->getAltura(), imgPPM->getLargura(), imgPPM->getMax(), imgPPM->getTipo()); //Al�m disso, � instanciado um objeto para armazenar o resultado do pr�ximo processamento, seus atributos s�o dados pelos atributos da imagem aberta (s� n�o � copiada a matriz)
                cout << "    [IMAGEM PPM CARREGADA COM SUCESSO]\n";

                break;

            case 99:
                ajuda(); //Chamando a fun��o de ajuda, para exibir as instru��es
                break;

            default:
                cout << "\n    [OPERA��O INV�LIDA]\n";
        }

        cout << "\n";
        system("pause"); //Esperando a leitura de alguma tecla para retornar ao menu
        system("cls"); //Limpando a tela

        //Nesse ponto algum processamento foi feito. Se imagem foi carregada nessa �ltima opera��o, opera��o = 2 ou 12.
        //Por�m, se a imagem j� foi aberta em algum processamento anterior, e a opera��o atual foi algum outro processamento (imgPGMResultado != NULL), os dados da imagem resultado s�o transferidos para a imagem atual de modo que ela armazene o processamento (imagem intermedi�ria)
        // e na pr�xima opera��o (se houver) o processamento � aplicado nesta imagem intermedi�ria (imagemPGM/PPM) e o resultado � salvo na imagem resultado.

        if(!erro) //Se n�o houve erros de execu��o (necess�rio para que opera��es nao seja feitas desnecess�riamente)
        {
            if(imgPGM != NULL && imgPGMResultado != NULL && operacao >= 3 && operacao <= 10) //Se j� foi feito algum processamento (sobre a imagem PGM)
                imgPGM->copiaImagem(imgPGMResultado); //transfere os dados, por meio do m�todo de c�pia de atributos, do resultado atual para a imagem intermedi�ria, de modo que na pr�xima itera��o o processamento seja feito nesse objeto e imgPGMResultado armazene o resultado a opera��o feita na proxima iteracao
            if(imgPPM != NULL && imgPPMResultado != NULL && operacao >= 13 && operacao <= 20) //Se j� foi feito algum processamento (sobre a imagem PPM), e essa opera��o n�o foi de extra��o de canais, j� que n�o foi feita nenhuma altera��o na imagem
                imgPPM->copiaImagem(imgPPMResultado); //transfere os dados, por meio do m�todo de c�pia de atributos, do resultado atual para a imagem intermedi�ria, de modo que na pr�xima itera��o o processamento seja feito nesse objeto e imgPPMResultado armazene o resultado a opera��o feita na proxima iteracao
        }
    }
    while(operacao!=0); //Enquando o usu�rio n�o quiser sair da aplica��o

    return(0);
}
