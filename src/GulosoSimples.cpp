#include "GulosoSimples.h"
#include <algorithm>

std::vector<char> GulosoSimples::encontrarConjuntoDominanteConectado(Grafo& grafo) {
    std::vector<No*> nos = grafo.lista_adj;
    std::unordered_map<char, No*> mapa;
    for (No* no : nos) {
        mapa[no->id] = no;
    }

    std::unordered_set<char> dominados;
    std::unordered_set<char> conjuntoD;

    // Etapa 1: Escolher o nó de maior grau inicial
    No* inicial = nullptr;
    int maxGrau = -1;
    for (No* no : nos) {
        if (no->arestas.size() > static_cast<size_t>(maxGrau)) {
            maxGrau = no->arestas.size();
            inicial = no;
        }
    }

    if (!inicial) return {};

    conjuntoD.insert(inicial->id);
    dominados.insert(inicial->id);
    for (Aresta* a : inicial->arestas) {
        dominados.insert(a->id_no_alvo);
    }

    // Etapa 2: Expansão gulosa
    while (dominados.size() < nos.size()) {
        char melhorNo = 0;
        int maxNovosDominados = -1;

        // Encontra a fronteira (vizinhos de D não em D)
        std::unordered_set<char> fronteira;
        for (char idD : conjuntoD) {
            for (Aresta* a : mapa[idD]->arestas) {
                char vizinho = a->id_no_alvo;
                if (!conjuntoD.count(vizinho)) {
                    fronteira.insert(vizinho);
                }
            }
        }

        // Seleciona o nó da fronteira que domina mais novos nós
        for (char candidato : fronteira) {
            int novos = 0;
            if (!dominados.count(candidato)) novos++;
            for (Aresta* a : mapa[candidato]->arestas) {
                if (!dominados.count(a->id_no_alvo)) novos++;
            }

            if (novos > maxNovosDominados) {
                melhorNo = candidato;
                maxNovosDominados = novos;
            }
        }

        if (melhorNo == 0) break;  // Nenhum nó válido encontrado

        // Atualiza os conjuntos
        conjuntoD.insert(melhorNo);
        dominados.insert(melhorNo);
        for (Aresta* a : mapa[melhorNo]->arestas) {
            dominados.insert(a->id_no_alvo);
        }
    }

    return std::vector<char>(conjuntoD.begin(), conjuntoD.end());
}