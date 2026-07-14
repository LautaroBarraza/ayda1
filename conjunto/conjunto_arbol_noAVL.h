#ifndef CONJUNTO_BST_H
#define CONJUNTO_BST_H

/*TDA Conjunto implementado con arbol binario de busqueda SIN balanceo.
    Requiere que T tenga operator< y operator== (orden total).
    agregar/quitar/pertenece: O(h), h = altura del arbol.
    - Mejor/promedio caso: O(log n).
    - Peor caso (insertar datos ya ordenados): O(n).
    tamanio: O(1). unionCon: O(m*h).
*/
template <typename T>
class Conjunto {
public:
    Conjunto();
    ~Conjunto();
    void agregar(const T& e);
    void quitar(const T& e);
    bool pertenece(const T& e) const;
    int tamanio() const;
    void unionCon(const Conjunto<T>& otro);

private:
    struct Nodo {
        T dato;
        Nodo* izq;
        Nodo* der;
    };
    Nodo* raiz;
    int cantidad;

    Nodo* insertar(Nodo* n, const T& e, bool& insertado);
    Nodo* eliminar(Nodo* n, const T& e, bool& eliminado);
    Nodo* minimo(Nodo* n) const;
    bool buscar(Nodo* n, const T& e) const;
    void destruir(Nodo* n);
    void inorden(Nodo* n, Conjunto<T>& destino) const;
};

#include "conjunto_arbol_noAVL.cpp"

#endif
