#ifndef ESTATISTICA_H_
#define ESTATISTICA_H_

#include "TituloNetflix.h"
#include "Manipulador.h"
#include "GeradorArquivos.h"

#define INF 1 << 30 // INFINITO

class Estatistica
{
    public:
        TituloNetflix menor, maior;

        int tamanho_menor, tamanho_maior;

        int quantidade_registros;
        double tamanho_medio_registro;

        Estatistica();

        // So precisa gerar as estatisticas de novo caso haja mudança nos registros.
        bool precisa_atualizar;

        void gerar_estatisticas(ifstream& arquivo_dados, ifstream& arquivo_indice);
        void gerar_estatisticas(ifstream& arquivo_dados, ifstream& arquivo_indice, vector<bool>& campos);
};

#endif