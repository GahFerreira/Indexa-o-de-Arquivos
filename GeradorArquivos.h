#ifndef GERADOR_ARQUIVOS_H_
#define GERADOR_ARQUIVOS_H_

#include <iostream>
#include <fstream>
#include <string>
#include "Manipulador.h"
#include "TituloNetflix.h"

using namespace std;

class GeradorArquivos
{
    public:
        bool criar_arquivo_inicial(const char *nome_arq_entrada, const char *nome_arq_inicial);
        bool criar_arquivo_indice_primario(const char *nome_arq_inicial, const char *nome_arq_indice_primario);
};

#endif // GERADOR_ARQUIVOS_H_