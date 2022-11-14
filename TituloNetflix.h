#ifndef TITULO_NETFLIX_H_
#define TITULO_NETFLIX_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

// Define a quantidade de campos do registro TituloNetflix
#define QTD_CAMPOS 12

// Define qual a posição de cada campo de TituloNetflix
#define ID_ID 0
#define TIPO_ID 1
#define TITULO_ID 2
#define DIRETOR_ID 3
#define ELENCO_ID 4
#define PAIS_ID 5
#define DATA_ID 6
#define ANO_LANCAMENTO_ID 7
#define AVALIACAO_ID 8
#define DURACAO_ID 9
#define LISTADO_EM_ID 10
#define DESCRICAO_ID 11

// Define o tamanho em bytes / caracteres de cada campo
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

        string to_string() const;
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

        TituloNetflix();
        TituloNetflix(string registro);
        TituloNetflix(string registro, vector<bool>& campos);

        void print(vector<bool>& campos) const;
        void string_para_titulo_netflix(string registro, vector<bool>& campos);
        string to_string(vector<bool>& campos);
        static int id_para_inteiro(string _id);
};

#endif // TITULO_NETFLIX_H_
