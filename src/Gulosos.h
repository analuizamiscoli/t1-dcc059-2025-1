#ifndef GULOSOS_H
#define GULOSOS_H

#include "Grafo.h"
#include <vector>
#include <string>

using namespace std;

class Gulosos {
public:
    // Construtor da classe.
    Gulosos(Grafo* grafo);
    // Destrutor da classe.
    ~Gulosos();

    // Executa o algoritmo GRA completo por N iterações.
    vector<char> ExecutarGraParaDominanteConectado(
        int numeroDeIteracoes, 
        float fatorDeAleatoriedadeInicial, 
        int sementeRandomica
    );

private:
    Grafo* grafo; // Ponteiro para o grafo original

    // Constrói uma única solução candidata com a lógica do GRA.
    vector<char> ConstruirSolucaoUnicaComGRA(float fatorDeAleatoriedade);

    // Verifica se um subconjunto de nós é conexo.
    bool VerificarConectividadeDoSubgrafo(const vector<char>& conjuntoDeNos);
};

#endif //GULOSOS_H