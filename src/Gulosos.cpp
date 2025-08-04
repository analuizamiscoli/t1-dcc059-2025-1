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

// Verifica se um subconjunto de nós é conexo.
bool Gulosos::VerificarConectividadeDoSubgrafo(const vector<char>& conjuntoDeNos) {
    if (conjuntoDeNos.empty() || conjuntoDeNos.size() == 1) {
        return true;
    }

    // Usa um 'set' para buscas rápidas de 'pertence ao conjunto'.
    set<char> verticesDoConjunto(conjuntoDeNos.begin(), conjuntoDeNos.end());
    map<char, bool> mapaDeVisitados;
    vector<char> pilhaParaBusca;
    
    // Inicializa o mapa de visitação para o DFS.
    for(char idNo : conjuntoDeNos) {
        mapaDeVisitados[idNo] = false;
    }

    // Começa a busca em profundidade a partir do primeiro nó do conjunto.
    pilhaParaBusca.push_back(conjuntoDeNos[0]);
    mapaDeVisitados[conjuntoDeNos[0]] = true;
    int contadorDeVisitados = 1;

    // Loop principal da busca em profundidade (DFS).
    while(!pilhaParaBusca.empty()){
        char idNoAtual = pilhaParaBusca.back();
        pilhaParaBusca.pop_back();

        No* ponteiroNoAtual = grafo->getNo(idNoAtual);
        if(!ponteiroNoAtual) continue;

        for(Aresta* aresta : ponteiroNoAtual->arestas){
            char idNoVizinho = aresta->id_no_alvo;
            
            // A condição de avanço do DFS: o vizinho deve estar no conjunto e não pode ter sido visitado ainda.
            if(verticesDoConjunto.count(idNoVizinho) && !mapaDeVisitados[idNoVizinho]){
                mapaDeVisitados[idNoVizinho] = true;
                pilhaParaBusca.push_back(idNoVizinho);
                contadorDeVisitados++;
            }
        }
    }
    
    // Se o número de nós visitados for igual ao tamanho do conjunto, ele é conexo.
    return contadorDeVisitados == conjuntoDeNos.size();
}

// Constrói uma única solução candidata com a lógica do GRA.
vector<char> Gulosos::ConstruirSolucaoUnicaComGRA(float fatorDeAleatoriedade) {
    vector<char> conjuntoDominanteAtual;
    set<char> verticesDominados;

    // Caso de um grafo vazio.
    if (grafo->ordem == 0) return conjuntoDominanteAtual;
    
    // Escolhe o nó com maior grau como ponto de partida guloso.
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

    // Adiciona o nó inicial e seus vizinhos ao conjunto de dominados.
    conjuntoDominanteAtual.push_back(noInicial->id);
    verticesDominados.insert(noInicial->id);
    for(Aresta* aresta : noInicial->arestas) {
        verticesDominados.insert(aresta->id_no_alvo);
    }

    // Loop principal de construção: continua até todos os nós do grafo serem dominados.
    while (verticesDominados.size() < grafo->ordem) {
        vector<pair<char, int>> candidatosComPontuacao;
        int melhorPontuacao = -1;

        // Identifica como candidatos todos os vizinhos da solução atual que ainda não estão nela.
        set<char> vizinhosCandidatos;
        for(char idNoDaSolucao : conjuntoDominanteAtual){
            No* ponteiroNoDaSolucao = grafo->getNo(idNoDaSolucao);
            if (!ponteiroNoDaSolucao) continue;
            for(Aresta* aresta : ponteiroNoDaSolucao->arestas){
                if(find(conjuntoDominanteAtual.begin(), conjuntoDominanteAtual.end(), aresta->id_no_alvo) == conjuntoDominanteAtual.end()){
                    vizinhosCandidatos.insert(aresta->id_no_alvo);
                }
            }
        }

        // Medida de segurança para grafos desconexos.
        if(vizinhosCandidatos.empty() && verticesDominados.size() < grafo->ordem){
            for(No* noAtual : grafo->lista_adj){
                if(verticesDominados.find(noAtual->id) == verticesDominados.end()){
                    vizinhosCandidatos.insert(noAtual->id);
                    break; 
                }
            }
        }

        // Calcula a "pontuação" de cada candidato (quantos *novos* nós ele domina).
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

        // Monta a Lista de Candidatos Restrita (RCL) com os "melhores" candidatos.
        vector<char> listaDeBonsCandidatos_RCL;
        // O limite da pontuação é definido pelo alfa (fator de aleatoriedade).
        float limiteDaPontuacao = melhorPontuacao * (1.0 - fatorDeAleatoriedade);
        for (const auto& candidato : candidatosComPontuacao) {
            if (candidato.second >= limiteDaPontuacao) {
                listaDeBonsCandidatos_RCL.push_back(candidato.first);
            }
        }
        
        // Se a RCL não estiver vazia, sorteia um candidato dela.
        if (!listaDeBonsCandidatos_RCL.empty()) {
            int indiceAleatorio = rand() % listaDeBonsCandidatos_RCL.size();
            char candidatoEscolhido = listaDeBonsCandidatos_RCL[indiceAleatorio];
            
            // Adiciona o candidato escolhido à solução e atualiza os nós dominados.
            conjuntoDominanteAtual.push_back(candidatoEscolhido);
            verticesDominados.insert(candidatoEscolhido);
            No* noEscolhido = grafo->getNo(candidatoEscolhido);
            if(!noEscolhido) continue;
            for (Aresta* aresta : noEscolhido->arestas) {
                verticesDominados.insert(aresta->id_no_alvo);
            }
        } else {
            // Encerra se não houver mais candidatos possíveis.
            break;
        }
    }
    
    // Verificação final de conectividade.
    while(!VerificarConectividadeDoSubgrafo(conjuntoDominanteAtual)){
        break; 
    }

    return conjuntoDominanteAtual;
}

// Executa o algoritmo GRA completo por N iterações.
vector<char> Gulosos::ExecutarGraParaDominanteConectado(int numeroDeIteracoes, float fatorDeAleatoriedadeInicial, int sementeRandomica) {
    srand(sementeRandomica);

    vector<char> melhorSolucaoEncontrada;
    float fatorDeAleatoriedadeAtual = fatorDeAleatoriedadeInicial; 
    int contadorDeIteracoesSemMelhora = 0;

    // Loop principal do GRA, que constrói uma solução a cada iteração.
    for (int i = 0; i < numeroDeIteracoes; ++i) {
        vector<char> solucaoGeradaNestaIteracao = ConstruirSolucaoUnicaComGRA(fatorDeAleatoriedadeAtual);

        // Se a nova solução for a melhor encontrada até agora, ela é salva.
        if (melhorSolucaoEncontrada.empty() || solucaoGeradaNestaIteracao.size() < melhorSolucaoEncontrada.size()) {
            melhorSolucaoEncontrada = solucaoGeradaNestaIteracao;
            cout << "Iteracao " << i << ": Nova melhor solucao encontrada com tamanho " << melhorSolucaoEncontrada.size() << endl;
            
            // Lógica adaptativa: fica mais "guloso" após encontrar uma solução melhor.
            contadorDeIteracoesSemMelhora = 0;
            fatorDeAleatoriedadeAtual = max(0.1f, fatorDeAleatoriedadeAtual - 0.1f);
        } else {
            contadorDeIteracoesSemMelhora++;
        }

        // Lógica adaptativa: fica mais "aleatório" se não houver melhora por um tempo.
        if (contadorDeIteracoesSemMelhora > (numeroDeIteracoes * 0.1)) { 
            fatorDeAleatoriedadeAtual = min(1.0f, fatorDeAleatoriedadeAtual + 0.1f);
            contadorDeIteracoesSemMelhora = 0;
        }
    }

    return melhorSolucaoEncontrada;
}