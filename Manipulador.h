#ifndef MANIPULADOR_H_
#define MANIPULADOR_H_

#include <iostream>
#include <fstream>

using namespace std;

class Manipulador
{
    public:
        int ler_inteiro(ifstream& arquivo);
        string ler_registro(ifstream& arquivo);
        void ler_dados(ifstream& arquivo, int tamanho, void *destino);

        bool escrever_inteiro(ofstream& arquivo, int inteiro);
        bool escrever_string(ofstream& arquivo, string dados);
        bool escrever_string(ofstream& arquivo, string dados, int tamanho);
        bool escrever_dados(ofstream& arquivo, void *dados, int tamanho);
};

#endif // MANIPULADOR_H_
