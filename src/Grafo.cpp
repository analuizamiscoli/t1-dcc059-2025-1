#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Grafo.h"
#include "No.h"
#include "Aresta.h"

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
}

Grafo::~Grafo() {
}

vector<char> Grafo::fecho_transitivo_direto(char id_no) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::fecho_transitivo_indireto(char id_no) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::caminho_minimo_dijkstra(char id_no_a, char id_no_b) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::caminho_minimo_floyd(char id_no, char id_no_b) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

Grafo * Grafo::arvore_geradora_minima_prim(vector<char> ids_nos) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}

Grafo * Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}

Grafo * Grafo::arvore_caminhamento_profundidade(char id_no) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
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

vector<char> Grafo::vertices_de_articulacao() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}
