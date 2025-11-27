#ifndef NODOGRAFO_H
#define NODOGRAFO_H

#include <string>
using namespace std;

class NodoPadre;
class ListaPadres;

class NodoGrafo {
protected:
    int id;
    ListaPadres* padres;

public:
    NodoGrafo(int id);
    virtual ~NodoGrafo();

    int getId();
    ListaPadres* getPadres();

    virtual bool esDirectorio() = 0;
};

class NodoPadre {
public:
    int idPadre;
    NodoPadre* siguiente;

    NodoPadre(int id) : idPadre(id), siguiente(nullptr) {}
    ~NodoPadre() {}
};

class ListaPadres {
private:
    NodoPadre* cabeza;

public:
    ListaPadres() : cabeza(nullptr) {}
    ~ListaPadres() {
        NodoPadre* actual = cabeza;
        while(actual) {
            NodoPadre* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }

    NodoPadre* getCabeza() { return cabeza; }

    void agregar(int idPadre) {
        NodoPadre* nuevo = new NodoPadre(idPadre);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
    }

    bool eliminar(int idPadre) {
        NodoPadre* actual = cabeza;
        NodoPadre* anterior = nullptr;

        while(actual && actual->idPadre != idPadre) {
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
