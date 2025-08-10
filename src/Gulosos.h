#ifndef GULOSOS_H
#define GULOSOS_H

#include "Grafo.h"
#include <vector>
#include <string>
// Includes necessários para as implementações no .cpp
#include <unordered_set>
#include <unordered_map>

class Gulosos {
public:
    // Construtor da classe.
    Gulosos(Grafo* grafo);
    // Destrutor da classe.
    ~Gulosos();

    // a) Algoritmo Guloso Simples (case 'i').
    std::vector<char> ExecutarGulosoSimples();

    // b) Algoritmo Guloso Randomizado Adaptativo (case 'j')
    std::vector<char> ExecutarGraspPadrao(
        int numeroDeIteracoes, 
        float fatorDeAleatoriedadeFixo, 
        int sementeRandomica
    );

     // c) Espaço para o Algoritmo Reativo (case 'k')

private:
    Grafo* grafo; // Ponteiro para o grafo original

    // Constrói uma única solução candidata com a lógica do GRA.
    vector<char> ConstruirSolucaoUnicaComGRA(float fatorDeAleatoriedade);

    // Verifica se um subconjunto de nós é conexo.
    bool VerificarConectividadeDoSubgrafo(const vector<char>& conjuntoDeNos);
};

#endif //GULOSOS_H