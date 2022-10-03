#include "Manipulador.h"
#include <iostream>

Manipulador::Manipulador()
{
}

bool Manipulador::criar_arquivo_inicial(const char *nome_arq_entrada)
{
    fstream arq_entrada, arq_dados;

    arq_entrada.open(nome_arq_entrada, ios_base::in);
    arq_dados.open("dados.txt", ios_base::out);

    cout << "Abertura dos arquivos bem sucedida\n";

    TituloNetflix tn;
    string registro_como_string;
    int tam_registro;

    cout << "Inicio do processamento\n";

    while (true)
    {
        getline(arq_entrada, registro_como_string);

        cout << registro_como_string << '\n';

        if ((int) registro_como_string.size() <= 0) break;

        tam_registro = (int) registro_como_string.size();

        arq_dados.write((char *)(&tam_registro), sizeof(tam_registro));
        arq_dados.write((char *) registro_como_string.c_str(), sizeof((int) registro_como_string.size()));
    }

    cout << "Processamento bem sucedido\n";

    arq_entrada.close();
    arq_dados.close();

    cout << "Fechamento dos arquivos bem sucedido\n";

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



