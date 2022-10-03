#include <iostream>

#include "Manipulador.h"
#include "TituloNetflix.h"
#include "GeradorArquivos.h"

using namespace std;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main()
{
    cout << "Inicio do Programa\n\n";

    GeradorArquivos geradorArquivos;

    cout
        << "Arquivo de entrada: netflix_titles.csv\n"
        << "Lendo arquivo de entrada e gerando arquivo inicial\n";

    if (geradorArquivos.criar_arquivo_inicial("netflix_titles.csv", "arquivo_inicial.txt"))
    {
        cout << "Leitura do arquivo de entrada e criacao do novo arquivo bem sucedidos\n";
    }

    else
    {
        cout << "Erro na leitura do arquivo de entrada ou criacao do novo arquivo\n";

        return -1;
    }

    return 0;
}
