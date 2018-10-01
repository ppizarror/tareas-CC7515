/**
 * Testea clase Point.
 *
 * @author Pablo Pizarro
 * @date 30/09/2018
 */

// Importación de librerías
#include <iostream>
#include <cassert>
#include "../elements/point.h"

/**
 * Testea la creación de un punto entero.
 */
void test2D_int() {
    // Crea punto inicial
    Point<int> p1 = Point<int>(420, 69);

    // Imprime punto
    p1.print();

    // Comprueba que se hayan definido bien los objetos
    assert(p1.get_coord_x() == 420);
    assert(p1.get_coord_y() != 0);

    // Comprueba sumas
    Point<int> p2 = Point<int>(0, -69);
    Point<int> p3 = p1 + p2;
    assert(p3.get_coord_x() == 420 && p3.get_coord_y() == 0);

    // Comprueba suma a sí mismo. (0, -69) + (420, 0) = (420, -69)
    p2 += p3;
    assert(p2.get_coord_x() == 420 && p2.get_coord_y() == -69);

    // Restas
    p2 = -p2;
    assert(p2.get_coord_x() == -420 && p2.get_coord_y() == 69);

    // Valor absoluto
    p2 = p2.abs();
    assert(p2.get_coord_x() == 420 && p2.get_coord_y() == 69);

    // Calcula distancia de un punto al eje
    Point<int> porigin = Point<int>(0, 0);
    assert(p2.dist(porigin) == 3 * sqrt(20129));

    // Divide por un valor
    p2.setCoordX(5);
    p2.setCoordY(5);
    p2 /= 5;
    assert(p2.get_coord_x() == 1 && p2.get_coord_y() == 1);

    // Point uno
    Point<int> pone = Point<int>(1, 1);
    assert(p2 == pone);
}

/**
 * Testea punto en flotante.
 */
void test2D_float() {
    Point<float> *p1, *p2;

    // Inicia los puntos
    p1 = new Point<float>(4.20, 2.10);
    p2 = new Point<float>(4.20, 2.10);

    // Comprobación a lo bruto sumas y restas
    assert(p1->get_coord_y() + p2->get_coord_y() == 4.20f);
}

/**
 * Testea igualdad y desigualdad.
 */
void test2D_eq_not() {
    Point<int> p1 = Point<int>(1, 1);
    Point<int> p2 = Point<int>(2, 4);
    Point<int> p3 = Point<int>(1, 1);

    // p1 == p3
    assert(p1 == p3);

    // p1 != p2
    assert(p1 != p2);
}

/**
 * Testea distancia y ángulo, usados por Graham-Scan.
 */
void test2D_dist_cos() {
    Point<int> p1 = Point<int>(0, 0);
    Point<int> p2 = Point<int>(3, 4);

    assert(p1.dist(p2) == 5);
    assert(p1.cos(p2) == acos(0.6));
}

/**
 * Corre los test.
 */
int main() {
    std::cout << "Testeando punto" << std::endl;

    // Carga los tests 2D
    test2D_int();
    test2D_float();
    test2D_eq_not();
    test2D_dist_cos();

    // Retorna
    return 0;
}