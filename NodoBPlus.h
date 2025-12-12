//
// Created by carva on 10-12-2025.
//

#ifndef TALLER___NODOBPLUS_H
#define TALLER___NODOBPLUS_H

#include "NodoGrafo.h"
#include <iostream>
using namespace std;


class NodoBPlusBase {
protected:
    int orden;        // indica el factor del arbol b plus
    bool esHoja;      // indica si este nodo es hoja
    int* claves;      // arreglo con las claves
    int numClaves;    // cantidad de claves usadas

public:
    NodoBPlusBase(int orden_, bool esHoja_);
    virtual ~NodoBPlusBase();

    bool es_hoja() { return esHoja; }
    int getNumClaves() { return numClaves; }
    void setNumClaves(int n) { numClaves = n; }

    int getClave(int idx) { return claves[idx]; }
    void setClave(int idx, int val) { claves[idx] = val; }

    // entrega acceso directo al arreglo de claves
    int* obtenerArregloClaves() { return claves; }
};

// nodo hoja del arbol b plus aqui vienen los datos reales
class NodoBHoja : public NodoBPlusBase {
private:
    NodoGrafo** datos;   // arreglo con punteros a nodogrefo asociados a cada clave
    NodoBHoja* siguiente; // puntero a la hoja siguiente para recorrido secuencial

public:
    NodoBHoja(int orden_);
    ~NodoBHoja();

    NodoGrafo* getDato(int idx) { return datos[idx]; }
    void setDato(int idx, NodoGrafo* d) { datos[idx] = d; }
    NodoBHoja* getSiguiente() { return siguiente; }
    void setSiguiente(NodoBHoja* s) { siguiente = s; }

    // entrega acceso al arreglo de datos
    NodoGrafo** obtenerArregloDatos() { return datos; }
};

// nodo interno con claves y punteros hijos
class NodoBInterno : public NodoBPlusBase {
private:
    NodoBPlusBase** hijos;  // arreglo de punteros a otros nodos

public:
    NodoBInterno(int orden_);
    ~NodoBInterno();

    NodoBPlusBase* getHijo(int idx) { return hijos[idx]; }
    void setHijo(int idx, NodoBPlusBase* h) { hijos[idx] = h; }


    NodoBPlusBase** obtenerArregloHijos() { return hijos; }
};

// arbol b plus principal maneja raiz inserciones y busquedas
class BPlusTree {
private:
    NodoBPlusBase* raiz;  // raiz del arbol
    int orden;            // orden global del arbol
    int contadorES;       // contador de operaciones es simula accesos a disco

public:
    BPlusTree(int orden_);
    ~BPlusTree();

    void insertar_nodo_grafo(int clave, NodoGrafo* nodo_grafo);
    NodoGrafo* buscar_nodo_grafo(int clave);

    int getContadorES() { return contadorES; }
    void resetContadorES() { contadorES = 0; }


    void imprimirHojas();

    // subprogramas amigos para permitir acceso interno
    friend NodoBHoja* subprograma_split_hoja(NodoBHoja* hoja, int orden, int& clavePromovida);
    friend int subprograma_insertar_en_hoja(NodoBHoja* hoja, int orden, int clave, NodoGrafo* dato);
    friend int subprograma_buscar_en_hoja(NodoBHoja* hoja, int clave);
    friend int subprograma_buscar_siguiente(NodoBInterno* interno, int clave);
    friend NodoBInterno* subprograma_split_interno(NodoBInterno* interno, int orden, int& clavePromovida);
    friend int subprograma_insertar_en_interno(NodoBInterno* interno, int orden, int idx, int clave, NodoBPlusBase* hijoDerecho);
};


#endif //TALLER___NODOBPLUS_H
