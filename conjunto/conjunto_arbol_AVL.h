#ifndef CONJUNTO_AVL_H
#define CONJUNTO_AVL_H

/* TDA Conjunto implementado con arbol AVL (BST autobalanceado).
    Requiere que T tenga operator< y operator== (orden total).
    agregar/quitar/pertenece: O(log n) en TODOS los casos,
    gracias a las rotaciones que mantienen |altura(izq)-altura(der)| <= 1
    en cada nodo. tamanio: O(1). unionCon: O(m*log n).
 */
template <typename T>
class ConjuntoAVL {
public:
    ConjuntoAVL();
    ~ConjuntoAVL();
    void agregar(const T& e);
    void quitar(const T& e);
    bool pertenece(const T& e) const;
    int tamanio() const;
    void unionCon(const ConjuntoAVL<T>& otro);

private:
    struct Nodo {
        T dato;
        Nodo* izq;
        Nodo* der;
        int altura;
    };
    Nodo* raiz;
    int cantidad;

    int altura(Nodo* n) const;
    int balance(Nodo* n) const;
    void actualizarAltura(Nodo* n);
    Nodo* rotarDerecha(Nodo* n);
    Nodo* rotarIzquierda(Nodo* n);
    Nodo* rebalancear(Nodo* n);

    Nodo* insertar(Nodo* n, const T& e, bool& insertado);
    Nodo* eliminar(Nodo* n, const T& e, bool& eliminado);
    Nodo* minimo(Nodo* n) const;
    bool buscar(Nodo* n, const T& e) const;
    void destruir(Nodo* n);
    void inorden(Nodo* n, ConjuntoAVL<T>& destino) const;
};

#include "conjunto_arbol_AVL.cpp"

#endif
