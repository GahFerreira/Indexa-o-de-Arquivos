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
 * Retorna a posição no arquivo de dados do registro procurado, ou -1 se não achar
 * registro correspondente.
*/
int GerenciadorRegistros::busca_id(ifstream& arquivo_indice, string id_buscado)
{
    // Transforma a string `id_buscado` em minúscula
    for (auto& caractere : id_buscado)
    {
        caractere = tolower(caractere);
    }

    if (id_eh_valido(id_buscado) == false) return -2;

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

bool GerenciadorRegistros::id_eh_valido(string id)
{
    // Entrada inválida
    if (id.front() != 's' or id.size() < 2) return false;
    for (int i = 1; i < (int) id.size(); i++)
    {
        if (id[i] < '0' or id[i] > '9') return false;
    }

    return true;
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

void GerenciadorRegistros::ordenar_arquivo_indice(fstream& arquivo_indice)
{
    // Lê todos os registros do arquivo de índices primário
    arquivo_indice.seekg(0, ios_base::beg);

    int quantidade_registros_arquivo_indice = Manipulador::ler_inteiro((ifstream&) arquivo_indice);

    RegistroIndice registros_indice[quantidade_registros_arquivo_indice];

    Manipulador::ler_dados((ifstream&) arquivo_indice, sizeof(RegistroIndice) * quantidade_registros_arquivo_indice, &registros_indice[0]);

    // Ordena os registros válidos (não inclui os marcados como excluídos)
    sort(registros_indice, registros_indice + quantidade_registros_arquivo_indice,
        [](RegistroIndice rI_1, RegistroIndice rI_2)
        {
            // O registro excluído sempre vai pro final
            if (rI_1.id[0] == '*') return false;
            if (rI_2.id[0] == '*') return true;

            return TituloNetflix::id_para_inteiro(rI_1.id) < TituloNetflix::id_para_inteiro(rI_2.id);
        }
    );

    // Conta quantos registros foram excluídos e retira essa quantidade
    // da quantidade total de registros.
    for (int i = quantidade_registros_arquivo_indice-1; i >= 0; i--)
    {
        if (registros_indice[i].id[0] == '*') quantidade_registros_arquivo_indice--;

        else break;
    }

    arquivo_indice.seekp(0, ios_base::beg);

    // Escreve os dados atualizados sobre os antigos
    Manipulador::escrever_inteiro((ofstream&) arquivo_indice, quantidade_registros_arquivo_indice);

    Manipulador::escrever_dados((ofstream&) arquivo_indice, registros_indice, sizeof(RegistroIndice) * quantidade_registros_arquivo_indice);
}

void GerenciadorRegistros::inserir_registro_final(ofstream& arquivo_dados, fstream& arquivo_indice, ofstream& arquivo_titulo, TituloNetflix tN)
{
    quantidade_registros++;

    // Inserção final do arquivo de dados
    arquivo_dados.seekp(0, ios_base::end);

    int posicao_arquivo_dados = arquivo_dados.tellp();

    string registro = tN.to_string(campos);

    int tamanho_registro = (int) registro.size();

    arquivo_dados.write((const char *) &tamanho_registro, sizeof(tamanho_registro));

    arquivo_dados.write((const char *) registro.c_str(), tamanho_registro);

    // Cabeçalho
    arquivo_dados.seekp(0, ios_base::beg);
    Manipulador::escrever_inteiro(arquivo_dados, quantidade_registros);


    // Atualização arquivo índice primário
    RegistroIndice registro_indice;
    strcpy(registro_indice.id, tN.id);
    registro_indice.bytes_do_inicio = posicao_arquivo_dados;

    arquivo_indice.seekp(0, ios_base::end);

    Manipulador::escrever_dados((ofstream&) arquivo_indice, &registro_indice, sizeof(RegistroIndice));

    // Cabeçalho
    arquivo_indice.seekp(0, ios_base::beg);
    Manipulador::escrever_inteiro((ofstream&) arquivo_indice, quantidade_registros);

    // Reordenação
    ordenar_arquivo_indice(arquivo_indice);

    // Atualização arquivo título
    RegistroTitulo registro_titulo;
    strcpy(registro_titulo.titulo, tN.titulo);
    strcpy(registro_titulo.id, tN.id);

    arquivo_titulo.seekp(0, ios_base::end);

    Manipulador::escrever_dados(arquivo_titulo, &registro_titulo, sizeof(RegistroTitulo));

    // Cabeçalho
    arquivo_titulo.seekp(0, ios_base::beg);
    Manipulador::escrever_inteiro(arquivo_titulo, quantidade_registros);
}