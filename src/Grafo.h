//
// Created by Rafael on 28/05/2025.
//

#ifndef GRAFO_H
#define GRAFO_H

#include "No.h"
#include <iostream>
#include <vector>

using namespace std;

class Grafo {
public:
    Grafo();
    ~Grafo();
    // Funções para manipulação do grafo
    void carregarGrafo(string nome_arquivo);
    void imprimirGrafo();
    void salvarGrafo(string nome_arquivo);

    vector<char> fecho_transitivo_direto(char id_no); // a
    vector<char> fecho_transitivo_indireto(char id_no); // b
    vector<char> caminho_minimo_dijkstra(char id_no_a, char id_no_b); // c
    vector<char> caminho_minimo_floyd(char id_no, char id_no_b); // d
    Grafo* arvore_geradora_minima_prim(vector<char> ids_nos); // e
    Grafo* arvore_geradora_minima_kruskal(vector<char> ids_nos); // f
    Grafo* arvore_caminhamento_profundidade(char id_no); // g
    int raio(); // h 1
    int diametro(); // h 2
    vector<char> centro(); // h 3
    vector<char> periferia(); // h 4
    vector<char> vertices_de_articulacao(); // i

    // Funções para imprimir e salvar os resultados
    void imprimirVetorVertices(vector<char> vertices, string titulo);
    void salvarVetorVertices(vector<char> vertices, string nome_arquivo, string titulo);
    void imprimirCaminho(vector<char> caminho, string titulo, int distancia_total = -1);
    void salvarCaminho(vector<char> caminho, string nome_arquivo, string titulo, int distancia_total = -1);
    void imprimirPropriedades(int raio, int diametro, vector<char> centro, vector<char> periferia);
    void salvarPropriedades(int raio, int diametro, vector<char> centro, vector<char> periferia, string nome_arquivo);

    int ordem;
    bool in_direcionado;
    bool in_ponderado_aresta;
    bool in_ponderado_vertice;
    vector<No*> lista_adj;
};



#endif //GRAFO_H
