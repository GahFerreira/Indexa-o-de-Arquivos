#ifndef TITULO_NETFLIX_H_
#define TITULO_NETFLIX_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

#define TAM_ID 6
#define TAM_TIPO 10
#define TAM_TITULO 105
#define TAM_DATA 10
#define TAM_AVALIACAO 10
#define TAM_DURACAO 12
#define TAM_DESCRICAO 1000

class Lista
{
    public:
        vector<string> lista;

        char *to_string() const;
};

class TituloNetflix
{
    public:
        char id[TAM_ID+1];
        char tipo[TAM_TIPO+1];
        char titulo[TAM_TITULO+1];
        Lista diretor;
        Lista elenco;
        Lista pais;
        char data[TAM_DATA+1];
        short ano_lancamento;
        char avaliacao[TAM_AVALIACAO+1];
        char duracao[TAM_DURACAO+1];
        Lista listado_em;
        char descricao[TAM_DESCRICAO+1];
        //pode precisar
        //int tamanho_registro;

        TituloNetflix();
        TituloNetflix(string registro);

        void print() const;
        void obter_registro();

        bool criar_arquivo_indice_primario(const char *nome_arq_inicial, const char *nome_arq_indice_primario);
};

#endif // TITULO_NETFLIX_H_
