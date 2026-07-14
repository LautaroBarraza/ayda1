#include <iostream>
#include "conjunto_lista.h"
#include "conjunto_arbol_noAVL.h"
#include "conjunto_arbol_AVL.h"

template <typename Conj>
void probar(const std::string& nombre) {
    std::cout << "=== " << nombre << " ===" << std::endl;

    Conj a;
    a.agregar(5);
    a.agregar(3);
    a.agregar(8);
    a.agregar(3); // repetido, no lo va a agregar

    std::cout << "tamanio(a): " << a.tamanio() << " (esperado 3)" << std::endl;
    std::cout << "pertenece(a, 3): " << a.pertenece(3) << " (esperado 1)" << std::endl;
    std::cout << "pertenece(a, 9): " << a.pertenece(9) << " (esperado 0)" << std::endl;

    a.quitar(3);
    std::cout << "tras quitar(3), tamanio(a): " << a.tamanio() << " (esperado 2)" << std::endl;
    std::cout << "pertenece(a, 3): " << a.pertenece(3) << " (esperado 0)" << std::endl;

    Conj b;
    b.agregar(8);
    b.agregar(1);
    b.agregar(2);

    a.unionCon(b);
    std::cout << "tras unionCon(b), tamanio(a): " << a.tamanio() << " (esperado 4: 5,8,1,2)" << std::endl;

    std::cout << std::endl;
}

int main() {
    probar<ConjuntoLista<int>>("ConjuntoLista");
    probar<Conjunto<int>>("Conjunto (BST sin balancear)");
    probar<ConjuntoAVL<int>>("ConjuntoAVL");
    return 0;
}
