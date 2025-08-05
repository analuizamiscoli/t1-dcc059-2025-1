#ifndef GULOSO_SIMPLES_H
#define GULOSO_SIMPLES_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "Grafo.h" 

class GulosoSimples {
public:
    static std::vector<char> encontrarConjuntoDominanteConectado(Grafo& grafo);
};

#endif