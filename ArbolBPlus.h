//
// Created by carva on 12-12-2025.
//

#ifndef TALLER___ARBOLBPLUS_H
#define TALLER___ARBOLBPLUS_H

#include "NodoBPlusBase.h"
#include "NodoBPlusInterno.h"
#include "NodoBPlusHoja.h"

// forward declaration
class NodoGrafo;

class ArbolBPlus {

private:
    NodoBPlusBase* raiz;
    int orden;
    int accesos;

    void arreglarHoja(NodoBPlusHoja* hoja);
    void arreglarInterno(NodoBPlusInterno* interno);

public:
    ArbolBPlus(int orden);
    ~ArbolBPlus();


    NodoGrafo* buscarNodo(int clave);
    NodoBPlusHoja* buscarHoja(int clave);

    void insertar(int clave, NodoGrafo* dato);
    void insertarEnHoja(NodoBPlusHoja* hoja, int clave, NodoGrafo* dato);
    void splitHoja(NodoBPlusHoja* hoja, int clave, NodoGrafo* dato);
    void splitInterno(NodoBPlusInterno* interno, int claveProm, NodoBPlusBase* nuevoHijo);

    void eliminar(int clave);

    void reiniciarAccesos();
    int getAccesos();
    NodoBPlusBase* getRaiz();
};


#endif //TALLER___ARBOLBPLUS_H