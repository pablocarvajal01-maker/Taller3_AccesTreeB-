//
// Created by carva on 11-12-2025.
//
#include "NodoBPlusBase.h"
#include <iostream>
using namespace std;

NodoBPlusBase::NodoBPlusBase(int orden, bool esHoja)
{
    this->orden = orden;
    this->esHoja = esHoja;
    this->numClaves = 0;
    this->padre = nullptr;

    claves = new int[orden];
    for (int i = 0; i < orden; i++) claves[i] = -1;
}

NodoBPlusBase::~NodoBPlusBase()
{
    delete[] claves;
}

int NodoBPlusBase::getNumClaves() { return numClaves; }

int NodoBPlusBase::getClave(int index)
{
    if (index < 0 || index >= numClaves) {
        cout << "Indice de clave fuera de rango\n";
        return -1;
    }
    return claves[index];
}

void NodoBPlusBase::setClave(int index, int valor)
{
    if (index < 0 || index >= orden) {
        cout << "Indice fuera de rango\n";
        return;
    }
    claves[index] = valor;
}

bool NodoBPlusBase::getEsHoja() { return esHoja; }
int  NodoBPlusBase::getOrden()  { return orden; }

void NodoBPlusBase::setNumClaves(int n)
{
    numClaves = n;
}

void NodoBPlusBase::incrementarNumClaves()
{
    numClaves++;
}

bool NodoBPlusBase::estaLlena()
{
    return numClaves == orden;
}

void NodoBPlusBase::setPadre(NodoBPlusBase* p)
{
    padre = p;
}

NodoBPlusBase* NodoBPlusBase::getPadre()
{
    return padre;
}
