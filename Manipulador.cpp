#include "Manipulador.h"

int Manipulador::ler_inteiro(ifstream& arquivo)
{
	int inteiro;

	arquivo.read((char *) &inteiro, sizeof(inteiro));

	return inteiro;
}

string Manipulador::ler_registro(ifstream& arquivo)
{
	int tamanho;

	tamanho = ler_inteiro(arquivo);

	if (tamanho <= 0)
	{
		cout << "Aviso: Tentativa de leitura de registro de tamanho 0." << endl;
		return "";
	}

	char *registro = new char[tamanho+1];

	arquivo.read((char *) registro, tamanho);

	registro[tamanho] = '\0';

	string registro_retorno = string(registro);

	delete[] registro;

	return registro_retorno;
}

bool Manipulador::escrever_dados(ofstream& arquivo, string dados)
{
	if (dados.size() <= 0)
	{
		cout << "Aviso: Escrita de dados de tamanho 0." << endl;
		return false;
	}

	arquivo.write((char *) dados.c_str(), (int) dados.size());

	if (arquivo.bad() == true)
	{
		cout << "Erro: Falha na escrita dos dados: " + dados << endl;
		return false;
	}

	return true;
}

bool Manipulador::escrever_dados(ofstream& arquivo, string dados, int tamanho)
{
	if (tamanho <= 0)
	{
		cout << "Erro: Escrita de dados de tamanho definido como 0 ou negativo." << endl;
		return false;
	}

	if (tamanho < dados.size())
	{
		cout << "Aviso: Escrita de dados de tamanho definido menor que o tamanho real dos dados." << endl;
	}

	arquivo.write((char *) dados.c_str(), tamanho);

	if (arquivo.bad() == true)
	{
		cout << "Erro: Falha na escrita dos dados: " + dados << endl;
		return false;
	}

	return true;
}

//IDEIA PRA FUNÇÃO

/*

TituloNetflix* Manipulador::gerar_array_de_titulos(const char *nome_arq_entrada){

	fstream arq_entrada;
	arq_entrada.open(nome_arq_entrada, ios_base::in);

	TituloNetflix titulos[8000];

	int contador_registro = 0;

	char c;

	while(!arq_entrada.eof()){

		while(c != '\n'){

			contador_registro++;

			string tam_registro ="";


			//supondo o tamanho máximo do campo que indica o tamanho igual a 6
			for(int contador=0; contador<6;contador++){

				c=arq_entrada.get();

				tam_registro += c;

			}

			//converter o valor do tamanho para int
			int tam_registro_int = stoi(tam_registro);


			//salva a informação do tamanho ocupado por aquele campo específico
			titulos[contador_registro].tamanho_registro = tam_registro_int;

			for(int contador = 0;contador < titulos[contador_registro].tamanho_registro;contador++){

				//CADA WHILE PEGA UM CAMPO E PASSA PARA O RESPECTIVO ATRIBUTO DE TituloNetflix

				while(c != ';'){

               	 			//primeiro campo...

					string auxiliar="";

					c=arq_entrada.get();

					if(c != ';'){
						auxiliar+= c;
					}



				}

				titulos[contador_registro].id = auxiliar;





				while(c != ';'){

                		//segundo campo...




				}

				while(c != ';'){

				//terceiro campo...


				}

				while(c != ';'){




				}

				while(c != ';'){




				}

				while(c != ';'){




				}

				while(c != ';'){




				}

				while(c != ';'){




				}

				while(c != ';'){




				}

				while(c != ';'){




				}

				while(c != ';'){




				}

				while(c != ';'){




				}



			}

		}

	}



}

*/

//OUTRAS FUNÇÕES

/*

bool Manipulador::criar_arquivo_indice(const char *nome_arq,TituloNetflix* TN ){

	fstream arq_saida;
	arq_saida.open(nome_arq,ios::out);

	int contador=0;
	int pos = 0;

	if (nome_arq.good()){

		while(!nome_arq.eof()){

			nome_arq<<TN.id;
			nome_arq<<";";
			nome_arq<<pos;
			nome_arq<<'\n';
			//6 são os bits salvos para guardar o tamanho
			pos=pos+TN[contador].tamanho_registro + 6;

		}

		nome_arq.close();

		return true;

	}

	else{return false;}

}


bool Manipulador::criar_arquivo_titulo(const char *nome_arq_entrada,TituloNetflix* TN ){


}


*/


