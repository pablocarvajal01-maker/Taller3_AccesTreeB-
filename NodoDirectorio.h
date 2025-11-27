#ifndef NODODIRECTORIO_H
#define NODODIRECTORIO_H

#include "NodoGrafo.h"

class NodoHijo;
class ListaHijos;

class NodoDirectorio : public NodoGrafo {
private:
    ListaHijos* hijos;

public:
    NodoDirectorio(int id);
    ~NodoDirectorio();

    bool esDirectorio() override { return true; }

    ListaHijos* getHijos();
    void agregarHijo(int idHijo);
    void eliminarHijo(int idHijo);
};





class NodoHijo {
public:
    int idHijo;
    NodoHijo* siguiente;

    NodoHijo(int id) : idHijo(id), siguiente(nullptr) {}
    ~NodoHijo() {}
};

class ListaHijos {
private:
    NodoHijo* cabeza;

public:
    ListaHijos() : cabeza(nullptr) {}
    ~ListaHijos() {
        NodoHijo* actual = cabeza;
        while(actual) {
            NodoHijo* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }

    NodoHijo* getCabeza() { return cabeza; }

    void agregar(int idHijo) {
        NodoHijo* nuevo = new NodoHijo(idHijo);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
    }

    bool eliminar(int idHijo) {
        NodoHijo* actual = cabeza;
        NodoHijo* anterior = nullptr;

        while(actual && actual->idHijo != idHijo) {
            anterior = actual;
            actual = actual->siguiente;
        }

        if(!actual) return false;

        if(!anterior) cabeza = actual->siguiente;
        else anterior->siguiente = actual->siguiente;

        delete actual;
        return true;
    }
};

#endif
