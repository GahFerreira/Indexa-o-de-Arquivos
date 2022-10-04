#include "TituloNetflix.h"

/**
* Retorna o conteúdo da lista em formato de string.
*/
string Lista::to_string() const
{
    string resultado;

    resultado += lista[0];

    for(int i = 1; i < (int) lista.size(); i++)
    {
        resultado += ", " + lista[i];
    }

    return resultado;
}

/**
* Construtor padrão de TituloNetflix.
*/
TituloNetflix::TituloNetflix()
{

}

/**
* Construtor de TituloNetflix a partir da string registro.
* Assume-se separação de campos por ';' e separação de subcampos por ','.
*
* registro: string que contém, idealmente, campos de TituloNetflix em formato de string.
*/
TituloNetflix::TituloNetflix(string registro)
{
    cout << "Chamou construtor\n";

    /**
     * id_campo: qual o campo atual do registro.
     * campo: string para ler de registro, um caractere por vez, até atingir o valor de um campo.
    */
    int id_campo = 0;
    string campo;

    // Itera-se pela string registro, extraindo os campos.
    // Lê-se um caractere de cada vez.
    // `auto` será substituindo por um iterador de string.
    for (auto i = registro.begin(); ; i++)
    {
        // Ao atingir o fim do registro, é necessário atribuir o valor da string `campo` a um campo de TituloNetflix.
        // Posteriormente, encerra-se o for.
        if (i == registro.end())
        {
            if (id_campo != 11)
            {
                cout << "Aviso: TituloNetflix criado a partir de string com campos faltando." << endl;
            }

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

            break;
        }

        // Se foi encontrado um ';', significa que `campo` tem que ser atribuído a um campo de TituloNetflix,
        // e seu valor zerado.
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

            // Ao achar um ';', o conteúdo de `campo` é assinalado a um campo de TituloNetflix,
            // e passamos ao próximo campo.
            id_campo++;

            // Aqui, verifica-se caso os caracteres após a vírgula são espaços
            // Enquanto forem espaços, pula aqueles caracteres
            // Faz isso até, no máximo, o fim do registro
            // Nota: pode ser que algum registro realmente use espaços, mas não está considerado aqui
            while (i+1 != registro.end())
            {
                if (*(i+1) == ' ') ++i;
                else break;
            }
        }

        // Em casos de subcampos, como elenco, que permite múltiplos atores, eles são separados por ','.
        // Nesse caso, salva-se cada ator individualmente dentro de uma lista.
        else if (*i == ',')
        {
            switch(id_campo)
            {
                case 3: this->diretor.lista.push_back(campo); break;
                case 4: this->elenco.lista.push_back(campo); break;
                case 5: this->pais.lista.push_back(campo); break;
                case 10: this->listado_em.lista.push_back(campo); break;

                // Se tiver ',' e não for um dos campos de lista,
                // significa que a ',' faz parte daquele campo
                // e não divide aquele campo em subcampos.
                // Ex.: ',' dentro de uma descrição (que não é lista).
                default: continue;
            }

            campo.clear();

            while (i+1 != registro.end())
            {
                if (*(i+1) == ' ') ++i;
                else break;
            }
        }

        // Caso não foi encontrado delimitador, acrescenta-se o caractere atual a `campo`.
        else campo += *i;
    }
}

/**
* Função que printa no console o conteúdo de um TituloNetlix.
*/
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
