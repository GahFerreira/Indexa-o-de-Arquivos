#include "GerenciadorRegistros.h"

GerenciadorRegistros::GerenciadorRegistros(string nome_arquivo_indice, string nome_arquivo_titulo, int quantidade_registros, vector<bool> campos)
{
    this->nome_arquivo_indice = nome_arquivo_indice;
    this->nome_arquivo_titulo = nome_arquivo_titulo;
    this->quantidade_registros = quantidade_registros;
    this->campos = campos;
}

/**
 * A partir do byte que indica a posição no arquivo de dados onde um determinado registro está,
 * retorna aquele registro.
*/
TituloNetflix GerenciadorRegistros::localizar_registro_arquivo_dados(ifstream& arquivo_dados, int bytes_do_inicio)
{
    arquivo_dados.seekg(bytes_do_inicio, ios_base::beg);

    string registro = Manipulador::ler_registro(arquivo_dados);

    return TituloNetflix(registro, campos);
}

/**
 * Busca o `id_buscado` através da busca binária no arquivo de índices primário.
*/
int GerenciadorRegistros::busca_id(ifstream& arquivo_indice, string id_buscado)
{
    // Transforma a string `id_buscado` em minúscula
    for (auto& caractere : id_buscado)
    {
        caractere = tolower(caractere);
    }

    // Entrada inválida
    if (id_buscado.front() != 's') return -2;
    for (int i = 1; i < (int) id_buscado.size(); i++)
    {
        if (id_buscado[i] < '0' or id_buscado[i] > '9') return -2;
    }

    // Guardará todo o conteúdo do arquivo em memória
    RegistroIndice registros_indice[quantidade_registros];

    // Pula o cabeçalho
    arquivo_indice.seekg(sizeof(int), ios_base::beg);

    // Leitura de todo o conteúdo do arquivo de índices direto de uma só vez.
    Manipulador::ler_dados(arquivo_indice, quantidade_registros * sizeof(RegistroIndice), &registros_indice[0]);

    // Busca binária
    return busca_binaria_id_para_bytes_do_inicio(registros_indice, id_buscado);
}

/**
 * Busca todos os registros com `titulo_registro` como substring de seu titulo.
 * A função retorna um vetor de string com os ids dos registros encontrados.
*/
vector<string> GerenciadorRegistros::busca_titulo(ifstream& arquivo_titulo, string titulo_registro)
{
    // Transforma a string em minúscula
    for (auto& caractere : titulo_registro)
    {
        caractere = tolower(caractere);
    }

    RegistroTitulo registros_titulo[quantidade_registros];

    // Salta o cabeçalho
    arquivo_titulo.seekg(sizeof(int), ios_base::beg);

    // Leitura de todo o conteúdo do arquivo de índices indireto de uma só vez
    Manipulador::ler_dados(arquivo_titulo, quantidade_registros * (int) sizeof(RegistroTitulo), &registros_titulo[0]);

    vector<string> respostas;

    for (int i = 0; i < quantidade_registros; i++)
    {
        // Encontrou um titulo no arquivo de titulos que pareou com a entrada
        if (string(registros_titulo[i].titulo).find(titulo_registro) != string::npos)
        {
            respostas.push_back(registros_titulo[i].id);
        }
    }

    return respostas;
}

int GerenciadorRegistros::busca_binaria_id_para_bytes_do_inicio(RegistroIndice registros_indice[], string id_buscado)
{
    int ini = 0, fim = quantidade_registros-1, mid;
    int id_buscado_inteiro = TituloNetflix::id_para_inteiro(id_buscado);

    while (ini <= fim)
    {
        mid = (ini+fim) / 2;

        if (TituloNetflix::id_para_inteiro(registros_indice[mid].id) == id_buscado_inteiro)
        {
            // Se achou, retorna a posição no arquivo de dados onde o registro está.
            return registros_indice[mid].bytes_do_inicio;
        }

        else if (TituloNetflix::id_para_inteiro(registros_indice[mid].id) < id_buscado_inteiro)
        {
            // Considera somente a metade maior do vector.
            ini = mid+1;
        }

        else
        {
            // Considera apenas a metade menor do vector.
            fim = mid-1;
        }
    }

    // Se não achou, retorna -1.
    return -1;
}

/**
 * Função que, a partir de um vetor de ids, retorna um vetor com a posição de cada registro
 * referente a um id no arquivo de dados.
 * {id_1, id_2, ..., id_n } -> {pos_1, pos_2, ..., pos_n }
 *
 * Sua grande vantagem é ler o arquivo de índices primários apenas uma vez.
*/
vector<int> GerenciadorRegistros::lista_de_ids_para_lista_de_posicoes(ifstream& arquivo_indice, vector<string> ids)
{
    // Guardará todo o conteúdo do arquivo em memória
    RegistroIndice registros_indice[quantidade_registros];

    // Pula o cabeçalho
    arquivo_indice.seekg(sizeof(int), ios_base::beg);

    // Leitura de todo o conteúdo do arquivo de índices direto de uma só vez.
    Manipulador::ler_dados(arquivo_indice, quantidade_registros * sizeof(RegistroIndice), &registros_indice[0]);

    vector<int> respostas;

    for (int i = 0; i < (int) ids.size(); i++)
    {
        respostas.push_back( busca_binaria_id_para_bytes_do_inicio(registros_indice, ids[i]) );
    }

    return respostas;
}
