//
// created by carva on 10 12 2025
//

#ifndef TALLER___NODOARCHIVO_H
#define TALLER___NODOARCHIVO_H

#include "NodoGrafo.h"

class NodoArchivo : public NodoGrafo {
private:
    string nombre;
    int tamano;
    int tipo;

public:
    NodoArchivo(int id, string nombre, int tamano, int tipo = 0);

    ~NodoArchivo();

    bool esDirectorio() override { return false; }

    int getTamano();

    int getTipo();

    string getNombre() override { return nombre; }
};

#endif //TALLER___NODOARCHIVO_H
