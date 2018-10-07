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
Point<int> c = abs(a); // Valor absoluto
double c = a.dist(b); // Distancia
double c = a.dist2(b); // Distancia al cuadrado
double c = a.dist_origin(); // Distancia al origen
int c = a.get_dim(); // Obtiene la dimensión del punto
int c = a.ccw(c,d); // Ve si el punto y c,d están orientados en ccw
Point<int> c = a.clone(); // Clona el punto
double c = a.cos(b); // Retorna el coseno vectorial
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
vec.normalize();
vec.get_normalized() // -> new Vector<T>
vec.cross(v1); // Producto cruz
vec.dot(v1); // Producto punto
vec.abs(); // -> new Vector<T> valor absoluto

// Otros
int d = vec.get_dim(); // Dimensión del vector
T d = vec.norm(); // Norma del vector
```

## Poligono

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
bool c = poly.inCCW(); // Punto en CCW (Counter clockwise)
poly.addPoint(p); // Añade un punto
T area = poly.area(); // Calcula el área del polígono
T perimetro = poly.perimeter(); // Calcula el perímetro del polígono
bool c = poly.in_poly(p); // Verifica si un punto está dentro del polígono
bool c = poly.is_vertex(p); // p vértice del polígono
bool c = poly.same_points(poly2); // poly y poly2 tienen iguales puntos
```

## Autor

<a href="http://ppizarror.com" title="ppizarror">Pablo Pizarro R.</a> | 2018