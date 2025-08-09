#include "Gulosos.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <cstdlib>
#include <ctime>

using namespace std;

// Construtor da classe.
Gulosos::Gulosos(Grafo* grafo) {
    this->grafo = grafo;
}

// Destrutor da classe.
Gulosos::~Gulosos() {}

// ===================================================================================
// ALGORITMO 1: GULOSO SIMPLES (DETERMINÍSTICO)
// ===================================================================================
std::vector<char> Gulosos::ExecutarGulosoPuro() {
    std::vector<No*> nosDoGrafo = this->grafo->lista_adj;

    if (nosDoGrafo.empty()) {
        return {}; // Retorna um vetor vazio se o grafo não tiver nós.
    }

    std::unordered_map<char, No*> mapaDeNos;
    for (No* no : nosDoGrafo) {
        mapaDeNos[no->id] = no;
    }

    std::unordered_set<char> conjuntoDominante;
    std::unordered_set<char> nosDominados;

    // Etapa 1: Escolher o nó de maior grau como ponto de partida.
    No* noInicial = nosDoGrafo[0];
    for (size_t i = 1; i < nosDoGrafo.size(); ++i) {
        if (nosDoGrafo[i]->arestas.size() > noInicial->arestas.size()) {
            noInicial = nosDoGrafo[i];
        }
    }

    conjuntoDominante.insert(noInicial->id);
    nosDominados.insert(noInicial->id);
    for (Aresta* aresta : noInicial->arestas) {
        nosDominados.insert(aresta->id_no_alvo);
    }

    // Etapa 2: Expansão gulosa até a solução ser completa.
    while (nosDominados.size() < nosDoGrafo.size()) {
        char melhorNoCandidato = 0;
        int maxNovosDominados = -1;

        std::unordered_set<char> fronteira;
        for (char idNoDaSolucao : conjuntoDominante) {
            for (Aresta* aresta : mapaDeNos[idNoDaSolucao]->arestas) {
                char idVizinho = aresta->id_no_alvo;
                if (conjuntoDominante.find(idVizinho) == conjuntoDominante.end()) {
                    fronteira.insert(idVizinho);
                }
            }
        }

        for (char idCandidato : fronteira) {
            int novosDominados = 0;
            if (nosDominados.find(idCandidato) == nosDominados.end()) {
                novosDominados++;
            }
            for (Aresta* aresta : mapaDeNos[idCandidato]->arestas) {
                if (nosDominados.find(aresta->id_no_alvo) == nosDominados.end()) {
                    novosDominados++;
                }
            }
            if (novosDominados > maxNovosDominados) {
                melhorNoCandidato = idCandidato;
                maxNovosDominados = novosDominados;
            }
        }

        if (melhorNoCandidato == 0) break;

        conjuntoDominante.insert(melhorNoCandidato);
        nosDominados.insert(melhorNoCandidato);
        for (Aresta* aresta : mapaDeNos[melhorNoCandidato]->arestas) {
            nosDominados.insert(aresta->id_no_alvo);
        }
    }
    
    return std::vector<char>(conjuntoDominante.begin(), conjuntoDominante.end());
}


// ===================================================================================
// ALGORITMO 2: GULOSO RANDOMIZADO ADAPTATIVO (GRA)
// ===================================================================================

// Verifica se um subconjunto de nós é conexo.
bool Gulosos::VerificarConectividadeDoSubgrafo(const vector<char>& conjuntoDeNos) {
    if (conjuntoDeNos.empty() || conjuntoDeNos.size() == 1) {
        return true;
    }
    set<char> verticesDoConjunto(conjuntoDeNos.begin(), conjuntoDeNos.end());
    map<char, bool> mapaDeVisitados;
    vector<char> pilhaParaBusca;
    for(char idNo : conjuntoDeNos) {
        mapaDeVisitados[idNo] = false;
    }
    pilhaParaBusca.push_back(conjuntoDeNos[0]);
    mapaDeVisitados[conjuntoDeNos[0]] = true;
    int contadorDeVisitados = 1;
    while(!pilhaParaBusca.empty()){
        char idNoAtual = pilhaParaBusca.back();
        pilhaParaBusca.pop_back();
        No* ponteiroNoAtual = grafo->getNo(idNoAtual);
        if(!ponteiroNoAtual) continue;
        for(Aresta* aresta : ponteiroNoAtual->arestas){
            char idNoVizinho = aresta->id_no_alvo;
            if(verticesDoConjunto.count(idNoVizinho) && !mapaDeVisitados[idNoVizinho]){
                mapaDeVisitados[idNoVizinho] = true;
                pilhaParaBusca.push_back(idNoVizinho);
                contadorDeVisitados++;
            }
        }
    }
    return contadorDeVisitados == conjuntoDeNos.size();
}

// Constrói uma única solução candidata com a lógica do GRA.
vector<char> Gulosos::ConstruirSolucaoUnicaComGRA(float fatorDeAleatoriedade) {
    vector<char> conjuntoDominanteAtual;
    set<char> verticesDominados;
    if (grafo->ordem == 0) return conjuntoDominanteAtual;
    No* noInicial = nullptr;
    if (!grafo->lista_adj.empty()){
        noInicial = grafo->lista_adj[0];
        for(No* noAtual : grafo->lista_adj) {
            if(noAtual->arestas.size() > noInicial->arestas.size()){
                noInicial = noAtual;
            }
        }
    }
    if(noInicial == nullptr) return conjuntoDominanteAtual;
    conjuntoDominanteAtual.push_back(noInicial->id);
    verticesDominados.insert(noInicial->id);
    for(Aresta* aresta : noInicial->arestas) {
        verticesDominados.insert(aresta->id_no_alvo);
    }
    while (verticesDominados.size() < grafo->ordem) {
        vector<pair<char, int>> candidatosComPontuacao;
        int melhorPontuacao = -1;
        set<char> nosNaSolucao(conjuntoDominanteAtual.begin(), conjuntoDominanteAtual.end());
        set<char> vizinhosCandidatos;
        for(char idNoDaSolucao : conjuntoDominanteAtual){
            No* ponteiroNoDaSolucao = grafo->getNo(idNoDaSolucao);
            if (!ponteiroNoDaSolucao) continue;
            for(Aresta* aresta : ponteiroNoDaSolucao->arestas){
                if(nosNaSolucao.count(aresta->id_no_alvo) == 0){
                    vizinhosCandidatos.insert(aresta->id_no_alvo);
                }
            }
        }
        if(vizinhosCandidatos.empty() && verticesDominados.size() < grafo->ordem){
            for(No* noAtual : grafo->lista_adj){
                if(verticesDominados.find(noAtual->id) == verticesDominados.end()){
                    vizinhosCandidatos.insert(noAtual->id);
                    break; 
                }
            }
        }
        for (char idCandidato : vizinhosCandidatos) {
            No* ponteiroNoCandidato = grafo->getNo(idCandidato);
            if(!ponteiroNoCandidato) continue;
            int pontuacaoDoCandidato = 0;
            if (verticesDominados.find(ponteiroNoCandidato->id) == verticesDominados.end()) {
                pontuacaoDoCandidato++;
            }
            for (Aresta* aresta : ponteiroNoCandidato->arestas) {
                if (verticesDominados.find(aresta->id_no_alvo) == verticesDominados.end()) {
                    pontuacaoDoCandidato++;
                }
            }
            candidatosComPontuacao.push_back({idCandidato, pontuacaoDoCandidato});
            if (pontuacaoDoCandidato > melhorPontuacao) {
                melhorPontuacao = pontuacaoDoCandidato;
            }
        }
        vector<char> listaDeBonsCandidatos_RCL;
        float limiteDaPontuacao = melhorPontuacao * (1.0 - fatorDeAleatoriedade);
        for (const auto& candidato : candidatosComPontuacao) {
            if (candidato.second >= limiteDaPontuacao) {
                listaDeBonsCandidatos_RCL.push_back(candidato.first);
            }
        }
        if (!listaDeBonsCandidatos_RCL.empty()) {
            int indiceAleatorio = rand() % listaDeBonsCandidatos_RCL.size();
            char candidatoEscolhido = listaDeBonsCandidatos_RCL[indiceAleatorio];
            conjuntoDominanteAtual.push_back(candidatoEscolhido);
            verticesDominados.insert(candidatoEscolhido);
            No* noEscolhido = grafo->getNo(candidatoEscolhido);
            if(!noEscolhido) continue;
            for (Aresta* aresta : noEscolhido->arestas) {
                verticesDominados.insert(aresta->id_no_alvo);
            }
        } else {
            break;
        }
    }
    return conjuntoDominanteAtual;
}

// Executa o algoritmo GRA completo por N iterações.
vector<char> Gulosos::ExecutarGraParaDominanteConectado(int numeroDeIteracoes, float fatorDeAleatoriedadeInicial, int sementeRandomica) {
    srand(sementeRandomica);
    vector<char> melhorSolucaoEncontrada;
    float fatorDeAleatoriedadeAtual = fatorDeAleatoriedadeInicial; 
    int contadorDeIteracoesSemMelhora = 0;
    for (int i = 0; i < numeroDeIteracoes; ++i) {
        vector<char> solucaoGeradaNestaIteracao = ConstruirSolucaoUnicaComGRA(fatorDeAleatoriedadeAtual);
        if (melhorSolucaoEncontrada.empty() || solucaoGeradaNestaIteracao.size() < melhorSolucaoEncontrada.size()) {
            melhorSolucaoEncontrada = solucaoGeradaNestaIteracao;
            cout << "Iteracao " << i << ": Nova melhor solucao encontrada com tamanho " << melhorSolucaoEncontrada.size() << endl;
            contadorDeIteracoesSemMelhora = 0;
            fatorDeAleatoriedadeAtual = max(0.1f, fatorDeAleatoriedadeAtual - 0.1f);
        } else {
            contadorDeIteracoesSemMelhora++;
        }
        if (contadorDeIteracoesSemMelhora > (numeroDeIteracoes * 0.1)) { 
            fatorDeAleatoriedadeAtual = min(1.0f, fatorDeAleatoriedadeAtual + 0.1f);
            contadorDeIteracoesSemMelhora = 0;
        }
    }
    return melhorSolucaoEncontrada;
}