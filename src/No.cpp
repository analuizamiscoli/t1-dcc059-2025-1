#include "No.h"

No::No() {
    this->peso = peso;
}

No::~No() {
    for (Aresta* aresta : arestas) {
        delete aresta;
    }
}