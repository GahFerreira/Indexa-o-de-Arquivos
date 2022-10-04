#ifndef GERADOR_ARQUIVOS_H_
#define GERADOR_ARQUIVOS_H_

#include <iostream>
#include <fstream>
#include <string>
#include <cctype> // `tolower` para deixar strings minusculas
#include "Manipulador.h"
#include "TituloNetflix.h"

using namespace std;

// A `quebra_linha` é para deixar visualmente mais apresentável a estrutura dos arquivos.
struct RegistroIndice
{
    char id[TAM_ID+1];
    int bytes_do_inicio;
    char quebra_linha = '\n';
};

struct RegistroTitulo
{
    char titulo[TAM_TITULO+1];
    char id[TAM_ID+1];
    char quebra_linha = '\n';
};

class GeradorArquivos
{
    public:
        bool criar_arquivo_inicial(const char *nome_arq_entrada, const char *nome_arq_inicial);
        bool criar_arquivo_indice_primario(const char *nome_arq_inicial, const char *nome_arq_indice_primario);
        bool criar_arquivo_titulo(const char *nome_arq_inicial,const char *nome_arq_titulo);

        TituloNetflix *busca_titulo(const char *nome_arq_inicial, const char *nome_arq_indice, const char *nome_arq_titulo, string entrada);
};

#endif // GERADOR_ARQUIVOS_H_
