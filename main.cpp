#include <iostream>
#include <limits>
#include "ArbolBPlus.h"
#include "NodoBPlusHoja.h"
using namespace std;

ArbolBPlus* arbol = nullptr;


void errorMenu(int &opcion) {
    while (true) {
        cin >> opcion;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ingrese un numero válido: ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }
}

void salir() {
    cout << "Volviendo al menu anterior..." << endl;
}


void insertarClave() {
    int clave;
    cout << "Ingrese clave a insertar: ";
    errorMenu(clave);

    arbol->insertar(clave, nullptr);
    cout << "Clave insertada correctamente.\n";
}

void buscarClave() {
    int clave;
    cout << "Ingrese clave a buscar: ";
    errorMenu(clave);

    NodoGrafo* resultado = arbol->buscarNodo(clave);
    if (resultado != nullptr) {
        cout << "Clave encontrada.\n";
    } else {
        cout << "Clave NO encontrada.\n";
    }
}

void mostrarHojaRaiz() {
    NodoBPlusBase* raiz = arbol->getRaiz();

    if (!raiz->getEsHoja()) {
        cout << "La raiz actual es interna. No puedo mostrar hoja.\n";
        return;
    }

    NodoBPlusHoja* hoja = (NodoBPlusHoja*) raiz;

    cout << "[Hoja raiz] Claves: ";
    for(int i = 0; i < hoja->getNumClaves(); i++) {
        cout << hoja->getClave(i) << " ";
    }
    cout << endl;
}

void mostrarAccesos() {
    cout << "Accesos acumulados: " << arbol->getAccesos() << endl;
}


void menuArbol() {
    int opcion = 0;

    do {
        cout << "\n====== MENU ARBOL B+ ======\n";
        cout << "1) Insertar clave\n";
        cout << "2) Buscar clave\n";
        cout << "3) Mostrar hoja raiz\n";
        cout << "4) Mostrar accesos\n";
        cout << "5) Salir\n";
        cout << "Seleccione: ";
        errorMenu(opcion);

        switch(opcion) {
            case 1: insertarClave(); break;
            case 2: buscarClave(); break;
            case 3: mostrarHojaRaiz(); break;
            case 4: mostrarAccesos(); break;
            case 5: salir(); break;
            default: cout << "Opción invalida.\n";
        }

    } while(opcion != 5);
}

int main() {
    int orden;
    cout << "Ingrese el orden del arbol B+: ";
    errorMenu(orden);

    arbol = new ArbolBPlus(orden);

    menuArbol();

    delete arbol;
    return 0;
}
