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
 * Transforma uma string, com conteúdo de um registro TituloNetflix
 * em um objeto TituloNetflix.
 *
 * A string possui os campos definidos em `campos`.
 * Se `campos` possui todos os campos, isso representa que todos os campos estão presentes.
*/
void TituloNetflix::string_para_titulo_netflix(string registro, vector<bool>& campos)
{
    /**
     * campo: string para ler de registro, um caractere por vez, até atingir o valor de um campo.
     * campos_registro: quais os campos a string `registro` possui.
     * id_campo_atual: qual o índice do campo atual do registro. Aponta inicialmente para a primeira posição de `campos_registro`.
    */
    string campo;
    vector<int> campos_registro;
    int id_campo_atual = 0;

    // Gera os campos que serão utilizados a partir da variável de entrada `campos`.
    for (int i_campo = 0; i_campo < QTD_CAMPOS; i_campo++)
    {
        if (campos[i_campo] == true) campos_registro.push_back(i_campo);
    }

    // Itera-se pela string registro, extraindo os campos.
    // Lê-se um caractere de cada vez.
    // `auto` será substituindo por um iterador de string.
    for (auto i = registro.begin(); ; i++)
    {
        // Caso a string passada para a conversão tenha mais caracteres que o esperado
        // a iteração é do `for` é finalizada assim que todos os campos definidos são
        // devidamente alocados.
        if (id_campo_atual >= (int) campos_registro.size())
        {
            cout << "Aviso: conversao de string para TituloNetflix com sucesso, mas possivelmente com campos a mais no final." << endl;

            break;
        }

        // Ao atingir o fim do registro, é necessário atribuir o valor da string `campo` a um campo de TituloNetflix.
        // Posteriormente, encerra-se o for.
        if (i == registro.end())
        {
            switch(campos_registro[id_campo_atual])
            {
                case 0: strcpy(this->id, campo.c_str()); break;
                case 1: strcpy(this->tipo, campo.c_str()); break;
                case 2: strcpy(this->titulo, campo.c_str()); break;
                case 3: this->diretor.lista.push_back(campo); break;
                case 4: this->elenco.lista.push_back(campo); break;
                case 5: this->pais.lista.push_back(campo); break;
                case 6: strcpy(this->data, campo.c_str()); break;
                case 7: 
                {
                    try
                    {
                        this->ano_lancamento = (short) stoi(campo);
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
                    }
                    break;
                }
                case 8: strcpy(this->avaliacao, campo.c_str()); break;
                case 9: strcpy(this->duracao, campo.c_str()); break;
                case 10: this->listado_em.lista.push_back(campo); break;
                case 11: strcpy(this->descricao, campo.c_str()); break;
            }

            break;
        }

        // Se foi encontrado um ';', significa que `campo` tem que ser atribuído a um campo de TituloNetflix,
        // e seu valor zerado.
        else if (*i == ';' and campos_registro[id_campo_atual] < 11)
        {
            switch(campos_registro[id_campo_atual])
            {
                case 0: strcpy(this->id, campo.c_str()); break;
                case 1: strcpy(this->tipo, campo.c_str()); break;
                case 2: strcpy(this->titulo, campo.c_str()); break;
                case 3: this->diretor.lista.push_back(campo); break;
                case 4: this->elenco.lista.push_back(campo); break;
                case 5: this->pais.lista.push_back(campo); break;
                case 6: strcpy(this->data, campo.c_str()); break;
                case 7:
                {
                    try
                    {
                        this->ano_lancamento = (short) stoi(campo);
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
                    }
                    break;
                }
                case 8: strcpy(this->avaliacao, campo.c_str()); break;
                case 9: strcpy(this->duracao, campo.c_str()); break;
                case 10: this->listado_em.lista.push_back(campo); break;
                case 11: strcpy(this->descricao, campo.c_str()); break;
            }

            campo.clear();

            // Ao achar um ';', o conteúdo de `campo` é assinalado a um campo de TituloNetflix,
            // e passamos ao próximo campo.
            id_campo_atual++;

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
            switch(campos_registro[id_campo_atual])
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
    // Como neste construtor todos os campos do registro serão utilizados,
    // criamos `campos` com QTD_CAMPOS posições, e todas as posições são `true`.
    // Ou seja, a string `registro` possui todos os campos de TituloNetflix.
    vector<bool> campos(QTD_CAMPOS, true);

    string_para_titulo_netflix(registro, campos);
}

/**
* Construtor de TituloNetflix a partir da string registro.
* Assume-se separação de campos por ';' e separação de subcampos por ','.
*
* registro: string que contém, idealmente, campos de TituloNetflix em formato de string.
* campos: um vetor de QTD_CAMPOS posições, em que cada uma possui `true` se a string `registro` tiver aquele campo, e `false` se não tiver.
*/
TituloNetflix::TituloNetflix(string registro, vector<bool>& campos)
{
    string_para_titulo_netflix(registro, campos);
}

/**
* Função que printa no console o conteúdo de um TituloNetlix.
*/
void TituloNetflix::print(vector<bool>& campos) const
{
    cout << "TituloNetflix:" << endl;

    for (int i_campo = 0; i_campo < QTD_CAMPOS; i_campo++)
    {
        if (campos[i_campo] == true)
        {
            switch(i_campo)
            {
                case ID_ID: cout << "\tId: " << id << endl; break;
                case TIPO_ID: cout << "\tTipo: " << tipo << endl; break;
                case TITULO_ID: cout << "\tTitulo: " << titulo << endl; break;
                case DIRETOR_ID: cout << "\tDiretor(es): " << diretor.to_string() << endl; break;
                case ELENCO_ID: cout << "\tElenco: " << elenco.to_string() << endl; break;
                case PAIS_ID: cout << "\tPais(es): " << pais.to_string() << endl; break;
                case DATA_ID: cout << "\tData: " << data << endl; break;
                case ANO_LANCAMENTO_ID: cout << "\tAno Lancamento: " << ano_lancamento << endl; break;
                case AVALIACAO_ID: cout << "\tAvaliacao: " << avaliacao << endl; break;
                case DURACAO_ID: cout << "\tDuracao: " << duracao << endl; break;
                case LISTADO_EM_ID: cout << "\tListado em: " << listado_em.to_string() << endl; break;
                case DESCRICAO_ID: cout << "\tDescricao: " << descricao << endl;; break;
            }
        }
    }
}

// Converte um TituloNetflix para uma string.
string TituloNetflix::to_string(vector<bool>& campos)
{
    string resposta;

    for (int i_campo = 0; i_campo < QTD_CAMPOS; i_campo++)
    {
        if (campos[i_campo] == true)
        {
            switch(i_campo)
            {
                case ID_ID: resposta += string(id) + ';'; break;
                case TIPO_ID: resposta += string(tipo) + ';'; break;
                case TITULO_ID: resposta += string(titulo) + ';'; break;
                case DIRETOR_ID: resposta += diretor.to_string() + ';'; break;
                case ELENCO_ID: resposta += elenco.to_string() + ';'; break;
                case PAIS_ID: resposta += pais.to_string() + ';'; break;
                case DATA_ID: resposta += string(data) + ';'; break;
                case ANO_LANCAMENTO_ID: resposta += std::to_string(ano_lancamento) + ';'; break;
                case AVALIACAO_ID: resposta += string(avaliacao) + ';'; break;
                case DURACAO_ID: resposta += string(duracao) + ';'; break;
                case LISTADO_EM_ID: resposta += listado_em.to_string() + ';'; break;
                case DESCRICAO_ID: resposta += string(descricao) + ';'; break;
            }
        }
    }

    return resposta;
}

// Converte o id para string e retorna a parte numérica do id.
int TituloNetflix::id_para_inteiro(string _id)
{
    try
    {
        return stoi(string(_id).substr(1));
    }
    catch (std::exception e)
    {
        cerr << "Erro: Conversão de id para inteiro invalida.";

        return -1;
    }
}
