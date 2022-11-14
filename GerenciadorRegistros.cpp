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
TituloNetflix GerenciadorRegistros::localizar_titulo_netflix_arquivo_dados(ifstream& arquivo_dados, int bytes_do_inicio)
{
    arquivo_dados.seekg(bytes_do_inicio, ios_base::beg);

    string registro = Manipulador::ler_registro(arquivo_dados);

    return TituloNetflix(registro, campos);
}

/**
 * Busca o `id_buscado` através da busca binária no arquivo de índices primário.
*/
int GerenciadorRegistros::busca_id(ifstream& arquivo_dados, ifstream& arquivo_indice, string id_buscado)
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

    // Pula o cabeçalho
    arquivo_indice.seekg(sizeof(int), ios_base::beg);

    // Guardará todo o conteúdo do arquivo em memória
    vector<RegistroIndice> registros_indice(quantidade_registros);

    // Leitura de todo o conteúdo do arquivo de índices direto de uma só vez.
    // `vector<MeuTipo>.data()` simula o comportamento de ler para um `MeuTipo array[]`.
    Manipulador::ler_dados(arquivo_indice, quantidade_registros * sizeof(RegistroIndice), registros_indice.data());

    // Busca binária
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
