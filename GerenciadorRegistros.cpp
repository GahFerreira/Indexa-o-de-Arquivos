#include "GerenciadorRegistros.h"

// `campos` define quais campos de TituloNetflix são utilizados para operações desse objeto.
GerenciadorRegistros::GerenciadorRegistros(string nome_arquivo_dados, string nome_arquivo_indice, string nome_arquivo_titulo, string nome_arquivo_reinsercao_dados, vector<bool> campos)
{
    this->nome_arquivo_dados = nome_arquivo_dados;
    this->nome_arquivo_indice = nome_arquivo_indice;
    this->nome_arquivo_titulo = nome_arquivo_titulo;
    this->nome_arquivo_reinsercao_dados = nome_arquivo_reinsercao_dados;
    this->campos = campos;
}

void GerenciadorRegistros::abrir_fstreams()
{
    arquivo_dados.open(nome_arquivo_dados, ios::in | ios::out | ios::binary);
    arquivo_indice.open(nome_arquivo_indice, ios::in | ios::out | ios::binary);
    arquivo_titulo.open(nome_arquivo_titulo, ios::in | ios::out | ios::binary);
    arquivo_reinsercao_dados.open(nome_arquivo_reinsercao_dados, ios::in | ios::out | ios::binary);
}

void GerenciadorRegistros::fechar_fstreams()
{
    arquivo_dados.close();
    arquivo_indice.close();
    arquivo_titulo.close();
    arquivo_reinsercao_dados.close();
}

/**
 * A partir do byte que indica a posição no arquivo de dados onde um determinado registro está,
 * retorna aquele registro.
*/
TituloNetflix GerenciadorRegistros::localizar_registro_arquivo_dados(int bytes_do_inicio)
{
    arquivo_dados.seekg(bytes_do_inicio, ios_base::beg);

    string registro = Manipulador::ler_registro((ifstream&) arquivo_dados);

    return TituloNetflix(registro, campos);
}

/**
 * Busca o `id_buscado` através da busca binária no arquivo de índices primário.
 * Retorna a posição no arquivo de dados do registro procurado, ou -1 se não achar
 * registro correspondente.
*/
int GerenciadorRegistros::busca_id(string id_buscado)
{
    // Transforma a string `id_buscado` em minúscula
    for (auto& caractere : id_buscado)
    {
        caractere = tolower(caractere);
    }

    if (id_eh_valido(id_buscado) == false) return ID_INVALIDO;

    arquivo_indice.seekg(0, ios_base::beg);
    int quantidade_registros_indice = Manipulador::ler_inteiro((ifstream&) arquivo_indice);

    // Guardará todo o conteúdo do arquivo em memória
    RegistroIndice registros_indice[quantidade_registros_indice];

    // Pula o cabeçalho
    arquivo_indice.seekg(sizeof(int), ios_base::beg);

    // Leitura de todo o conteúdo do arquivo de índices direto de uma só vez.
    Manipulador::ler_dados((ifstream&) arquivo_indice, quantidade_registros_indice * sizeof(RegistroIndice), &registros_indice[0]);

    // Busca binária
    return busca_binaria_id_para_bytes_do_inicio(registros_indice, quantidade_registros_indice, id_buscado);
}

/**
 * Busca todos os registros com `titulo_registro` como substring de seu titulo.
 * A função retorna um vetor de string com os ids dos registros encontrados.
*/
vector<string> GerenciadorRegistros::busca_titulo(string titulo_registro)
{
    // Transforma a string em minúscula
    for (auto& caractere : titulo_registro)
    {
        caractere = tolower(caractere);
    }

    // Lê o cabeçalho
    arquivo_titulo.seekg(0, ios_base::beg);
    int quantidade_registros_titulo = Manipulador::ler_inteiro((ifstream&) arquivo_titulo);

    RegistroTitulo registros_titulo[quantidade_registros_titulo];

    // Leitura de todo o conteúdo do arquivo de índices indireto, ou títulos, de uma só vez
    Manipulador::ler_dados((ifstream&) arquivo_titulo, quantidade_registros_titulo * (int) sizeof(RegistroTitulo), &registros_titulo[0]);

    vector<string> respostas;

    for (int i = 0; i < quantidade_registros_titulo; i++)
    {
        // Encontrou um titulo no arquivo de titulos que pareou com a entrada e não foi deletado
        if (registros_titulo[i].id[0] != '*' and
            string(registros_titulo[i].titulo).find(titulo_registro) != string::npos)
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

/**
 * Realiza uma busca binária entre os índices dos registros do arquivo de índices, e
 * se encontrar um índice igual ao pesquisado, retorna a posição desse registro no
 * arquivo de dados.
*/
int GerenciadorRegistros::busca_binaria_id_para_bytes_do_inicio(RegistroIndice registros_indice[], int quantidade_registros_indice, string id_buscado)
{
    // Usa uma versão particular do algoritmo mais geral implementado pela função abaixo.
    return busca_binaria_para_posicoes_nos_arquivos(registros_indice, quantidade_registros_indice, id_buscado).first;
}

/**
 * Realiza uma busca binária entre os índices dos registros do arquivo de índices, e
 * se encontrar um índice igual ao pesquisado, retorna a posição desse registro no
 * arquivo de dados e no arquivo de índices.
 *
 * Retorno: (pair<int, int>) { pos_arquivo_dados, pos_arquivo_indices }
*/
pair<int, int> GerenciadorRegistros::busca_binaria_para_posicoes_nos_arquivos(RegistroIndice registros_indice[], int quantidade_registros_indice, string id_buscado)
{
    int ini = 0, fim = quantidade_registros_indice-1, mid;

    int id_buscado_inteiro = TituloNetflix::id_para_inteiro(id_buscado);

    while (ini <= fim)
    {
        mid = (ini+fim) / 2;

        if (TituloNetflix::id_para_inteiro(registros_indice[mid].id) == id_buscado_inteiro)
        {
            // Se achou, retorna a posição no arquivo de dados onde o registro está.
            return {registros_indice[mid].bytes_do_inicio, sizeof(int) + (mid * sizeof(RegistroIndice))};
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
    return {NAO_ENCONTROU, NAO_ENCONTROU};
}


/**
 * Função que, a partir de um vetor de ids, retorna um vetor com a posição de cada registro
 * referente a um id no arquivo de dados.
 * {id_1, id_2, ..., id_n } -> {pos_1, pos_2, ..., pos_n }
 *
 * Sua grande vantagem é ler o arquivo de índices primários apenas uma vez.
*/
vector<int> GerenciadorRegistros::lista_de_ids_para_lista_de_posicoes(vector<string> ids)
{
    arquivo_indice.seekg(0, ios_base::beg);
    int quantidade_registros_indice = Manipulador::ler_inteiro((ifstream&) arquivo_indice);

    // Guardará todo o conteúdo do arquivo em memória
    RegistroIndice registros_indice[quantidade_registros_indice];

    // Leitura de todo o conteúdo do arquivo de índices direto de uma só vez.
    Manipulador::ler_dados((ifstream&) arquivo_indice, quantidade_registros_indice * sizeof(RegistroIndice), &registros_indice[0]);

    vector<int> respostas;

    for (int i = 0; i < (int) ids.size(); i++)
    {
        respostas.push_back( busca_binaria_id_para_bytes_do_inicio(registros_indice, quantidade_registros_indice, ids[i]) );
    }

    return respostas;
}

// Reordena os índices do arquivo de índices
void GerenciadorRegistros::ordenar_arquivo_indice()
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

    Manipulador::escrever_dados((ofstream&) arquivo_indice, (void *) registros_indice, sizeof(RegistroIndice) * quantidade_registros_arquivo_indice);
}

// Funciona de forma extremamente similar à ordenação de arquivo de índices
// Ordena os registros pelo número de bytes disponível para reinserção naquela posição.
// Ordena decrescente.
void GerenciadorRegistros::ordenar_arquivo_reinsercao()
{
    // Lê todos os registros do arquivo de índices primário
    arquivo_reinsercao_dados.seekg(0, ios_base::beg);

    int quantidade_registros_arquivo_reinsercao_dados = Manipulador::ler_inteiro((ifstream&) arquivo_reinsercao_dados);

    RegistroReinsercaoDados registros_reinsercao_dados[quantidade_registros_arquivo_reinsercao_dados];

    Manipulador::ler_dados((ifstream&) arquivo_reinsercao_dados, sizeof(RegistroReinsercaoDados) * quantidade_registros_arquivo_reinsercao_dados, &registros_reinsercao_dados[0]);

    // Primeiro ordena pela posição no arquivo de dados.
    // Isso é útil para combinar registros adjacentes que foram apagados em um só registro
    // de reinserção com os dois tamanhos somados.
    sort(registros_reinsercao_dados,
         registros_reinsercao_dados + quantidade_registros_arquivo_reinsercao_dados,
        [](RegistroReinsercaoDados rRD_1, RegistroReinsercaoDados rRD_2)
        {
            return rRD_1.bytes_do_inicio < rRD_2.bytes_do_inicio;
        }
    );

    for(int i = 0; i < quantidade_registros_arquivo_reinsercao_dados-1; i++)
    {
        // Combina dois registros diretamente adjacentes em apenas 1 registro
        // com os seus tamanhos somados.
        if (registros_reinsercao_dados[i].bytes_do_inicio + registros_reinsercao_dados[i].quantidade_bytes == registros_reinsercao_dados[i+1].bytes_do_inicio)
        {
            registros_reinsercao_dados[i+1].bytes_do_inicio = registros_reinsercao_dados[i].bytes_do_inicio;
            registros_reinsercao_dados[i+1].quantidade_bytes+= registros_reinsercao_dados[i].quantidade_bytes;

            registros_reinsercao_dados[i].quantidade_bytes = 0;
        }
    }

    // Agora ordena baseado em quantidade de bytes.
    // Note que se houve alguma combinação na etapa anterior, a quantidade de bytes de um dos 
    // dois registros será 0, ou seja, será removido.
    sort(registros_reinsercao_dados,
         registros_reinsercao_dados + quantidade_registros_arquivo_reinsercao_dados,
        [](RegistroReinsercaoDados rRD_1, RegistroReinsercaoDados rRD_2)
        {
            return rRD_1.quantidade_bytes > rRD_2.quantidade_bytes;
        }
    );

    // Conta quantos registros foram excluídos e retira essa quantidade
    // da quantidade total de registros.
    for (int i = quantidade_registros_arquivo_reinsercao_dados-1; i >= 0; i--)
    {
        // Registros com quantidade de bytes <= 0 são removidos do arquivo.
        if (registros_reinsercao_dados[i].quantidade_bytes <= 0)
        {
            quantidade_registros_arquivo_reinsercao_dados--;
        }

        else break;
    }

    // Volta pra escrever o novo cabeçalho
    arquivo_reinsercao_dados.seekp(0, ios_base::beg);

    // Escreve os dados atualizados sobre os antigos
    Manipulador::escrever_inteiro((ofstream&) arquivo_reinsercao_dados, quantidade_registros_arquivo_reinsercao_dados);

    Manipulador::escrever_dados((ofstream&) arquivo_reinsercao_dados, (void *) registros_reinsercao_dados, sizeof(RegistroReinsercaoDados) * quantidade_registros_arquivo_reinsercao_dados);
}

/**
 * Função que recebe um registro e o insere no final do arquivo de dados.
 * Além disso, faz todas as modificações adequadas aos arquivos.
*/
void GerenciadorRegistros::inserir_registro_final(TituloNetflix tN)
{
    // Inserção final do arquivo de dados
    arquivo_dados.seekg(0, ios_base::beg);
    int quantidade_registros_dados = Manipulador::ler_inteiro((ifstream&) arquivo_dados);

    arquivo_dados.seekp(0, ios_base::end);

    int posicao_arquivo_dados = arquivo_dados.tellp();

    string registro = tN.to_string(campos);

    int tamanho_registro = (int) registro.size();

    Manipulador::escrever_inteiro((ofstream&) arquivo_dados, tamanho_registro);

    Manipulador::escrever_dados((ofstream&) arquivo_dados, (void *) registro.c_str(), tamanho_registro);

    // Cabeçalho
    arquivo_dados.seekp(0, ios_base::beg);
    Manipulador::escrever_inteiro((ofstream&) arquivo_dados, quantidade_registros_dados+1);


    // Atualização arquivo índice primário
    // Primeiro, obtem o número de registros salvos no arquivo (igual a quantidade_registros, idealmente)
    arquivo_indice.seekg(0, ios_base::beg);
    int qtd_registros_indice = Manipulador::ler_inteiro((ifstream&) arquivo_indice);

    RegistroIndice registro_indice;
    strcpy(registro_indice.id, tN.id);
    registro_indice.bytes_do_inicio = posicao_arquivo_dados;

    // Vai pra primeira posição após o último registro válido
    arquivo_indice.seekp(sizeof(int) + (qtd_registros_indice * sizeof(RegistroIndice)), ios_base::beg);
    Manipulador::escrever_dados((ofstream&) arquivo_indice, (void *) &registro_indice, sizeof(RegistroIndice));

    // Atualiza o cabeçalho
    arquivo_indice.seekp(0, ios_base::beg);
    Manipulador::escrever_inteiro((ofstream&) arquivo_indice, qtd_registros_indice+1);

    // Reordenação
    ordenar_arquivo_indice();


    // Atualização arquivo título
    arquivo_titulo.seekg(0, ios_base::beg);
    int qtd_registros_titulo = Manipulador::ler_inteiro((ifstream&) arquivo_titulo);

    // Transforma o título para apenas letras minúsculas
    for (int i = 0; i < TAM_TITULO+1 and tN.titulo[i] != '\0'; i++)
    {
        tN.titulo[i] = tolower(tN.titulo[i]);
    }

    RegistroTitulo registro_titulo[1];
    strcpy(registro_titulo[0].titulo, tN.titulo);
    strcpy(registro_titulo[0].id, tN.id);

    // Vai pra primeira posição após o último registro válido
    arquivo_titulo.seekp(sizeof(int) + (qtd_registros_titulo * sizeof(RegistroTitulo)), ios_base::beg);

    Manipulador::escrever_dados((ofstream&) arquivo_titulo, (void *) registro_titulo, sizeof(RegistroTitulo));

    // Atualiza o cabeçalho
    arquivo_titulo.seekp(0, ios_base::beg);
    Manipulador::escrever_inteiro((ofstream&) arquivo_titulo, qtd_registros_titulo+1);
}

/**
 * Função que recebe um registro e o insere de forma inteligente no arquivo de dados.
 * Isso quer dizer que o registro vai possivelmente ocupar bytes de um registro previamente excluido.
 * Além disso, faz todas as modificações adequadas aos arquivos.
*/
void GerenciadorRegistros::inserir_registro_inteligente(TituloNetflix tN)
{
    string registro = tN.to_string(campos);

    // Checa para ver se há lugares vagos no arquivo.
    // Se não houver, insere ao final.
    arquivo_reinsercao_dados.seekg(0, ios_base::beg);
    if (Manipulador::ler_inteiro((ifstream&) arquivo_reinsercao_dados) <= 0)
    {
        inserir_registro_final(tN);

        return;
    }

    // Checa a posição WORST FIT pra ver se ela serve pro registro.
    RegistroReinsercaoDados registro_reinsercao_dados;

    Manipulador::ler_dados((ifstream&) arquivo_reinsercao_dados, sizeof(RegistroReinsercaoDados), &registro_reinsercao_dados);

    // Se não couber, insere no final.
    if ((int) registro.size() + (int) sizeof(int) > registro_reinsercao_dados.quantidade_bytes)
    {
        inserir_registro_final(tN);

        return;
    }

    // Se couber, insere naquela posição.
    // Atualização arquivo dados
    arquivo_dados.seekg(0, ios_base::beg);
    int quantidade_registros_dados = Manipulador::ler_inteiro((ifstream&) arquivo_dados);

    arquivo_dados.seekp(registro_reinsercao_dados.bytes_do_inicio, ios_base::beg);

    int tamanho_registro = (int) registro.size();

    Manipulador::escrever_inteiro((ofstream&) arquivo_dados, tamanho_registro);

    Manipulador::escrever_dados((ofstream&) arquivo_dados, (void *) registro.c_str(), tamanho_registro);

    // Cabeçalho
    arquivo_dados.seekp(0, ios_base::beg);
    Manipulador::escrever_inteiro((ofstream&) arquivo_dados, quantidade_registros_dados+1);


    // Atualização arquivo índice primário
    // Primeiro, obtem o número de registros salvos no arquivo (igual a quantidade_registros, idealmente)
    arquivo_indice.seekg(0, ios_base::beg);
    int qtd_registros_indice = Manipulador::ler_inteiro((ifstream&) arquivo_indice);

    RegistroIndice registro_indice[1];
    strcpy(registro_indice[0].id, tN.id);
    registro_indice[0].bytes_do_inicio = registro_reinsercao_dados.bytes_do_inicio;

    // Vai pra primeira posição após o último registro válido
    arquivo_indice.seekp(sizeof(int) + (qtd_registros_indice * sizeof(RegistroIndice)), ios_base::beg);
    Manipulador::escrever_dados((ofstream&) arquivo_indice, (void *) registro_indice, sizeof(RegistroIndice));

    // Atualiza o cabeçalho
    arquivo_indice.seekp(0, ios_base::beg);
    Manipulador::escrever_inteiro((ofstream&) arquivo_indice, qtd_registros_indice+1);

    // Reordenação
    ordenar_arquivo_indice();


    // Atualização arquivo título
    arquivo_titulo.seekg(0, ios_base::beg);
    int qtd_registros_titulo = Manipulador::ler_inteiro((ifstream&) arquivo_titulo);

    // Transforma o título para apenas letras minúsculas
    for (int i = 0; i < TAM_TITULO+1 and tN.titulo[i] != '\0'; i++)
    {
        tN.titulo[i] = tolower(tN.titulo[i]);
    }

    RegistroTitulo registro_titulo[1];
    strcpy(registro_titulo[0].titulo, tN.titulo);
    strcpy(registro_titulo[0].id, tN.id);

    // Vai pra primeira posição após o último registro válido
    arquivo_titulo.seekp(sizeof(int) + (qtd_registros_titulo * sizeof(RegistroTitulo)), ios_base::beg);

    Manipulador::escrever_dados((ofstream&) arquivo_titulo, (void *) registro_titulo, sizeof(RegistroTitulo));

    // Atualiza o cabeçalho
    arquivo_titulo.seekp(0, ios_base::beg);
    Manipulador::escrever_inteiro((ofstream&) arquivo_titulo, qtd_registros_titulo+1);


    // Atualização arquivo reinserção de dados
    registro_reinsercao_dados.bytes_do_inicio += (int) sizeof(int) + (int) registro.size();
    registro_reinsercao_dados.quantidade_bytes -= (int) sizeof(int) + (int) registro.size();

    arquivo_reinsercao_dados.seekp(sizeof(int), ios_base::beg);
    Manipulador::escrever_dados((ofstream&) arquivo_reinsercao_dados, (void *) &registro_reinsercao_dados, sizeof(RegistroReinsercaoDados));

    ordenar_arquivo_reinsercao();
}

bool GerenciadorRegistros::deletar_registro(string id_registro)
{
    // Checagem de validade do id
    if (id_eh_valido(id_registro) == false) return false;

    // Checagem de existência do id
    arquivo_indice.seekg(0, ios_base::beg);

    int quantidade_registros_indice = Manipulador::ler_inteiro((ifstream&) arquivo_indice);

    RegistroIndice registros_indice[quantidade_registros_indice];

    Manipulador::ler_dados((ifstream&) arquivo_indice, sizeof(RegistroIndice) * quantidade_registros_indice, &registros_indice[0]);

    int pos_registro_arquivo_dados, pos_registro_arquivo_indices;

    tie(pos_registro_arquivo_dados, pos_registro_arquivo_indices) = busca_binaria_para_posicoes_nos_arquivos(registros_indice, quantidade_registros_indice, id_registro);

    if (pos_registro_arquivo_dados < 0) return false;

    // Caso o id seja válido e exista: deleção confirmada.
    char asterisco = '*';

    // Atualização arquivo índice
    arquivo_indice.seekp(pos_registro_arquivo_indices, ios_base::beg);

    Manipulador::escrever_dados((ofstream&) arquivo_indice, (void *) &asterisco, sizeof(char));

    // O cabeçalho é atualizado diretamente na ordenação
    // Reordenação
    ordenar_arquivo_indice();


    // Atualização arquivo dados
    // Move-se para a posição onde fica o 's' do id do registro.
    arquivo_dados.seekp(pos_registro_arquivo_dados + sizeof(int), ios_base::beg);

    Manipulador::escrever_dados((ofstream&) arquivo_dados, (void *) &asterisco, sizeof(char));

    // Cabeçalho
    arquivo_dados.seekg(0, ios_base::beg);
    int quantidade_registros_dados = Manipulador::ler_inteiro((ifstream&) arquivo_dados);

    arquivo_dados.seekp(0, ios_base::beg);
    Manipulador::escrever_inteiro((ofstream&) arquivo_dados, quantidade_registros_dados-1);


    // Atualização arquivo titulo
    arquivo_titulo.seekg(0, ios_base::beg);

    int quantidade_registros_titulo = Manipulador::ler_inteiro((ifstream&) arquivo_titulo);

    RegistroTitulo registros_titulo[quantidade_registros_titulo];

    Manipulador::ler_dados((ifstream&) arquivo_titulo, sizeof(RegistroTitulo) * quantidade_registros_titulo, &registros_titulo[0]);

    for (int i = 0; i < quantidade_registros_titulo; i++)
    {
        if (id_registro == registros_titulo[i].id)
        {
            // Move-se para a posição onde fica o 's' do id do registro.
            arquivo_titulo.seekp(sizeof(int) + (i * sizeof(RegistroTitulo)) + TAM_TITULO+1);

            Manipulador::escrever_dados((ofstream&) arquivo_titulo, (void *) &asterisco, sizeof(char));

            break;
        }
    }

    // Não atualiza o cabeçalho, pois não realmente deleta registros.


    // Atualização arquivo reinserção de registros de dados
    // Vai para a posição do registro no arquivo de dados e lê seu tamanho
    arquivo_dados.seekg(pos_registro_arquivo_dados, ios_base::beg);
    int tamanho_registro = Manipulador::ler_inteiro((ifstream&) arquivo_dados);

    arquivo_reinsercao_dados.seekg(0, ios_base::beg);
    int quantidade_registros_reinsercao_dados = Manipulador::ler_inteiro((ifstream&) arquivo_reinsercao_dados);

    RegistroReinsercaoDados registro_reinsercao_dados[1];
    registro_reinsercao_dados[0].bytes_do_inicio = pos_registro_arquivo_dados;
    registro_reinsercao_dados[0].quantidade_bytes = tamanho_registro;

    // Vai pra primeira posição após o último registro válido e escreve o novo registro
    arquivo_reinsercao_dados.seekp(sizeof(int) + (quantidade_registros_reinsercao_dados * sizeof(RegistroReinsercaoDados)), ios_base::beg);
    Manipulador::escrever_dados((ofstream&) arquivo_reinsercao_dados, (void *) registro_reinsercao_dados, sizeof(RegistroReinsercaoDados));

    // Atualiza o cabeçalho
    arquivo_reinsercao_dados.seekp(0, ios_base::beg);
    Manipulador::escrever_inteiro((ofstream&) arquivo_reinsercao_dados, quantidade_registros_reinsercao_dados+1);

    // Reordena o arquivo
    ordenar_arquivo_reinsercao();

    return true;
}
