#include <iostream>

#include "Manipulador.h"
#include "TituloNetflix.h"
#include "GeradorArquivos.h"

#define ARQUIVO_ENTRADA "netflix_titles.csv"
#define ARQUIVO_INICIAL "arquivo_inicial.txt"
#define ARQUIVO_INDICE "arquivo_indice.txt"
#define ARQUIVO_TITULO "arquivo_titulo.txt"

#define INF 1 << 30 // INFINITO

#define ESCOLHA_ID 1
#define ESCOLHA_TITULO 2
#define ESTATISTICA 3
#define FINALIZAR 4

using namespace std;

struct Estatistica
{
    TituloNetflix menor, maior;
    int tamanho_menor, tamanho_maior;
    int posicao_menor, posicao_maior;

    int quantidade_registros;
    double tamanho_medio_registro;
};

int main()
{
    // INÍCIO PROGRAMA

    cout << "Inicio do Programa\n\n";

    GeradorArquivos geradorArquivos;
    Manipulador manipulador;

    // INÍCIO CRIAÇÃO DOS ARQUIVOS

    cout
        << "Arquivo de entrada: " << ARQUIVO_ENTRADA << endl
        << "Lendo arquivo de entrada e gerando arquivo inicial" << endl;

    // ARQUIVO INICIAL

    if (geradorArquivos.criar_arquivo_inicial(ARQUIVO_ENTRADA, ARQUIVO_INICIAL))
    {
        cout << "Leitura do arquivo de entrada e criacao do novo arquivo bem sucedidos\n";
    }

    else
    {
        cout << "Erro na leitura do arquivo de entrada ou criacao do novo arquivo\n";

        return -1;
    }

    // ARQUIVO ÍNDICE PRIMÁRIO (ID -> POSIÇÃO)

    if (geradorArquivos.criar_arquivo_indice_primario(ARQUIVO_INICIAL, ARQUIVO_INDICE))
    {
        cout << "Criacao do arquivo indice bem sucedido\n";
    }

    else
    {
        cout << "Erro na criacao do arquivo de indice\n";

        return -1;
    }

    // ARQUIVO ÍNDICE SECUNDÁRIO (TÍTULOS -> ID)

    if (geradorArquivos.criar_arquivo_titulo(ARQUIVO_INICIAL, ARQUIVO_TITULO))
    {
        cout << "Criacao do arquivo titulo bem sucedido\n";
    }

    else
    {
        cout << "Erro na criacao do arquivo titulo\n";

        return -1;
    }

    cout << endl;

    // INÍCIO GERAÇÃO ESTATISTICA

    ifstream arquivo_inicial;

    Estatistica estatistica;
    estatistica.tamanho_maior = -1;
    estatistica.tamanho_menor = INF;
    estatistica.tamanho_medio_registro = 0;

    int pos_atual = 0;

    arquivo_inicial.open(ARQUIVO_INICIAL, ios_base::in | ios_base::binary);

    estatistica.quantidade_registros = manipulador.ler_inteiro(arquivo_inicial);
    pos_atual += sizeof(int);

    for (int i = 0; i < estatistica.quantidade_registros; i++)
    {
        string registro = manipulador.ler_registro(arquivo_inicial);

        if ((int) registro.size() < estatistica.tamanho_menor)
        {
            estatistica.menor = TituloNetflix(registro);
            estatistica.tamanho_menor = (int) registro.size();
            estatistica.posicao_menor = pos_atual;
        }

        if ((int) registro.size() > estatistica.tamanho_maior)
        {
            estatistica.maior = TituloNetflix(registro);
            estatistica.tamanho_maior = (int) registro.size();
            estatistica.posicao_maior = pos_atual;
        }
        
        estatistica.tamanho_medio_registro += (double) registro.size();
        pos_atual += (int) sizeof(int) + (int) registro.size();
    }

    estatistica.tamanho_medio_registro /= (double) estatistica.quantidade_registros;

    cout << "Estatisticas geradas." << endl;

    arquivo_inicial.close();

    // INÍCIO DA BUSCA

    // `arquivo_inicial` já foi declarado anteriormente
    ifstream arquivo_indice, arquivo_titulo;

    arquivo_inicial.open(ARQUIVO_INICIAL, ios_base::in | ios_base::binary);
	arquivo_indice.open(ARQUIVO_INDICE, ios_base::in | ios_base::binary);
	arquivo_titulo.open(ARQUIVO_TITULO, ios_base::in | ios_base::binary);

    string escolha, entrada;
    int numero, quantidade_registros;
    
    quantidade_registros = manipulador.ler_inteiro(arquivo_inicial);

    while (true)
    {
        cout 
        << "Digite " << ESCOLHA_ID << " para buscar um registro pelo SHOW_ID." << endl
        << "Digite " << ESCOLHA_TITULO << " para buscar um registro pelo TITULO." << endl
        << "Digite " << ESTATISTICA << " para ver as estatisticas do arquivo de dados." << endl
        << "Digite " << FINALIZAR << " para encerrar o programa." << endl
        << endl
        << "Digite o numero desejado: ";

        cin >> escolha;

        try
        {
            // Tenta converter a `escolha` em um número
            numero = stoi(escolha);

            if (numero != ESCOLHA_ID and 
                numero != ESCOLHA_TITULO and
                numero != ESTATISTICA and
                numero != FINALIZAR) throw exception();
        }

        catch(const std::exception& e)
        {
            cout << "Falha: Entrada invalida. Tente novamente." << endl << endl;

            continue;
        }
        
        if (numero == ESCOLHA_ID)
        {
            cout << endl << "Voce escolheu ID." << endl << "Digite o ID: ";

            cin >> entrada;

            // Transforma a string em minúscula
            for (auto& caractere : entrada)
            {
                caractere = tolower(caractere);
            }

            // Salta o cabeçalho
            arquivo_indice.seekg(sizeof(int), ios_base::beg);

            RegistroIndice registrosIndice[quantidade_registros];          

            // Leitura de todo o conteúdo do arquivo de índices primário de uma só vez
            manipulador.ler_dados(arquivo_indice, quantidade_registros * sizeof(RegistroIndice), &registrosIndice[0]);

            vector<TituloNetflix> respostas;

            for (int i = 0; i < quantidade_registros; i++)
            {
                // Encontrou par
                if (string(registrosIndice[i].id).find(entrada) != string::npos)
                {
                    arquivo_inicial.seekg(registrosIndice[i].bytes_do_inicio, ios_base::beg);

                    string registro = manipulador.ler_registro(arquivo_inicial);

                    respostas.push_back(TituloNetflix(registro));
                }
            }

            cout << endl;

            if (respostas.size() <= 0) cout << "Nao foi encontrado registro com esse parametro." << endl << endl;

            else
            {
                cout << "RESULTADO: " << respostas.size() << " resultado(s) encontrado(s)." << endl;

                for (int i = 0; i < respostas.size(); i++)
                {
                    respostas[i].print();
                    cout << endl;
                }
            }
        }

        else if (numero == ESCOLHA_TITULO)
        {
            cout << endl << "Voce escolheu TITULO." << endl << "Digite o TITULO: ";

            cin >> entrada;

            // Transforma a string em minúscula
            for (auto& caractere : entrada)
            {
                caractere = tolower(caractere);
            }

            // Salta o cabeçalho
            arquivo_indice.seekg(sizeof(int), ios_base::beg);
            arquivo_titulo.seekg(sizeof(int), ios_base::beg);

            RegistroIndice registrosIndice[quantidade_registros];          

            // Leitura de todo o conteúdo do arquivo de índices primário de uma só vez
            manipulador.ler_dados(arquivo_indice, quantidade_registros * sizeof(RegistroIndice), &registrosIndice[0]);

            RegistroTitulo registrosTitulo[quantidade_registros];

            // Leitura de todo o conteúdo do arquivo de índices secundário de uma só vez
            manipulador.ler_dados(arquivo_titulo, quantidade_registros * (int) sizeof(RegistroTitulo), &registrosTitulo[0]);

            vector<TituloNetflix> respostas;

            for (int i = 0; i < quantidade_registros; i++)
            {
                // Encontrou um titulo no arquivo de titulos que pareou com a entrada
                if (string(registrosTitulo[i].titulo).find(entrada) != string::npos)
                {
                    // Procura dentro dos registros do arquivo de índices primários
                    // pelo id do par encontrado
                    for (int j = 0; j < quantidade_registros; j++)
                    {
                        // Uma vez encontrado o id no arquivo de índices primário
                        // adiciona o TituloNetflix a respostas
                        if (string(registrosTitulo[i].id) == string(registrosIndice[j].id))
                        {
                            // Posiciona a leitura no arquivo inicial na posição
                            // do registro pareado
                            arquivo_inicial.seekg(registrosIndice[j].bytes_do_inicio, ios_base::beg);

                            string registro = manipulador.ler_registro(arquivo_inicial);

                            respostas.push_back(TituloNetflix(registro));

                            break;
                        }
                    }
                }
            }

            cout << endl;

            if (respostas.size() <= 0) cout << "Nao foi encontrado registro com esse parametro" << endl << endl;

            else
            {
                cout << "RESULTADO: " << respostas.size() << " resultado(s) encontrado(s)." << endl;

                for (int i = 0; i < respostas.size(); i++)
                {
                    respostas[i].print();
                    cout << endl;
                }
            }
        }

        else if (numero == ESTATISTICA)
        {
            cout
                << "ESTATISTICAS:" << endl
                << '\t' << "Quantidade de Registros: " << estatistica.quantidade_registros << endl
                << '\t' << "Tamanho Medio dos Registros: " << estatistica.tamanho_medio_registro << endl;
                
            // Maior Registro
            arquivo_inicial.seekg(estatistica.posicao_maior, ios_base::beg);
            cout 
                << '\t' << "Maior Registro:" << endl
                << '\t' << "Tamanho: " << estatistica.tamanho_maior << endl
                << '\t';
                
            TituloNetflix(manipulador.ler_registro(arquivo_inicial)).print();
                
            cout << endl;

            // Menor Registro
            arquivo_inicial.seekg(estatistica.posicao_menor, ios_base::beg);
            cout 
                << '\t' << "Menor Registro:" << endl
                << '\t' << "Tamanho: " << estatistica.tamanho_menor << endl
                << '\t';
                
            TituloNetflix(manipulador.ler_registro(arquivo_inicial)).print();
                
            cout << endl;
        }

        else
        {
            cout << endl << "O programa sera finalizado." << endl;

            break;
        }
    }

    arquivo_inicial.close();
    arquivo_indice.close();
    arquivo_titulo.close();

    return 0;
}
