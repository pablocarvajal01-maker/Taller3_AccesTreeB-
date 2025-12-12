//
// Created by carva on 12-12-2025.
//

#ifndef TALLER___NODOBPLUSHOJA_H
#define TALLER___NODOBPLUSHOJA_H

#include "NodoBPlusBase.h"

// forward
class NodoGrafo;

class NodoBPlusHoja : public NodoBPlusBase {

private:
    NodoGrafo** datos;     // arreglo de punteros a NodoGrafo
    NodoBPlusHoja* siguiente; // puntero a la siguiente hoja

public:
    NodoBPlusHoja(int orden);
    ~NodoBPlusHoja();

    NodoGrafo* getDato(int index);
    NodoBPlusHoja* getSiguiente();

    void setDato(int index, NodoGrafo* ptr);
    void setSiguiente(NodoBPlusHoja* hoja);

    bool estaLlena();
    int buscarPosicion(int clave);
    void insertarEnHoja(int clave, NodoGrafo* dato);

    NodoBPlusHoja* dividir(int& clavePromocionada);

    void mostrar();

};


#endif //TALLER___NODOBPLUSHOJA_H