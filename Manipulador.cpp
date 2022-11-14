#include "Manipulador.h"

/**
* Lê e retorna um inteiro de `arquivo`.
*
* arquivo: um arquivo que aceita operações de leitura.
*/
int Manipulador::ler_inteiro(ifstream& arquivo)
{
	// Inicializado para detecção de possíveis erros.
	int inteiro = -1;

	try
	{
		arquivo.read((char *) &inteiro, sizeof(inteiro));
	}
	catch(const std::exception& e)
	{
		cerr << "Falha na leitura de inteiro do arquivo.\n";
	}

	return inteiro;
}

/**
* Lê e retorna um registro do arquivo em formato de string.
* Primeiramente, lê o tamanho do registro, e depois lê a quantidade de bytes indicada por tamanho.
*/
string Manipulador::ler_registro(ifstream& arquivo)
{
	// Inicializado para detecção de possíveis erros.
	int tamanho = -1;

	tamanho = ler_inteiro(arquivo);

	if (tamanho <= 0)
	{
		cout << "Aviso: Tentativa de leitura de registro de tamanho 0 ou invalido." << endl;
		return "";
	}

	char *registro = new char[tamanho+1];

	try
	{
		arquivo.read((char *) registro, tamanho);
	}
	catch(const std::exception& e)
	{
		cerr << "Falha na leitura de registro com tamanho " << tamanho << "." << endl;
		delete[] registro;

		return "";
	}

	registro[tamanho] = '\0';

	string registro_retorno = string(registro);

    delete[] registro;

	return registro_retorno;
}

/**
 * Lê dados de um arquivo, de tamanho indicado por `tamanho`.
 * Os dados são salvos na variável `destino`. 
*/
void Manipulador::ler_dados(ifstream& arquivo, int tamanho, void *destino)
{
	if (tamanho <= 0)
	{
		cout << "Aviso: Tentativa de leitura de dados de tamanho 0 ou invalido." << endl;
		return;
	}

	try
	{
		arquivo.read((char *) destino, tamanho);
	}
	catch(const std::exception& e)
	{
		cerr << "Falha na tentativa de leitura de dados de tamanho " << tamanho << "." << endl;
	}
}

/**
 * Escreve um inteiro no arquivo.
*/
bool Manipulador::escrever_inteiro(ofstream& arquivo, int inteiro)
{
	try
	{
		arquivo.write((char *) &inteiro, sizeof(int));
	}
	catch(const std::exception& e)
	{
		cerr << "Erro: Falha na escrita do inteiro: " << inteiro << endl;
		return false;
	}

    return true;
}

/**
 * Escreve uma string no arquivo.
*/
bool Manipulador::escrever_string(ofstream& arquivo, string str)
{
	if (str.size() <= 0)
	{
		cout << "Aviso: Escrita de string de tamanho 0 ou invalido." << endl;
		return false;
	}

	try
	{
		arquivo.write((char *) str.c_str(), (int) str.size());
	}
	catch(const std::exception& e)
	{
		cerr << "Erro: Falha na escrita da string: " + str << endl;
		return false;
	}

	return true;
}

/**
 * Escreve `tamanho` caracteres no arquivo,
 * começando pelo início da string `str`.
*/
bool Manipulador::escrever_string(ofstream& arquivo, string str, int tamanho)
{
	if (tamanho <= 0)
	{
		cout << "Aviso: Escrita de string de tamanho 0 ou invalido." << endl;
		return false;
	}

	if (tamanho < (int) str.size())
	{
		cout << "Aviso: Escrita de string de tamanho definido menor que o tamanho real da string." << endl;
	}

	try
	{
		arquivo.write((char *) str.c_str(), tamanho);
	}
	catch(const std::exception& e)
	{
		cout << "Erro: Falha na escrita da string: " + str << endl;
		return false;
	}

	return true;
}

/**
 * Escreve dados provindos da variável `dados`, 
 * de tamanho indicado por `tamanho` no arquivo.
*/
bool Manipulador::escrever_dados(ofstream& arquivo, void *dados, int tamanho)
{
    if (tamanho <= 0)
	{
		cout << "Erro: Escrita de dados de tamanho 0 ou invalido." << endl;
		return false;
	}

	try
	{
		arquivo.write((char *) dados, tamanho);
	}
	catch(const std::exception& e)
	{
		cout << "Erro: Falha na escrita de dados de tamanho " << tamanho << "." << endl;
		return false;
	}

	return true;
}