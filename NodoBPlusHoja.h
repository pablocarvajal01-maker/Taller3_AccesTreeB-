//
// Created by carva on 12-12-2025.
//

#ifndef TALLER___NODOBPLUSHOJA_H
#define TALLER___NODOBPLUSHOJA_H

#include "NodoBPlusBase.h"

class NodoGrafo;
class NodoBPlusInterno;

class NodoBPlusHoja : public NodoBPlusBase {

private:
    NodoGrafo** datos;
    NodoBPlusHoja* siguiente;

    void actualizarClavePadre(int indexClavePadre, int nuevaClave);

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


    void removerClaveDato(int pos);

    void prestarDeDerecha(NodoBPlusHoja* hoja, int indexClavePadre);
    void prestarDeIzquierda(NodoBPlusHoja* hoja, int indexClavePadre);


    void fusionarConHermanoDerecho(NodoBPlusHoja* hoja_a_fusionar, int indexClavePadre);

    void mostrar();
};


#endif //TALLER___NODOBPLUSHOJA_H