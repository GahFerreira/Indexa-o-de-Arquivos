#include "Manipulador.h"

bool Manipulador::checar_integridade_leitura(ifstream& arquivo)
{
	bool resposta = true;

	if (arquivo.fail() == true)
	{
		cout << "Operacao de leitura ativou bit fail.\n";
		resposta = false;
	}

	if (arquivo.bad() == true)
	{
		cout << "Operacao de leitura ativou bit bad.\n";
		resposta = false;
	}

	if (arquivo.eof() == true)
	{
		cout << "Operacao de leitura ativou bit eof.\n";
		resposta = false;
	}

	return resposta;
}

bool Manipulador::checar_integridade_escrita(ofstream& arquivo)
{
	bool resposta = true;

	if (arquivo.fail() == true)
	{
		cout << "Operacao de escrita ativou bit fail.\n";
		resposta = false;
	}

	if (arquivo.bad() == true)
	{
		cout << "Operacao de escrita ativou bit bad.\n";
		resposta = false;
	}

	if (arquivo.eof() == true)
	{
		cout << "Operacao de escrita ativou bit eof.\n";
		resposta = false;
	}

	return resposta;
}

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

	if (checar_integridade_leitura(arquivo) == false)
	{
		cout << "[ERRO] Operacao: Leitura de Inteiro.\n";
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
	string resultado;

	tamanho = ler_inteiro(arquivo);

	if (tamanho <= 0)
	{
		cout << "Aviso: Tentativa de leitura de registro de tamanho 0 ou invalido." << endl;
		resultado = "";
	}

	else
	{
		char *registro = new char[tamanho+1];

		try
		{
			arquivo.read((char *) registro, tamanho);

			registro[tamanho] = '\0';

			resultado = string(registro);
		}

		catch(const std::exception& e)
		{
			cerr << "Falha na leitura de registro com tamanho " << tamanho << "." << endl;

			resultado = "";
		}

		delete[] registro;
	}

	if (checar_integridade_leitura(arquivo) == false)
	{
		cout << "[ERRO] Operacao: Leitura de Registro.\n";
	}
	
	return resultado;
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
	}

	else
	{
		try
		{
			arquivo.read((char *) destino, tamanho);
		}
		catch(const std::exception& e)
		{
			cerr << "Falha na tentativa de leitura de dados de tamanho " << tamanho << "." << endl;
		}
	}

	if (checar_integridade_leitura(arquivo) == false)
	{
		cout << "[ERRO] Operacao: Leitura de Dados.\n";
	}
}

/**
 * Escreve um inteiro no arquivo.
*/
bool Manipulador::escrever_inteiro(ofstream& arquivo, int inteiro)
{
	bool resultado = true;

	try
	{
		arquivo.write(reinterpret_cast<const char *>(&inteiro), sizeof(inteiro));
	}
	catch(const std::exception& e)
	{
		cerr << "Erro: Falha na escrita do inteiro: " << inteiro << endl;
		resultado = false;
	}

	if (checar_integridade_escrita(arquivo) == false)
	{
		cout << "[ERRO] Operacao: Escrita de Inteiro.\n";
	}

    return resultado;
}

/**
 * Escreve uma string no arquivo.
*/
bool Manipulador::escrever_string(ofstream& arquivo, string str)
{
	bool resultado = true;

	if (str.empty())
	{
		cout << "Aviso: Escrita de string de tamanho 0 ou invalido." << endl;
		resultado = false;
	}

	else
	{
		try
		{
			arquivo.write((char *) str.c_str(), (int) str.size());
		}
		catch(const std::exception& e)
		{
			cerr << "Erro: Falha na escrita da string: " + str << endl;
			resultado = false;
		}
	}

	if (checar_integridade_escrita(arquivo) == false)
	{
		cout << "[ERRO] Operacao: Escrita de String.\n";
	}

	return resultado;
}

/**
 * Escreve `tamanho` caracteres no arquivo,
 * começando pelo início da string `str`.
*/
bool Manipulador::escrever_string(ofstream& arquivo, string str, int tamanho)
{
	bool resultado = true;

	if (tamanho <= 0)
	{
		cout << "Aviso: Escrita de string de tamanho 0 ou invalido." << endl;
		resultado = false;
	}

	else if (tamanho < (int) str.size())
	{
		cout << "Aviso: Escrita de string de tamanho definido menor que o tamanho real da string." << endl;
	}

	else
	{
		try
		{
			arquivo.write((char *) str.c_str(), tamanho);
		}
		catch(const std::exception& e)
		{
			cout << "Erro: Falha na escrita da string: " + str << endl;
			resultado = false;
		}
	}

	if (checar_integridade_escrita(arquivo) == false)
	{
		cout << "[ERRO] Operacao: Escrita de String.\n";
	}

	return resultado;
}

/**
 * Escreve dados provindos da variável `dados`, 
 * de tamanho indicado por `tamanho` no arquivo.
*/
bool Manipulador::escrever_dados(ofstream& arquivo, void *dados, int tamanho)
{
	bool resultado = true;

    if (tamanho <= 0)
	{
		cout << "Erro: Escrita de dados de tamanho 0 ou invalido." << endl;
		resultado = false;
	}

	else
	{
		try
		{
			arquivo.write((char *) dados, tamanho);
		}
		catch(const std::exception& e)
		{
			cout << "Erro: Falha na escrita de dados de tamanho " << tamanho << "." << endl;
			resultado = false;
		}
	}

	if (checar_integridade_escrita(arquivo) == false)
	{
		cout << "[ERRO] Operacao: Escrita de Dados.\n";
	}

	return resultado;
}