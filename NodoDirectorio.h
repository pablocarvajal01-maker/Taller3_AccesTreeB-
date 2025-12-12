#ifndef NODODIRECTORIO_H
#define NODODIRECTORIO_H

#include "NodoGrafo.h"

class NodoHijo;
class ListaHijos;


class NodoDirectorio : public NodoGrafo {
private:
    string nombre;
    ListaHijos* hijos;

public:
    NodoDirectorio(int id, string nombre);

    ~NodoDirectorio();
    bool esDirectorio() override { return true; }

    ListaHijos* getHijos();

    void agregarHijo(int idHijo);

    bool eliminarHijo(int idHijo);

    string getNombre() override { return nombre; }
};


// nodo individual de la lista de hijos
class NodoHijo {
public:
    int idHijo;            // id del nodo hijo srchivo o directorio
    NodoHijo* siguiente;   // siguiente nodo en la lista

    // constructor simple
    NodoHijo(int id) : idHijo(id), siguiente(nullptr) {}

    // destructor vacío
    ~NodoHijo() {}
};



// contenedor de nodos hijo, lista enlazada simple
class ListaHijos {
private:
    NodoHijo* cabeza;  // primer elemento de la lista

public:
    // constructor, lista vacía
    ListaHijos() : cabeza(nullptr) {}
//
    // destructor, recorre y borra toda la lista
    ~ListaHijos() {
        NodoHijo* actual = cabeza;
        while(actual) {
            NodoHijo* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }

    // retorna el primer nodo
    NodoHijo* getCabeza() { return cabeza; }

    // inserta un hijo al inicio de la lista
    void agregar(int idHijo) {
        NodoHijo* nuevo = new NodoHijo(idHijo);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
    }

    // elimina un nodo por id
    bool eliminar(int idHijo) {
        NodoHijo* actual = cabeza;
        NodoHijo* anterior = nullptr;

        // busca el nodo que coincide con el id
        while(actual && actual->idHijo != idHijo) {
            anterior = actual;
            actual = actual->siguiente;
        }

        // no se encontró el hijo
        if(!actual) return false;

        // eliminar al inicio
        if(!anterior) cabeza = actual->siguiente;
        // eliminar en medio/final
        else anterior->siguiente = actual->siguiente;

        delete actual;
        return true;
    }
};

#endif
