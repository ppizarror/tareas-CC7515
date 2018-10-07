/**
 * Tests de cerradura convexa.
 *
 * @package tests.utils
 * @author Pablo Pizarro
 * @date 18/06/18
 */

// Inicia el bloque
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err60-cpp"

// Importación de librerías
#include "../../utils/convex_hull.h"
#include <cassert>
#include <ctime>
#include <iostream>
#include <random>
#include <stdio.h>
#include <time.h>

/**
 * Genera un número aleatorio entre dos valores flotantes.
 *
 * @param a - Número menor
 * @param b - Número mayor
 * @return
 */
float random_float(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX; // NOLINT
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

/**
 * Genera un número aleatorio entero entre dos valores.
 *
 * @param a - Número mayor
 * @param b - Número menor
 * @return
 */
int random_int(int a, int b) {
    return (int) random_float(a, b);
}

/**
 * Testeo básico, genera figuras simples de pocos puntos.
 */
void test_ch_basic() {
    Point<float> plist[] = {Point<float>(5, 0), Point<float>(6, 4), Point<float>(4, 5), Point<float>(1, 5),
                            Point<float>(1, 0)}; // 5

    // [Graham scan]
    std::pair<Polygon<float>, int> cerraduraGS = graham_scan(plist, 5);
    assert(cerraduraGS.second == 5);

    // [GiftWrapping]
    std::pair<Polygon<float>, int> cerraduraGW = gift_wrapping(plist, 5);
    assert(cerraduraGW.second == 5);

    // Verifica que ambos polígonos tengan iguales puntos
    assert(cerraduraGW.first.same_points(cerraduraGS.first));
}

/**
 * Testeo triangulo con puntos en hipotenusa.
 */
void test_ch_triangle() {
    Point<int> triangulo[] = {Point<int>(0, 0), Point<int>(-1, -1), Point<int>(2, 2), Point<int>(3, 3),
                              Point<int>(4, 4), Point<int>(5, 5), Point<int>(0, 0), Point<int>(5, 0)}; // 8

    // [GiftWrapping]
    std::pair<Polygon<int>, int> cerraduraGW = gift_wrapping(triangulo, 8);
    assert(cerraduraGW.second == 3);

    // [Graham scan]
    std::pair<Polygon<int>, int> cerraduraGS = graham_scan(triangulo, 8);
    assert(cerraduraGS.second == 3);

    // Verifica que ambos polígonos tengan iguales puntos
    assert(cerraduraGW.first.same_points(cerraduraGS.first));

}

template<class T>
/**
 * Testea un rombo con distintas rotaciones.
 */
void _test_ch_diamond(Point<T> *rombo) {
    // [GiftWrapping]
    std::pair<Polygon<int>, int> cerraduraGW = gift_wrapping(rombo, 4);
    assert(cerraduraGW.second == 4);

    // [Graham scan]
    std::pair<Polygon<int>, int> cerraduraGS = graham_scan(rombo, 4);
    assert(cerraduraGS.second == 4);

    // Verifica que ambos polígonos tengan iguales puntos
    assert(cerraduraGW.first.same_points(cerraduraGS.first));
}

/**
 * Testea la cerradura con la posición del menor en distintos lugares.
 */
void test_ch_diamond() {
    Point<int> rombo[] = {Point<int>(-1, 0), Point<int>(0, 1), Point<int>(1, 0), Point<int>(0, -1)}; // 4
    _test_ch_diamond(rombo);
    swap_Point(rombo, 0, 1);
    _test_ch_diamond(rombo);
    swap_Point(rombo, 1, 2);
    _test_ch_diamond(rombo);
    swap_Point(rombo, 2, 3);
    _test_ch_diamond(rombo);
    swap_Point(rombo, 3, 0);
    _test_ch_diamond(rombo);
}

/**
 * Testeo duplicados.
 */
void test_ch_dupl() {
    Point<int> plist[] = {Point<int>(0, 0), Point<int>(0, 0), Point<int>(0, 0), Point<int>(1, 1), Point<int>(1, 0),
                          Point<int>(1, 0)}; // 6

    // [GiftWrapping]
    std::pair<Polygon<int>, int> cerraduraGW = gift_wrapping(plist, 6);
    assert(cerraduraGW.second == 3);

    // [Graham scan]
    std::pair<Polygon<int>, int> cerraduraGS = graham_scan(plist, 6);
    assert(cerraduraGS.second == 3);

    // Verifica que ambos polígonos tengan iguales puntos
    assert(cerraduraGW.first.same_points(cerraduraGS.first));
}

/**
 * Genera un cuadrado de 1x1 en flotante con 10 puntos dentro,
 * la cerradura debe ser (0,0),(0,1),(1,1),(1,0)
 */
void test_ch_square_small() {
    Point<float> *cuadrado = new Point<float>[10]; // NOLINT

    // Añade los puntos
    for (int i = 0; i < 10; i++) {
        cuadrado[i] = Point<float>(random_float(0, 1), random_float(0, 1));
    }
    cuadrado[2] = Point<float>(0, 0);
    cuadrado[5] = Point<float>(0, 1);
    cuadrado[7] = Point<float>(1, 1);
    cuadrado[8] = Point<float>(1, 0);

    std::cout << "\n[GiftWrapping] Cerradura cuadrado perfecto 1x1 con 10 puntos" << std::endl;
    std::pair<Polygon<float>, int> cerraduraGW = gift_wrapping(cuadrado, 10);
    cerraduraGW.first.print();
    assert(cerraduraGW.second == 4);

    std::cout << "\n[Graham Scan] Cerradura cuadrado perfecto 1x1 con 10 puntos" << std::endl;
    std::pair<Polygon<float>, int> cerraduraGS = graham_scan(cuadrado, 10);
    cerraduraGS.first.print();
    assert(cerraduraGS.second == 4);

    // Verifica que ambos polígonos tengan iguales puntos
    assert(cerraduraGW.first.same_points(cerraduraGS.first));

    // Borra la memoria
    delete[] cuadrado;
}

/**
 * Genera un cuadrado de 1x1 en flotante con 1000 puntos dentro,
 * la cerradura debe ser (0,0),(0,1),(1,1),(1,0)
 */
void test_ch_square_med() {
    Point<float> *cuadrado = new Point<float>[1000]; // NOLINT

    // Añade los puntos
    for (int i = 0; i < 1000; i++) {
        cuadrado[i] = Point<float>(random_float(0, 1), random_float(0, 1));
    }
    cuadrado[100] = Point<float>(0, 0);
    cuadrado[200] = Point<float>(0, 1);
    cuadrado[300] = Point<float>(1, 1);
    cuadrado[400] = Point<float>(1, 0);

    std::cout << "\n[GiftWrapping] Cerradura cuadrado perfecto 1x1 con 1000 puntos" << std::endl;
    std::pair<Polygon<float>, int> cerraduraGW = gift_wrapping(cuadrado, 1000);
    cerraduraGW.first.print();
    assert(cerraduraGW.second == 4);

    std::cout << "\n[Graham Scan] Cerradura cuadrado perfecto 1x1 con 1000 puntos" << std::endl;
    std::pair<Polygon<float>, int> cerraduraGS = graham_scan(cuadrado, 1000);
    cerraduraGS.first.print();
    assert(cerraduraGS.second == 4);

    // Verifica que ambos polígonos tengan iguales puntos
    assert(cerraduraGW.first.same_points(cerraduraGS.first));
    assert(cerraduraGW.second == cerraduraGS.second);

    // Borra la memoria
    delete[] cuadrado;
}

/**
 * Genera un rectángulo de 10000 puntos completamente aleatorio, altura 1 y ancho 10.
 */
void test_ch_rect() {
    Point<float> *rectangulo = new Point<float>[10000]; // NOLINT

    // Añade los puntos
    for (int i = 0; i < 10000; i++) {
        rectangulo[i] = Point<float>(random_float(0, 10), random_float(0, 1));
    }
    rectangulo[1000] = Point<float>(0, 0);
    rectangulo[2000] = Point<float>(10, 0);
    rectangulo[3000] = Point<float>(0, 1);
    rectangulo[4000] = Point<float>(10, 1);

    // Calcula la cerradura
    std::cout << "\n[GiftWrapping] Cerradura rectangulo 10x1 con 10000 puntos" << std::endl;
    std::pair<Polygon<float>, int> cerraduraGW = gift_wrapping(rectangulo, 10000);
    cerraduraGW.first.print();
    assert(cerraduraGW.second == 4);

    // Calcula la cerradura
    std::cout << "\n[Graham Scan] Cerradura rectangulo 10x1 con 10000 puntos" << std::endl;
    std::pair<Polygon<float>, int> cerraduraGS = gift_wrapping(rectangulo, 10000);
    cerraduraGS.first.print();
    std::cout << "" << std::endl;
    assert(cerraduraGS.second == 4);

    // Verifica que ambos polígonos tengan iguales puntos
    assert(cerraduraGW.first.same_points(cerraduraGS.first));

    // Borra la memoria
    delete[] rectangulo;
}

/**
 * Testea una figura sin anclajes predefinidos.
 *
 * @param sz - Número de puntos
 * @param r - Porcentaje de repetidos en vértices
 */
void __tesh_ch_random_figure(double sz, int r) {
    auto n = static_cast<int>(sz);
    Point<float> *figura = new Point<float>[n]; // NOLINT
    int tinit;

    // Total de aleatorios
    auto alt = (int) ((((float) r) / 100) * (float) n);

    // Añade los puntos aleatorios
    for (int i = 0; i < n; i++) {
        figura[i] = Point<float>(random_float(-1, 1), random_float(-1, 1));
    }

    // Añade los puntos repetidos
    int rindex;
    int rpos;
    for (int i = 0; i < alt; i++) {
        rindex = random_int(0, alt - 1);
        rpos = random_int(i, n - 1);
        figura[rpos] = Point<float>(figura[rindex].get_coord_x(), figura[rindex].get_coord_y());
    }

    // Calcula la cerradura con Gift Wrapping
    tinit = static_cast<int>(clock());
    std::pair<Polygon<float>, int> cerraduraGW = gift_wrapping(figura, n);
    int t1 = compute_time_between(tinit);

    // Calcula cerradura con Graham Scan
    tinit = static_cast<int>(clock());
    std::pair<Polygon<float>, int> cerraduraGS = graham_scan(figura, n);
    int t2 = compute_time_between(tinit);

    // Imprime tiempos
    std::cout << r << "\t" << t1 << "\t" << t2 << std::endl;

    // Verifica que ambos polígonos tengan iguales puntos
    assert(cerraduraGW.first.same_points(cerraduraGS.first));

    // Borra la memoria
    delete[] figura;
}

/**
 * Test figura deforme, llama a testFiguraDeforme con distintos valores de r (%) porcentaje de puntos repetidos.
 * @param n - Número de puntos
 */
void test_ch_random(double n) {
    int r[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90};
    std::cout << "[Puntos repetidos r%] Cerradura figura aleatoria con " << n << " puntos" << std::endl;
    for (int i : r) {
        __tesh_ch_random_figure(n, i);
    }
    std::cout << "============================================================" << std::endl;
}

/**
 * Revisa los algoritmos con un porcentaje de ellos en la cerradura, figura aleatoria.
 * @param sz - Tamaño de la nube de puntos
 * @param s - Porcentaje de la nube en la cerradura
 */
void __test_ch_point_in_hull(double sz, int s) {
    auto n = static_cast<int>(sz);
    Point<float> *figura = new Point<float>[n]; // NOLINT
    int tinit;

    // Total de puntos aleatorios
    auto alt = (int) ((((float) s) / 100) * (float) n);

    // Añade los puntos dentro de la cerradura
    for (int i = 0; i < n; i++) {
        figura[i] = Point<float>(random_float(0, 1), random_float(0, 1));
    }

    // Calcula la cerradura para añadir los puntos
    std::pair<Point<float> *, int> cerradura = __graham_scan(figura, n);

    // Añade los puntos de la cerradura a la figura
    int randomPosc = 0, randomPosFig = 0;
    for (int i = 0; i < alt; i++) {
        randomPosc = random_int(0, cerradura.second - 1); // Posición random dentro de la cerradura
        randomPosFig = random_int(i, n - 1); // Posición radom dentro de la figura
        figura[randomPosFig] = cerradura.first[randomPosc];
    }

    // Calcula la cerradura con Gift Wrapping
    tinit = static_cast<int>(clock());
    std::pair<Polygon<float>, int> cerraduraGW = gift_wrapping(figura, n);
    int t1 = compute_time_between(tinit);

    // Calcula cerradura con Graham Scan
    tinit = static_cast<int>(clock());
    std::pair<Polygon<float>, int> cerraduraGS = graham_scan(figura, n);
    int t2 = compute_time_between(tinit);

    // Imprime tiempos
    std::cout << s << "\t" << t1 << "\t" << t2 << std::endl;

    // Verifica que ambos polígonos tengan iguales puntos
    assert(cerraduraGW.first.same_points(cerraduraGS.first));

    // Borra la memoria
    delete[] figura;
}

/**
 * Testea puntos en cerradura con distintos porcentajes de s, figura cualquiera.
 * @param n - Número de puntos
 */
void test_ch_in_hull(double n) {
    int s[] = {0, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90};
    std::cout << "[Puntos en cerradura s%] Cerradura figura aleatoria con " << n << " puntos" << std::endl;
    for (int i : s) {
        __test_ch_point_in_hull(n, i);
    }
    std::cout << "============================================================" << std::endl;
}

/**
 * Forza un cuadrado perfecto.
 */
bool SQUARE_PERCECT = false;

/**
 * Revisa los algoritmos con un porcentaje de ellos en la cerradura, cuadrado 1x1.
 *
 * @param sz - Tamaño de la nube de puntos dentro del cuadrado
 * @param s - Porcentaje de la nube en la cerradura
 */
void __test_ch_point_in_square_hull(double sz, int s) {
    auto n = static_cast<int>(sz);
    Point<float> *figura = new Point<float>[n]; // NOLINT
    int tinit;

    // Total de puntos aleatorios
    auto alt = (int) ((((float) s) / 100) * (float) n);

    // Añade los puntos dentro de la cerradura
    for (int i = 0; i < n; i++) {
        figura[i] = Point<float>(random_float(0, 1), random_float(0, 1));
    }

    // Añade los puntos aleatorios
    int ladoCuadrado = 0, posAleatoria = 0;
    for (int i = 0; i < alt; i++) {
        ladoCuadrado = random_int(1, 4);
        posAleatoria = random_int(i, n - 1);
        switch (ladoCuadrado) {
            case 1: // Lado izquierdo
                figura[posAleatoria] = Point<float>(0, random_float(0, 1));
                break;
            case 2: // Lado derecho
                figura[posAleatoria] = Point<float>(1, random_float(0, 1));
                break;
            case 3: // Lado superior
                figura[posAleatoria] = Point<float>(random_float(0, 1), 0);
                break;
            case 4: // Lado inferior
                figura[posAleatoria] = Point<float>(random_float(0, 1), 1);
                break;
            default:
                break;
        }
    }

    // Añade aristas para hacer un cuadrado perfecto
    if (SQUARE_PERCECT) {
        auto nstart = static_cast<int>(n * 0.1);
        figura[2 * nstart] = Point<float>(0, 0); // 20%
        figura[4 * nstart] = Point<float>(1, 1); // 40%
        figura[6 * nstart] = Point<float>(0, 1); // 60%
        figura[8 * nstart] = Point<float>(1, 0); // 80%
    }

    // Calcula la cerradura con Gift Wrapping
    tinit = static_cast<int>(clock());
    std::pair<Polygon<float>, int> cerraduraGW = gift_wrapping(figura, n);
    int t1 = compute_time_between(tinit);

    // Calcula cerradura con Graham Scan
    tinit = static_cast<int>(clock());
    std::pair<Polygon<float>, int> cerraduraGS = graham_scan(figura, n);
    int t2 = compute_time_between(tinit);

    // Imprime tiempos
    std::cout << s << "\t" << t1 << "\t" << t2 << std::endl;

    // Verifica que ambos polígonos tengan iguales puntos
    assert(cerraduraGW.first.same_points(cerraduraGS.first));

    // Borra la memoria
    delete[] figura;
}

/**
 * Testea puntos en cerradura con distintos porcentajes de s, cuadrado 1x1.
 *
 * @param n - Número de puntos
 */
void test_ch_in_square_hull(double n) {
    int s[] = {0, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90};
    std::cout << "[Puntos en cerradura s%] Cerradura cuadrado 1x1 con " << n << " puntos" << std::endl;
    for (int i : s) {
        __test_ch_point_in_square_hull(n, i);
    }
    std::cout << "============================================================" << std::endl;
}

/**
 * Tipo de test experimento.
 *
 *  1:  Testeo puntos con porcentaje de repetidos
 *  2:  Testeo puntos en cerradura para figura cualquiera
 *  3:  Testeo puntos en cerradura para un cuadrado 1x1 perfecto
 */
int TYPE_TEST = 2;

/**
* Corre los test.
*/
int main() {
    std::cout << "Test convex-hull" << std::endl;

    // Carga los tests
    test_ch_basic(); // Test sencillo con puros puntos en la cerradura
    return 0;
    test_ch_dupl(); // Testea varios puntos duplicados
    test_ch_triangle(); // Testea puntos colineales en la hipotenusa
    test_ch_diamond(); // Test que comprueba distintas posiciones para los pivotes
    test_ch_square_small(); // Test cuadrado 1x1 con 10 puntos aleatorios
    test_ch_square_med(); // Test cuadrado 1x1 con 1e4 puntos aleatorios
    test_ch_rect(); // Test rectángulo 10x1 con 1e4 puntos aleatorios

    double sz[] = {3 * 1e5, 2 * 1e5, 1e5, 5 * 1e4};
    std::cout << "============================================================" << std::endl;
    switch (TYPE_TEST) {
        case 1: // Testeo puntos con porcentaje r (%) repetidos, figura aleatoria
            for (double i:sz) test_ch_random(i);
            break;
        case 2: // Testeo puntos con porcentaje s (%) en cerradura, figura aleatoria
            for (double i:sz) test_ch_in_hull(i);
            break;
        case 3: // Testeo puntos con porcentaje s (%) en cerradura, cuadrado 1x1
            for (double i:sz) test_ch_in_square_hull(i);
            break;
        default:
            throw std::logic_error("Unknown test");
    }

    // Retorna
    return 0;
}