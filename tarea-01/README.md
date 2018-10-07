# T1-CC7515

Tarea N°1 curso Computación en GPU CI7515, uchile. Para ejecutar cargar el proyecto en CLion y ejecutar los distintos casos de prueba.

1. **TEST-HEDGE**: Testea la estructura HalEdge.
2. **TEST-HEDGE-OFF**: Testea la estructura HalEdge cargando un archivo OFF.
3. **TEST-POINT**: Testea la clase Point&lt;T>.
4. **TEST-POLYGON**: Testea la clase Polygon&lt;T>.
5. **TEST-SEGMENT**: Testea la clase Segment&lt;T>.
6. **TEST-VECTOR**: Testea la clase Vector&lt;T>.

## Punto

Para crear un punto en 2D o 3D:

```cpp
#include "elements/point.h"

Point<int> p1 = Point<int>(0, 1); // Punto coordenadas enteras
Point<double> p2 = Point<double>(10, 5, 7);
```

Funciones de Punto:

```cpp
#include "elements/point.h"

Point<int> a = Point<int>(1, 2, 3);
Point<int> b = Point<int>(4, 5, 6);

// Imprime el punto
a.print();
std::cout << a << std::endl;

// Operaciones puntos
Point<int> c = a + b;
Point<int> c = a - b;
Point<int> c = a / 5;
Point<int> c = a * 5;
a *= 5;
a /= 5;

// Igualdad
bool c = a == b;
bool c = a != b;

// Otras operaciones
double c = a.cos(b); // Retorna el coseno vectorial
double c = a.dist_origin(); // Distancia al origen
double c = a.dist(b); // Distancia
double c = a.dist2(b); // Distancia al cuadrado
int c = a.ccw(c,d); // Ve si el punto y c,d están orientados en ccw
int c = a.get_dim(); // Obtiene la dimensión del punto
Point<int> c = a.clone(); // Clona el punto
Point<int> c = abs(a); // Valor absoluto
```

## Segmento

Permite crear un segmento entre dos puntos:

```cpp
#include "elements/segment.h"

Point<double> p1 = Point<double>(0, 1);
Point<double> p2 = Point<double>(2, 3);

Segment<double> s = Segment<double>(p1, p2);
```

Funciones del segmento:

```cpp
double c = s.getLength(); // Obtiene largo del segmento, igual a norm();

// Verifica posiciones
Point<double> p = Point<double>(4, 8);
bool c = s.left(p); // Ve si p esta a la izquierda de s
bool c = s.right(p);
bool c = s.on(p);
bool c = s.left_on(p);
bool c = s.right_on(p);
```

## Vector

Permite crear un vector entre (0,0,0) y un punto:

```cpp
#include "elements/vector.h"

Point<double> p1 = Point<double>(0, 1);

Vector<double> vec = Vector<double>(p1);
```

Funciones:

```cpp
// Imprimir en consola
vec.print();
std::cout << vec << std::endl;

// Operaciones
vec.abs(); // -> new Vector<T> valor absoluto
vec.cross(v1); // Producto cruz
vec.dot(v1); // Producto punto
vec.get_normalized() // -> new Vector<T>
vec.normalize();

// Otros
int d = vec.get_dim(); // Dimensión del vector
T d = vec.norm(); // Norma del vector
```

## Polígono

Permite crear un polígono a partir de una lista de puntos:

```cpp
#include "elements/polygon.h"

Point<double> *p = {Point<double>(0,1)....}; // #n elementos

Polygon<double> poly = Polygon<double>(p, n);
```

```cpp
// Imprimir en consola
poly.print();
std::cout << poly << std::endl;

// Operaciones geométricas
bool c = poly.in_ccw(); // Punto en CCW (Counter clockwise)
bool c = poly.in_poly(p); // Verifica si un punto está dentro del polígono
bool c = poly.is_vertex(p); // p vértice del polígono
bool c = poly.same_points(poly2); // poly y poly2 tienen iguales puntos
poly.add_point(p); // Añade un punto
T area = poly.area(); // Calcula el área del polígono
T perimetro = poly.perimeter(); // Calcula el perímetro del polígono
```

## HalfEdge

Esta estructura de datos se basó en la siguiente topología:

<center>
    <img src="http://pointclouds.org/blog/_images/half_edge.png" alt="half-edge" width="400"/>
</center>

Para eso se crearon las siguientes clases:

```cpp
Face<T>; // half_edge/face.h
Vertex<T>; // half_edge/vertex.h
H_Edge<T>; // half_edge/h_edge.h
LoadOff<T>; // half_edge/load_off.h
```

Para crear una figura se requiere crear los vértices directores de cada half-edge, luego se crea una cara que contiene sólo 1 referencia al Half-Edge. A modo de ejemplo se ilustra uno de los test creados en ```tests/half_edge/test_half_edge.cpp``` *test_2face()*:

```cpp
#include "half_edge/h_edge.h"

// Crea las caras
Face<double> f1 = Face<double>("F1");
Face<double> f2 = Face<double>("F2");

// Crea los puntos
Point<double> p1 = Point<double>(0, 0);
Point<double> p2 = Point<double>(1, 0);
Point<double> p3 = Point<double>(0, 1);
Point<double> p4 = Point<double>(1, 1);

// Crea las estructuras
H_Edge<double> he12 = H_Edge<double>(&p2, &f1, "12");
H_Edge<double> he24 = H_Edge<double>(&p4, &f1, "24");
H_Edge<double> he41 = H_Edge<double>(&p1, &f1, "41");
H_Edge<double> he23 = H_Edge<double>(&p3, &f2, "23");
H_Edge<double> he34 = H_Edge<double>(&p4, &f2, "34");
H_Edge<double> he42 = H_Edge<double>(&p2, &f2, "42");
```

Notar que el ejemplo anterior permite representar la siguiente figura:

<center>
    <img src="https://i.imgur.com/xHo3sKd.png" alt="figura-he" width="300"/>
</center>

Luego, las relaciones de topología entre cada par de HalfEdge se realiza a continuación:

```cpp
// F1
f1.set_hedge(&he12);
he12.set_next(&he24);
he24.set_next(&he41);
he41.set_next(&he12);

// F2
f2.set_hedge(&he34);
he23.set_next(&he34);
he34.set_next(&he42);
he42.set_next(&he23);

// Par
he24.set_pair(&he42);
```

La librería se construyó lo suficientemente robusta como para arrojar errores por la salida estándar si se realizan operaciones inválidas o potencialmente peligrosas, como por ejemplo generar una cara no cíclica (por ejemplo ```he12->he23->he31->he23...```), añadir un HalfEdge siguiente que no esté en la misma cara, añadir el mismo objeto como next HalfEdge, entre otros casos.

La idea de la estructura es que una cara, al conocer un puntero a un HalfEdge (y la cara está bien definida) se puede recorrer siempre accediento a .get_next() de cada elemento. Algunas operaciones de las caras:

```cpp
Face<double> f = Face<double>("F");

f.get_area(); // Obtiene el área de la cara
f.get_chain_length(); // Retorna el largo de la cadena
f.get_faces(); // Retorna un vector con todas las caras vecinas
f.get_hedge(); // Obtiene el puntero a la cara
f.get_name(); // Retorna el nombre
f.get_perimeter(); // Obtiene el perímetro
f.in_face(&H_Edge<T>); // Indica si un HalfEdge pertenece a la cara
f.is_ccw(); // Indica que la cara está en CCW
f.is_surrounded(); // Indica si la cara está rodeada
f.is_valid(); // Indica si la cara es cíclica, conexa
f.print_hedges(); // Imprime la relación entre los HalfEdge
f.print_points(); // Imprime los vértices
f.set_hedge(&H_Edge<T>); // Define el primer puntero a un HalfEdge
```

Un ejemplo de ```f.print_hedges()``` para el ejemplo actual se puede visualizar en el bloque siguiente. Notar que el sistema es capaz de imprimir el HalfEdge par, el nombre de la cara y si la estructura es cíclica o no.

```cpp
----------------------------------------------------------
Test-2face
----------------------------------------------------------
<F1> : {12} ⇄ {24/42<F2>} ⇄ {41} ↻
<F2> : {34} ⇄ {42/24<F1>} ⇄ {23} ↻
<F1> : (1.000000,0.000000) ⇄ (0.000000,1.000000) ⇄ (0.000000,0.000000)
<F2> : (0.000000,1.000000) ⇄ (1.000000,0.000000) ⇄ (1.000000,1.000000)
```

## LoadOff

A modo de testeo de la clase HalfEdge se creó una clase que permite cargar un archivo .OFF y generar una estructura HalfEdge. Para ello se lee un archivo, se recorre cada línea, se crean los puntos ```Point<T>```, las caras y finalmente utilizando un *hashmap* se crean las referencias pares para cada HalfEdge.

Funciones de clase:
```cpp
#include "half_edge/loff/load_off.h"

// Crea un loadOff
LoadOff<double> lo = LoadOff<double>();

// Carga un archivo y genera un offObject de nombre "name"
offObject<double> off = lo.load(filename, name);

// Operaciones con la etructura offobject
lo.get_total_area(&off); // Calcula el área total
lo.is_ccw(&off); // Indica que está en ccw
lo.print_area(&off); // Imprime el área
lo.print_hedges(&off); // Imprime la estructura
lo.print_points(&off); // Imprime los puntos
```

A modo de ejemplo se crearon tests ```tests/half_edge/test_load_off.cpp``` en donde se carga un .off en ccw y otro que no. Para un cubo en 3D se obtuvo la siguiente estructura:

```cpp
OFF Object {Cube-good} area:
	Face <F1> area: 0.5
	Face <F2> area: 0.5
	Face <F3> area: 0.5
	Face <F4> area: 0.5
	Face <F5> area: 0.5
	Face <F6> area: 0.5
	----------
	Total area: 3

<F1> : {0-1/1-0<F4>} ⇄ {1-3/3-1<F5>} ⇄ {3-2/2-3<F2>} ⇄ {2-0/0-2<F6>} ↻
<F2> : {2-3/3-2<F1>} ⇄ {3-5/5-3<F5>} ⇄ {5-4/4-5<F3>} ⇄ {4-2/2-4<F6>} ↻
<F3> : {4-5/5-4<F2>} ⇄ {5-7/7-5<F5>} ⇄ {7-6/6-7<F4>} ⇄ {6-4/4-6<F6>} ↻
<F4> : {6-7/7-6<F3>} ⇄ {7-1/1-7<F5>} ⇄ {1-0/0-1<F1>} ⇄ {0-6/6-0<F6>} ↻
<F5> : {1-7/7-1<F4>} ⇄ {7-5/5-7<F3>} ⇄ {5-3/3-5<F2>} ⇄ {3-1/1-3<F1>} ↻
<F6> : {6-0/0-6<F4>} ⇄ {0-2/2-0<F1>} ⇄ {2-4/4-2<F2>} ⇄ {4-6/6-4<F3>} ↻
```

Notar que la aplicación es capaz de reconocer de forma correcta todas las relaciones pares de los HalfEdge gracias a la estructura map.

## Autor

[Pablo Pizarro R.](http://ppizarror.com) | 2018