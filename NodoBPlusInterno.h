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
    int* claves;               // arreglo de claves
    NodoBPlusBase** hijos;     // arreglo de punteros a nodos hijos
    int cantidadClaves;        // cuántas claves están en uso
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




};



#endif //TALLER___NODOBPLUSINTERNO_H