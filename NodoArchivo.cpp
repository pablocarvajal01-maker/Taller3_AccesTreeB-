//
// creado por carva el 10 12 2025
//
#include "NodoArchivo.h"

NodoArchivo::NodoArchivo(int id, int tamano, int tipo)
    : NodoGrafo(id), tamano(tamano), tipo(tipo) {}

NodoArchivo::~NodoArchivo() {}

int NodoArchivo::getTamano() {
    return tamano;
}

int NodoArchivo::getTipo() {
    return tipo;
}
