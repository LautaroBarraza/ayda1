#include "conjunto_arbol_noAVL.h"

template <typename T>
Conjunto<T>::Conjunto() {
    raiz = nullptr;
    cantidad = 0;
}

template <typename T>
void Conjunto<T>::destruir(typename Conjunto<T>::Nodo* n) {
    if (n == nullptr) return;
    destruir(n->izq);
    destruir(n->der);
    delete n;
}

template <typename T>
Conjunto<T>::~Conjunto() {
    destruir(raiz);
}

template <typename T>
typename Conjunto<T>::Nodo* Conjunto<T>::insertar(typename Conjunto<T>::Nodo* n, const T& e, bool& insertado) {
    if (n == nullptr) {
        Nodo* nuevo = new Nodo;
        nuevo->dato = e;
        nuevo->izq = nullptr;
        nuevo->der = nullptr;
        insertado = true;
        return nuevo;
    }
    if (e == n->dato) {
        insertado = false; // si ya existe, no se duplica
        return n;
    }
    if (e < n->dato){
        n->izq = insertar(n->izq, e, insertado);
    } 
    else {
        n->der = insertar(n->der, e, insertado);
    }
    return n;
}

template <typename T>
void Conjunto<T>::agregar(const T& e) {
    bool insertado = false;
    raiz = insertar(raiz, e, insertado);
    if (insertado){
        ++cantidad;
    } 
}

template <typename T>
typename Conjunto<T>::Nodo* Conjunto<T>::minimo(typename Conjunto<T>::Nodo* n) const {
    while (n->izq != nullptr) n = n->izq;
    return n;
}

template <typename T>
typename Conjunto<T>::Nodo* Conjunto<T>::eliminar(typename Conjunto<T>::Nodo* n, const T& e, bool& eliminado) {
    if (n == nullptr) {
        eliminado = false;
        return nullptr;
    }
    if (e < n->dato) {
        n->izq = eliminar(n->izq, e, eliminado);
        return n;
    }
    if (n->dato < e) {
        n->der = eliminar(n->der, e, eliminado);
        return n;
    }
    // encontrado: n->dato == e
    eliminado = true;
    if (n->izq == nullptr) {
        Nodo* aux = n->der;
        delete n;
        return aux;
    }
    if (n->der == nullptr) {
        Nodo* aux = n->izq;
        delete n;
        return aux;
    }
    // dos hijos: reemplazar por el sucesor inorden (minimo del subarbol derecho)
    Nodo* sucesor = minimo(n->der);
    n->dato = sucesor->dato;
    bool aux;
    n->der = eliminar(n->der, sucesor->dato, aux);
    return n;
}

template <typename T>
void Conjunto<T>::quitar(const T& e) {
    bool eliminado = false;
    raiz = eliminar(raiz, e, eliminado);
    if (eliminado) --cantidad;
}

template <typename T>
bool Conjunto<T>::buscar(typename Conjunto<T>::Nodo* n, const T& e) const {
    if (n == nullptr) return false;
    if (e == n->dato) return true;
    if (e < n->dato) return buscar(n->izq, e);
    return buscar(n->der, e);
}

template <typename T>
bool Conjunto<T>::pertenece(const T& e) const {
    return buscar(raiz, e);
}

template <typename T>
int Conjunto<T>::tamanio() const {
    return cantidad;
}

template <typename T>
void Conjunto<T>::inorden(typename Conjunto<T>::Nodo* n, Conjunto<T>& destino) const {
    if (n == nullptr) return;
    inorden(n->izq, destino);
    destino.agregar(n->dato);
    inorden(n->der, destino);
}

template <typename T>
void Conjunto<T>::unionCon(const Conjunto<T>& otro) {
    inorden(otro.raiz, *this);
}
