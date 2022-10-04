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

    ifstream A("arquivo_inicial.txt", ios_base::in | ios_base::binary);

    int tam = manipulador.ler_inteiro(A);
    int inteiro = 0;

    cout << "tam: " << tam << endl;

    for (int i = 0; i < tam; i++)
    {
        if (A.eof() == true)
        {
            cout << "EOF ";

            if (i == tam)
            {
                cout << "Fim do arquivo quando i == tam\n";
            }

            else cout << "Fim do arquivo antes i == tam\n";

            break;
        }

        else
        {
            cout << ++inteiro << " vezes\n";
            manipulador.ler_registro(A);
        }
    }

    return 0;
}
