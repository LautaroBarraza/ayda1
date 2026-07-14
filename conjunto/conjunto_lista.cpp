// Constructor: conjunto vacio
template <typename T>
ConjuntoLista<T>::ConjuntoLista() {
    primero = nullptr;
    cantidad = 0;
}

template <typename T>
ConjuntoLista<T>::~ConjuntoLista() {
    while (primero != nullptr) {
        Nodo* aux = primero;
        primero = primero->sig;
        delete aux;
    }
}

template <typename T>
void ConjuntoLista<T>::agregar(const T& e) {
    for (Nodo* p = primero; p != nullptr; p = p->sig) { // O(n) chequeo de pertenencia
        if (p->dato == e) return;
    }
    Nodo* aux = new Nodo;
    aux->dato = e;
    aux->sig = primero;
    primero = aux;
    ++cantidad;
}

template <typename T>
void ConjuntoLista<T>::quitar(const T& e) {
    Nodo* p = primero;
    Nodo* anterior = nullptr;
    while (p != nullptr) { // O(n)
        if (p->dato == e) {
            if (anterior == nullptr) primero = p->sig;
            else anterior->sig = p->sig;
            delete p;
            --cantidad;
            return;
        }
        anterior = p;
        p = p->sig;
    }
}

template <typename T>
bool ConjuntoLista<T>::pertenece(const T& e) const {
    for (Nodo* p = primero; p != nullptr; p = p->sig) { // O(n)
        if (p->dato == e) return true;
    }
    return false;
}

template <typename T>
int ConjuntoLista<T>::tamanio() const {
    return cantidad; // O(1)
}

template <typename T>
void ConjuntoLista<T>::unionCon(const ConjuntoLista<T>& otro) { // O(n*m)
    for (Nodo* p = otro.primero; p != nullptr; p = p->sig) {
        agregar(p->dato); // agregar ya descarta duplicados (O(n) cada uno)
    }
}
