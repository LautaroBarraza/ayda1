#include "conjunto.h"


// Constructor (crear): secuencia vacía
template <typename T>
Conjunto<T>::Conjunto(){
 	primero=nullptr;
	cantidad=0;
}

template <typename T>
Conjunto<T>::~Conjunto() {
    while (primero != nullptr) {
        Nodo* aux = primero;
        primero = primero->sig;
        delete aux;
    }
}

template <typename T>
void Conjunto<T>::agregar(const T& e) {
for (Nodo* p=primero; p != nullptr; p=p->sig) { //O(n)pertenencia
        if (p->dato == e) return; }
	Nodo * aux= new Nodo;
      aux->dato = e;
      aux->sig=primero;
 	primero= aux;
	++cantidad;
}


template <typename T>
void Conjunto<T>::unionCon(const Conjunto<T>& otro) {  // O(n*m)
    for (Nodo* p = otro.primero; p != nullptr; p = p->sig) {
        agregar(p->dato); //agregar ya descarta duplicados (O(n) cada uno)
    }
