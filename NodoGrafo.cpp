#include "NodoGrafo.h"

NodoGrafo::NodoGrafo(int id) : id(id) {
    padres = new ListaPadres();
}

NodoGrafo::~NodoGrafo() {
    delete padres;
}

int NodoGrafo::getId() {
    return id;
}

ListaPadres* NodoGrafo::getPadres() {
    return padres;
}
