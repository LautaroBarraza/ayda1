# Diseño: Conjunto con BST/AVL para mejorar tiempos computacionales

## Contexto

Ejercicio de examen de Análisis y Diseño de Algoritmos. Ya existe una implementación
del TDA `Conjunto<T>` basada en lista enlazada simple (`conjunto.h`/`conjunto.cpp`),
con constructor, destructor y `agregar` implementados (O(n) por la verificación de
duplicados), y las firmas de `quitar`, `pertenece`, `tamanio` y `unionCon` declaradas
pero incompletas.

El objetivo de esta resolución es mostrar, como parte de la respuesta del examen,
cómo cambiar la estructura interna del TDA (lista → árbol binario de búsqueda) mejora
la complejidad temporal de las operaciones, comparando además un BST simple (no
balanceado) contra un AVL (autobalanceado), con evidencia empírica (benchmarks) y una
visualización interactiva.

## Alcance

Tres implementaciones independientes del mismo TDA `Conjunto<T>`, intercambiables por
tener la misma interfaz pública, más una herramienta de benchmark, una visualización
interactiva y un informe.

**Fuera de alcance:** persistencia en disco, concurrencia/thread-safety, balanceo
distinto de AVL (ej. Red-Black), y operaciones de conjunto más allá de `unionCon`
(intersección/diferencia no se piden).

## Estructura de archivos

Todo en `algortimos/conjunto/`, sin subcarpetas:

```
conjunto/
  conjunto_lista.h / .cpp   # baseline: lista enlazada (completa el original)
  conjunto_bst.h / .cpp     # BST simple, no balanceado
  conjunto_avl.h / .cpp     # BST autobalanceado (AVL)
  benchmark.cpp             # mide tiempos reales, exporta CSV
  resultados.csv            # salida del benchmark (input para el HTML)
  visualizacion.html        # visualización interactiva (Artifact)
  informe.md                # informe con tabla de complejidades + conclusiones
```

Los archivos `conjunto.h`/`conjunto.cpp` originales se dejan como están (o se renombran
a `conjunto_lista.h/.cpp`, completando las operaciones faltantes) para servir de
baseline real en el benchmark — no tendría sentido comparar contra una implementación
incompleta.

## TDA común (interfaz)

Las tres clases (`ConjuntoLista`, `ConjuntoBST`, `ConjuntoAVL`) implementan la misma
interfaz pública para ser intercambiables en el benchmark:

```cpp
template <typename T>
class ConjuntoX {
public:
    ConjuntoX();
    ~ConjuntoX();
    void agregar(const T& e);
    void quitar(const T& e);
    bool pertenece(const T& e) const;
    int tamanio() const;
    void unionCon(const ConjuntoX<T>& otro);
};
```

**Diferencia conceptual importante a documentar en el informe:** al pasar de lista a
árbol, `T` deja de necesitar solo `operator==` y pasa a necesitar un orden total
(`operator<`). El TDA Conjunto en sí no es un tipo ordenado, pero la implementación
con árbol exige orden sobre `T` para decidir en qué subárbol insertar/buscar.

## ConjuntoBST (árbol simple, no balanceado)

- Nodo: `{ T dato; Nodo* izq; Nodo* der; }`.
- `agregar`: inserción recursiva estándar, O(h) donde h es la altura del árbol.
- `pertenece`: búsqueda recursiva, O(h).
- `quitar`: borrado estándar con los 3 casos (nodo hoja, un hijo, dos hijos —
  reemplazo por sucesor inorden).
- `tamanio`: contador `cantidad` mantenido de forma incremental, O(1) (no recorre el
  árbol).
- `unionCon`: recorrido inorden de `otro`, insertando cada elemento en `this`
  — O(m·h) donde m es el tamaño de `otro`.
- Complejidad a documentar: mejor/promedio caso O(log n) por operación, peor caso
  O(n) cuando el árbol degenera en una lista (ej. insertar datos ya ordenados).

## ConjuntoAVL (autobalanceado)

- Igual estructura que ConjuntoBST, pero cada nodo guarda además `altura`.
- `agregar` y `quitar` reequilibran el árbol con rotaciones (simple izq/der, doble
  izq-der/der-izq) tras cada modificación, manteniendo `|altura(izq) - altura(der)| ≤ 1`
  en todo nodo.
- Garantiza O(log n) en **todos los casos** (mejor, promedio y peor) para `agregar`,
  `quitar` y `pertenece` — es el punto central para mostrar la mejora real frente al
  BST simple, que sí puede degradar a O(n).

## Benchmark (`benchmark.cpp`)

- Para tamaños de n crecientes (100, 500, 1000, 5000, 10000, 50000):
  - Genera n enteros aleatorios, y por separado una secuencia ya ordenada (para
    forzar el peor caso del BST simple).
  - Mide con `<chrono>` el tiempo de `agregar` (n inserciones), `pertenece` (n
    búsquedas) y `unionCon` (unión de dos conjuntos de tamaño n), para las tres
    estructuras.
- Exporta `resultados.csv` con columnas:
  `estructura, operacion, n, escenario, tiempo_ms`
  donde `escenario` es `aleatorio` u `ordenado`.
- Se compila y ejecuta de forma independiente del resto (`g++ benchmark.cpp -o benchmark`).

## Visualización interactiva (`visualizacion.html`)

- Página HTML autocontenida (los datos de `resultados.csv` se embeben directamente en
  el HTML, ya que un Artifact no puede hacer fetch a archivos externos).
- Gráficos: tiempo vs n por estructura y operación (líneas comparativas), separando
  claramente el escenario `aleatorio` del `ordenado`.
- El punto clave a resaltar visualmente: en el escenario `ordenado`, la curva del BST
  simple se dispara hacia O(n) mientras que la del AVL se mantiene logarítmica.
- Se sigue la skill `dataviz` para paleta y estilo antes de escribir el HTML.

## Informe (`informe.md`)

- Definición del TDA Conjunto (operaciones y su contrato, independiente de la
  implementación).
- Tabla comparativa de complejidad (mejor/promedio/peor caso) para `agregar`,
  `quitar`, `pertenece`, `tamanio`, `unionCon` en las tres implementaciones.
- Explicación breve de por qué el BST simple puede degradar y cómo el AVL lo evita
  mediante rotaciones.
- Conclusión con los resultados concretos del benchmark (tabla o resumen de
  `resultados.csv`).

## Testing / verificación

- Cada clase (`ConjuntoLista`, `ConjuntoBST`, `ConjuntoAVL`) se prueba con un `main`
  de prueba simple (o casos ad-hoc) que verifique: agregar sin duplicados, quitar
  elementos existentes/inexistentes, pertenece, tamanio consistente tras
  agregar/quitar, y unionCon sin duplicados. No se requiere framework de testing
  formal (no hay uno instalado en este proyecto de examen); alcanza con verificación
  manual por consola antes de correr el benchmark completo.
- El benchmark en sí sirve como verificación adicional de que las tres estructuras
  producen tamaños finales consistentes para la misma entrada.
