#include "Gulosos.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <cstdlib>
#include <ctime>
#include <random>

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
        if (vizinhosCandidatos.empty()) {
          for (No* n : grafo->lista_adj) {
              if (!verticesDominados.count(n->id)) {
                  vizinhosCandidatos.insert(n->id);
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

       if (melhorPontuacao < 0) break;

      // Constrói a RCL com verificação proativa de conectividade
      vector<char> rcl;
      float limite = melhorPontuacao * (1.0f - fatorDeAleatoriedade);
      for (const auto& c : candidatosComPontuacao) {
          if (c.second >= limite) {
              vector<char> copia = conjuntoDominanteAtual;
              copia.push_back(c.first);
              if (VerificarConectividadeDoSubgrafo(copia)) {
                  rcl.push_back(c.first);
              }
          }
      }

      // Fallback: Se nenhum candidato que mantém a conectividade for bom o suficiente,
      // relaxamos a condição e pegamos qualquer candidato bom
      if (rcl.empty()) {
          for (const auto& c : candidatosComPontuacao) {
              if (c.second >= limite) rcl.push_back(c.first);
          }
      }

        if (!rcl.empty()) {
            int indiceAleatorio = rand() % rcl.size();
            char candidatoEscolhido = rcl[indiceAleatorio];
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

// ===================================================================================
// ALGORITMO GULOSO RANDOMIZADO ADAPTATIVO REATIVO
// ===================================================================================
std::vector<char> Gulosos::ExecutarGrarReativo(int numIteracoes, float limiteInferiorAleatoriedade, unsigned int semente) {
    std::vector<float> alfas = {0.1f, 0.3f, 0.5f};
    std::vector<float> probabilidades(alfas.size(), 1.0f / alfas.size());
    std::vector<float> desempenho(alfas.size(), 0.0f);

    int bloco = 30;
    int iteracao = 0;

    std::mt19937 gen(semente);

    int melhorTamanho = grafo->ordem + 1;
    std::vector<char> melhorSolucao;

    int melhorIndiceAlfaGlobal = -1;
    int iteracaoMelhorSolucaoGlobal = -1;

    while (iteracao < numIteracoes) {
        std::discrete_distribution<> dist(probabilidades.begin(), probabilidades.end());
        int indiceAlfa = dist(gen);
        float alfaEscolhido = alfas[indiceAlfa];

        std::vector<char> solucaoAtual = ConstruirSolucaoComAlfa(alfaEscolhido, gen);
        float desempenhoAtual = 1.0f / (float)solucaoAtual.size();
        desempenho[indiceAlfa] += desempenhoAtual;

        if ((int)solucaoAtual.size() < melhorTamanho) {
            melhorTamanho = (int)solucaoAtual.size();
            melhorSolucao = solucaoAtual;

            melhorIndiceAlfaGlobal = indiceAlfa;
            iteracaoMelhorSolucaoGlobal = iteracao + 1;
        }

        iteracao++;

        if (iteracao % bloco == 0 || iteracao == numIteracoes) {
            float somaDesempenho = std::accumulate(desempenho.begin(), desempenho.end(), 0.0f);
            if (somaDesempenho > 0.0f) {
                for (size_t i = 0; i < probabilidades.size(); i++) {
                    probabilidades[i] = desempenho[i] / somaDesempenho;
                    probabilidades[i] = std::max(probabilidades[i], limiteInferiorAleatoriedade);
                }

                float somaProb = std::accumulate(probabilidades.begin(), probabilidades.end(), 0.0f);
                for (float& p : probabilidades) p /= somaProb;
            }

            /*
            std::cout << "\n--- Resumo do bloco " << iteracao / bloco << " ---" << std::endl;
            std::cout << "Melhor alfa (indice): " << melhorIndiceAlfaGlobal << " (alfa = " << alfas[melhorIndiceAlfaGlobal] << ")" << std::endl;
            std::cout << "Melhor solucao encontrada na iteracao: " << iteracaoMelhorSolucaoGlobal << std::endl;
            std::cout << "Tamanho da melhor solucao: " << melhorTamanho << std::endl;
            std::cout << "Probabilidades atuais dos alfas:" << std::endl;
            for (size_t i = 0; i < alfas.size(); ++i) {
                std::cout << "  Alfa " << alfas[i] << ": " << probabilidades[i] * 100 << "%" << std::endl;
            } */

            std::fill(desempenho.begin(), desempenho.end(), 0.0f);
        }
    }
    std::cout << "Melhor tamanho encontrado: " << melhorTamanho << std::endl;
    return melhorSolucao;
}


std::vector<char> Gulosos::ConstruirSolucaoComAlfa(float alfa, std::mt19937& gen) {
    std::vector<char> conjuntoDominanteAtual;
    std::set<char> verticesDominados;

    if (grafo->ordem == 0) 
        return conjuntoDominanteAtual;
    
    int grauMax = 0;
    
    for (No* noAtual : grafo->lista_adj) {
        if ((int)noAtual->arestas.size() > grauMax) {
            grauMax = (int)noAtual->arestas.size();
        }
    }

    std::vector<No*> candidatos;
    for (No* noAtual : grafo->lista_adj) {
        if ((int)noAtual->arestas.size() == grauMax) {
            candidatos.push_back(noAtual);
        }
    }

    if (candidatos.empty()) return {}; 

    std::uniform_int_distribution<> dist(0, candidatos.size() - 1);
    int escolhidoIdx = dist(gen);
    No* noInicial = candidatos[escolhidoIdx];
        
    conjuntoDominanteAtual.push_back(noInicial->id);
    verticesDominados.insert(noInicial->id);
    for (Aresta* a : noInicial->arestas) {
        verticesDominados.insert(a->id_no_alvo);
    }

    while ((int)verticesDominados.size() < grafo->ordem) {
        std::vector<std::pair<char, int>> candidatosComPontuacao;
        std::set<char> nosNaSolucao(conjuntoDominanteAtual.begin(), conjuntoDominanteAtual.end());
        std::set<char> vizinhosCandidatos;

        for (char idNoDaSolucao : conjuntoDominanteAtual) {
            No* noDaSolucao = grafo->getNo(idNoDaSolucao);
            if (!noDaSolucao) continue;
            for (Aresta* aresta : noDaSolucao->arestas) {
                if (nosNaSolucao.count(aresta->id_no_alvo) == 0) {
                    vizinhosCandidatos.insert(aresta->id_no_alvo);
                }
            }
        }

        if (vizinhosCandidatos.empty()) {
            for (No* n : grafo->lista_adj) {
                if (verticesDominados.count(n->id) == 0) {
                    vizinhosCandidatos.insert(n->id);
                    break;
                }
            }
        }

        if (vizinhosCandidatos.empty()) break;

        int melhorPontuacao = -1;
        for (char idCandidato : vizinhosCandidatos) {
            No* noCandidato = grafo->getNo(idCandidato);
            if (!noCandidato) continue;

            int pontuacao = 0;
            if (verticesDominados.count(noCandidato->id) == 0) pontuacao++;
            for (Aresta* a : noCandidato->arestas) {
                if (verticesDominados.count(a->id_no_alvo) == 0) pontuacao++;
            }
            candidatosComPontuacao.push_back({idCandidato, pontuacao});
            if (pontuacao > melhorPontuacao) melhorPontuacao = pontuacao;
        }

        if (melhorPontuacao < 0) break;

        std::vector<char> rcl;
        float limite = melhorPontuacao * (1.0f - alfa);
        
        for (const auto& c : candidatosComPontuacao) {
            if (c.second >= limite) {
                if (candidatoConectaAoConjunto(c.first, conjuntoDominanteAtual)) {
                    rcl.push_back(c.first);
                }
            }
        }

        if (rcl.empty()) break;
        
        std::uniform_int_distribution<> distRCL(0, rcl.size() - 1);
        char escolhido = rcl[distRCL(gen)];
        
        conjuntoDominanteAtual.push_back(escolhido);
        verticesDominados.insert(escolhido);
        No* noEscolhido = grafo->getNo(escolhido);
        if (noEscolhido) {
            for (Aresta* a : noEscolhido->arestas) {
                verticesDominados.insert(a->id_no_alvo);
            }
        }
    }
    return conjuntoDominanteAtual;
}


bool Gulosos::candidatoConectaAoConjunto(char candidato, const vector<char>& conjuntoDominanteAtual) {
    No* noCandidato = grafo->getNo(candidato);
    if (!noCandidato) return false;
    
    unordered_set<char> conjuntoSet(conjuntoDominanteAtual.begin(), conjuntoDominanteAtual.end());
    
    // verifica se candidato conecta com algum nó do conjunto
    for (Aresta* a : noCandidato->arestas) {
        if (conjuntoSet.count(a->id_no_alvo)) {
            return true;
        }
    }
    return false;
}



vector<char> Gulosos::ExecutarGraspPadrao(int numeroDeIteracoes, float fatorDeAleatoriedadeFixo, int sementeRandomica) {
    srand(sementeRandomica);
    vector<char> melhorSolucaoEncontrada;
    
    for (int i = 0; i < numeroDeIteracoes; ++i) {
        // O fator de aleatoriedade é sempre o mesmo, não muda.
        vector<char> solucao = ConstruirSolucaoUnicaComGRA(fatorDeAleatoriedadeFixo);

        if (!solucao.empty() && VerificarConectividadeDoSubgrafo(solucao)) {
            if (melhorSolucaoEncontrada.empty() || solucao.size() < melhorSolucaoEncontrada.size()) {
            melhorSolucaoEncontrada = solucao;
            cout << "Iteracao " << i << ": Nova melhor solucao conexa encontrada com tamanho " << melhorSolucaoEncontrada.size() << endl;
            }
        }
    }

    if (melhorSolucaoEncontrada.empty()) {
        cout << "AVISO: Nenhuma solução conexa foi encontrada!" << endl;
    }

    return melhorSolucaoEncontrada;
}