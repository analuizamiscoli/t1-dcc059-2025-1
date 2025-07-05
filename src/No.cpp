#include "No.h"

No::No() {
    this->peso = 0;
}

No::~No() {
    for (Aresta* aresta : arestas) {
        delete aresta;
    }
}