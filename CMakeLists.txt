cmake_minimum_required(VERSION 4.0)
project(Taller__)

set(CMAKE_CXX_STANDARD 20)

add_executable(Taller__ main.cpp
        ArregloDinamico.h
        NodoGrafo.h
        NodoGrafo.cpp
        NodoDirectorio.h
        NodoDirectorio.cpp
        NodoArchivo.h
        NodoArchivo.cpp
        NodoBPlusBase.h
        NodoBPlusBase.cpp
        NodoBPlusInterno.h
        NodoBPlusInterno.cpp
        NodoBPlusHoja.h
        NodoBPlusHoja.cpp
        ArbolBPlus.h
        ArbolBPlus.cpp
)
