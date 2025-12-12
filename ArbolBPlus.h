//
// Created by carva on 12-12-2025.
//

#ifndef TALLER___ARBOLBPLUS_H
#define TALLER___ARBOLBPLUS_H

#include "NodoBPlusBase.h"
#include "NodoBPlusInterno.h"
#include "NodoBPlusHoja.h"

class ArbolBPlus {

private:
    NodoBPlusBase* raiz;
    int orden;
    int accesos;

public:
    ArbolBPlus(int orden);
    ~ArbolBPlus();

    //  BUSQUEDA
    NodoGrafo* buscarNodo(int clave);
    NodoBPlusHoja* buscarHoja(int clave);

    //  INSERCIÓN
    void insertar(int clave, NodoGrafo* dato);
    void insertarEnHoja(NodoBPlusHoja* hoja, int clave, NodoGrafo* dato);
    void splitHoja(NodoBPlusHoja* hoja, int clave, NodoGrafo* dato);
    void splitInterno(NodoBPlusInterno* interno, int claveProm, NodoBPlusBase* nuevoHijo);

    //  UTILIDADES
    void reiniciarAccesos();
    int getAccesos();
    NodoBPlusBase* getRaiz();
};


#endif //TALLER___ARBOLBPLUS_H