#ifndef MANIPULADOR_H_
#define MANIPULADOR_H_

#include <fstream>
#include "TituloNetflix.h"

using namespace std;

class Manipulador
{
    public:
        char delimitador = '\0';
        string dados;
        int prox = 0;

        Manipulador();

        bool criar_arquivo_inicial(const char *nome_arq_entrada);
};

#endif // MANIPULADOR_H
