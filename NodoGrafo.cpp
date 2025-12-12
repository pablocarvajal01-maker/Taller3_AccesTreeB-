#include "NodoGrafo.h"

NodoGrafo::NodoGrafo(int id) : id(id) {
    padres = new ListaPadres() // se crea la lista de padres
}

NodoGrafo::~NodoGrafo() {
    delete padres // liberar memoria de la lista de padres
}

int NodoGrafo::getId() {
    return id // retorna el id del nodo
}

ListaPadres* NodoGrafo::getPadres() {
    return padres // retorna la lista de padres
}
