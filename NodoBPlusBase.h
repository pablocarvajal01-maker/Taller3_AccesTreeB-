//
// Created by carva on 11-12-2025.
//

#ifndef TALLER___NODOBPLUSBASE_H
#define TALLER___NODOBPLUSBASE_H

class NodoBPlusBase {

protected:
    int* claves;            // arreglo dinámico de claves
    int orden;              // máximo de claves
    int numClaves;          // cuántas claves tiene
    bool esHoja;            // indica si es hoja o no
    NodoBPlusBase* padre;   // puntero al nodo padre


public:
    NodoBPlusBase(int orden, bool esHoja);
    virtual ~NodoBPlusBase();

    // getters simples
    int getNumClaves();
    int getClave(int index);
    bool getEsHoja();
    int getOrden();

    // setters simples
    void setClave(int index, int valor);
    void setNumClaves(int n);
    void incrementarNumClaves();

    // utilidades necesarias para el Árbol B+
    bool estaLlena();
    NodoBPlusBase* getPadre();
    void setPadre(NodoBPlusBase* p);
};

#endif //TALLER___NODOBPLUSBASE_H
