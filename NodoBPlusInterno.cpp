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

    for (int i = 0; i <= numClaves; i++) {
        delete hijos[i];
    }

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
        nuevo->hijos[idxNuevo]->setPadre(nuevo);
        idxNuevo++;
    }

    nuevo->hijos[idxNuevo] = hijos[numClaves];
    if (nuevo->hijos[idxNuevo] != nullptr) {
        nuevo->hijos[idxNuevo]->setPadre(nuevo);
    }

    nuevo->numClaves = idxNuevo;

    numClaves = mitad;
    for (int i = mitad; i < orden; i++) {
        claves[i] = -1;
    }
    for (int i = mitad + 1; i <= orden; i++) {
        hijos[i] = nullptr;
    }

    return nuevo;
}

void NodoBPlusInterno::mostrar() {
    cout << "[Interno: ";
    for (int i = 0; i < numClaves; i++) {
        cout << claves[i] << " ";
    }
    cout << "]\n";
}



void NodoBPlusInterno::removerClaveHijo(int posClave, int posHijo) {
    if (posClave < 0 || posClave >= numClaves || posHijo < 0 || posHijo > numClaves) return;

    if (hijos[posHijo] != nullptr) {
        delete hijos[posHijo];
    }


    for (int i = posHijo; i < numClaves; i++) {
        hijos[i] = hijos[i + 1];
    }
    hijos[numClaves] = nullptr;

    for (int i = posClave; i < numClaves - 1; i++) {
        claves[i] = claves[i + 1];
    }
    claves[numClaves - 1] = -1;

    numClaves--;
}


void NodoBPlusInterno::prestarDeDerechaInterno(NodoBPlusInterno* hermano_derecho, int indexClavePadre) {
    claves[numClaves] = padre->getClave(indexClavePadre);

    hijos[numClaves + 1] = hermano_derecho->hijos[0];
    if (hijos[numClaves + 1] != nullptr) {
        hijos[numClaves + 1]->setPadre(this);
    }

    numClaves++;

    int nuevaClavePadre = hermano_derecho->claves[0];
    padre->setClave(indexClavePadre, nuevaClavePadre);


    for (int i = 0; i < hermano_derecho->numClaves - 1; i++) {
        hermano_derecho->claves[i] = hermano_derecho->claves[i + 1];
    }
    hermano_derecho->claves[hermano_derecho->numClaves - 1] = -1;


    for (int i = 0; i < hermano_derecho->numClaves; i++) {
        hermano_derecho->hijos[i] = hermano_derecho->hijos[i + 1];
    }
    hermano_derecho->hijos[hermano_derecho->numClaves] = nullptr;

    hermano_derecho->numClaves--;
}



void NodoBPlusInterno::prestarDeIzquierdaInterno(NodoBPlusInterno* hermano_izquierdo, int indexClavePadre) {

    for (int i = numClaves; i > 0; i--) {
        claves[i] = claves[i - 1];
    }
    for (int i = numClaves + 1; i > 0; i--) {
        hijos[i] = hijos[i - 1];
    }

    claves[0] = padre->getClave(indexClavePadre);


    hijos[0] = hermano_izquierdo->hijos[hermano_izquierdo->numClaves];
    if (hijos[0] != nullptr) {
        hijos[0]->setPadre(this);
    }

    numClaves++;


    int nuevaClavePadre = hermano_izquierdo->claves[hermano_izquierdo->numClaves - 1];
    padre->setClave(indexClavePadre, nuevaClavePadre);


    hermano_izquierdo->claves[hermano_izquierdo->numClaves - 1] = -1;
    hermano_izquierdo->hijos[hermano_izquierdo->numClaves] = nullptr;
    hermano_izquierdo->numClaves--;
}



void NodoBPlusInterno::fusionarConHermanoDerechoInterno(NodoBPlusInterno* hermano_derecho, int indexClavePadre) {

    claves[numClaves] = padre->getClave(indexClavePadre);
    numClaves++;


    int inicioFusion = numClaves;


    for (int i = 0; i <= hermano_derecho->numClaves; i++) {
        hijos[inicioFusion + i] = hermano_derecho->hijos[i];
        if (hijos[inicioFusion + i] != nullptr) {
            hijos[inicioFusion + i]->setPadre(this);
        }
        hermano_derecho->hijos[i] = nullptr;
    }

    for (int i = 0; i < hermano_derecho->numClaves; i++) {
        claves[inicioFusion + i] = hermano_derecho->claves[i];
    }

    numClaves += hermano_derecho->numClaves;


    if (padre != nullptr) {
        NodoBPlusInterno* padreInterno = (NodoBPlusInterno*)padre;
        padreInterno->removerClaveHijo(indexClavePadre, indexClavePadre + 1);
    }

    hermano_derecho->numClaves = 0;
}