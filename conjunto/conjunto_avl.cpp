template <typename T>
ConjuntoAVL<T>::ConjuntoAVL() {
    raiz = nullptr;
    cantidad = 0;
}

template <typename T>
int ConjuntoAVL<T>::altura(typename ConjuntoAVL<T>::Nodo* n) const {
    return n == nullptr ? 0 : n->altura;
}

template <typename T>
int ConjuntoAVL<T>::balance(typename ConjuntoAVL<T>::Nodo* n) const {
    return n == nullptr ? 0 : altura(n->izq) - altura(n->der);
}

template <typename T>
void ConjuntoAVL<T>::actualizarAltura(typename ConjuntoAVL<T>::Nodo* n) {
    int hi = altura(n->izq);
    int hd = altura(n->der);
    n->altura = 1 + (hi > hd ? hi : hd);
}

template <typename T>
typename ConjuntoAVL<T>::Nodo* ConjuntoAVL<T>::rotarDerecha(typename ConjuntoAVL<T>::Nodo* n) {
    Nodo* izq = n->izq;
    n->izq = izq->der;
    izq->der = n;
    actualizarAltura(n);
    actualizarAltura(izq);
    return izq;
}

template <typename T>
typename ConjuntoAVL<T>::Nodo* ConjuntoAVL<T>::rotarIzquierda(typename ConjuntoAVL<T>::Nodo* n) {
    Nodo* der = n->der;
    n->der = der->izq;
    der->izq = n;
    actualizarAltura(n);
    actualizarAltura(der);
    return der;
}

template <typename T>
typename ConjuntoAVL<T>::Nodo* ConjuntoAVL<T>::rebalancear(typename ConjuntoAVL<T>::Nodo* n) {
    actualizarAltura(n);
    int b = balance(n);
    if (b > 1) { // pesado a la izquierda
        if (balance(n->izq) < 0) n->izq = rotarIzquierda(n->izq); // caso izq-der
        return rotarDerecha(n);
    }
    if (b < -1) { // pesado a la derecha
        if (balance(n->der) > 0) n->der = rotarDerecha(n->der); // caso der-izq
        return rotarIzquierda(n);
    }
    return n;
}

template <typename T>
typename ConjuntoAVL<T>::Nodo* ConjuntoAVL<T>::insertar(typename ConjuntoAVL<T>::Nodo* n, const T& e, bool& insertado) {
    if (n == nullptr) {
        Nodo* nuevo = new Nodo;
        nuevo->dato = e;
        nuevo->izq = nullptr;
        nuevo->der = nullptr;
        nuevo->altura = 1;
        insertado = true;
        return nuevo;
    }
    if (e == n->dato) {
        insertado = false;
        return n;
    }
    if (e < n->dato) n->izq = insertar(n->izq, e, insertado);
    else n->der = insertar(n->der, e, insertado);
    return rebalancear(n);
}

template <typename T>
void ConjuntoAVL<T>::agregar(const T& e) {
    bool insertado = false;
    raiz = insertar(raiz, e, insertado);
    if (insertado) ++cantidad;
}

template <typename T>
typename ConjuntoAVL<T>::Nodo* ConjuntoAVL<T>::minimo(typename ConjuntoAVL<T>::Nodo* n) const {
    while (n->izq != nullptr) n = n->izq;
    return n;
}

template <typename T>
typename ConjuntoAVL<T>::Nodo* ConjuntoAVL<T>::eliminar(typename ConjuntoAVL<T>::Nodo* n, const T& e, bool& eliminado) {
    if (n == nullptr) {
        eliminado = false;
        return nullptr;
    }
    if (e < n->dato) {
        n->izq = eliminar(n->izq, e, eliminado);
    } else if (n->dato < e) {
        n->der = eliminar(n->der, e, eliminado);
    } else {
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
        Nodo* sucesor = minimo(n->der);
        n->dato = sucesor->dato;
        bool aux;
        n->der = eliminar(n->der, sucesor->dato, aux);
    }
    return rebalancear(n);
}

template <typename T>
void ConjuntoAVL<T>::quitar(const T& e) {
    bool eliminado = false;
    raiz = eliminar(raiz, e, eliminado);
    if (eliminado) --cantidad;
}

template <typename T>
bool ConjuntoAVL<T>::buscar(typename ConjuntoAVL<T>::Nodo* n, const T& e) const {
    if (n == nullptr) return false;
    if (e == n->dato) return true;
    if (e < n->dato) return buscar(n->izq, e);
    return buscar(n->der, e);
}

template <typename T>
bool ConjuntoAVL<T>::pertenece(const T& e) const {
    return buscar(raiz, e);
}

template <typename T>
int ConjuntoAVL<T>::tamanio() const {
    return cantidad;
}

template <typename T>
void ConjuntoAVL<T>::destruir(typename ConjuntoAVL<T>::Nodo* n) {
    if (n == nullptr) return;
    destruir(n->izq);
    destruir(n->der);
    delete n;
}

template <typename T>
ConjuntoAVL<T>::~ConjuntoAVL() {
    destruir(raiz);
}

template <typename T>
void ConjuntoAVL<T>::inorden(typename ConjuntoAVL<T>::Nodo* n, ConjuntoAVL<T>& destino) const {
    if (n == nullptr) return;
    inorden(n->izq, destino);
    destino.agregar(n->dato);
    inorden(n->der, destino);
}

template <typename T>
void ConjuntoAVL<T>::unionCon(const ConjuntoAVL<T>& otro) {
    inorden(otro.raiz, *this);
}
