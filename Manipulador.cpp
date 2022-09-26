#include "Manipulador.h"
#include <iostream>

Manipulador::Manipulador()
{
}

bool Manipulador::criar_arquivo_inicial(const char *nome_arq_entrada)
{
    fstream arq_entrada, arq_dados;

    arq_entrada.open(nome_arq_entrada, ios_base::in);
    arq_dados.open("dados.txt", ios_base::out);

    cout << "Abertura dos arquivos bem sucedida\n";

    TituloNetflix tn;
    string registro_como_string;
    int tam_registro;

    cout << "Inicio do processamento\n";

    while (true)
    {
        getline(arq_entrada, registro_como_string);

        cout << registro_como_string << '\n';

        if ((int) registro_como_string.size() <= 0) break;

        tam_registro = (int) registro_como_string.size();

        arq_dados.write((char *)(&tam_registro), sizeof(tam_registro));
        arq_dados.write((char *) registro_como_string.c_str(), sizeof((int) registro_como_string.size()));
    }

    cout << "Processamento bem sucedido\n";

    arq_entrada.close();
    arq_dados.close();

    cout << "Fechamento dos arquivos bem sucedido\n";

    return true;
}
