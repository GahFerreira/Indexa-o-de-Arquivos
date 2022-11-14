#ifndef GERADOR_ARQUIVOS_H_
#define GERADOR_ARQUIVOS_H_

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
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

/**
 * Classe para gerar os arquivos de dados e de indexação. 
*/
class GeradorArquivos
{
    public:
        vector<bool> campos_a_serem_usados;

        GeradorArquivos();
        GeradorArquivos(vector<bool>& campos);

        bool criar_arquivo_dados(const char *nome_arq_entrada, const char *nome_arq_dados);
        bool criar_arquivo_indice_primario(const char *nome_arq_dados, const char *nome_arq_indice_primario);
        bool criar_arquivo_titulo(const char *nome_arq_dados, const char *nome_arq_titulo);
};

#endif // GERADOR_ARQUIVOS_H_
