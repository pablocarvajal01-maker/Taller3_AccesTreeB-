//
// Created by carva on 10-12-2025.
//
#include "NodoDirectorio.h"

NodoDirectorio::NodoDirectorio(int id, string nombre) : NodoGrafo(id), nombre(nombre) {
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

bool NodoDirectorio::eliminarHijo(int idHijo) {
    hijos->eliminar(idHijo);
}
