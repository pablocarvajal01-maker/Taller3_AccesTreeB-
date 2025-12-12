//
// Created by carva on 11-12-2025.
//

#ifndef TALLER___NODOBPLUSINTERNO_H
#define TALLER___NODOBPLUSINTERNO_H

#include "NodoBPlusBase.h"

class NodoBPlusHoja;
class NodoBPlusInterno;

class NodoBPlusInterno : public NodoBPlusBase {
private:
    int* claves;
    NodoBPlusBase** hijos;     // arreglo de punteros a nodos hijos
    int cantidadClaves;
    int orden;                 // m

public:
    NodoBPlusInterno(int orden);
    ~NodoBPlusInterno();

    int getCantidadClaves();
    int getClave(int index);
    NodoBPlusBase* getHijo(int index);

    void setClave(int index, int clave);
    void setHijo(int index, NodoBPlusBase* ptr);

    void insertarClaveYHijo(int clave, NodoBPlusBase* hijoDerecho);

    int buscarPosicion(int clave);

    bool estaLleno();
    NodoBPlusInterno* dividir(int& clavePromocionada);

    void mostrar();


    void removerClaveHijo(int posClave, int posHijo);

    void prestarDeIzquierdaInterno(NodoBPlusInterno* hermano_izquierdo, int indexClavePadre);
    void prestarDeDerechaInterno(NodoBPlusInterno* hermano_derecho, int indexClavePadre);

    void fusionarConHermanoDerechoInterno(NodoBPlusInterno* hermano_derecho, int indexClavePadre);

};

#endif //TALLER___NODOBPLUSINTERNO_H