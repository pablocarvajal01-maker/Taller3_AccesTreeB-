//
// Created by carva on 12-12-2025.
//

#include "NodoBPlusHoja.h"
#include "NodoBPlusInterno.h"
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
        cout << "Indice de dato fuera de rango en setDato\n";
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


void NodoBPlusHoja::actualizarClavePadre(int indexClavePadre, int nuevaClave) {
    if (padre != nullptr) {
        NodoBPlusInterno* padreInterno = (NodoBPlusInterno*)padre;
        padreInterno->setClave(indexClavePadre, nuevaClave);
    }
}


void NodoBPlusHoja::removerClaveDato(int pos) {
    if (pos < 0 || pos >= numClaves) return;

    bool esPrimeraClave = (pos == 0);

    for (int i = pos; i < numClaves - 1; i++) {
        claves[i] = claves[i + 1];
        datos[i] = datos[i + 1];
    }

    claves[numClaves - 1] = -1;
    datos[numClaves - 1] = nullptr;

    numClaves--;

    if (esPrimeraClave && padre != nullptr) {
        NodoBPlusInterno* padreInterno = (NodoBPlusInterno*)padre;
        for (int i = 0; i <= padreInterno->getCantidadClaves(); ++i) {
            if (padreInterno->getHijo(i) == this) {

                if (i > 0) {
                    padreInterno->setClave(i - 1, claves[0]);
                }
                break;
            }
        }
    }
}



void NodoBPlusHoja::prestarDeDerecha(NodoBPlusHoja* hoja_receptora, int indexClavePadre) {
    for (int i = hoja_receptora->numClaves; i > 0; i--) {
        hoja_receptora->claves[i] = hoja_receptora->claves[i - 1];
        hoja_receptora->datos[i] = hoja_receptora->datos[i - 1];
    }

    hoja_receptora->claves[0] = this->claves[this->numClaves - 1];
    hoja_receptora->datos[0] = this->datos[this->numClaves - 1];

    this->claves[this->numClaves - 1] = -1;
    this->datos[this->numClaves - 1] = nullptr;
    this->numClaves--;
    hoja_receptora->numClaves++;

    hoja_receptora->actualizarClavePadre(indexClavePadre, hoja_receptora->claves[0]);
}

void NodoBPlusHoja::prestarDeIzquierda(NodoBPlusHoja* hoja_lender, int indexClavePadre) {

    this->claves[this->numClaves] = hoja_lender->claves[0];
    this->datos[this->numClaves] = hoja_lender->datos[0];
    this->numClaves++;

    for (int i = 0; i < hoja_lender->numClaves - 1; i++) {
        hoja_lender->claves[i] = hoja_lender->claves[i + 1];
        hoja_lender->datos[i] = hoja_lender->datos[i + 1];
    }
    hoja_lender->claves[hoja_lender->numClaves - 1] = -1;
    hoja_lender->datos[hoja_lender->numClaves - 1] = nullptr;
    hoja_lender->numClaves--;

    hoja_lender->actualizarClavePadre(indexClavePadre, hoja_lender->claves[0]);
}


void NodoBPlusHoja::fusionarConHermanoDerecho(NodoBPlusHoja* hoja_a_fusionar, int indexClavePadre) {
    int inicioFusion = this->numClaves;
    for (int i = 0; i < hoja_a_fusionar->numClaves; i++) {
        this->claves[inicioFusion + i] = hoja_a_fusionar->claves[i];
        this->datos[inicioFusion + i] = hoja_a_fusionar->datos[i];
    }
    this->numClaves += hoja_a_fusionar->numClaves;

    this->siguiente = hoja_a_fusionar->getSiguiente();

    NodoBPlusInterno* padreInterno = (NodoBPlusInterno*)this->padre;

    padreInterno->removerClaveHijo(indexClavePadre, indexClavePadre + 1);

    hoja_a_fusionar->numClaves = 0;
}