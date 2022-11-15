#include "GeradorArquivos.h"

/**
 * Os arquivos a serem gerador conterão apenas os campos de TituloNetflix
 * definidos pelo vetor `campos_a_serem_usados`.
*/
GeradorArquivos::GeradorArquivos()
{
	this->campos_a_serem_usados = vector<bool>(QTD_CAMPOS, true);
}

GeradorArquivos::GeradorArquivos(vector<bool>& campos)
{
	this->campos_a_serem_usados = campos;
}

/**
 * Função com objetivo de, a partir de um arquivo de entrada
 * que contém 1 registro por linha, ler todos os registros do arquivo
 * e gerar um novo arquivo, denominado arquivo_dados,
 * que contém os mesmos registros com o seu tamanho escrito antes.
 *
 * Nessa etapa, apenas os seguintes campos pré-definidos serão inclusos:
 *   show_id
 *   type
 *   title
 *   contry
 *   release_year
 *
 * nome_arq_entrada: nome do arquivo de entrada
 * nome_arq_dados: nome do arquivo de dados (com registros e seu respectivo tamanho antes)
*/
bool GeradorArquivos::criar_arquivo_dados(const char *nome_arq_entrada, const char *nome_arq_dados)
{
	ifstream arquivo_entrada;
    ofstream arquivo_dados;
	string registro;
	int tamanho_registro, quantidade_registros = 0;

	arquivo_entrada.open(nome_arq_entrada, ios_base::in | ios_base::binary);
	arquivo_dados.open(nome_arq_dados, ios_base::out | ios_base::binary);

	// Checa a abertura dos arquivos
	if (arquivo_entrada.fail() == true or arquivo_dados.fail() == true)
	{
		if (arquivo_entrada.fail() == true)
		{
			cout << "Erro na abertura de arquivo " + string(nome_arq_entrada) << endl;
		}

		else arquivo_entrada.close();

		if (arquivo_dados.fail() == true)
		{
			cout << "Erro na abertura de arquivo " + string(nome_arq_dados) << endl;
		}

		else arquivo_dados.close();

		return false;
	}

	// Pula a primeira linha, que só contem os nomes das colunas
	// Isso interfere na criação posterior dos outros arquivos
	std::getline(arquivo_entrada, registro);

	// Guarda os primeiros bytes do arquivo para salvar a quantidade de registros
	arquivo_dados.seekp(sizeof(quantidade_registros), ios_base::beg);

	// Lê registros até não achar mais registros no arquivo (EOF)
	while (arquivo_entrada.eof() == false)
	{
		std::getline(arquivo_entrada, registro);

		// Se algum registro for lido com tamanho 0, passa para o próximo
		// Idealmente isso só ocorrerá no final do arquivo
		// Mas também ajuda com problemas de leitura de linhas vazias no meio do arquivo
		if (registro.empty()) continue;

		quantidade_registros++;

		TituloNetflix tN(registro);

		string registro_adaptado =
			string(tN.id) + ';' +
			string(tN.tipo) + ';' +
			string(tN.titulo) + ';' +
			tN.pais.to_string() + ';' +
			to_string(tN.ano_lancamento);

		tamanho_registro = (int) registro_adaptado.size();

		arquivo_dados.write((const char *) &tamanho_registro, sizeof(tamanho_registro));

		arquivo_dados.write((const char *) registro_adaptado.c_str(), tamanho_registro);
	}

	// Volta para o início do arquivo
	arquivo_dados.seekp(0, ios_base::beg);

	// Escreve a quantidade de registros no início (cabeçalho) do arquivo
	arquivo_dados.write((const char *) &quantidade_registros, sizeof(quantidade_registros));

	arquivo_entrada.close();
	arquivo_dados.close();

	// Checa se houve algum erro ocorreu, diferente de EOF / FAIL
	// EOF / FAIL acontecem ao tentar ler no final do arquivo
	if (arquivo_entrada.bad() or arquivo_dados.bad()) return false;

	return true;
}

/**
 * Função com objetivo de criar o arquivo de índices direto.
 * O arquivo começa com um cabeçalho indicando quantos registros guarda.
 * Cada registro tem tamanho fixo, e dois campos: {id, posição_arquivo_dados}
*/
bool GeradorArquivos::criar_arquivo_indice_primario(const char *nome_arq_dados, const char *nome_arq_indice_primario)
{
	ifstream arquivo_dados;
    ofstream arquivo_indice_primario;

    /**
    * registro: string que recebe o registro lido do arquivo de dados
    * quantidade_registros: variável que recebe a quantidade de registros lida do arquivo de dados
    * tamanho_registro: variável que recebe o tamanho de um registro lido do arquivo de dados
    * byte_atual: guarda o byte a partir do início do arquivo de dados em que começa um registro
    */
    string registro;
    int quantidade_registros, byte_atual = 0;

    arquivo_dados.open(nome_arq_dados, ios_base::in | ios_base::binary);
    arquivo_indice_primario.open(nome_arq_indice_primario, ios_base::out | ios_base::binary);

    // Checa a abertura dos arquivos
	if (arquivo_dados.fail() == true or arquivo_indice_primario.fail() == true)
	{
		if (arquivo_dados.fail() == true)
		{
			cout << "Erro na abertura de arquivo " + string(nome_arq_dados) << endl;
		}

		else arquivo_dados.close();

		if (arquivo_indice_primario.fail() == true)
		{
			cout << "Erro na abertura de arquivo " + string(nome_arq_indice_primario) << endl;
		}

		else arquivo_indice_primario.close();

		return false;
	}

    // Lê-se a quantidade de registros do arquivo de dados
    quantidade_registros = Manipulador::ler_inteiro(arquivo_dados);
    byte_atual += sizeof(int);

    // Escreve-se a quantidade de registros no arquivo de índices
    Manipulador::escrever_inteiro(arquivo_indice_primario, quantidade_registros);

    for (int i = 0; i < quantidade_registros; i++)
    {
		if(arquivo_dados.eof() == true)
		{
			cout 
				<< "Aviso: Bit EOF ativado durante criacao do arquivo de indices primario. "
				<< "Quantidade de registros lido: " << i << " registro(s).\n";
			break;
		}

		// Lê-se um registro do arquivo de dados
        registro = Manipulador::ler_registro(arquivo_dados);

		// Caso haja alguma linha vazia, pula ela.
		// Idealmente não há, por isso o aviso.
        if (registro.empty())
        {
			cout << "Aviso: Registro vazio lido durante criacao do arquivo de indices primario.\n";

			continue;
        }

        // Converte o registro lido (string) para TituloNetflix
        TituloNetflix tN(registro, campos_a_serem_usados);

		// Transforma o `id` para apenas minúsculas
		for (int i = 0; i < TAM_ID+1 and tN.id[i] != '\0'; i++)
		{
			tN.id[i] = tolower(tN.id[i]);
		}

        // Cria um registro formatado, que será salvo no arquivo de índices direto
        RegistroIndice rI;
        strcpy(rI.id, tN.id);
        rI.bytes_do_inicio = byte_atual;

        Manipulador::escrever_dados(arquivo_indice_primario, (void *) &rI, sizeof(rI));

        // Atualiza o byte_atual para pular a quantidade de bytes lida do arquivo de dados
        // `sizeof(int)` que é o inteiro indicador do tamanho do registro
        // `registro.size()` que é o tamanho do registro
        byte_atual += sizeof(int) + (int) registro.size();
    }

    arquivo_dados.close();
	arquivo_indice_primario.close();

    // Checa se houve algum erro ocorreu, diferente de EOF / FAIL
	// EOF / FAIL acontecem ao tentar ler no final do arquivo
	if (arquivo_dados.bad() or arquivo_indice_primario.bad()) return false;

	return true;
}

/**
 * Função que tem como objetivo criar um arquivo de índices indiretos.
 * O arquivo começa com um cabeçalho indicando quantos registros guarda.
 * Cada registro tem tamanho fixo, e dois campos: {titulo, id}
*/
bool GeradorArquivos::criar_arquivo_titulo(const char *nome_arq_dados, const char *nome_arq_titulo)
{
	ifstream arquivo_dados;
	ofstream arquivo_titulo;

	arquivo_dados.open(nome_arq_dados,ios::in | ios::binary);
	arquivo_titulo.open(nome_arq_titulo,ios::out | ios::binary);

	if (arquivo_dados.good() == false || arquivo_titulo.good() == false) { return false; }

	string registro;
	int quantidade_registros;

	// Escreve a quantidade de registros no cabeçalho do arquivo de títulos
	quantidade_registros = Manipulador::ler_inteiro(arquivo_dados);
	arquivo_titulo.write((char *) &quantidade_registros, sizeof(quantidade_registros));

	for (int i = 0; i < quantidade_registros; i++)
    {
		if(arquivo_dados.eof() == true)
		{
			cout 
				<< "Aviso: Bit EOF ativado durante criacao do arquivo de titulos. "
				<< "Quantidade de registros lido: " << i << " registro(s).\n";
			break;
		}

		registro = Manipulador::ler_registro(arquivo_dados);

		if (registro.empty()) continue;

		TituloNetflix tN(registro, campos_a_serem_usados);

		// Transforma o título e o id para apenas letras minúsculas
		for (int i = 0; i < TAM_TITULO+1 and tN.titulo[i] != '\0'; i++)
		{
			tN.titulo[i] = tolower(tN.titulo[i]);
		}

		for (int i = 0; i < TAM_ID+1 and tN.id[i] != '\0'; i++)
		{
			tN.id[i] = tolower(tN.id[i]);
		}

		RegistroTitulo rT;
		strcpy(rT.titulo, tN.titulo);
		strcpy(rT.id, tN.id);

		Manipulador::escrever_dados(arquivo_titulo, (void *) &rT, (int) sizeof(rT));
	}

	arquivo_dados.close();
	arquivo_titulo.close();

	if (arquivo_dados.bad() or arquivo_titulo.bad()) return false;

	return true;
}

bool GeradorArquivos::criar_arquivo_reinsercao_dados(const char *nome_arq_reinsercao_dados)
{
	fstream arquivo_reinsercao_dados;

	arquivo_reinsercao_dados.open(nome_arq_reinsercao_dados, ios::out | ios::binary);

	if(arquivo_reinsercao_dados.good()==false) { return false; }

	int quantidade_registros = 0;

	Manipulador::escrever_inteiro((ofstream&) arquivo_reinsercao_dados, quantidade_registros);

	arquivo_reinsercao_dados.close();

	if (arquivo_reinsercao_dados.bad()) return false;

	return true;
}