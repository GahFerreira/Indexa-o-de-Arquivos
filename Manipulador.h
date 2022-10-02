#ifndef MANIPULADOR_H_
#define MANIPULADOR_H_

#include <fstream>
#include "TituloNetflix.h"

using namespace std;

class Manipulador
{
    public:
        char delimitador = '\0';
        string dados;
        int prox = 0;

        Manipulador();

        bool criar_arquivo_inicial(const char *nome_arq_entrada);
    
    
    
        //criar essas funções depois
    /*
        
        bool criar_arquivo_indice(const char *nome_arq_entrada,TituloNetflix* TN );
        bool criar_arquivo_titulo(const char *nome_arq_entrada,TituloNetflix* TN );
        //RETORNA O O QUE SERÁ PASSADO COMO TN
        TituloNetflix* gerar_array_de_titulos(const char *nome_arq_entrada);
        
    */
    
};

#endif // MANIPULADOR_H
