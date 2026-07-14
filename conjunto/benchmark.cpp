#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <numeric>
#include "conjunto_lista.h"
#include "conjunto_bst.h"
#include "conjunto_avl.h"

using Clock = std::chrono::high_resolution_clock;

template <typename Conjunto>
double medirAgregar(const std::vector<int>& datos) {
    Conjunto c;
    auto t0 = Clock::now();
    for (int x : datos) c.agregar(x);
    auto t1 = Clock::now();
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

template <typename Conjunto>
double medirPertenece(const std::vector<int>& datos, const std::vector<int>& consultas) {
    Conjunto c;
    for (int x : datos) c.agregar(x);
    volatile bool dummy = false;
    auto t0 = Clock::now();
    for (int x : consultas) dummy = c.pertenece(x);
    auto t1 = Clock::now();
    (void)dummy;
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

template <typename Conjunto>
double medirUnion(const std::vector<int>& datosA, const std::vector<int>& datosB) {
    Conjunto a, b;
    for (int x : datosA) a.agregar(x);
    for (int x : datosB) b.agregar(x);
    auto t0 = Clock::now();
    a.unionCon(b);
    auto t1 = Clock::now();
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

int main() {
    std::ofstream out("resultados.csv");
    out << "estructura,operacion,n,escenario,tiempo_ms\n";

    std::mt19937 gen(12345);

    // Escenario "aleatorio": orden de insercion al azar (caso promedio de BST/AVL).
    std::vector<int> tamanios_aleatorio = {100, 500, 1000, 5000, 10000, 50000};
    // Escenario "ordenado": fuerza el peor caso del BST simple (se degrada a lista).
    // Se usan tamanios mas chicos porque ConjuntoLista y ConjuntoBST degradado son O(n^2).
    std::vector<int> tamanios_ordenado = {100, 500, 1000, 5000, 10000};

    auto datosAleatorios = [&](int n) {
        std::vector<int> v(n);
        std::iota(v.begin(), v.end(), 0);
        std::shuffle(v.begin(), v.end(), gen);
        return v;
    };
    auto datosOrdenados = [&](int n) {
        std::vector<int> v(n);
        std::iota(v.begin(), v.end(), 0);
        return v;
    };

    for (int n : tamanios_aleatorio) {
        auto datos = datosAleatorios(n);
        auto consultas = datosAleatorios(n); // mismas claves 0..n-1, distinto orden de consulta

        out << "ConjuntoLista,agregar," << n << ",aleatorio," << medirAgregar<ConjuntoLista<int>>(datos) << "\n";
        out << "ConjuntoBST,agregar,"   << n << ",aleatorio," << medirAgregar<ConjuntoBST<int>>(datos) << "\n";
        out << "ConjuntoAVL,agregar,"   << n << ",aleatorio," << medirAgregar<ConjuntoAVL<int>>(datos) << "\n";

        out << "ConjuntoLista,pertenece," << n << ",aleatorio," << medirPertenece<ConjuntoLista<int>>(datos, consultas) << "\n";
        out << "ConjuntoBST,pertenece,"   << n << ",aleatorio," << medirPertenece<ConjuntoBST<int>>(datos, consultas) << "\n";
        out << "ConjuntoAVL,pertenece,"   << n << ",aleatorio," << medirPertenece<ConjuntoAVL<int>>(datos, consultas) << "\n";

        auto datosB = datosAleatorios(n);
        for (auto& x : datosB) x += n / 2; // desplazado: mitad se solapa, mitad es nuevo

        out << "ConjuntoLista,unionCon," << n << ",aleatorio," << medirUnion<ConjuntoLista<int>>(datos, datosB) << "\n";
        out << "ConjuntoBST,unionCon,"   << n << ",aleatorio," << medirUnion<ConjuntoBST<int>>(datos, datosB) << "\n";
        out << "ConjuntoAVL,unionCon,"   << n << ",aleatorio," << medirUnion<ConjuntoAVL<int>>(datos, datosB) << "\n";

        std::cout << "n=" << n << " (aleatorio) listo\n";
    }

    for (int n : tamanios_ordenado) {
        auto datos = datosOrdenados(n);
        auto consultas = datosOrdenados(n);

        out << "ConjuntoLista,agregar," << n << ",ordenado," << medirAgregar<ConjuntoLista<int>>(datos) << "\n";
        out << "ConjuntoBST,agregar,"   << n << ",ordenado," << medirAgregar<ConjuntoBST<int>>(datos) << "\n";
        out << "ConjuntoAVL,agregar,"   << n << ",ordenado," << medirAgregar<ConjuntoAVL<int>>(datos) << "\n";

        out << "ConjuntoLista,pertenece," << n << ",ordenado," << medirPertenece<ConjuntoLista<int>>(datos, consultas) << "\n";
        out << "ConjuntoBST,pertenece,"   << n << ",ordenado," << medirPertenece<ConjuntoBST<int>>(datos, consultas) << "\n";
        out << "ConjuntoAVL,pertenece,"   << n << ",ordenado," << medirPertenece<ConjuntoAVL<int>>(datos, consultas) << "\n";

        std::cout << "n=" << n << " (ordenado) listo\n";
    }

    out.close();
    std::cout << "Resultados guardados en resultados.csv\n";
    return 0;
}
