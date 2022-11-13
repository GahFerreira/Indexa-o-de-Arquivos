#ifndef GERENCIADOR_REGISTROS_H_
#define GERENCIADOR_REGISTROS_H_

#include <cstdio> // `rename` e `delete` para arquivos
#include <string>
#include <vector>
#include "TituloNetflix.h"
#include "GeradorArquivos.h"

/**
 * Classe para operações de busca, inserção e deleção de registros.
*/
class GerenciadorRegistros
{
    public:
        const string nome_arquivo_indice, nome_arquivo_titulo;

        GerenciadorRegistros(string nome_arquivo_indice, string nome_arquivo_titulo);

        TituloNetflix busca_id(ifstream& arquivo_dados, ifstream& arquivo_indice, string id_registro);
        vector<TituloNetflix> busca_titulo(ifstream& arquivo_dados, ifstream& arquivo_indice, ifstream& arquivo_titulo, string titulo_registro);
        void inserir_registro(ofstream& arquivo_dados, ifstream& arquivo_indice, TituloNetflix tN);
        bool deletar_registro(ofstream& arquivo_dados, ofstream& arquivo_indice, ofstream& arquivo_titulo, string id_registro);
        vector<string> deletar_multiplos_registros(ofstream& arquivo_dados, ofstream& arquivo_indice, ofstream& arquivo_titulo, vector<string> id_regitros);
};

#endif