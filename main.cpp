#include <iostream>

#include "Manipulador.h"
#include "TituloNetflix.h"
#include "GeradorArquivos.h"
#include "GerenciadorRegistros.h"
#include "Estatistica.h"

#define ARQUIVO_ENTRADA "netflix_titles.csv"
#define ARQUIVO_DADOS "arquivo_dados.txt"
#define ARQUIVO_INDICE "arquivo_indice.txt"
#define ARQUIVO_TITULO "arquivo_titulo.txt"
#define ARQUIVO_REINSERCAO_DADOS "arquivo_reinsercao_dados.txt"

#define BUSCAR_POR_ID 1
#define BUSCAR_POR_TITULO 2
#define INSERIR_REGISTRO 3
#define DELETAR_REGISTRO 4
#define ESTATISTICA 5
#define FINALIZAR 6

using namespace std;

int main()
{
    // INÍCIO PROGRAMA

    cout << "Inicio do Programa.\n\n";

    // Variáveis globais ao programa
    vector<bool> campos(QTD_CAMPOS, false);
    campos[ID_ID] = true;
	campos[TIPO_ID] = true;
	campos[TITULO_ID] = true;
	campos[PAIS_ID] = true;
	campos[ANO_LANCAMENTO_ID] = true;

    Estatistica estatistica;

    // CRIAÇÃO DOS ARQUIVOS
    {
        GeradorArquivos geradorArquivos(campos);

        cout
            << "Arquivo de entrada: " << ARQUIVO_ENTRADA << endl
            << "Lendo arquivo de entrada e gerando arquivo de dados." << endl;

        // ARQUIVO DADOS

        if (geradorArquivos.criar_arquivo_dados(ARQUIVO_ENTRADA, ARQUIVO_DADOS))
        {
            cout << "Leitura do arquivo de entrada e criacao do arquivo de dados bem sucedidos.\n";
        }

        else
        {
            cout << "Erro na leitura do arquivo de entrada ou criacao do novo arquivo.\n";

            return -1;
        }

        // ARQUIVO ÍNDICE DIRETO (ID -> POSIÇÃO)
        if (geradorArquivos.criar_arquivo_indice_primario(ARQUIVO_DADOS, ARQUIVO_INDICE))
        {
            cout << "Criacao do arquivo indice bem sucedido.\n";
        }

        else
        {
            cout << "Erro na criacao do arquivo de indice.\n";

            return -1;
        }

        // ARQUIVO ÍNDICE indireto (TÍTULOS -> ID)

        if (geradorArquivos.criar_arquivo_titulo(ARQUIVO_DADOS, ARQUIVO_TITULO))
        {
            cout << "Criacao do arquivo titulo bem sucedido.\n";
        }

        else
        {
            cout << "Erro na criacao do arquivo titulo.\n";

            return -1;
        }

        // ARQUIVO REINSERÇÃO DE REGISTROS NO ARQUIVO DE DADOS

        if (geradorArquivos.criar_arquivo_reinsercao_dados(ARQUIVO_REINSERCAO_DADOS))
        {
            cout << "Criacao do arquivo de reinsercao de dados bem sucedido.\n";
        }

        else
        {
            cout << "Erro na criacao do arquivo de reinsercao de dados.\n";

            return -1;
        }
    }

    // GERAÇÃO ESTATISTICA
    {
        ifstream arquivo_dados, arquivo_indice;
        arquivo_dados.open(ARQUIVO_DADOS, ios_base::in | ios_base::binary);
        arquivo_indice.open(ARQUIVO_INDICE, ios_base::in | ios_base::binary);

        estatistica.gerar_estatisticas(arquivo_dados, arquivo_indice, campos);

        cout << "Estatisticas iniciais geradas.\n\n";

        arquivo_dados.close();
    }

    // INTERAÇÃO COM O USUÁRIO
    {
        GerenciadorRegistros gerenciador_registros(ARQUIVO_DADOS, ARQUIVO_INDICE, ARQUIVO_TITULO, ARQUIVO_REINSERCAO_DADOS, campos);

        gerenciador_registros.abrir_fstreams();

        string escolha;
        int numero_escolha;

        while (true)
        {
            escolha.clear();

            cout
            << "Digite " << BUSCAR_POR_ID << " para buscar um registro pelo SHOW_ID." << endl
            << "Digite " << BUSCAR_POR_TITULO << " para buscar um registro pelo TITULO." << endl
            << "Digite " << INSERIR_REGISTRO << " para inserir um novo registro." << endl
            << "Digite " << DELETAR_REGISTRO << " para deletar um registro pelo SHOW_ID." << endl
            << "Digite " << ESTATISTICA << " para ver as estatisticas do arquivo de dados." << endl
            << "Digite " << FINALIZAR << " para encerrar o programa." << endl
            << endl
            << "Digite o numero_escolha desejado: ";

            while (escolha.empty())
            {
                getline(cin, escolha);
            }

            try
            {
                for (int i = 0; i < (int) escolha.size(); i++)
                {
                    if (escolha[i] < '0' or escolha[i] > '9') throw exception();
                }

                // Tenta converter a `escolha` em um número
                numero_escolha = stoi(escolha);

                if (numero_escolha != BUSCAR_POR_ID and
                    numero_escolha != BUSCAR_POR_TITULO and
                    numero_escolha != INSERIR_REGISTRO and
                    numero_escolha != DELETAR_REGISTRO and
                    numero_escolha != ESTATISTICA and
                    numero_escolha != FINALIZAR) throw exception();
            }

            catch(const std::exception& e)
            {
                cout << "Falha: Entrada invalida. Tente novamente." << endl << endl;

                continue;
            }


            if (numero_escolha == BUSCAR_POR_ID)
            {
                cout << endl << "Voce escolheu ID." << endl << "Digite o ID (ex: s" << rand()%7788 << "): ";

                string entrada_id;

                while (entrada_id.empty())
                {
                    getline(cin, entrada_id);
                }

                cout << endl;

                int resposta_em_bytes_do_inicio = gerenciador_registros.busca_id(entrada_id);

                if (resposta_em_bytes_do_inicio == -1) cout << "Nao foi encontrado registro com esse parametro." << endl << endl;

                else if (resposta_em_bytes_do_inicio == -2) cout << "Entrada invalida." << endl << endl;

                else
                {
                    cout << "RESULTADO: " << endl;
                    gerenciador_registros.localizar_registro_arquivo_dados(resposta_em_bytes_do_inicio).print(campos);
                }
            }

            else if (numero_escolha == BUSCAR_POR_TITULO)
            {
                cout << endl << "Voce escolheu TITULO." << endl << "Digite o TITULO: ";

                string entrada_titulo;

                while (entrada_titulo.empty())
                {
                    getline(cin, entrada_titulo);
                }

                // Retorna os ids dos registros que tem `entrada` como substring de seu título.
                vector<string> ids = gerenciador_registros.busca_titulo(entrada_titulo);

                cout << endl;

                // Se não encontrou nenhum, não faz nada.
                if (ids.empty() == true) cout << "Nao foi encontrado registro com esse parametro" << endl << endl;

                else
                {
                    // Se encontrou, mostra os registros na tela.
                    vector<int> respostas = gerenciador_registros.lista_de_ids_para_lista_de_posicoes(ids);

                    cout << "RESULTADO: " << respostas.size() << " resultado(s) encontrado(s)." << endl;

                    for (int i = 0; i < (int) respostas.size(); i++)
                    {
                        gerenciador_registros.localizar_registro_arquivo_dados(respostas[i]).print(campos);

                        cout << endl;
                    }
                }
            }

            else if (numero_escolha == INSERIR_REGISTRO)
            {
                // Novo registro a ser inserido.
                TituloNetflix tN = TituloNetflix();

                cout << "Voce escolheu INSERIR REGISTRO." << endl;

                // Para cada campo de `campos`, aparece a opção para o usuário preencher aquele campo.
                for (int i_campo = 0; i_campo < QTD_CAMPOS; i_campo++)
                {
                    if (campos[i_campo] == true)
                    {
                        switch(i_campo)
                        {
                            // O ID é o único campo que verifica seu conteúdo.
                            case ID_ID:
                            {
                                string entrada_id;

                                while (entrada_id.empty() == true)
                                {
                                    cout << "\tDigite o ID [MAX 6 Caracteres] (Ex: s" << rand()%7788 << "): ";

                                    getline(cin, entrada_id);

                                    if (gerenciador_registros.id_eh_valido(entrada_id) == false or
                                        entrada_id.size() > 6)
                                    {
                                        cout << "\tID invalido. Tente novamente.\n";
                                        entrada_id.clear();
                                    }

                                    else if (gerenciador_registros.busca_id(entrada_id) != -1)
                                    {
                                        cout << "\tID ja existente. Tente novamente.\n";
                                        entrada_id.clear();
                                    }
                                }

                                strcpy(tN.id, entrada_id.c_str());

                                break;
                            }

                            case TIPO_ID:
                            {
                                string entrada_tipo;

                                while (entrada_tipo.empty() == true)
                                {
                                    cout << "\tTipo [MAX 10 Caracteres]: ";

                                    getline(cin, entrada_tipo);

                                    if (entrada_tipo.size() > 10)
                                    {
                                        cout << "\tLimite de caracteres excedido. Tente novamente.\n";
                                        entrada_tipo.clear();
                                    }
                                }

                                strcpy(tN.tipo, entrada_tipo.c_str());

                                break;
                            }

                            case TITULO_ID:
                            {
                                string entrada_titulo;

                                while (entrada_titulo.empty() == true)
                                {
                                    cout << "\tTitulo [MAX 105 Caracteres]: ";

                                    getline(cin, entrada_titulo);

                                    if (entrada_titulo.size() > 105)
                                    {
                                        cout << "\tLimite de caracteres excedido. Tente novamente.\n";
                                        entrada_titulo.clear();
                                    }
                                }

                                strcpy(tN.titulo, entrada_titulo.c_str());

                                break;
                            }

                            case DIRETOR_ID:
                            {
                                int qtd;
                                string entrada_qtd_diretores;

                                while (entrada_qtd_diretores.empty() == true)
                                {
                                    cout << "\tDiretores - Digite quantos a serem inseridos: ";

                                    getline(cin, entrada_qtd_diretores);

                                    try
                                    {
                                        for (int i = 0; i < (int) entrada_qtd_diretores.size(); i++)
                                        {
                                            if (entrada_qtd_diretores[i] < '0' or entrada_qtd_diretores[i] > '9') throw exception();
                                        }

                                        qtd = stoi(entrada_qtd_diretores);
                                    }
                                    catch(const std::exception& e)
                                    {
                                        cerr << "\tQuantidade invalida. Tente novamente.\n";
                                        entrada_qtd_diretores.clear();
                                    }
                                }

                                string entrada_diretor;

                                for (int i_diretor = 0; i_diretor < qtd; i_diretor++)
                                {
                                    cout << "\t" << (i_diretor+1) << "o(a) diretor(a): ";

                                    getline(cin, entrada_diretor);

                                    tN.diretor.lista.push_back(entrada_diretor);
                                }

                                break;
                            }

                            case ELENCO_ID:
                            {
                                int qtd;

                                string entrada_qtd_elenco;

                                while (entrada_qtd_elenco.empty())
                                {
                                    cout << "\tElenco - Digite quantos atores / atrizes a serem inseridos: ";

                                    getline(cin, entrada_qtd_elenco);

                                    try
                                    {
                                        for (int i = 0; i < (int) entrada_qtd_elenco.size(); i++)
                                        {
                                            if (entrada_qtd_elenco[i] < '0' or entrada_qtd_elenco[i] > '9') throw exception();
                                        }

                                        qtd = stoi(entrada_qtd_elenco);
                                    }
                                    catch(const std::exception& e)
                                    {
                                        cerr << "\tQuantidade invalida. Tente novamente.\n";
                                        entrada_qtd_elenco.clear();
                                    }
                                }

                                string entrada_elenco;

                                for (int i_elenco = 0; i_elenco < qtd; i_elenco++)
                                {
                                    cout << "\t" << (i_elenco+1) << "o(a) ator / atriz: ";

                                    getline(cin, entrada_elenco);

                                    tN.elenco.lista.push_back(entrada_elenco);
                                }

                                break;
                            }

                            case PAIS_ID:
                            {
                                int qtd;
                                string entrada_qtd_paises;

                                while (entrada_qtd_paises.empty())
                                {
                                    cout << "\tPaises - Digite quantos paises a serem inseridos: ";

                                    getline(cin, entrada_qtd_paises);

                                    try
                                    {
                                        for (int i = 0; i < (int) entrada_qtd_paises.size(); i++)
                                        {
                                            if (entrada_qtd_paises[i] < '0' or entrada_qtd_paises[i] > '9') throw exception();
                                        }

                                        qtd = stoi(entrada_qtd_paises);

                                        if (qtd > 193 or qtd < 0) throw exception();
                                    }
                                    catch(const std::exception& e)
                                    {
                                        cerr << "\tQuantidade invalida. Tente novamente.\n";
                                        entrada_qtd_paises.clear();
                                    }
                                }

                                string entrada_pais;

                                for (int i_paises = 0; i_paises < qtd; i_paises++)
                                {
                                    cout << "\t" << (i_paises+1) << "o pais: ";

                                    getline(cin, entrada_pais);

                                    tN.pais.lista.push_back(entrada_pais);
                                }

                                break;
                            }

                            case DATA_ID:
                            {
                                string entrada_data;

                                while (entrada_data.empty())
                                {
                                    cout << "\tData [10 Caracteres] (Ex: ";

                                    printf("%02d/%02d/%04d", 1 + (rand() % 28), 1 + (rand() % 12), 1980 + (rand() % 43));

                                    cout << "): ";

                                    getline(cin, entrada_data);

                                    if (entrada_data.size() != 10)
                                    {
                                        cout << "\tNumero de caracteres diferente de 10. Tente novamente.\n";
                                        entrada_data.clear();
                                    }

                                    else if (entrada_data[2] != '/' or 
                                            entrada_data[5] != '/' or
                                            entrada_data[0] < '0' or 
                                            entrada_data[0] > '9' or
                                            entrada_data[1] < '0' or 
                                            entrada_data[1] > '9' or
                                            entrada_data[3] < '0' or 
                                            entrada_data[3] > '9' or
                                            entrada_data[4] < '0' or 
                                            entrada_data[4] > '9' or
                                            entrada_data[6] < '0' or 
                                            entrada_data[6] > '9' or
                                            entrada_data[7] < '0' or 
                                            entrada_data[7] > '9' or
                                            entrada_data[8] < '0' or 
                                            entrada_data[8] > '9' or
                                            entrada_data[9] < '0' or 
                                            entrada_data[9] > '9')
                                    {
                                        cout << "\tEntrada invalida. Tente novamente.\n";
                                        entrada_data.clear();
                                    }
                                }

                                strcpy(tN.data, entrada_data.c_str());

                                break;
                            }

                            case ANO_LANCAMENTO_ID:
                            {
                                string entrada_ano_lancamento;

                                while (entrada_ano_lancamento.empty())
                                {
                                    cout << "\tAno de Lancamento [4 Caracteres] (Ex: ";

                                    printf("%04d", 1950 + (rand() % 73));

                                    cout << "): ";

                                    getline(cin, entrada_ano_lancamento);

                                    if (entrada_ano_lancamento.size() != 4)
                                    {
                                        cout << "\tNumero de caracteres diferente de 4. Tente novamente.\n";
                                        entrada_ano_lancamento.clear();
                                    }

                                    else if (entrada_ano_lancamento[0] < '0' or
                                            entrada_ano_lancamento[0] > '9' or
                                            entrada_ano_lancamento[1] < '0' or
                                            entrada_ano_lancamento[1] > '9' or
                                            entrada_ano_lancamento[2] < '0' or
                                            entrada_ano_lancamento[2] > '9' or
                                            entrada_ano_lancamento[3] < '0' or
                                            entrada_ano_lancamento[3] > '9')
                                    {
                                        cout << "\tEntrada invalida. Tente novamente.\n";
                                        entrada_ano_lancamento.clear();
                                    }
                                }

                                tN.ano_lancamento = (short) stoi(entrada_ano_lancamento);

                                break;
                            }

                            case AVALIACAO_ID:
                            {
                                string entrada_avaliacao;

                                while (entrada_avaliacao.empty())
                                {
                                    cout << "\tAvaliacao [MAX 10 Caracteres]: ";

                                    getline(cin, entrada_avaliacao);

                                    if (entrada_avaliacao.size() > 10)
                                    {
                                        cout << "\tLimite de caracteres excedido. Tente novamente.\n";
                                        entrada_avaliacao.clear();
                                    }
                                }

                                strcpy(tN.avaliacao, entrada_avaliacao.c_str());

                                break;
                            }

                            case DURACAO_ID:
                            {
                                string entrada_duracao;

                                while (entrada_duracao.empty())
                                {
                                    cout << "\tDuracao [MAX 12 Caracteres]: ";

                                    getline(cin, entrada_duracao);

                                    if (entrada_duracao.size() > 12)
                                    {
                                        cout << "\tLimite de caracteres excedido. Tente novamente.\n";
                                        entrada_duracao.clear();
                                    }
                                }

                                strcpy(tN.duracao, entrada_duracao.c_str());

                                break;
                            }

                            case LISTADO_EM_ID:
                            {
                                int qtd;
                                string entrada_qtd_listagens;

                                while (entrada_qtd_listagens.empty())
                                {
                                    cout << "\tListado em - Digite quantas listagens a serem inseridas: ";

                                    getline(cin, entrada_qtd_listagens);

                                    try
                                    {
                                        for (int i = 0; i < (int) entrada_qtd_listagens.size(); i++)
                                        {
                                            if (entrada_qtd_listagens[i] < '0' or entrada_qtd_listagens[i] > '9') throw exception();
                                        }

                                        qtd = stoi(entrada_qtd_listagens);
                                    }
                                    catch(const std::exception& e)
                                    {
                                        cerr << "\tQuantidade invalida. Tente novamente.\n";
                                        entrada_qtd_listagens.clear();
                                    }
                                }

                                string entrada_listagem;

                                for (int i_listagens = 0; i_listagens < qtd; i_listagens++)
                                {
                                    cout << "\t" << (i_listagens+1) << "a listagem: ";

                                    getline(cin, entrada_listagem);

                                    tN.listado_em.lista.push_back(entrada_listagem);
                                }

                                break;
                            }

                            case DESCRICAO_ID:
                            {
                                string entrada_descricao;

                                while (entrada_descricao.empty())
                                {
                                    cout << "\tDescricao [MAX 1000 Caracteres]: ";

                                    getline(cin, entrada_descricao);

                                    if (entrada_descricao.size() > 1000)
                                    {
                                        cout << "\tLimite de caracteres excedido. Tente novamente.\n";
                                        entrada_descricao.clear();
                                    }
                                }

                                strcpy(tN.descricao, entrada_descricao.c_str());

                                break;
                            }
                        }
                    }
                }

                cout << "REGISTRO A SER ADICIONADO:\n";
                tN.print(campos);

                gerenciador_registros.inserir_registro_inteligente(tN);

                estatistica.precisa_atualizar = true;
            }

            else if (numero_escolha == DELETAR_REGISTRO)
            {
                cout 
                    << "Voce escolheu DELETAR REGISTRO." << endl
                    << "Digite o ID do registro a ser deletado (ex: s" << rand()%7788 << "): ";

                string entrada_id, entrada_confirmar;

                while (entrada_id.empty())
                {
                    getline(cin, entrada_id);
                }

                bool resposta = gerenciador_registros.deletar_registro(entrada_id);

                if (resposta == false)
                {
                    cout << "RESULTADO: Registro com id definido nao encontrado para delecao.\n\n";
                }

                else
                {
                    cout << "RESULTADO: Registro com id: " << entrada_id << " deletado.\n\n";

                    estatistica.precisa_atualizar = true;
                }
            }

            else if (numero_escolha == ESTATISTICA)
            {
                if (estatistica.precisa_atualizar == true)
                {
                    estatistica.gerar_estatisticas((ifstream&) gerenciador_registros.arquivo_dados, (ifstream&) gerenciador_registros.arquivo_indice, campos);

                    estatistica.precisa_atualizar = false;
                }

                cout
                    << "ESTATISTICAS:" << endl
                    << '\t' << "Quantidade de Registros: " << estatistica.quantidade_registros << endl
                    << '\t' << "Tamanho Medio dos Registros: " << estatistica.tamanho_medio_registro << " caracteres" << endl;

                // Maior Registro
                cout
                    << '\t' << "Maior Registro:" << endl
                    << '\t' << "Tamanho: " << estatistica.tamanho_maior << endl
                    << "\t--------------------------------\n";

                estatistica.maior.print(campos);

                cout << "\t--------------------------------\n";

                // Menor Registro
                cout
                    << '\t' << "Menor Registro:" << endl
                    << '\t' << "Tamanho: " << estatistica.tamanho_menor << endl
                    << "\t--------------------------------\n";

                estatistica.menor.print(campos);

                cout << "\t--------------------------------\n";
            }

            else
            {
                cout << endl << "O programa sera finalizado." << endl;

                break;
            }
        }

        gerenciador_registros.fechar_fstreams();
    }

    return 0;
}
