#include "Aresta.h"

Aresta::Aresta() {
    this->id_no_alvo = '\0';
    this->peso = 0;
}

Aresta::Aresta(char id_no_alvo, int peso) {
    this->id_no_alvo = id_no_alvo;
    this->peso = peso;
}

Aresta::~Aresta() {
}