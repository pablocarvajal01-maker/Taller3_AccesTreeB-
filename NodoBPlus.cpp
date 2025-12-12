//
// Created by carva on 10-12-2025.
//
#include "NodoBPlus.h"
#include <functional>

// constructor base con reserva inicial de arreglo de claves, se marca si es hoja o no
NodoBPlusBase::NodoBPlusBase(int orden_, bool esHoja_)
    : orden(orden_), esHoja(esHoja_), numClaves(0) {
    claves = new int[orden_];
}

// destructor base libera solo arreglo de claves porque es lo unico asignado aqui
NodoBPlusBase::~NodoBPlusBase() {
    delete[] claves;
}

//hoja

// constructor de hoja, se crea arreglo de punteros a datos y se inicializan en null, se deja siguiente en null
NodoBHoja::NodoBHoja(int orden_) : NodoBPlusBase(orden_, true) {
    datos = new NodoGrafo*[orden_];
    for (int i = 0; i < orden_; ++i) datos[i] = nullptr;
    siguiente = nullptr;
}

// destructor de hoja libera arreglo de datos pero no destruye los nodos grafo ya que no le pertenecen
NodoBHoja::~NodoBHoja() {
    delete[] datos;
}

/* ------------------ INTERNO ------------------ */

// constructor de interno, se crea arreglo de hijos con capacidad orden mas uno, todos inicializados en null
NodoBInterno::NodoBInterno(int orden_) : NodoBPlusBase(orden_, false) {
    hijos = new NodoBPlusBase*[orden_ + 1];
    for (int i = 0; i < orden_ + 1; ++i) hijos[i] = nullptr;
}

// destructor de interno libera solo el arreglo de hijos, los hijos se liberan en destructor de arbol
NodoBInterno::~NodoBInterno() {
    delete[] hijos;
}

// constructor de arbol comienza vacio con contador de es en cero
BPlusTree::BPlusTree(int orden_) : raiz(nullptr), orden(orden_), contadorES(0) {}

// destructor del arbol si la raiz es hoja se recorre lista de hojas, si no se libera recursivamente por hijos
BPlusTree::~BPlusTree() {
    if (!raiz) return;

    // caso raiz, se avanza por el siguiente para liberar todas
    if (raiz->es_hoja()) {
        NodoBHoja* h = (NodoBHoja*)raiz;
        NodoBHoja* cur = h;
        while (cur) {
            NodoBHoja* next = cur->getSiguiente();
            delete cur;
            cur = next;
        }
    } else {
        // caso nodo interno, liberar con recorrido recursivo pre orden
        std::function<void(NodoBPlusBase*)> liberar = [&](NodoBPlusBase* nodo) {
            if (!nodo) return;
            if (nodo->es_hoja()) {
                delete nodo;
                return;
            }
            NodoBInterno* ni = (NodoBInterno*)nodo;
            NodoBPlusBase** hijos = ni->obtenerArregloHijos();
            int n = ni->getNumClaves();
            for (int i = 0; i <= n; ++i) {
                if (hijos[i]) liberar(hijos[i]);
            }
            delete ni;
        };
        liberar(raiz);
    }
}


// busca posicion exacta de clave en hoja, retorna indice o menos uno si no esta
int subprograma_buscar_en_hoja(NodoBHoja* hoja, int clave) {
    if (!hoja) return -1;
    int n = hoja->getNumClaves();
    int* claves = hoja->obtenerArregloClaves();
    for (int i = 0; i < n; ++i) {
        if (claves[i] == clave) return i;
    }
    return -1;
}

// inserta clave y dato dentro de hoja manteniendo orden, retorna uno si hay overflow
int subprograma_insertar_en_hoja(NodoBHoja* hoja, int orden, int clave, NodoGrafo* dato) {
    int n = hoja->getNumClaves();
    int* claves = hoja->obtenerArregloClaves();
    NodoGrafo** datos = hoja->obtenerArregloDatos();

    // mover hacia adelante para insertar ordenado
    int i = n - 1;
    while (i >= 0 && claves[i] > clave) {
        claves[i + 1] = claves[i];
        datos[i + 1] = datos[i];
        --i;
    }
    claves[i + 1] = clave;
    datos[i + 1] = dato;
    hoja->setNumClaves(n + 1);

    if (hoja->getNumClaves() >= orden) return 1;
    return 0;
}

// divide una hoja en dos mitades, copia elementos superiores a una nueva, enlaza la lista y devuelve nueva hoja
NodoBHoja* subprograma_split_hoja(NodoBHoja* hoja, int orden, int& clavePromovida) {
    int total = hoja->getNumClaves();
    int mitad = (total + 1) / 2;

    NodoBHoja* nueva = new NodoBHoja(orden);

    int* claves = hoja->obtenerArregloClaves();
    NodoGrafo** datos = hoja->obtenerArregloDatos();
    int idx = 0;

    for (int i = mitad; i < total; ++i) {
        nueva->obtenerArregloClaves()[idx] = claves[i];
        nueva->obtenerArregloDatos()[idx] = datos[i];
        idx++;
    }

    nueva->setNumClaves(idx);
    hoja->setNumClaves(mitad);

    // ajustar punteros de siguiente hoja
    nueva->setSiguiente(hoja->getSiguiente());
    hoja->setSiguiente(nueva);

    clavePromovida = nueva->obtenerArregloClaves()[0];
    return nueva;
}

// determina cual hijo seguir en un nodo interno segun clave, avanza hasta clave mayor
int subprograma_buscar_siguiente(NodoBInterno* interno, int clave) {
    int n = interno->getNumClaves();
    int* claves = interno->obtenerArregloClaves();
    int i = 0;
    while (i < n && clave >= claves[i]) ++i;
    return i;
}

// inserta en nodo interno moviendo claves y punteros, retorna uno si overflow
int subprograma_insertar_en_interno(NodoBInterno* interno, int orden, int idx, int clave, NodoBPlusBase* hijoDerecho) {
    int n = interno->getNumClaves();
    int* claves = interno->obtenerArregloClaves();
    NodoBPlusBase** hijos = interno->obtenerArregloHijos();

    for (int i = n - 1; i >= idx; --i) {
        claves[i + 1] = claves[i];
    }
    for (int i = n; i >= idx + 1; --i) {
        hijos[i + 1] = hijos[i];
    }

    claves[idx] = clave;
    hijos[idx + 1] = hijoDerecho;

    interno->setNumClaves(n + 1);

    if (interno->getNumClaves() >= orden) return 1;
    return 0;
}

// divide un nodo interno, promueve clave central y crea un nuevo interno con claves superiores
NodoBInterno* subprograma_split_interno(NodoBInterno* interno, int orden, int& clavePromovida) {
    int n = interno->getNumClaves();
    int mid = n / 2;

    clavePromovida = interno->obtenerArregloClaves()[mid];

    NodoBInterno* nuevo = new NodoBInterno(orden);

    int idx = 0;
    for (int i = mid + 1; i < n; ++i) {
        nuevo->obtenerArregloClaves()[idx] = interno->obtenerArregloClaves()[i];
        idx++;
    }
    nuevo->setNumClaves(idx);

    NodoBPlusBase** hijosOrig = interno->obtenerArregloHijos();
    NodoBPlusBase** hijosNuevo = nuevo->obtenerArregloHijos();

    int hidx = 0;
    for (int i = mid + 1; i <= n; ++i) {
        hijosNuevo[hidx++] = hijosOrig[i];
    }

    interno->setNumClaves(mid);
    return nuevo;
}

// operaciones principales del arbol

// busca una clave bajando por internos hasta hoja, incrementa al bajar
NodoGrafo* BPlusTree::buscar_nodo_grafo(int clave) {
    if (!raiz) return nullptr;
    NodoBPlusBase* cur = raiz;

    contadorES++;

    while (!cur->es_hoja()) {
        NodoBInterno* ni = (NodoBInterno*)cur;
        int idx = subprograma_buscar_siguiente(ni, clave);
        cur = ni->getHijo(idx);
        contadorES++;
        if (!cur) return nullptr;
    }

    NodoBHoja* hoja = (NodoBHoja*)cur;
    int pos = subprograma_buscar_en_hoja(hoja, clave);
    if (pos >= 0) return hoja->getDato(pos);

    return nullptr;
}

// inserta clave y dato, maneja split de hoja, luego split de internos en cadena, crea nueva raiz si necesario
void BPlusTree::insertar_nodo_grafo(int clave, NodoGrafo* nodo_grafo) {
    if (!raiz) {
        NodoBHoja* h = new NodoBHoja(orden);
        subprograma_insertar_en_hoja(h, orden, clave, nodo_grafo);
        raiz = h;
        return;
    }

    const int MAXH = 1024;
    NodoBPlusBase* pila[MAXH];
    int indices[MAXH];
    int altura = 0;

    NodoBPlusBase* cur = raiz;
    pila[altura] = cur;

    while (!cur->es_hoja()) {
        NodoBInterno* ni = (NodoBInterno*)cur;
        int idx = subprograma_buscar_siguiente(ni, clave);
        indices[altura] = idx;
        cur = ni->getHijo(idx);
        altura++;
        pila[altura] = cur;
    }

    NodoBHoja* hoja = (NodoBHoja*)cur;

    int posEx = subprograma_buscar_en_hoja(hoja, clave);
    if (posEx >= 0) {
        hoja->setDato(posEx, nodo_grafo);
        return;
    }

    int res = subprograma_insertar_en_hoja(hoja, orden, clave, nodo_grafo);
    if (res == 0) return;

    int clavePromovida = -1;
    NodoBHoja* nuevaHoja = subprograma_split_hoja(hoja, orden, clavePromovida);

    int nivel = altura - 1;
    NodoBPlusBase* padre = (nivel >= 0) ? pila[nivel] : nullptr; //es un if/else magico
    int idxParaInsertar = (nivel >= 0) ? indices[nivel] : 0;
    NodoBPlusBase* nodoIzq = hoja;
    NodoBPlusBase* nodoDerecho = nuevaHoja;
    int claveParaSubir = clavePromovida;

    while (true) {
        if (!padre) {
            NodoBInterno* nuevaRaiz = new NodoBInterno(orden);
            nuevaRaiz->obtenerArregloHijos()[0] = nodoIzq;
            nuevaRaiz->obtenerArregloHijos()[1] = nodoDerecho;
            nuevaRaiz->obtenerArregloClaves()[0] = claveParaSubir;
            nuevaRaiz->setNumClaves(1);
            raiz = nuevaRaiz;
            return;
        }

        NodoBInterno* pInterno = (NodoBInterno*)padre;
        int r = subprograma_insertar_en_interno(pInterno, orden, idxParaInsertar, claveParaSubir, nodoDerecho);

        if (r == 0) return;

        int claveProm2 = -1;
        NodoBInterno* nuevoInterno = subprograma_split_interno(pInterno, orden, claveProm2);

        nodoIzq = pInterno;
        nodoDerecho = nuevoInterno;
        claveParaSubir = claveProm2;

        nivel--;
        padre = (nivel >= 0) ? pila[nivel] : nullptr;
        idxParaInsertar = (nivel >= 0) ? indices[nivel] : 0;
    }
}

// imprime recorrido por hojas mostrando claves contenidas
void BPlusTree::imprimirHojas() {
    if (!raiz) {
        cout << "Arbol vacio\n";
        return;
    }

    NodoBPlusBase* cur = raiz;
    while (!cur->es_hoja()) {
        NodoBInterno* ni = (NodoBInterno*)cur;
        cur = ni->getHijo(0);
    }

    NodoBHoja* h = (NodoBHoja*)cur;
    while (h) {
        cout << "[";
        for (int i = 0; i < h->getNumClaves(); ++i) {
            cout << h->getClave(i);
            if (i+1 < h->getNumClaves()) cout << ", ";
        }
        cout << "] -> ";
        h = h->getSiguiente();
    }
    cout << "NULL\n";
}
