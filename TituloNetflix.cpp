#include "TituloNetflix.h"

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

void TituloNetflix::print() const
{
    cout
        << "TituloNetflix:" << endl
        << "\tId: " << id << endl
        << "\tTipo: " << tipo << endl
        << "\tTitulo: " << titulo << endl
        << "\tDiretor(es): " << diretor.to_string() << endl
        << "\tElenco: " << elenco.to_string() << endl
        << "\tPais(es): " << pais.to_string() << endl
        << "\tData: " << data << endl
        << "\tAno Lancamento: " << ano_lancamento << endl
        << "\tAvaliacao: " << avaliacao << endl
        << "\tDuracao: " << duracao << endl
        << "\tListado em: " << listado_em.to_string() << endl
        << "\tDescricao: " << descricao << endl;
}

//void TituloNetflix::obter_registro(const char *registro, int tamanho, char delimitador)
//{
//
//}