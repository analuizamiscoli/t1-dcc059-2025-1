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

    // --- ALGORITMO GULOSO SIMPLES ---
    // Executa o algoritmo Guloso Simples (determinístico).
    std::vector<char> ExecutarGulosoPuro();

    // --- ALGORITMO GULOSO RANDOMIZADO ADAPTATIVO (GRA) ---
    // Executa o algoritmo GRA completo por N iterações.
    std::vector<char> ExecutarGraParaDominanteConectado(
        int numeroDeIteracoes, 
        float fatorDeAleatoriedadeInicial, 
        int sementeRandomica
    );

private:
    Grafo* grafo; // Ponteiro para o grafo original

    // Constrói uma única solução candidata com a lógica do GRA.
    std::vector<char> ConstruirSolucaoUnicaComGRA(float fatorDeAleatoriedade);

    // Verifica se um subconjunto de nós é conexo.
    bool VerificarConectividadeDoSubgrafo(const std::vector<char>& conjuntoDeNos);
};

#endif //GULOSOS_H