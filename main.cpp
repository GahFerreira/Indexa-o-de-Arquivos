#include <iostream>

#include "Manipulador.h"
#include "TituloNetflix.h"
#include "GeradorArquivos.h"
#include "GerenciadorRegistros.h"

#define ARQUIVO_ENTRADA "netflix_titles.csv"
#define ARQUIVO_DADOS "arquivo_dados.txt"
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

    vector<bool> campos(QTD_CAMPOS, false);
    campos[ID_ID] = true;
	campos[TIPO_ID] = true;
	campos[TITULO_ID] = true;
	campos[PAIS_ID] = true;
	campos[ANO_LANCAMENTO_ID] = true;

    GeradorArquivos geradorArquivos(campos);

    // INÍCIO CRIAÇÃO DOS ARQUIVOS

    cout
        << "Arquivo de entrada: " << ARQUIVO_ENTRADA << endl
        << "Lendo arquivo de entrada e gerando arquivo de dados" << endl;

    // ARQUIVO DADOS

    if (geradorArquivos.criar_arquivo_dados(ARQUIVO_ENTRADA, ARQUIVO_DADOS))
    {
        cout << "Leitura do arquivo de entrada e criacao do arquivo de dados bem sucedidos\n";
    }

    else
    {
        cout << "Erro na leitura do arquivo de entrada ou criacao do novo arquivo\n";

        return -1;
    }

    // ARQUIVO ÍNDICE DIRETO (ID -> POSIÇÃO)
    if (geradorArquivos.criar_arquivo_indice_primario(ARQUIVO_DADOS, ARQUIVO_INDICE))
    {
        cout << "Criacao do arquivo indice bem sucedido\n";
    }

    else
    {
        cout << "Erro na criacao do arquivo de indice\n";

        return -1;
    }

    // ARQUIVO ÍNDICE indireto (TÍTULOS -> ID)

    if (geradorArquivos.criar_arquivo_titulo(ARQUIVO_DADOS, ARQUIVO_TITULO))
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

    ifstream arquivo_dados;

    Estatistica estatistica;
    estatistica.tamanho_maior = -1;
    estatistica.tamanho_menor = INF;
    estatistica.tamanho_medio_registro = 0;

    int pos_atual = 0;

    arquivo_dados.open(ARQUIVO_DADOS, ios_base::in | ios_base::binary);

    estatistica.quantidade_registros = Manipulador::ler_inteiro(arquivo_dados);
    pos_atual += sizeof(int);

    for (int i = 0; i < estatistica.quantidade_registros; i++)
    {
        string registro = Manipulador::ler_registro(arquivo_dados);

        if ((int) registro.size() < estatistica.tamanho_menor)
        {
            estatistica.menor = TituloNetflix(registro, campos);
            estatistica.tamanho_menor = (int) registro.size();
            estatistica.posicao_menor = pos_atual;
        }

        if ((int) registro.size() > estatistica.tamanho_maior)
        {
            estatistica.maior = TituloNetflix(registro, campos);
            estatistica.tamanho_maior = (int) registro.size();
            estatistica.posicao_maior = pos_atual;
        }

        estatistica.tamanho_medio_registro += (double) registro.size();
        pos_atual += (int) sizeof(int) + (int) registro.size();
    }

    estatistica.tamanho_medio_registro /= (double) estatistica.quantidade_registros;

    cout << "Estatisticas geradas." << endl;

    arquivo_dados.close();

    // INÍCIO DA BUSCA

    // `arquivo_dados` já foi declarado anteriormente
    ifstream arquivo_indice, arquivo_titulo;

    arquivo_dados.open(ARQUIVO_DADOS, ios_base::in | ios_base::binary);
	arquivo_indice.open(ARQUIVO_INDICE, ios_base::in | ios_base::binary);
	arquivo_titulo.open(ARQUIVO_TITULO, ios_base::in | ios_base::binary);

    GerenciadorRegistros gerenciador_registros(ARQUIVO_INDICE, ARQUIVO_TITULO, Manipulador::ler_inteiro(arquivo_dados), campos);
    string escolha, entrada;
    int numero_escolha;

    while (true)
    {
        escolha.clear();
        entrada.clear();

        cout
        << "Digite " << ESCOLHA_ID << " para buscar um registro pelo SHOW_ID." << endl
        << "Digite " << ESCOLHA_TITULO << " para buscar um registro pelo TITULO." << endl
        << "Digite " << ESTATISTICA << " para ver as estatisticas do arquivo de dados." << endl
        << "Digite " << FINALIZAR << " para encerrar o programa." << endl
        << endl
        << "Digite o numero_escolha desejado: ";

        while (escolha.size() <= 0)
        {
            getline(cin, escolha);
        }

        try
        {
            // Tenta converter a `escolha` em um número
            numero_escolha = stoi(escolha);

            if (numero_escolha != ESCOLHA_ID and
                numero_escolha != ESCOLHA_TITULO and
                numero_escolha != ESTATISTICA and
                numero_escolha != FINALIZAR) throw exception();
        }

        catch(const std::exception& e)
        {
            cout << "Falha: Entrada invalida. Tente novamente." << endl << endl;

            continue;
        }


        if (numero_escolha == ESCOLHA_ID)
        {
            cout << endl << "Voce escolheu ID." << endl << "Digite o ID (ex: s" << rand()%7788 << "): ";

            while (entrada.size() <= 0)
            {
                getline(cin, entrada);
            }

            cout << endl;

            int resposta_em_bytes_do_inicio = gerenciador_registros.busca_id(arquivo_dados, arquivo_indice, entrada);

            if (resposta_em_bytes_do_inicio == -1) cout << "Nao foi encontrado registro com esse parametro." << endl << endl;

            else if (resposta_em_bytes_do_inicio == -2) cout << "Entrada invalida." << endl << endl;

            else
            {
                cout << "RESULTADO: " << endl;
                gerenciador_registros.localizar_titulo_netflix_arquivo_dados(arquivo_dados, resposta_em_bytes_do_inicio).print(campos);
            }
        }

        else if (numero_escolha == ESCOLHA_TITULO)
        {
            cout << endl << "Voce escolheu TITULO." << endl << "Digite o TITULO: ";

            while (entrada.size() <= 0)
            {
                getline(cin, entrada);
            }

            // Transforma a string em minúscula
            for (auto& caractere : entrada)
            {
                caractere = tolower(caractere);
            }

            // Salta o cabeçalho
            arquivo_indice.seekg(sizeof(int), ios_base::beg);
            arquivo_titulo.seekg(sizeof(int), ios_base::beg);

            RegistroIndice registrosIndice[gerenciador_registros.quantidade_registros];

            // Leitura de todo o conteúdo do arquivo de índices direto de uma só vez
            Manipulador::ler_dados(arquivo_indice, gerenciador_registros.quantidade_registros * sizeof(RegistroIndice), &registrosIndice[0]);

            RegistroTitulo registrosTitulo[gerenciador_registros.quantidade_registros];

            // Leitura de todo o conteúdo do arquivo de índices indireto de uma só vez
            Manipulador::ler_dados(arquivo_titulo, gerenciador_registros.quantidade_registros * (int) sizeof(RegistroTitulo), &registrosTitulo[0]);

            vector<TituloNetflix> respostas;

            for (int i = 0; i < gerenciador_registros.quantidade_registros; i++)
            {
                // Encontrou um titulo no arquivo de titulos que pareou com a entrada
                if (string(registrosTitulo[i].titulo).find(entrada) != string::npos)
                {
                    // Procura dentro dos registros do arquivo de índices primários
                    // pelo id do par encontrado
                    for (int j = 0; j < gerenciador_registros.quantidade_registros; j++)
                    {
                        // Uma vez encontrado o id no arquivo de índices direto
                        // adiciona o TituloNetflix a respostas
                        if (string(registrosTitulo[i].id) == string(registrosIndice[j].id))
                        {
                            // Posiciona a leitura no arquivo de dados na posição
                            // do registro pareado
                            arquivo_dados.seekg(registrosIndice[j].bytes_do_inicio, ios_base::beg);

                            string registro = Manipulador::ler_registro(arquivo_dados);

                            respostas.push_back(TituloNetflix(registro, campos));

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

                for (int i = 0; i < (int) respostas.size(); i++)
                {
                    respostas[i].print(campos);
                    cout << endl;
                }
            }
        }

        else if (numero_escolha == ESTATISTICA)
        {
            cout
                << "ESTATISTICAS:" << endl
                << '\t' << "Quantidade de Registros: " << estatistica.quantidade_registros << endl
                << '\t' << "Tamanho Medio dos Registros: " << estatistica.tamanho_medio_registro << endl;

            // Maior Registro
            arquivo_dados.seekg(estatistica.posicao_maior, ios_base::beg);
            cout
                << '\t' << "Maior Registro:" << endl
                << '\t' << "Tamanho: " << estatistica.tamanho_maior << endl
                << '\t';

            TituloNetflix(Manipulador::ler_registro(arquivo_dados), campos).print(campos);

            cout << endl;

            // Menor Registro
            arquivo_dados.seekg(estatistica.posicao_menor, ios_base::beg);
            cout
                << '\t' << "Menor Registro:" << endl
                << '\t' << "Tamanho: " << estatistica.tamanho_menor << endl
                << '\t';

            TituloNetflix(Manipulador::ler_registro(arquivo_dados), campos).print(campos);

            cout << endl;
        }

        else
        {
            cout << endl << "O programa sera finalizado." << endl;

            break;
        }
    }

    arquivo_dados.close();
    arquivo_indice.close();
    arquivo_titulo.close();

    return 0;
}
