//
// Created by carva on 11-12-2025.
//
#include "NodoBPlusInterno.h"
#include <iostream>
using namespace std;

NodoBPlusInterno::NodoBPlusInterno(int orden)
        : NodoBPlusBase(orden, false)
{


    hijos = new NodoBPlusBase*[orden + 1];

    for (int i = 0; i < orden + 1; i++) {
        hijos[i] = nullptr;
    }
}

NodoBPlusInterno::~NodoBPlusInterno() {
    delete[] hijos;
}

int NodoBPlusInterno::getCantidadClaves() {
    return numClaves;
}

int NodoBPlusInterno::getClave(int index) {
    return claves[index];
}

NodoBPlusBase* NodoBPlusInterno::getHijo(int index) {
    return hijos[index];
}

void NodoBPlusInterno::setClave(int index, int clave) {
    claves[index] = clave;
}

void NodoBPlusInterno::setHijo(int index, NodoBPlusBase* ptr) {
    hijos[index] = ptr;
}

bool NodoBPlusInterno::estaLleno() {
    return numClaves == orden;
}

int NodoBPlusInterno::buscarPosicion(int clave) {
    int i = 0;
    while (i < numClaves && clave >= claves[i]) {
        i++;
    }
    return i;
}

void NodoBPlusInterno::insertarClaveYHijo(int clave, NodoBPlusBase* hijoDerecho) {
    int pos = buscarPosicion(clave);

    for (int i = numClaves; i > pos; i--) {
        claves[i] = claves[i - 1];
        hijos[i + 1] = hijos[i];
    }

    claves[pos] = clave;
    hijos[pos + 1] = hijoDerecho;

    numClaves++;
}

NodoBPlusInterno* NodoBPlusInterno::dividir(int& clavePromocionada) {
    int mitad = numClaves / 2;

    NodoBPlusInterno* nuevo = new NodoBPlusInterno(orden);

    clavePromocionada = claves[mitad];

    int idxNuevo = 0;
    for (int i = mitad + 1; i < numClaves; i++) {
        nuevo->setClave(idxNuevo, claves[i]);
        nuevo->hijos[idxNuevo] = hijos[i];
        idxNuevo++;
    }
    nuevo->hijos[idxNuevo] = hijos[numClaves];

    nuevo->numClaves = idxNuevo;

    numClaves = mitad;

    return nuevo;
}

void NodoBPlusInterno::mostrar() {
    cout << "[Interno: ";
    for (int i = 0; i < numClaves; i++) {
        cout << claves[i] << " ";
    }
    cout << "]\n";
}

