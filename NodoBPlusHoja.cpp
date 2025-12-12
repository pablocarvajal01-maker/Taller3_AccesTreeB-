//
// Created by carva on 12-12-2025.
//

#include "NodoBPlusHoja.h"
#include <iostream>
using namespace std;

NodoBPlusHoja::NodoBPlusHoja(int orden)
    : NodoBPlusBase(orden, true)
{
    datos = new NodoGrafo*[orden];
    siguiente = nullptr;

    for (int i = 0; i < orden; i++) {
        datos[i] = nullptr;
    }
}

NodoBPlusHoja::~NodoBPlusHoja() {
    delete[] datos;
}

NodoGrafo* NodoBPlusHoja::getDato(int index) {
    if (index < 0 || index >= numClaves) {
        cout << "Indice de dato fuera de rango\n";
        return nullptr;
    }
    return datos[index];
}

NodoBPlusHoja* NodoBPlusHoja::getSiguiente() {
    return siguiente;
}

void NodoBPlusHoja::setDato(int index, NodoGrafo* ptr) {
    if (index < 0 || index >= orden) {
        cout << "Indice de dato fuera de rango\n";
        return;
    }
    datos[index] = ptr;
}

void NodoBPlusHoja::setSiguiente(NodoBPlusHoja* hoja) {
    siguiente = hoja;
}

bool NodoBPlusHoja::estaLlena() {
    return numClaves == orden;
}

// busca donde insertar la clave dentro de la hoja
int NodoBPlusHoja::buscarPosicion(int clave) {
    int i = 0;
    while (i < numClaves && clave > claves[i]) {
        i++;
    }
    return i;
}

// Inserta clave y puntero al dato
void NodoBPlusHoja::insertarEnHoja(int clave, NodoGrafo* dato) {

    int pos = buscarPosicion(clave);

    for (int i = numClaves; i > pos; i--) {
        claves[i] = claves[i - 1];
        datos[i] = datos[i - 1];
    }

    claves[pos] = clave;
    datos[pos] = dato;

    numClaves++;
}

// Divide hoja en dos y retorna puntero a la nueva hoja
NodoBPlusHoja* NodoBPlusHoja::dividir(int& clavePromocionada) {

    int mitad = numClaves / 2;

    NodoBPlusHoja* nueva = new NodoBPlusHoja(orden);

    int idxNuevo = 0;

    for (int i = mitad; i < numClaves; i++) {
        nueva->claves[idxNuevo] = claves[i];
        nueva->datos[idxNuevo] = datos[i];
        idxNuevo++;
    }

    nueva->numClaves = idxNuevo;

    numClaves = mitad;

    clavePromocionada = nueva->claves[0];

    nueva->siguiente = this->siguiente;
    this->siguiente = nueva;

    return nueva;
}

void NodoBPlusHoja::mostrar() {
    cout << "[Hoja: ";
    for (int i = 0; i < numClaves; i++) {
        cout << "(" << claves[i] << ") ";
    }
    cout << "]\n";
}
