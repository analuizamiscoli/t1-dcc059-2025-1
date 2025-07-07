#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <sstream>
#include <algorithm>

#include "Grafo.h"
#include "No.h"
#include "Aresta.h"
#include <climits>


using namespace std;

void Grafo::carregarGrafo(string nome_arquivo) {
    ifstream arquivo(nome_arquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << nome_arquivo << endl;
        return;
    }

    int direcionado, ponderado_aresta, ponderado_vertice;
    arquivo >> direcionado >> ponderado_aresta >> ponderado_vertice;
    this->in_direcionado = direcionado;
    this->in_ponderado_aresta = ponderado_aresta;
    this->in_ponderado_vertice = ponderado_vertice;

    int num_vertices;
    arquivo >> num_vertices;
    this->ordem = num_vertices;

    for (int i = 0; i < num_vertices; ++i) {
        string linha;
        string id;
        int peso = 0;

        arquivo >> id;

        if (ponderado_vertice)
            arquivo >> peso;

        No *novo = new No();
        novo->id = id[0];
        novo->peso = peso;
        this->lista_adj.push_back(novo);
    }
    
    string id_a, id_b;
    int peso = 0;
    while (arquivo >> id_a >> id_b) {
        if (ponderado_aresta)
            arquivo >> peso;
        else
            peso = 0;

        No *origem = nullptr;
        for (No *no : lista_adj) {
            if (no->id == id_a[0]) {
                origem = no;
                break;
            }
        }

        if(origem) {
            Aresta *aresta = new Aresta();
            aresta->id_no_alvo = id_b[0];
            aresta->peso = peso;
            origem->arestas.push_back(aresta);
        }

        if(!direcionado) {
            No *destino = nullptr;
            for (No *no : lista_adj) {
                if (no->id == id_b[0]) {
                    destino = no;
                    break;
                }
            }

            if (destino) {
                Aresta *aresta = new Aresta();
                aresta->id_no_alvo = id_a[0];
                aresta->peso = peso;
                destino->arestas.push_back(aresta);
            }
        }

    }

    arquivo.close();
}

//Função para imprimir o grafo na tela
void Grafo::imprimirGrafo() {
    cout<<"==========ESTRUTURA DO GRAFO=========="<<endl;
    cout<<"Ordem: "<<this->ordem<< " vertices"<<endl;
    cout<<"Tipo: "<< (this->in_direcionado ? "Direcionado" : "Nao direcionado") <<endl;
    cout<<"Ponderado (vertices): "<<(this->in_ponderado_vertice ? "Sim" : "Nao") <<endl;
    cout<<"Ponderado (arestas): "<<(this->in_ponderado_aresta ? "Sim" : "Nao") <<endl;
    cout<<"========================================="<<endl;

    cout<<"\nLISTA DE ADJACENCIA:"<<endl;
    for(No* no : lista_adj){
        cout<<"Vertice "<<no->id;
        if(this->in_ponderado_vertice) {
            cout<<" (Peso: "<<no->peso<<")";
        }
    
        cout <<" -> ";

        if(no->arestas.empty()) {
            cout<<"sem adjacentes";
        } else {
            for (size_t i = 0; i < no->arestas.size(); ++i) {
                cout << no->arestas[i]->id_no_alvo;
                if (this->in_ponderado_aresta) {
                    cout << " (Peso: " << no->arestas[i]->peso << ")";
                }
                if (i < no->arestas.size() - 1) {
                    cout << ", ";
                }
            }
        }

        cout << endl;
    }
    cout << endl;
}

//Função para salvar o grafo em arquivo
void Grafo::salvarGrafo(string nome_arquivo){
    ofstream arquivo(nome_arquivo);
    if (!arquivo.is_open()) {
        cout << "Erro ao criar o arquivo:" << nome_arquivo << endl;
        return;
    }

    //Cabeçalho do arquivo
    arquivo << (this->in_direcionado ? 1 : 0) << " " << (this->in_ponderado_aresta ? 1 : 0) << " " << (this->in_ponderado_vertice ? 1 : 0) << endl;

    arquivo << this->ordem << endl;

    //Salvando vértices
    for (No* no : lista_adj) {
        arquivo << no->id;
        if (this->in_ponderado_vertice) {
            arquivo << " " << no->peso;
        }
        arquivo << endl;
    }

    //Salvando arestas (evitando duplicatas em grafos não direcionados)
    for (No* no : lista_adj){
        for(Aresta* aresta : no->arestas){
            //Para grafos não direcionados, só salva se o ID de origem for menor que o destino
            //para evitar duplicatas
            if(!this->in_direcionado && no->id > aresta->id_no_alvo) {
                continue;
            }
            arquivo << no->id << " " << aresta->id_no_alvo;
            if (this->in_ponderado_aresta) {
                arquivo << " " << aresta->peso;
            }
            arquivo << endl;
        }
    }
    arquivo.close();
    cout << "Grafo salvo em: " << nome_arquivo << endl;
}

//Função para imprimir vetor de vértices (para resultados como fecha transitivo)
void Grafo::imprimirVetorVertices(vector<char> vertices, string titulo){
    cout<<"========== "<<titulo<<" =========="<<endl;
    if(vertices.empty()){
        cout<<"Conjunto Vazio"<<endl;
    } else {
        cout<<"Vertices: ";
        for(size_t i = 0; i < vertices.size(); ++i) {
            cout << vertices[i];
            if (i < vertices.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }

    cout << "=========================================" << endl << endl;
}

//Função para salvar vetor de vértices em arquivo
void Grafo::salvarVetorVertices(vector<char> vertices, string nome_arquivo, string titulo){
    ofstream arquivo(nome_arquivo);
    if (!arquivo.is_open()) {
        cout << "Erro ao criar o arquivo: " << nome_arquivo << endl;
        return;
    }

    arquivo << titulo << endl;
    arquivo << "Numero de vertices: " << vertices.size() << endl;
    arquivo << "Vertices: ";

    for(size_t i=0; i < vertices.size(); ++i) {
        arquivo << vertices[i];
        if (i < vertices.size() - 1) {
            arquivo << ", ";
        }
    }
    arquivo << endl;
    arquivo.close();
    cout << "Resultado salvo em " << nome_arquivo << endl;
}

//Função para imprimir caminho mínimo
void Grafo::imprimirCaminho(vector<char> caminho, string titulo, int distancia_total){
    cout<<"========== "<<titulo<<" =========="<<endl;
    if(caminho.empty()){
        cout<<"Caminho Vazio"<<endl;
    } else {
        cout<<"Caminho: ";
        for(size_t i = 0; i < caminho.size(); ++i) {
            cout << caminho[i];
            if (i < caminho.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;
        if(distancia_total >= 0) {
            cout << "Distancia total: " << distancia_total << endl;
        }
    }

    cout << "=========================================" << endl << endl;
}

//Função para salvar caminho em arquivo
void Grafo::salvarCaminho(vector<char> caminho, string nome_arquivo, string titulo, int distancia_total){
    ofstream arquivo(nome_arquivo);
    if (!arquivo.is_open()) {
        cout << "Erro ao criar o arquivo: " << nome_arquivo << endl;
        return;
    }

    arquivo << titulo << endl;
    arquivo << "Numero de vertices no caminho: " << caminho.size() << endl;
    arquivo << "Caminho: ";
    for(size_t i = 0; i < caminho.size(); ++i) {
        arquivo << caminho[i];
        if (i < caminho.size() - 1) {
            arquivo << " -> ";
        }
    }
    arquivo << endl;
    if(distancia_total >= 0) {
        arquivo << "Distancia total: " << distancia_total << endl;
    }
    arquivo.close();
    cout << "Resultado salvo em " << nome_arquivo << endl;
}

//Função para imprimir propriedades do grafo (raio, diâmetro, etc...)
void Grafo::imprimirPropriedades(int raio, int diametro, vector<char> centro, vector<char> periferia) {
    cout << "========== PROPRIEDADES DO GRAFO ==========" << endl;
    cout << "Raio: " << raio << endl;
    cout << "Diametro: " << diametro << endl;

    cout << "Centro: ";
    if (centro.empty()) {
        cout << "Vazio" << endl;
    } else {
        for (size_t i = 0; i < centro.size(); ++i) {
            cout << centro[i];
            if (i < centro.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }

    cout << "Periferia: ";
    if (periferia.empty()) {
        cout << "Vazio" << endl;
    } else {
        for (size_t i = 0; i < periferia.size(); ++i) {
            cout << periferia[i];
            if (i < periferia.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }

    cout << "============================================" << endl << endl;
}

//Função para salvar propriedades em arquivo
void Grafo::salvarPropriedades(int raio, int diametro, vector<char> centro, vector<char> periferia, string nome_arquivo){
    ofstream arquivo(nome_arquivo);
    if (!arquivo.is_open()) {
        cout << "Erro ao criar o arquivo: " << nome_arquivo << endl;
        return;
    }

    arquivo << "PROPRIEDADES DO GRAFO" << endl;
    arquivo << "Raio: " << raio << endl;
    arquivo << "Diametro: " << diametro << endl;

    arquivo << "Centro: ";
    for(size_t i = 0; i < centro.size(); ++i) {
        arquivo << centro[i];
        if (i < centro.size() - 1) {
            arquivo << " ";
        }
    }
        arquivo << endl;

    arquivo << "Periferia: ";
    for(size_t i = 0; i < periferia.size(); ++i) {
        arquivo << periferia[i];
        if (i < periferia.size() - 1) {
            arquivo << " ";
        }
    }
    arquivo << endl;
    arquivo.close();
    cout << "Resultado em: " << nome_arquivo << endl;       
}

Grafo::Grafo() {
    this->ordem = 0;
    this->in_direcionado = false;
    this->in_ponderado_aresta = false;
    this->in_ponderado_vertice = false;
}

Grafo::~Grafo() {
}

No* Grafo::getNo(char id) {
    for (No* no : this->lista_adj) {
        if (no->id == id) {
            return no;
        }
    }
    return nullptr;   
}

void Grafo::dfs_fecho_direto(No* no_atual, map<char, bool>& visitados, vector<char>& fecho) {
    visitados[no_atual->id] = true;

    for (Aresta* aresta : no_atual->arestas) {
        char id_vizinho = aresta->id_no_alvo;

        if (!visitados[id_vizinho]) {
            fecho.push_back(id_vizinho);
            No* proximo_no = getNo(id_vizinho);

            if (proximo_no != nullptr) {
                dfs_fecho_direto(proximo_no, visitados, fecho);
            }
        }
    }
}

vector<char> Grafo::fecho_transitivo_direto(char id_no) {
    vector<char> fecho;
    No* no_inicial = getNo(id_no);

    if (no_inicial == nullptr) {
        cout << "Erro: vertice de partida '" << id_no << "' nao encontrado." << endl;
        return fecho;
    }

    map<char, bool> visitados;

    for (No* no : lista_adj) {
        visitados[no->id] = false;
    }
    
    dfs_fecho_direto(no_inicial, visitados, fecho);
    return fecho;
}

void Grafo::dfs_fecho_indireto(No* no_atual, char id_destino, map<char, bool>& visitados, bool& encontrado) {
    if (encontrado) return;  // Já encontrou, não precisa continuar

    visitados[no_atual->id] = true;

    if (no_atual->id == id_destino) {
        encontrado = true;
        return;
    }

    for (Aresta* aresta : no_atual->arestas) {
        char id_vizinho = aresta->id_no_alvo;

        if (!visitados[id_vizinho]) {
            No* proximo_no = getNo(id_vizinho);

            if (proximo_no != nullptr) {
                dfs_fecho_indireto(proximo_no, id_destino, visitados, encontrado);
            }
        }
    }
}

vector<char> Grafo::fecho_transitivo_indireto(char id_no) {
    vector<char> fecho_indireto;

    if (getNo(id_no) == nullptr) {
        cout << "Erro: vertice de destino '" << id_no << "' nao encontrado." << endl;
        return fecho_indireto;
    }

    for (No* no : lista_adj) {
        if (no->id == id_no) continue;

        map<char, bool> visitados;
        for (No* n : lista_adj) {
            visitados[n->id] = false;
        }

        bool encontrado = false;
        dfs_fecho_indireto(no, id_no, visitados, encontrado);

        if (encontrado) {
            fecho_indireto.push_back(no->id);
        }
    }

    return fecho_indireto;
}

vector<char> Grafo::caminho_minimo_dijkstra(char id_no_a, char id_no_b) {
    vector<char> caminho_minimo;
    const int INT_MAX = 2147483647; // Valor máximo para inteiros

    // Verifica se os nós existem
    No* no_a = getNo(id_no_a);
    No* no_b = getNo(id_no_b);

    if (no_a == nullptr || no_b == nullptr) {
        cout << "Erro: um ou ambos os vertices nao existem." << endl;
        return caminho_minimo;
    }

    map<char, int> distancias;
    map<char, char> anteriores;
    map<char, bool> visitados;

    // Inicializa as estruturas
    for (No* no : lista_adj) {
        distancias[no->id] = INT_MAX;
        anteriores[no->id] = '\0';
        visitados[no->id] = false;
    }

    distancias[id_no_a] = 0;

    while (true) {
        char no_nao_visitado_mais_proximo = '\0';
        int menor_distancia = INT_MAX;

        for (const auto& par : distancias) {
            if (!visitados[par.first] && par.second < menor_distancia) {
                menor_distancia = par.second;
                no_nao_visitado_mais_proximo = par.first;
            }
        }

        if (no_nao_visitado_mais_proximo == '\0' || no_nao_visitado_mais_proximo == id_no_b) {
            break;
        }

        visitados[no_nao_visitado_mais_proximo] = true;

        No* no_atual = getNo(no_nao_visitado_mais_proximo);
        if (no_atual == nullptr) continue;

        for (Aresta* aresta : no_atual->arestas) {
            char id_vizinho = aresta->id_no_alvo;
            int peso = aresta->peso;

            if (!visitados[id_vizinho] && distancias[no_nao_visitado_mais_proximo] != INT_MAX) {
                int nova_distancia = distancias[no_nao_visitado_mais_proximo] + peso;
                if (nova_distancia < distancias[id_vizinho]) {
                    distancias[id_vizinho] = nova_distancia;
                    anteriores[id_vizinho] = no_nao_visitado_mais_proximo;
                }
            }
        }
    }

    if (distancias[id_no_b] == INT_MAX) {
        cout << "Nao existe caminho entre " << id_no_a << " e " << id_no_b << "." << endl;
        return caminho_minimo;
    }

    // Reconstrói o caminho de trás pra frente
    for (char v = id_no_b; v != '\0'; v = anteriores[v]) {
        caminho_minimo.push_back(v);
        if (v == id_no_a) break;
    }

    reverse(caminho_minimo.begin(), caminho_minimo.end());

    // Mostra o caminho e o custo
    cout << "Caminho minimo de " << id_no_a << " para " << id_no_b << ": ";
    for (char v : caminho_minimo) {
        cout << v << " ";
    }
    cout << "\nCusto total: " << distancias[id_no_b] << endl;

    return caminho_minimo;
}

vector<char> Grafo::caminho_minimo_floyd(char id_no, char id_no_b) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

Grafo* Grafo::arvore_geradora_minima_prim(vector<char> ids_nos) {
    cout << "Implementando metodo AGM PRIM" << endl;
    if (!in_ponderado_aresta) {
        cout << "Erro: Grafo nao ponderado por arestas." << endl;
        return nullptr;
    }

    int n = ids_nos.size();
    if (n == 0) {
        cout << "Subconjunto de nos vazio." << endl;
        return nullptr;
    }
    
    map<char, int> id_para_indice;
    for(int i = 0; i < n; ++i) {
        id_para_indice[ids_nos[i]] = i;
    }

    vector<vector<int>> matriz_adj(n, vector<int>(n, INT_MAX));
    for (int i = 0; i < n; ++i) {
        No* no_origem = getNo(ids_nos[i]);
        if (!no_origem) continue;

        for (Aresta* aresta : no_origem->arestas) {
            if (id_para_indice.count(aresta->id_no_alvo)) { 
                int j = id_para_indice[aresta->id_no_alvo];
                matriz_adj[i][j] = aresta->peso;
            }
        }
    }

    vector<int> chave(n, INT_MAX);       
    vector<int> pai(n, -1);              
    vector<bool> na_agm(n, false);       
    
    chave[0] = 0;

    for (int count = 0; count < n; ++count) {
        
        int min_chave = INT_MAX;
        int u = -1;

        for (int v_idx = 0; v_idx < n; ++v_idx) {
            if (!na_agm[v_idx] && chave[v_idx] < min_chave) {
                min_chave = chave[v_idx];
                u = v_idx;
            }
        }
        
    
        if (u == -1) break; 
        
        na_agm[u] = true; 

        
        for (int v_idx = 0; v_idx < n; ++v_idx) {
            if (matriz_adj[u][v_idx] != INT_MAX && !na_agm[v_idx] && matriz_adj[u][v_idx] < chave[v_idx]) {
                pai[v_idx] = u;
                chave[v_idx] = matriz_adj[u][v_idx];
            }
        }
    }

   
    Grafo* agm = new Grafo();
    agm->in_direcionado = false;
    agm->in_ponderado_aresta = true;
    agm->ordem = n;

    for (char id : ids_nos) {
        No* novo = new No();
        novo->id = id;
        agm->lista_adj.push_back(novo);
    }

    int custo_total = 0;
    for (int i = 1; i < n; ++i) {
        if (pai[i] != -1) {
            char id_u = ids_nos[pai[i]];
            char id_v = ids_nos[i];
            int peso = matriz_adj[i][pai[i]];
            
            agm->getNo(id_u)->arestas.push_back(new Aresta(id_v, peso));
            agm->getNo(id_v)->arestas.push_back(new Aresta(id_u, peso));
            custo_total += peso;
        }
    }
    
    cout << "Custo total da AGM: " << custo_total << endl;
    
    return agm;
}



Grafo * Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos) {
    cout<<"Implementando metodo de AGM KRUSKAL"<<endl;

    if (!in_ponderado_aresta) {
        cout << "Erro: Grafo nao ponderado por arestas." << endl;
        return nullptr;
    }

    int n = ids_nos.size();

    struct ArestaK {
        int u, v, peso;
    };

    vector<ArestaK> L;

    // montar lista de arestas (somente em X)
    for (int i = 0; i < n; ++i) {
        No* no_u = getNo(ids_nos[i]);
        for (Aresta* a : no_u->arestas) {
            for (int j = 0; j < n; ++j) {
                if (ids_nos[j] == a->id_no_alvo) {
                    if (i < j) {
                        L.push_back({i, j, a->peso});
                    }
                    break;
                }
            }
        }
    }

    sort(L.begin(), L.end(), [](const ArestaK& a, const ArestaK& b) {
        return a.peso < b.peso;
    });

    vector<int> pai(n), rank(n, 0);
    for (int i = 0; i < n; ++i) pai[i] = i;

    auto find = [&](int u) {
        while (pai[u] != u) {
            pai[u] = pai[pai[u]];
            u = pai[u];
        }
        return u;
    };

    auto unir = [&](int u, int v) {
        u = find(u);
        v = find(v);
        if (u == v) return false;

        if (rank[u] < rank[v]) pai[u] = v;
        else if (rank[u] > rank[v]) pai[v] = u;
        else {
            pai[v] = u;
            rank[u]++;
        }
        return true;
    };

    Grafo* AGM = new Grafo();
    AGM->in_direcionado = false;
    AGM->in_ponderado_aresta = true;

    for (char id : ids_nos) {
        No* novo = new No();
        novo->id = id;
        AGM->lista_adj.push_back(novo);
    }
    AGM->ordem = n;

    int arestas_aceitas = 0;

    for (auto& e : L) {
        if (unir(e.u, e.v)) {
            AGM->getNo(ids_nos[e.u])->arestas.push_back(new Aresta(ids_nos[e.v], e.peso));
            AGM->getNo(ids_nos[e.v])->arestas.push_back(new Aresta(ids_nos[e.u], e.peso));
            arestas_aceitas++;
            if (arestas_aceitas == n-1) break;
        }
    }

    if (arestas_aceitas != n-1) {
        cout << "Subgrafo desconexo — AGM incompleta." << endl;
    }

    return AGM;
}


void Grafo::dfs_arvore(No* u, 
                       map<char, int>& cores, 
                       vector<pair<char, char>>& arestas_arvore, 
                       vector<pair<char, char>>& arestas_retorno) {

    cores[u->id] = 1; 

    for (Aresta* aresta : u->arestas) {
        char v_id = aresta->id_no_alvo;
        No* v = getNo(v_id);
        if (v == nullptr) continue; 

        if (cores[v_id] == 0) {
            arestas_arvore.push_back({u->id, v_id}); 
            dfs_arvore(v, cores, arestas_arvore, arestas_retorno); 
        }
        else if (cores[v_id] == 1) {
            arestas_retorno.push_back({u->id, v_id}); 
        }
    }
    cores[u->id] = 2; 
}

Grafo* Grafo::arvore_caminhamento_profundidade(char id_no) {
    No* no_inicial = getNo(id_no);
    if (no_inicial == nullptr) {
        cout << "Erro: Vertice de partida '" << id_no << "' nao encontrado." << endl;
        return nullptr;
    }
    
    map<char, int> cores;
    for (No* no : this->lista_adj) {
        cores[no->id] = 0; 
    }
    
    vector<pair<char, char>> arestas_arvore;
    vector<pair<char, char>> arestas_retorno;
    
    dfs_arvore(no_inicial, cores, arestas_arvore, arestas_retorno);
    
    Grafo* arvore = new Grafo(); 
    arvore->in_direcionado = true; 

    set<char> nos_na_arvore;
    nos_na_arvore.insert(no_inicial->id);
    for (const auto& aresta : arestas_arvore) {
        nos_na_arvore.insert(aresta.first);
        nos_na_arvore.insert(aresta.second);
    }

    for (char id : nos_na_arvore) {
        No* novo_no = new No();
        novo_no->id = id;
        arvore->lista_adj.push_back(novo_no);
    }
    arvore->ordem = nos_na_arvore.size();

    for (const auto& par_aresta : arestas_arvore) {
        No* origem = arvore->getNo(par_aresta.first);
        if (origem != nullptr) {
            origem->arestas.push_back(new Aresta(par_aresta.second, 0));
        }
    }
    arvore->arestas_de_retorno = arestas_retorno;
    
    return arvore;
}

int Grafo::raio() {
    cout<<"Metodo nao implementado"<<endl;
    return 0;
}

int Grafo::diametro() {
    cout<<"Metodo nao implementado"<<endl;
    return 0;
}

vector<char> Grafo::centro() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::periferia() {
    cout<<"Metodo nao implementado"<<endl;
    return {};

}