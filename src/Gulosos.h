#ifndef GULOSOS_H
#define GULOSOS_H

#include "Grafo.h"
#include <vector>
#include <string>
// Includes necessários para as implementações no .cpp
#include <unordered_set>
#include <unordered_map>
#include <random>


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
    /*std::vector<char> ConstruirSolucaoUnicaComGRAReativo(
    float fatorDeAleatoriedadeInicial,
    float limiteInferiorAleatoriedade,
    unsigned int semente
    ); */

    std::vector<char> ExecutarGrarReativo(int numIteracoes, float limiteInferiorAleatoriedade, unsigned int semente);

private:
    Grafo* grafo; // Ponteiro para o grafo original

    // Constrói uma única solução candidata com a lógica do GRA.
    vector<char> ConstruirSolucaoUnicaComGRA(float fatorDeAleatoriedade);

    std::vector<char> ConstruirSolucaoComAlfa(float alfa, std::mt19937& gen);

    // Verifica se um subconjunto de nós é conexo.
    bool VerificarConectividadeDoSubgrafo(const vector<char>& conjuntoDeNos);

    // Função auxiliar para construir a RCL (Lista de Candidatos Restrita)
    bool candidatoConectaAoConjunto(char candidato, const vector<char>& conjuntoDominanteAtual);
};

#endif //GULOSOS_H