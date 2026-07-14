#include <iostream>
#include <cassert>
#include "conjunto_lista.h"
#include "conjunto_bst.h"
#include "conjunto_avl.h"

template <typename Conjunto>
void probar(const char* nombre) {
    std::cout << "== " << nombre << " ==\n";
    Conjunto c;
    assert(c.tamanio() == 0);

    c.agregar(5);
    c.agregar(3);
    c.agregar(8);
    c.agregar(3); // duplicado, no debe sumar
    assert(c.tamanio() == 3);
    assert(c.pertenece(5));
    assert(c.pertenece(3));
    assert(c.pertenece(8));
    assert(!c.pertenece(99));

    c.quitar(3);
    assert(c.tamanio() == 2);
    assert(!c.pertenece(3));
    c.quitar(3); // quitar algo que no existe no debe romper ni bajar el tamanio
    assert(c.tamanio() == 2);

    Conjunto otro;
    otro.agregar(8);  // repetido con c
    otro.agregar(1);
    otro.agregar(9);
    c.unionCon(otro);
    assert(c.tamanio() == 4); // 5,8,1,9
    assert(c.pertenece(1));
    assert(c.pertenece(9));

    std::cout << "OK - tamanio final: " << c.tamanio() << "\n";
}

int main() {
    probar<ConjuntoLista<int>>("ConjuntoLista");
    probar<ConjuntoBST<int>>("ConjuntoBST");
    probar<ConjuntoAVL<int>>("ConjuntoAVL");
    std::cout << "Todas las pruebas pasaron.\n";
    return 0;
}
