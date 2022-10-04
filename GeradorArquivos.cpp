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

bool GeradorArquivos::criar_arquivo_indice_primario(const char *nome_arq_inicial, const char *nome_arq_indice_primario)
{
    struct RegistroFormatado
    {
        char id[TAM_ID+1];
        int bytes_do_inicio;
    };

    ifstream arquivo_inicial;
    ofstream arquivo_indice_primario;
    Manipulador manipulador;
    TituloNetflix tN;

    /**
    * registro: string que recebe o registro lido do arquivo inicial
    * quantidade_registros: variável que recebe a quantidade de registros lida do arquivo inicial
    * tamanho_registro: variável que recebe o tamanho de um registro lido do arquivo inicial
    * byte_atual: guarda o byte a partir do início do arquivo inicial em que começa um registro
    */
    string registro;
    int quantidade_registros, tamanho_registro, byte_atual;

    arquivo_inicial.open(nome_arq_inicial, ios_base::in);
    arquivo_indice_primario.open(nome_arq_indice_primario, ios_base::out);

    // Lê-se a quantidade de registros do arquivo inicial
    quantidade_registros = manipulador.ler_inteiro(arquivo_inicial);
    byte_atual += sizeof(int);

    // Escreve-se a quantidade de registros no arquivo de índices
    manipulador.escrever_inteiro(arquivo_indice_primario, quantidade_registros);

    while(arquivo_inicial.eof() == false)
    {
        // Lê-se um registro do arquivo inicial
        registro = manipulador.ler_registro(arquivo_inicial);

        // Converte o registro lido (string) para TituloNetflix
        tN.string_para_titulo_netflix(registro);

        // Cria um registro formatado, que será salvo no arquivo de índices primário
        RegistroFormatado rF;
        strcpy(rF.id, tN.id);
        rF.bytes_do_inicio = byte_atual;

        manipulador.escrever_dados(arquivo_indice_primario, (void *) &rF, (int) sizeof(rF));

        // Atualiza o byte_atual para pular a quantidade de bytes lida do arquivo inicial
        // `sizeof(int)` que é o inteiro indicador do tamanho do registro
        // `registro.size()` que é o tamanho do registro
        byte_atual += sizeof(int) + (int) registro.size();
    }
}

bool GeradorArquivos::criar_arquivo_titulo(const char *nome_arq_inicial, const char *nome_arq_titulo){

	Manipulador manipulador;

	ifstream arquivo1;
	ofstream arquivo2;

	arquivo1.open(nome_arq_inicial,ios::in);
	arquivo2.open(nome_arq_titulo,ios::out);

	string linha;

	if(arquivo1.good()==false || arquivo2.good()==false){return false;}


	//para ler a primeira linha que será descartada
    linha = manipulador.ler_registro(arquivo1);

	while(arquivo1.eof()==false){

		linha = manipulador.ler_registro(arquivo1);

		TituloNetflix Titulo(linha);


		arquivo2<<Titulo.id;

		string auxiliar1(Titulo.id);


		int sobra1 = 6 - auxiliar1.length();
        for(int contador = 0;contador<sobra1;contador++){

            arquivo2<<" ";
		}


		arquivo2<<';';


		arquivo2<<Titulo.titulo;

		string auxiliar2(Titulo.titulo);

		int sobra2 = 105 - auxiliar2.length();
		for(int contador = 0;contador<sobra2;contador++){

            arquivo2<<" ";
		}


		arquivo2<<'\n';

	}

	return true;

}
