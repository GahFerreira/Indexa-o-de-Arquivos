#include "Estatistica.h"

Estatistica::Estatistica()
{
    precisa_atualizar = false;
}

void Estatistica::gerar_estatisticas(ifstream& arquivo_dados, ifstream& arquivo_indice)
{
    vector<bool> campos(QTD_CAMPOS, true);

    gerar_estatisticas(arquivo_dados, arquivo_indice, campos);
}

void Estatistica::gerar_estatisticas(ifstream& arquivo_dados, ifstream& arquivo_indice,  vector<bool>& campos)
{
    tamanho_maior = -1;
    tamanho_menor = INF;
    tamanho_medio_registro = 0;

    arquivo_indice.seekg(0, ios_base::beg);
    quantidade_registros = Manipulador::ler_inteiro(arquivo_indice);

    for (int i = 0; i < quantidade_registros; i++)
    {
        RegistroIndice registro_indice;

        // Lê registro por registro do arquivo de índices e do arquivo de dados
        Manipulador::ler_dados((ifstream&) arquivo_indice, sizeof(RegistroIndice), (void *) &registro_indice);

        arquivo_dados.seekg(registro_indice.bytes_do_inicio, ios_base::beg);

        string registro = Manipulador::ler_registro((ifstream&) arquivo_dados);

        if ((int) registro.size() < tamanho_menor)
        {
            menor = TituloNetflix(registro, campos);
            tamanho_menor = (int) registro.size();
        }

        if ((int) registro.size() > tamanho_maior)
        {
            maior = TituloNetflix(registro, campos);
            tamanho_maior = (int) registro.size();
        }

        tamanho_medio_registro += (double) registro.size();
    }

    tamanho_medio_registro /= (double) quantidade_registros;
}