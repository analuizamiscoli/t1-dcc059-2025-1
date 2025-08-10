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
vector<char> Gulosos::ExecutarGulosoSimples() {
    vector<No*> nos = this->grafo->lista_adj;
    if (nos.empty()) return {};

    unordered_map<char, No*> mapa;
    for (No* no : nos) {
        mapa[no->id] = no;
    }

    unordered_set<char> dominados;
    unordered_set<char> conjuntoD;

    // Etapa 1: Escolher o nó de maior grau inicial
    No* inicial = nullptr;
    int maxGrau = -1;
    for (No* no : nos) {
        if ((int)no->arestas.size() > maxGrau) {
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
    while (dominados.size() < (size_t)this->grafo->ordem) {
        char melhorNo = 0;
        int maxNovosDominados = -1;

        // Encontra a fronteira (vizinhos de D não em D)
        unordered_set<char> fronteira;
        for (char idD : conjuntoD) {
            No* noAtual = mapa[idD];
            if (!noAtual) continue;
            for (Aresta* a : noAtual->arestas) {
                if (conjuntoD.count(a->id_no_alvo) == 0) {
                    fronteira.insert(a->id_no_alvo);
                }
            }
        }

        if (fronteira.empty()) {
             bool encontrado = false;
             for(No* no : nos){
                 if(dominados.find(no->id) == dominados.end()){
                     fronteira.insert(no->id);
                     encontrado = true;
                     break;
                 }
             }
             if(!encontrado) break;
        }

        for (char candidato : fronteira) {
            int novos = 0;
            if (dominados.count(candidato) == 0) novos++;
            
            No* noCandidato = mapa[candidato];
            if (!noCandidato) continue;

            for (Aresta* a : noCandidato->arestas) {
                if (dominados.count(a->id_no_alvo) == 0) novos++;
            }

            if (novos > maxNovosDominados) {
                melhorNo = candidato;
                maxNovosDominados = novos;
            }
        }

        if (melhorNo == 0) break;

        conjuntoD.insert(melhorNo);
        dominados.insert(melhorNo);
        
        No* noAdicionado = mapa[melhorNo];
        if (!noAdicionado) continue;

        for (Aresta* a : noAdicionado->arestas) {
            dominados.insert(a->id_no_alvo);
        }
    }

    return vector<char>(conjuntoD.begin(), conjuntoD.end());
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

//

//


vector<char> Gulosos::ExecutarGraspPadrao(int numeroDeIteracoes, float fatorDeAleatoriedadeFixo, int sementeRandomica) {
    srand(sementeRandomica);
    vector<char> melhorSolucaoEncontrada;
    
    for (int i = 0; i < numeroDeIteracoes; ++i) {
        // O fator de aleatoriedade é sempre o mesmo, não muda.
        vector<char> solucaoGeradaNestaIteracao = ConstruirSolucaoUnicaComGRA(fatorDeAleatoriedadeFixo);
        
        if (melhorSolucaoEncontrada.empty() || solucaoGeradaNestaIteracao.size() < melhorSolucaoEncontrada.size()) {
            melhorSolucaoEncontrada = solucaoGeradaNestaIteracao;
            cout << "Iteracao " << i << ": Nova melhor solucao encontrada com tamanho " << melhorSolucaoEncontrada.size() << endl;
        }
    }
    return melhorSolucaoEncontrada;
}