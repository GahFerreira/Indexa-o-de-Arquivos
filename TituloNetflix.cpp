#include "TituloNetflix.h"

char *Lista::to_string() const
{
    string str_resultado;

    str_resultado += lista[0];

    for(int i = 1; i < lista.size(); i++)
    {
        str_resultado+= ", " + lista[i];
    }

    char *resultado = new char[str_resultado.size() + 1];

    strcpy(resultado, str_resultado.c_str());

    return resultado;
}

TituloNetflix::TituloNetflix(string registro)
{
    cout << "Chamou construtor\n";

    /**
     * id_campo: qual o campo atual do registro
     * campo: string para ler de registro, um caractere por vez, até atingir o valor de um campo
    */
    int id_campo;
    string campo;

    for (auto i = registro.begin(); ; i++)
    {
        cout << "REPETE\n";

        if (*i == ',')
        {
            switch(id_campo)
            {
                case 3: this->diretor.lista.push_back(campo); break;
                case 4: this->elenco.lista.push_back(campo); break;
                case 5: this->pais.lista.push_back(campo); break;
                case 10: this->listado_em.lista.push_back(campo); break;
            }

            campo.clear();

            // Aqui, verifica-se caso os caracteres após a vírgula são espaços
            // Enquanto forem espaços, pula aqueles caracteres
            // Faz isso até, no máximo, o fim do registro
            // Nota: pode ser que algum registro realmente use espaços, mas não está considerado aqui
            while (i+1 != registro.end())
            {
                if (*(i+1) == ' ') ++i;
            }
        }

        else if (*i == ';')
        {
            switch(id_campo)
            {
                case 0: strcpy(this->id, campo.c_str()); break;
                case 1: strcpy(this->tipo, campo.c_str()); break;
                case 2: strcpy(this->titulo, campo.c_str()); break;
                case 3: this->diretor.lista.push_back(campo); break;
                case 4: this->elenco.lista.push_back(campo); break;
                case 5: this->pais.lista.push_back(campo); break;
                case 6: strcpy(this->data, campo.c_str()); break;
                case 7: this->ano_lancamento = (short) stoi(campo); break;
                case 8: strcpy(this->avaliacao, campo.c_str()); break;
                case 9: strcpy(this->duracao, campo.c_str()); break;
                case 10: this->listado_em.lista.push_back(campo); break;
                case 11: strcpy(this->descricao, campo.c_str()); break;
            }

            campo.clear();

            // Mesma lógica do `if` anterior
            while (i+1 != registro.end())
            {
                if (*(i+1) == ' ') ++i;
            }
        }

        else campo += *i;
    }
}

void TituloNetflix::print() const
{
    cout << "Chamada do print " << endl << endl;

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