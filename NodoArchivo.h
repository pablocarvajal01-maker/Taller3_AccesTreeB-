//
// created by carva on 10 12 2025
//

#ifndef TALLER___NODOARCHIVO_H
#define TALLER___NODOARCHIVO_H

#include "NodoGrafo.h"

// guarda tamaño y tipo del archivo
// un archivo no puede tener hijos por lo que no implementa listas de hijos

class NodoArchivo : public NodoGrafo {
private:
    int tamano;
    int tipo;        // tipo de archivo por ejemplo 0 imagen 1 documento etc

public:
    NodoArchivo(int id, int tamano, int tipo);

    ~NodoArchivo();

    // metodo que indica que no es un directorio
    bool esDirectorio() override { return false; }

    int getTamano();

    int getTipo();
};

#endif //TALLER___NODOARCHIVO_H
