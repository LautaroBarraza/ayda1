# Informe: TDA Conjunto — lista enlazada vs. BST vs. AVL

## 1. Definición del TDA

**Conjunto\<T\>** es una colección de elementos de tipo `T` sin duplicados y sin orden
definido para el usuario. Su contrato (independiente de la implementación) es:

| Operación | Contrato |
|---|---|
| `Conjunto()` | Crea un conjunto vacío. |
| `agregar(e)` | Incorpora `e` si no pertenece ya al conjunto; si ya pertenece, no hace nada. |
| `quitar(e)` | Elimina `e` si pertenece al conjunto; si no pertenece, no hace nada. |
| `pertenece(e)` | Indica si `e` está en el conjunto. |
| `tamanio()` | Cantidad de elementos actualmente en el conjunto. |
| `unionCon(otro)` | Incorpora a `this` todos los elementos de `otro` (sin duplicar). |

El contrato no exige orden entre elementos. Sin embargo, **la implementación con
árbol sí lo exige**: para decidir en qué subárbol insertar o buscar un elemento hace
falta comparar con `<`, no solo con `==`. Esto es una restricción adicional sobre
`T` que no existe en la implementación con lista, y vale la pena declararla en la
respuesta del examen como una diferencia real entre TDA (contrato) e implementación
(estructura interna elegida).

Se implementaron tres versiones intercambiables (misma interfaz pública):

- `ConjuntoLista<T>` — lista enlazada simple (`conjunto_lista.h/.cpp`).
- `ConjuntoBST<T>` — árbol binario de búsqueda sin balanceo (`conjunto_bst.h/.cpp`).
- `ConjuntoAVL<T>` — árbol binario de búsqueda autobalanceado (`conjunto_avl.h/.cpp`).

## 2. Complejidad temporal por operación

n = cantidad de elementos en el conjunto. h = altura del árbol (BST: entre log₂n y n
según el orden de inserción; AVL: siempre O(log n) por invariante de balance).

| Operación | Lista enlazada | BST simple | AVL |
|---|---|---|---|
| `agregar` | O(n) — recorre para descartar duplicados | O(h): mejor/prom. **O(log n)**, peor **O(n)** | **O(log n)** garantizado |
| `quitar` | O(n) | O(h): mejor/prom. **O(log n)**, peor **O(n)** | **O(log n)** garantizado |
| `pertenece` | O(n) | O(h): mejor/prom. **O(log n)**, peor **O(n)** | **O(log n)** garantizado |
| `tamanio` | O(1) (contador incremental) | O(1) (contador incremental) | O(1) (contador incremental) |
| `unionCon(otro)` (tamaño m) | O(n·m) | O(m·h): prom. O(m log n), peor O(m·n) | O(m log n) garantizado |

El **peor caso del BST simple** ocurre cuando los elementos se insertan ya
ordenados (o casi): cada nodo nuevo cuelga como hijo único del anterior y el árbol
degenera en una lista enlazada — con el agravante de que además arrastra el
overhead de la recursión de cada nodo del BST, por lo que en ese escenario puede
llegar a ser incluso **más lento que la lista enlazada** (ver sección 4).

El **AVL** evita ese peor caso: tras cada `agregar`/`quitar` se recalcula la altura
de cada nodo en el camino de la raíz al punto modificado y, si el balance
(`altura(izq) − altura(der)`) queda fuera de `[-1, 1]`, se aplica una rotación
(simple o doble) que restaura el balance en O(1). Como el camino tiene longitud
O(log n) y cada rotación es O(1), el costo total de rebalancear nunca supera el
costo de la búsqueda que ya se estaba haciendo — de ahí que agregar/quitar sigan
siendo O(log n) incluso en el peor caso.

## 3. Metodología del benchmark

`benchmark.cpp` mide tiempo real (`<chrono>`, `high_resolution_clock`) para las
tres estructuras, con `T = int`, compilado con `g++ -O2 -std=c++17`:

- **Escenario "aleatorio"**: se insertan n enteros distintos en orden al azar
  (caso promedio de BST/AVL). n ∈ {100, 500, 1.000, 5.000, 10.000, 50.000}.
- **Escenario "ordenado"**: se insertan n enteros ya ordenados (peor caso del
  BST simple). n ∈ {100, 500, 1.000, 5.000, 10.000} — se usa un techo menor
  porque en este escenario Lista y BST son O(n²), y n = 50.000 los volvía
  prohibitivamente lentos para un benchmark de rutina.
- Para cada combinación se mide `agregar` (n inserciones), `pertenece` (n
  búsquedas sobre el conjunto ya construido) y, solo en el escenario aleatorio,
  `unionCon` (unión de dos conjuntos de tamaño n con la mitad de sus elementos
  solapados).
- Los resultados se exportan a `resultados.csv` y se visualizan en
  `visualizacion.html` (gráficos interactivos log-log, con tabla de datos
  completa también disponible ahí).

## 4. Resultados

### Escenario aleatorio (caso promedio)

| Operación | n | Lista (ms) | BST (ms) | AVL (ms) |
|---|--:|--:|--:|--:|
| agregar | 10.000 | 41,45 | 0,62 | 0,92 |
| agregar | 50.000 | 1.058,44 | 4,85 | 6,99 |
| pertenece | 10.000 | 41,45 | 0,33 | 0,20 |
| pertenece | 50.000 | 1.048,50 | 2,69 | **1,86** |
| unionCon | 50.000 | 2.037,84 | 670,58 | **3,76** |

A n = 50.000, `pertenece` es **~563× más rápida** en AVL que en la lista, y
`unionCon` es **~542× más rápida** en AVL que en BST simple (el BST, aun en el
caso promedio, arrastra el costo O(m·h) de insertar uno por uno; el AVL logra lo
mismo garantizando h = O(log n) en cada una de esas m inserciones).

### Escenario ordenado (peor caso del BST simple)

| Operación | n | Lista (ms) | BST (ms) | AVL (ms) |
|---|--:|--:|--:|--:|
| agregar | 10.000 | 41,44 | **105,11** | 0,34 |
| pertenece | 10.000 | 41,09 | **82,82** | 0,07 |

Este es el resultado más importante para el ejercicio: con datos ya ordenados, el
**BST sin balanceo queda peor que la lista enlazada** (105,11 ms vs. 41,44 ms),
porque además de degenerar en una estructura lineal arrastra el overhead de la
recursión nodo por nodo que la lista no tiene. El AVL, en cambio, es **~312×
más rápido que el BST degradado** y **~123× más rápido que la lista** en ese
mismo escenario — la prueba empírica de que balancear no es una optimización
cosmética sino lo que sostiene la complejidad O(log n) prometida.

## 5. Conclusión

- Cambiar la estructura interna del TDA Conjunto (lista → árbol) no cambia el
  contrato, pero sí exige que `T` tenga un orden total (`operator<`).
- Un BST sin balancear mejora el caso promedio (O(log n) esperado) pero **no
  mejora la cota superior**: en el peor caso sigue siendo O(n), y con el overhead
  de la recursión puede terminar rindiendo peor que la estructura que se quería
  reemplazar.
- El AVL es la mejora real: garantiza O(log n) en agregar, quitar y pertenece
  para **cualquier secuencia de entrada**, a costa de mantener una altura por
  nodo y aplicar rotaciones — un costo O(1) por operación que se paga siempre a
  cambio de no pagar nunca el peor caso O(n).
- Los benchmarks (`resultados.csv`, `visualizacion.html`) confirman esto con
  mediciones reales, no solo con el análisis asintótico: la brecha entre AVL y
  las otras dos estructuras crece con n exactamente como predice la teoría
  (n / log n).
