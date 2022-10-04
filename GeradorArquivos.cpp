#include "GeradorArquivos.h"

/**
 * Função com objetivo de, a partir de um arquivo de entrada
 * que contém 1 registro por linha, ler todos os registros do arquivo
 * e gerar um novo arquivo, denominado arquivo_inicial,
 * que contém os mesmos registros com o seu tamanho escrito antes.
 *
 * nome_arq_entrada: nome do arquivo de entrada
 * nome_arq_inicial: nome do arquivo inicial (com registros e seu respectivo tamanho antes)
*/
bool GeradorArquivos::criar_arquivo_inicial(const char *nome_arq_entrada, const char *nome_arq_inicial)
{
	ifstream arquivo_entrada;
    ofstream arquivo_inicial;
	string registro;
	int tamanho_registro, quantidade_registros = 0;

	arquivo_entrada.open(nome_arq_entrada, ios_base::in);
	arquivo_inicial.open(nome_arq_inicial, ios_base::out | ios_base::binary);

	// Checa a abertura dos arquivos
	if (arquivo_entrada.fail() == true or arquivo_inicial.fail() == true)
	{
		if (arquivo_entrada.fail() == true)
		{
			cout << "Erro na abertura de arquivo " + string(nome_arq_entrada) << endl;
		}

		else arquivo_entrada.close();

		if (arquivo_inicial.fail() == true)
		{
			cout << "Erro na abertura de arquivo " + string(nome_arq_inicial) << endl;
		}

		else arquivo_inicial.close();

		return false;
	}

	// Pula a primeira linha, que só contem os nomes das colunas
	// Isso interfere na criação posterior dos outros arquivos
	std::getline(arquivo_entrada, registro);

	// Guarda os primeiros bytes do arquivo para salvar a quantidade de registros
	arquivo_inicial.seekp(sizeof(quantidade_registros), ios_base::beg);

	// Lê registros até não achar mais registros no arquivo (EOF)
	while (arquivo_entrada.eof() == false)
	{
		std::getline(arquivo_entrada, registro);

		// Se algum registro for lido com tamanho 0, passa para o próximo
		// Idealmente isso só ocorrerá no final do arquivo
		// Mas também ajuda com problemas de leitura de linhas vazias no meio do arquivo
		if (registro.size() <= 0)
        {
            continue;
        }

		quantidade_registros++;

		registro += '\n';

		tamanho_registro = (int) registro.size();

		arquivo_inicial.write((const char *) &tamanho_registro, sizeof(tamanho_registro));

		arquivo_inicial.write((const char *) registro.c_str(), tamanho_registro);
	}

	// Volta para o início do arquivo
	arquivo_inicial.seekp(0, ios_base::beg);

	// Escreve a quantidade de registros no início (cabeçalho) do arquivo
	arquivo_inicial.write((const char *) &quantidade_registros, sizeof(quantidade_registros));

	arquivo_entrada.close();
	arquivo_inicial.close();

	// Checa se houve algum erro ocorreu, diferente de EOF / FAIL
	// EOF / FAIL acontecem ao tentar ler no final do arquivo
	if (arquivo_entrada.bad() or arquivo_inicial.bad()) return false;

	return true;
}

/**
 * Função com objetivo de criar o arquivo de índices direto.
 * O arquivo começa com um cabeçalho indicando quantos registros guarda.
 * Cada registro tem tamanho fixo, e dois campos: {id, posição_arquivo_inicial} 
*/
bool GeradorArquivos::criar_arquivo_indice_primario(const char *nome_arq_inicial, const char *nome_arq_indice_primario)
{
    ifstream arquivo_inicial;
    ofstream arquivo_indice_primario;
    Manipulador manipulador;

    /**
    * registro: string que recebe o registro lido do arquivo inicial
    * quantidade_registros: variável que recebe a quantidade de registros lida do arquivo inicial
    * tamanho_registro: variável que recebe o tamanho de um registro lido do arquivo inicial
    * byte_atual: guarda o byte a partir do início do arquivo inicial em que começa um registro
    */
    string registro;
    int quantidade_registros, byte_atual = 0;

    arquivo_inicial.open(nome_arq_inicial, ios_base::in | ios_base::binary);
    arquivo_indice_primario.open(nome_arq_indice_primario, ios_base::out | ios_base::binary);

    // Checa a abertura dos arquivos
	if (arquivo_inicial.fail() == true or arquivo_indice_primario.fail() == true)
	{
		if (arquivo_inicial.fail() == true)
		{
			cout << "Erro na abertura de arquivo " + string(nome_arq_inicial) << endl;
		}

		else arquivo_inicial.close();

		if (arquivo_indice_primario.fail() == true)
		{
			cout << "Erro na abertura de arquivo " + string(nome_arq_indice_primario) << endl;
		}

		else arquivo_indice_primario.close();

		return false;
	}

    // Lê-se a quantidade de registros do arquivo inicial
    quantidade_registros = manipulador.ler_inteiro(arquivo_inicial);
    byte_atual += sizeof(int);

    // Escreve-se a quantidade de registros no arquivo de índices
    manipulador.escrever_inteiro(arquivo_indice_primario, quantidade_registros);

    while(arquivo_inicial.eof() == false)
    {
        // Lê-se um registro do arquivo inicial
        registro = manipulador.ler_registro(arquivo_inicial);

		// Caso haja alguma linha vazia, pula ela
        if (registro.size() <= 0)
        {
            continue;
        }

        // Converte o registro lido (string) para TituloNetflix
        TituloNetflix tN(registro);

		// Transforma o `id` para apenas minúsculas
		for (int i = 0; i < TAM_ID+1 and tN.id[i] != '\0'; i++)
		{
			tN.id[i] = tolower(tN.id[i]);
		}

        // Cria um registro formatado, que será salvo no arquivo de índices direto
        RegistroIndice rI;
        strcpy(rI.id, tN.id);
        rI.bytes_do_inicio = byte_atual;

        manipulador.escrever_dados(arquivo_indice_primario, (void *) &rI, (int) sizeof(rI));

        // Atualiza o byte_atual para pular a quantidade de bytes lida do arquivo inicial
        // `sizeof(int)` que é o inteiro indicador do tamanho do registro
        // `registro.size()` que é o tamanho do registro
        byte_atual += sizeof(int) + (int) registro.size();
    }

    arquivo_inicial.close();
	arquivo_indice_primario.close();

    // Checa se houve algum erro ocorreu, diferente de EOF / FAIL
	// EOF / FAIL acontecem ao tentar ler no final do arquivo
	if (arquivo_inicial.bad() or arquivo_indice_primario.bad()) return false;

	return true;
}

/**
 * Função que tem como objetivo criar um arquivo de índices indiretos.
 * O arquivo começa com um cabeçalho indicando quantos registros guarda.
 * Cada registro tem tamanho fixo, e dois campos: {titulo, id} 
*/
bool GeradorArquivos::criar_arquivo_titulo(const char *nome_arq_inicial, const char *nome_arq_titulo)
{
	Manipulador manipulador;

	ifstream arquivo_inicial;
	ofstream arquivo_titulo;

	arquivo_inicial.open(nome_arq_inicial,ios::in | ios::binary);
	arquivo_titulo.open(nome_arq_titulo,ios::out | ios::binary);

	if(arquivo_inicial.good()==false || arquivo_titulo.good()==false){return false;}

	string registro;
	int quantidade_registros;

	// Escreve a quantidade de registros no cabeçalho do arquivo de títulos
	quantidade_registros = manipulador.ler_inteiro(arquivo_inicial);
	arquivo_titulo.write((char *) &quantidade_registros, sizeof(quantidade_registros));

	while(arquivo_inicial.eof()==false)
	{
		registro = manipulador.ler_registro(arquivo_inicial);

		if (registro.size() <= 0) continue;

		TituloNetflix tN(registro);

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

		manipulador.escrever_dados(arquivo_titulo, (void *) &rT, (int) sizeof(rT));
	}

	arquivo_inicial.close();
	arquivo_titulo.close();

	if (arquivo_inicial.bad() or arquivo_titulo.bad()) return false;

	return true;
}