# conjunto

Tres implementaciones del TDA Conjunto en C++:

- `ConjuntoLista` (`conjunto_lista.h`): lista enlazada.
- `Conjunto` (`conjunto_arbol_noAVL.h`): arbol binario de busqueda sin balanceo.
- `ConjuntoAVL` (`conjunto_arbol_AVL.h`): arbol AVL (autobalanceado).

Las tres exponen la misma interfaz: `agregar`, `quitar`, `pertenece`, `tamanio`, `unionCon`.

## Bajar el repo

```bash
git clone git@github.com:LautaroBarraza/ayda1.git
cd ayda1
```

## Probar el codigo

`main.cpp` prueba las tres implementaciones con la misma bateria de casos.

```bash
g++ main.cpp
./a.out
```
