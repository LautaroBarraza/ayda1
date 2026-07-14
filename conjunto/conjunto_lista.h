#ifndef CONJUNTO_LISTA_H
#define CONJUNTO_LISTA_H

template <typename T>
class ConjuntoLista {
public:
    ConjuntoLista();
    ~ConjuntoLista();
    void agregar(const T& e);
    void quitar(const T& e);
    bool pertenece(const T& e) const;
    int tamanio() const;
    void unionCon(const ConjuntoLista<T>& otro);

private:
    struct Nodo {
        T dato;
        Nodo* sig;
    };
    Nodo* primero;
    int cantidad;
};

#include "conjunto_lista.cpp"

#endif
