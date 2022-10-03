#include "Manipulador.h"

/**
 * Função com objetivo de, a partir de um arquivo de entrada, que contém 1 registro por linha,
 * lê todos os registros do arquivo e gera um novo arquivo, denominado arquivo_inicial, que
 * contém os mesmos registros com o seu tamanho escrito antes.
 * 
 * nome_arq_entrada: nome do arquivo de entrada  
 * nome_arq_inicial: nome do arquivo inicial (com registros e seu respectivo tamanho antes)
*/
bool Manipulador::criar_arquivo_inicial(const char *nome_arq_entrada, const char *nome_arq_inicial)
{
	ifstream arquivo_entrada;
    ofstream arquivo_inicial;
	string registro;
	int tamanho_registro, quantidade_registros;

	arquivo_entrada.open(nome_arq_entrada, ios_base::in);
	arquivo_inicial.open(nome_arq_inicial, ios_base::out);

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

		if (registro.size() <= 0) break;

		quantidade_registros++;

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


