#include <iostream>

#include "Manipulador.h"
#include "TituloNetflix.h"
#include "GeradorArquivos.h"

using namespace std;

int main()
{
    cout << "Inicio do Programa\n\n";

    GeradorArquivos geradorArquivos;
    Manipulador manipulador;

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

    fstream A("arquivo_inicial.txt", ios_base::in);

    TituloNetflix superT(string("s1;TV Show;3%;;João Miguel, Bianca Comparato, Michel Gomes, Rodolfo Valente, Vaneza Oliveira, Rafael Lozano, Viviane Porto, Mel Fronckowiak, Sergio Mamberti, Zezé Motta, Celso Frateschi;Brazil;14/08/2020;2020;TV-MA;4 Seasons;International TV Shows, TV Dramas, TV Sci-Fi & Fantasy;In a future where the elite inhabit an island paradise far from the crowded slums, you get one chance to join the 3% saved from squalor."));

    return 0;
}
