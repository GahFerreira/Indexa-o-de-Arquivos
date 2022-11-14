#ifndef GERENCIADOR_REGISTROS_H_
#define GERENCIADOR_REGISTROS_H_

#include <cstdio> // `rename` e `delete` para arquivos
#include <algorithm> // `sort`
#include <tuple> // `tie`
#include <string>
#include <vector>
#include "TituloNetflix.h"
#include "GeradorArquivos.h"

#define NAO_ENCONTROU -1
#define ID_INVALIDO -2

/**
 * Classe para operações de busca, inserção e deleção de registros.
*/
class GerenciadorRegistros
{
    public:
        string nome_arquivo_dados, nome_arquivo_indice, nome_arquivo_titulo;
        fstream arquivo_dados, arquivo_indice, arquivo_titulo;
        int quantidade_registros;
        vector<bool> campos;

        GerenciadorRegistros(string nome_arquivo_dados, string nome_arquivo_indice, string nome_arquivo_titulo, int quantidade_registros, vector<bool> campos);

        // Abrir & fechar fstreams
        void abrir_fstreams();
        void fechar_fstreams();

        // Métodos de Busca
        int busca_id(string id_registro);
        vector<string> busca_titulo(string titulo_registro);

        // Métodos auxiliares para busca
        TituloNetflix localizar_registro_arquivo_dados(int bytes_do_inicio);
        bool id_eh_valido(string id);
        int busca_binaria_id_para_bytes_do_inicio(RegistroIndice registros_indice[], string id_buscado);
        vector<int> lista_de_ids_para_lista_de_posicoes(vector<string> ids);

        // Inserção e deleção
        void inserir_registro_final(TituloNetflix tN);
        bool deletar_registro(string id_registro);
        vector<string> deletar_multiplos_registros(vector<string> id_regitros);

        // Métodos auxiliares para inserção e deleção
        void ordenar_arquivo_indice();
        void atualizar_quantidade_de_registros(int variacao);
        pair<int, int> busca_binaria_para_posicoes_nos_arquivos(RegistroIndice registros_indice[], string id_buscado);
};

#endif