//
// Created by carva on 10-12-2025.
//
#include "NodoDirectorio.h"

NodoDirectorio::NodoDirectorio(int id) : NodoGrafo(id) {
    hijos = new ListaHijos();
}

NodoDirectorio::~NodoDirectorio() {
    delete hijos;
}

ListaHijos* NodoDirectorio::getHijos() {
    return hijos;
}

void NodoDirectorio::agregarHijo(int idHijo) {
    hijos->agregar(idHijo);
}

void NodoDirectorio::eliminarHijo(int idHijo) {
    hijos->eliminar(idHijo);
}
