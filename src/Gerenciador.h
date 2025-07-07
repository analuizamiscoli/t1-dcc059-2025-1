#ifndef GERENCIADOR_H
#define GERENCIADOR_H

#include <iostream>
#include "Grafo.h"
#include <algorithm>

using namespace std;
class Gerenciador {
public:
    static void comandos(Grafo* grafo);
    static char get_id_entrada(Grafo* grafo);
    static vector<char> get_conjunto_ids(Grafo* grafo, int tam);
    static bool pergunta_imprimir_arquivo(string nome_arquivo);
    static void imprimirResultadoDFS(Grafo* arvore);
};


#endif //GERENCIADOR_H
