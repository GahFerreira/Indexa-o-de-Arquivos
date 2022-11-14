#ifndef MANIPULADOR_H_
#define MANIPULADOR_H_

#include <iostream>
#include <fstream>

using namespace std;

/**
 * Classe para ler e escrever em arquivo.
 * É estática, pois apenas usamos seus métodos e passamos todos os parâmetros por fora.
*/
class Manipulador
{
    public:
        static int ler_inteiro(ifstream& arquivo);
        static string ler_registro(ifstream& arquivo);
        static void ler_dados(ifstream& arquivo, int tamanho, void *destino);

        static bool escrever_inteiro(ofstream& arquivo, int inteiro);
        static bool escrever_string(ofstream& arquivo, string dados);
        static bool escrever_string(ofstream& arquivo, string dados, int tamanho);
        static bool escrever_dados(ofstream& arquivo, void *dados, int tamanho);
};

#endif // MANIPULADOR_H_
