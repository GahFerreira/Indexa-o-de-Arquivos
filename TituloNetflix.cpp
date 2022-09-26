#include "TituloNetflix.h"
#include <cstring>

char *Lista::to_string() const
{
    string str_resultado;

    str_resultado += lista[0];

    for(int i = 1; i < quantidade; i++)
    {
        str_resultado+= ", " + lista[i];
    }

    char *resultado = new char[str_resultado.size() + 1];

    strcpy(resultado, str_resultado.c_str());

    return resultado;
}

void TituloNetflix::Print()
{

}
