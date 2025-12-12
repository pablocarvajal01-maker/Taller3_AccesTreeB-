#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include "ArbolBPlus.h"
#include "NodoBPlusHoja.h"
#include "NodoGrafo.h"
#include "NodoDirectorio.h"
#include "NodoArchivo.h"
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
    int id_nodo, id_padre = 0, tipo, tamano = 0;
    string nombre;

    cout << "\n--- CREAR NUEVO NODO (ARCHIVO/DIRECTORIO) ---\n";
    cout << "Ingrese ID (clave) del nuevo nodo: ";
    errorMenu(id_nodo);

    if (arbol->buscarNodo(id_nodo) != nullptr) {
        cout << "Error: Ya existe un nodo con el ID " << id_nodo << ". No se puede insertar.\n";
        return;
    }

    cout << "¿El nodo es (1) Directorio o (2) Archivo?: ";
    errorMenu(tipo);

    if (tipo != 1 && tipo != 2) {
        cout << "Tipo de nodo inválido. Cancelando operación.\n";
        return;
    }

    cout << "Ingrese nombre del nodo: ";
    getline(cin, nombre);

    if (tipo == 2) { // Archivo
        cout << "Ingrese tamaño del archivo: ";
        errorMenu(tamano);
    }

    cout << "¿Tiene un Directorio Padre? (0 para No, ID del padre para Sí): ";
    errorMenu(id_padre);

    NodoGrafo* nuevoNodo = nullptr;
    if (tipo == 1) {
        nuevoNodo = new NodoDirectorio(id_nodo, nombre);
    } else {
        nuevoNodo = new NodoArchivo(id_nodo, nombre, tamano);
    }

    arbol->insertar(id_nodo, nuevoNodo);
    cout << "\nNodo ID " << id_nodo << " insertado en el Árbol B+.\n";
    if (id_padre != 0) {
        NodoGrafo* nodoPadre = arbol->buscarNodo(id_padre);

        if (nodoPadre == nullptr) {
            cout << "Advertencia: ID de padre " << id_padre << " no encontrado. El nodo se creó sin enlace de padre.\n";
        } else if (!nodoPadre->esDirectorio()) {
            cout << "Advertencia: El nodo padre ID " << id_padre << " no es un Directorio. No se creó el enlace.\n";
        } else {
            NodoDirectorio* dirPadre = dynamic_cast<NodoDirectorio*>(nodoPadre);

            dirPadre->agregarHijo(id_nodo);

            nuevoNodo->getPadres()->agregar(id_padre);

            cout << "Enlace creado: " << nombre << " (" << id_nodo << ") es hijo de " << dirPadre->getNombre() << " (" << id_padre << ").\n";
        }
    } else {
        cout << "Nodo creado como raiz o sin un padre directo.\n";
    }
}

void buscarClave() {
    int clave;
    cout << "Ingrese clave a buscar: ";
    errorMenu(clave);

    NodoGrafo* resultado = arbol->buscarNodo(clave);
    if (resultado != nullptr) {
        cout << "Clave encontrada. ID: " << resultado->getId() << " (Puntero: " << resultado << ").\n";
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

bool eliminar_referencia_hijo(int id_padre, int id_hijo) {
    NodoGrafo* nodo_padre = arbol->buscarNodo(id_padre);
    NodoGrafo* nodo_hijo = arbol->buscarNodo(id_hijo);

    if (nodo_padre == nullptr || nodo_hijo == nullptr) {
        cout << "Error: Padre (" << id_padre << ") o hijo (" << id_hijo << ") no encontrado en el arbol." << endl;
        return false;
    }

    if (nodo_padre->esDirectorio()) {
        NodoDirectorio* dir_padre = (NodoDirectorio*)nodo_padre;
        if (dir_padre->eliminarHijo(id_hijo)) {
            cout << "Referencia del hijo " << id_hijo << " eliminada de la lista de hijos del padre " << id_padre << "." << endl;
        } else {
            cout << "Advertencia: El hijo " << id_hijo << " no era hijo directo del padre " << id_padre << "." << endl;
        }
    } else {
        cout << "Error: El nodo " << id_padre << " no es un directorio, no puede tener hijos." << endl;
        return false;
    }

    if (nodo_hijo->getPadres()->eliminar(id_padre)) {
        cout << "Referencia al padre " << id_padre << " eliminada de la lista de padres del hijo " << id_hijo << "." << endl;
    } else {
        cout << "Advertencia: El padre " << id_padre << " no estaba en la lista de padres del hijo " << id_hijo << "." << endl;
    }

    if (nodo_hijo->getPadres()->getCabeza() == nullptr) {
        cout << "\nEl nodo " << id_hijo << " ya no tiene padres. Eliminando completamente del arbol B+ y liberando memoria." << endl;
        arbol->eliminar(id_hijo);
        delete nodo_hijo;
        return true;
    }

    cout << "\nEl nodo " << id_hijo << " aun tiene referencias, solo se elimino el enlace." << endl;
    return false;
}

void eliminarEnlace() {
    int id_padre, id_hijo;
    cout << "\n--- ELIMINAR ENLACE/NODO ---\n";
    cout << "Ingrese ID del Directorio Padre: ";
    errorMenu(id_padre);
    cout << "Ingrese ID del Hijo a Desvincular: ";
    errorMenu(id_hijo);

    eliminar_referencia_hijo(id_padre, id_hijo);
}



bool esVisitado(int id, int* arr_visitados, int tam) {
    for (int i = 0; i < tam; i++) {
        if (arr_visitados[i] == id) {
            return true;
        }
    }
    return false;
}


void marcarVisitado(int id, int* &arr_visitados, int &tam) {
    int* nuevoArr = new int[tam + 1];
    for (int i = 0; i < tam; i++) {
        nuevoArr[i] = arr_visitados[i];
    }
    nuevoArr[tam] = id;

    if (arr_visitados != nullptr) {
        delete[] arr_visitados;
    }
    arr_visitados = nuevoArr;
    tam++;
}


long long calcularEspacioOcupadoRecursivo(int id_actual, int* &visitados, int &num_visitados) {


    if (esVisitado(id_actual, visitados, num_visitados)) {
        return 0;
    }
    marcarVisitado(id_actual, visitados, num_visitados);

    NodoGrafo* nodo = arbol->buscarNodo(id_actual);
    if (nodo == nullptr) {
        cout << "Advertencia: Nodo con ID " << id_actual << " encontrado en enlace pero no en Árbol B+.\n";
        return 0;
    }

    long long espacio_total = 0;

    if (!nodo->esDirectorio()) {
        NodoArchivo* archivo = dynamic_cast<NodoArchivo*>(nodo);
        if (archivo != nullptr) {
            espacio_total = archivo->getTamano();
        } else {
            cout << "Error de tipo: Nodo " << id_actual << " no es Directorio, pero tampoco Archivo.\n";
        }
    } else {
        NodoDirectorio* dir = (NodoDirectorio*)nodo;
        NodoHijo* actualHijo = dir->getHijos()->getCabeza();

        while (actualHijo != nullptr) {
            espacio_total += calcularEspacioOcupadoRecursivo(actualHijo->idHijo, visitados, num_visitados);
            actualHijo = actualHijo->siguiente;
        }
    }

    return espacio_total;
}

void calcularEspacio() {
    int id_directorio;
    cout << "\n--- CALCULAR ESPACIO OCUPADO ---\n";
    cout << "Ingrese ID del Directorio raíz para iniciar el calculo: ";
    errorMenu(id_directorio);

    NodoGrafo* nodo_raiz = arbol->buscarNodo(id_directorio);

    if (nodo_raiz == nullptr) {
        cout << "Error: Directorio con ID " << id_directorio << " no encontrado.\n";
        return;
    }

    if (!nodo_raiz->esDirectorio()) {
        cout << "Error: El ID " << id_directorio << " es incorrecto. Use el ID de un Directorio.\n";
        return;
    }

    int* visitados = nullptr;
    int num_visitados = 0;

    long long espacio = calcularEspacioOcupadoRecursivo(id_directorio, visitados, num_visitados);

    cout << "\n================================================\n";
    cout << "Espacio total ocupado por el Directorio " << id_directorio << " y su subgrafo:\n";
    cout << espacio << " unidades de tamaño.\n";
    cout << "Nodos visitados (evitando doble conteo por enlaces): " << num_visitados << endl;
    cout << "================================================\n";

    delete[] visitados;
}

void listarContenido() {
    int id_directorio;
    cout << "\n--- LISTAR CONTENIDO DE DIRECTORIO ---\n";
    cout << "Ingrese ID del Directorio a listar: ";
    errorMenu(id_directorio);

    NodoGrafo* nodo_padre = arbol->buscarNodo(id_directorio);

    if (nodo_padre == nullptr) {
        cout << "Error: Directorio con ID " << id_directorio << " no encontrado.\n";
        return;
    }

    if (!nodo_padre->esDirectorio()) {
        cout << "Error: El ID " << id_directorio << " no corresponde a ningun Directorio conocido, liste un Directorio.\n";
        return;
    }

    NodoDirectorio* dir_padre = dynamic_cast<NodoDirectorio*>(nodo_padre);

    cout << "\nContenido del directorio: " << dir_padre->getNombre() << " (ID: " << id_directorio << ")\n";
    cout << "---------------------------------------------------------\n";

    NodoHijo* actualHijo = dir_padre->getHijos()->getCabeza();
    int contador = 0;

    if (actualHijo == nullptr) {
        cout << "(Directorio vacio)\n";
    }

    while (actualHijo != nullptr) {
        int id_hijo = actualHijo->idHijo;

        NodoGrafo* nodo_hijo = arbol->buscarNodo(id_hijo);

        if (nodo_hijo != nullptr) {

            string tipo_str = nodo_hijo->esDirectorio() ? "DIR" : "ARCH";
            string tamano_str = "";

            if (!nodo_hijo->esDirectorio()) {
                NodoArchivo* archivo = dynamic_cast<NodoArchivo*>(nodo_hijo);
                if (archivo != nullptr) {
                    tamano_str = " (" + to_string(archivo->getTamano()) + " unidades)";
                }
            }

            cout << tipo_str << " | " << nodo_hijo->getId() << ": " << nodo_hijo->getNombre() << tamano_str << "\n";
            contador++;
        } else {
            cout << "Advertencia: ID de hijo " << id_hijo << " encontrado en enlace pero no en arbol B+.\n";
        }

        actualHijo = actualHijo->siguiente;
    }

    cout << "---------------------------------------------------------\n";
    cout << "Total de elementos listados: " << contador << "\n";
}




void eliminarContenidoRecursivo(int id_directorio_a_limpiar) {
    NodoGrafo* nodo_actual = arbol->buscarNodo(id_directorio_a_limpiar);

    if (nodo_actual == nullptr || !nodo_actual->esDirectorio()) {
        return;
    }

    NodoDirectorio* dir_actual = dynamic_cast<NodoDirectorio*>(nodo_actual);

    ListaHijos* hijos_lista = dir_actual->getHijos();
    std::vector<int> ids_a_eliminar;
    NodoHijo* temp_hijo = hijos_lista->getCabeza();

    while (temp_hijo != nullptr) {
        ids_a_eliminar.push_back(temp_hijo->idHijo);
        temp_hijo = temp_hijo->siguiente;
    }


    for (int id_hijo : ids_a_eliminar) {
        NodoGrafo* nodo_hijo = arbol->buscarNodo(id_hijo);

        if (nodo_hijo != nullptr) {
            if (nodo_hijo->esDirectorio()) {


                eliminarContenidoRecursivo(id_hijo);
            }




            cout << " -> Desvinculando y potencialmente eliminando a '" << nodo_hijo->getNombre() << "' (ID: " << id_hijo << ").\n";
            eliminar_referencia_hijo(id_directorio_a_limpiar, id_hijo);
        }
    }

    cout << "Contenido de '" << dir_actual->getNombre() << "' (ID: " << id_directorio_a_limpiar << ") limpiado.\n";
}

void eliminarDirectorio() {
    int id_directorio;
    cout << "\n--- ELIMINAR DIRECTORIO (Y TODO SU CONTENIDO) ---\n";
    cout << "Ingrese ID del Directorio a eliminar en cascada: ";
    errorMenu(id_directorio);

    NodoGrafo* nodo_a_eliminar = arbol->buscarNodo(id_directorio);

    if (nodo_a_eliminar == nullptr) {
        cout << "Error: Directorio con ID " << id_directorio << " no encontrado.\n";
        return;
    }

    if (!nodo_a_eliminar->esDirectorio()) {
        cout << "Error: El ID " << id_directorio << " es un Archivo. Use la opcion 5 para eliminar su enlace.\n";
        return;
    }

    NodoDirectorio* dir_a_eliminar = dynamic_cast<NodoDirectorio*>(nodo_a_eliminar);
    string nombre_dir = dir_a_eliminar->getNombre();

    cout << "\n[FASE 1] Limpiando contenido interno de '" << nombre_dir << "'...\n";
    eliminarContenidoRecursivo(id_directorio);

    cout << "\n[FASE 2] Eliminando el directorio '" << nombre_dir << "' de sus padres...\n";

    ListaPadres* padres_lista = nodo_a_eliminar->getPadres();
    std::vector<int> ids_padres_final;
    NodoPadre* temp_padre = padres_lista->getCabeza();

    while (temp_padre != nullptr) {
        ids_padres_final.push_back(temp_padre->idPadre);
        temp_padre = temp_padre->siguiente;
    }

    bool eliminado_finalmente = false;

    if (!ids_padres_final.empty()) {
        for (int id_padre : ids_padres_final) {
            cout << " -> Eliminando enlace con padre ID: " << id_padre << "...\n";

            eliminado_finalmente = eliminar_referencia_hijo(id_padre, id_directorio);
        }
    } else {

        cout << "El directorio '" << nombre_dir << "' (ID: " << id_directorio << ") no tenia padres. Eliminacion directa.\n";
        if (arbol->buscarNodo(id_directorio) != nullptr) {
            arbol->eliminar(id_directorio);
            delete nodo_a_eliminar;
            eliminado_finalmente = true;
            cout << "Eliminacion COMPLETA del nodo ID " << id_directorio << ".\n";
        }
    }

    if (eliminado_finalmente) {
        cout << "\n--- Directorio y todo su contenido eliminado correctamente en cascada. ---\n";
    } else {
        cout << "\n--- Directorio desvinculado de la cascada, pero aun tiene enlaces externos. ---\n";
    }
}


void menuArbol() {
    int opcion = 0;

    do {
        cout << "\n====== MENU SISTEMA DE ARCHIVOS ======\n";
        cout << "1) Insertar clave (Grafo: Crear Nodo)\n";
        cout << "2) Buscar clave (Grafo: Acceder Nodo)\n";
        cout << "3) Mostrar hoja raiz \n";
        cout << "4) Mostrar accesos\n";
        cout << "5) Eliminar Enlace (Grafo) / Nodo Completo\n";
        cout << "6) Calcular Espacio Ocupado (DFS)\n";
        cout << "7) Listar Contenido de Directorio\n";
        cout << "8) Eliminar Directorio en Cascada\n";
        cout << "9) Salir\n";
        cout << "Seleccione: ";
        errorMenu(opcion);

        switch(opcion) {
            case 1: insertarClave(); break;
            case 2: buscarClave(); break;
            case 3: mostrarHojaRaiz(); break;
            case 4: mostrarAccesos(); break;
            case 5: eliminarEnlace(); break;
            case 6: calcularEspacio(); break;
            case 7: listarContenido(); break;
            case 8: eliminarDirectorio(); break;
            case 9: salir(); break;
            default: cout << "Opcion invalida.\n";
        }

    } while(opcion != 9);
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