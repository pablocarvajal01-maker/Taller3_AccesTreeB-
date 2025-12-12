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
    virtual string getNombre() = 0;
};

// nodo de una lista enlazada que guarda el id de un padre
class NodoPadre {
public:
    int idPadre; // id del nodo padre
    NodoPadre* siguiente; // puntero al siguiente elemento de la lista

    NodoPadre(int id) : idPadre(id), siguiente(nullptr) {} // constructor que inicializa el id y deja siguiente como nulo
    ~NodoPadre() {} // destructor vacio
};

// lista enlazada de ids de padres de un nodo
class ListaPadres {
private:
    NodoPadre* cabeza; // puntero al primer nodo padre de la lista

public:
    ListaPadres() : cabeza(nullptr) {} // constructor que inicia la lista vacia

    // destructor que elimina uno por uno todos los nodos de la lista
    ~ListaPadres() {
        NodoPadre* actual = cabeza;
        while(actual) {
            NodoPadre* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }

    NodoPadre* getCabeza() { return cabeza; } // retorna el nodo cabeza

    // agrega un padre al inicio de la lista enlazada
    void agregar(int idPadre) {
        NodoPadre* nuevo = new NodoPadre(idPadre); // se crea un nuevo nodo padre
        nuevo->siguiente = cabeza; // se enlaza al antiguo inicio
        cabeza = nuevo; // nuevo nodo pasa a ser la cabeza
    }

    // elimina un padre cuyo id coincida
    bool eliminar(int idPadre) {
        NodoPadre* actual = cabeza; // nodo actual durante la busqueda
        NodoPadre* anterior = nullptr; // nodo previo para poder reconectar la lista

        // se recorre la lista hasta encontrar coincidencia
        while(actual && actual->idPadre != idPadre) {
            anterior = actual;
            actual = actual->siguiente;
        }

        if(!actual) return false; // si no se encontro se retorna false

        // si el elemento esta al inicio se mueve la cabeza
        if(!anterior) cabeza = actual->siguiente;
        else anterior->siguiente = actual->siguiente; // si no se enlaza el anterior con el siguiente

        delete actual; // se elimina el nodo padre
        return true;
    }
};

#endif
