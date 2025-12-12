//
// Created by carva on 12-12-2025.
//
#include "ArbolBPlus.h"
#include <iostream>
using namespace std;

ArbolBPlus::ArbolBPlus(int orden) {
    this->orden = orden;
    this->accesos = 0;
    this->raiz = new NodoBPlusHoja(orden); // el arbol parte con una sola hoja
}

ArbolBPlus::~ArbolBPlus() {
    delete raiz;
}

NodoBPlusBase* ArbolBPlus::getRaiz() {
    return raiz;
}

void ArbolBPlus::reiniciarAccesos() {
    accesos = 0;
}

int ArbolBPlus::getAccesos() {
    return accesos;
}

NodoBPlusHoja* ArbolBPlus::buscarHoja(int clave) {
    NodoBPlusBase* actual = raiz;

    while(!actual->getEsHoja()) {
        accesos++;
        NodoBPlusInterno* interno = (NodoBPlusInterno*) actual;

        int pos = 0;
        while(pos < interno->getNumClaves() && clave >= interno->getClave(pos)) {
            pos++;
        }

        NodoBPlusBase* hijo = interno->getHijo(pos);
        hijo->setPadre(interno);
        actual = hijo;
    }

    accesos++;
    return (NodoBPlusHoja*) actual;
}

NodoGrafo* ArbolBPlus::buscarNodo(int clave) {
    NodoBPlusHoja* hoja = buscarHoja(clave);

    for(int i = 0; i < hoja->getNumClaves(); i++) {
        if(hoja->getClave(i) == clave) {
            return hoja->getDato(i);
        }
    }

    return nullptr;
}

void ArbolBPlus::insertar(int clave, NodoGrafo* dato) {
    NodoBPlusHoja* hoja = buscarHoja(clave);

    if (hoja->estaLlena()) {
        splitHoja(hoja, clave, dato);
    } else {
        insertarEnHoja(hoja, clave, dato);
    }
}

void ArbolBPlus::insertarEnHoja(NodoBPlusHoja* hoja, int clave, NodoGrafo* dato) {

    int i = hoja->getNumClaves() - 1;

    // correr claves a la derecha
    while(i >= 0 && hoja->getClave(i) > clave) {
        hoja->setClave(i + 1, hoja->getClave(i));
        hoja->setDato(i + 1, hoja->getDato(i));
        i--;
    }

    hoja->setClave(i + 1, clave);
    hoja->setDato(i + 1, dato);

    hoja->incrementarNumClaves();
}

void ArbolBPlus::splitHoja(NodoBPlusHoja* hoja, int clave, NodoGrafo* dato) {

    NodoBPlusHoja* nuevaHoja = new NodoBPlusHoja(orden);

    int maxTotal = orden + 1;
    int* tempClaves = new int[maxTotal];
    NodoGrafo** tempDatos = new NodoGrafo*[maxTotal];

    int total = hoja->getNumClaves();

    // copiar a un arreglo temporal
    for(int i = 0; i < total; i++) {
        tempClaves[i] = hoja->getClave(i);
        tempDatos[i] = hoja->getDato(i);
    }

    // insertar clave nueva en el temporal
    int i = total - 1;
    while(i >= 0 && tempClaves[i] > clave) {
        tempClaves[i + 1] = tempClaves[i];
        tempDatos[i + 1] = tempDatos[i];
        i--;
    }
    tempClaves[i + 1] = clave;
    tempDatos[i + 1] = dato;

    total++;

    int mitad = total / 2;
    int claveProm = tempClaves[mitad]; // La clave que sube es la primera de la nueva hoja

    hoja->setNumClaves(0);

    // recargar primera mitad en el nodo original
    for(int j = 0; j < mitad; j++) {
        hoja->setClave(j, tempClaves[j]);
        hoja->setDato(j, tempDatos[j]);
        hoja->incrementarNumClaves();
    }

    // recargar segunda mitad en la nueva hoja
    for(int j = mitad; j < total; j++) {
        nuevaHoja->setClave(j - mitad, tempClaves[j]);
        nuevaHoja->setDato(j - mitad, tempDatos[j]);
        nuevaHoja->incrementarNumClaves();
    }

    nuevaHoja->setSiguiente(hoja->getSiguiente());
    hoja->setSiguiente(nuevaHoja);

    delete[] tempClaves;
    delete[] tempDatos;


    // caso raiz
    if(raiz == hoja) {
        NodoBPlusInterno* nuevaRaiz = new NodoBPlusInterno(orden);
        nuevaRaiz->setClave(0, claveProm);
        nuevaRaiz->setHijo(0, hoja);
        nuevaRaiz->setHijo(1, nuevaHoja);
        nuevaRaiz->setNumClaves(1);

        hoja->setPadre(nuevaRaiz);
        nuevaHoja->setPadre(nuevaRaiz);

        raiz = nuevaRaiz;
        return;
    }

    // si hay raiz interna, split interno
    splitInterno((NodoBPlusInterno*)hoja->getPadre(), claveProm, nuevaHoja);
}

void ArbolBPlus::splitInterno(NodoBPlusInterno* interno, int claveProm, NodoBPlusBase* nuevoHijo) {


    int totalClavesTemp = interno->getNumClaves() + 1;
    int* tempClaves = new int[totalClavesTemp];
    NodoBPlusBase** tempHijos = new NodoBPlusBase*[totalClavesTemp + 1];

    // copiar claves e hijos a arreglos temporales
    for (int i = 0; i < interno->getNumClaves(); i++) {
        tempClaves[i] = interno->getClave(i);
        tempHijos[i] = interno->getHijo(i);
    }
    tempHijos[interno->getNumClaves()] = interno->getHijo(interno->getNumClaves()); // ultimo hijo


    int posInsercion = 0;
    while(posInsercion < interno->getNumClaves() && claveProm >= interno->getClave(posInsercion)) {
        posInsercion++;
    }

    // Mover claves a la derecha
    for (int j = interno->getNumClaves(); j > posInsercion; j--) {
        tempClaves[j] = tempClaves[j - 1];
    }
    tempClaves[posInsercion] = claveProm;

    // Mover hijos a la derecha
    for (int j = interno->getNumClaves() + 1; j > posInsercion + 1; j--) {
        tempHijos[j] = tempHijos[j - 1];
    }
    tempHijos[posInsercion + 1] = nuevoHijo;



    int mitad_index = totalClavesTemp / 2; // indice central
    int claveAscendente = tempClaves[mitad_index];

    NodoBPlusInterno* nuevoInterno = new NodoBPlusInterno(orden);

    interno->setNumClaves(0);
    for (int j = 0; j < mitad_index; j++) {
        interno->setClave(j, tempClaves[j]);
        interno->setHijo(j, tempHijos[j]);
        interno->getHijo(j)->setPadre(interno);
        interno->incrementarNumClaves();
    }
    // Hijo a la derecha de la ultima clave de la primera mitad
    interno->setHijo(mitad_index, tempHijos[mitad_index]);
    interno->getHijo(mitad_index)->setPadre(interno);



    int idxNuevo = 0;
    for (int j = mitad_index + 1; j < totalClavesTemp; j++) {
        nuevoInterno->setClave(idxNuevo, tempClaves[j]);
        nuevoInterno->setHijo(idxNuevo, tempHijos[j]);
        nuevoInterno->getHijo(idxNuevo)->setPadre(nuevoInterno);
        nuevoInterno->incrementarNumClaves();
        idxNuevo++;
    }
    // El ultimo hijo
    nuevoInterno->setHijo(idxNuevo, tempHijos[totalClavesTemp]);
    if (tempHijos[totalClavesTemp]) {
        nuevoInterno->getHijo(idxNuevo)->setPadre(nuevoInterno);
    }

    delete[] tempClaves;
    delete[] tempHijos;




    // caso raiz.
    if (interno == raiz) {
        NodoBPlusInterno* nuevaRaiz = new NodoBPlusInterno(orden);
        nuevaRaiz->setClave(0, claveAscendente);
        nuevaRaiz->setHijo(0, interno);
        nuevaRaiz->setHijo(1, nuevoInterno);
        nuevaRaiz->setNumClaves(1);

        interno->setPadre(nuevaRaiz);
        nuevoInterno->setPadre(nuevaRaiz);

        raiz = nuevaRaiz;
        return;
    }

    // caso recursion
    NodoBPlusInterno* padreInterno = (NodoBPlusInterno*)interno->getPadre();

    if (padreInterno->estaLleno()) {
        splitInterno(padreInterno, claveAscendente, nuevoInterno);
    } else {
        nuevoInterno->setPadre(padreInterno);
        padreInterno->insertarClaveYHijo(claveAscendente, nuevoInterno);
    }
}