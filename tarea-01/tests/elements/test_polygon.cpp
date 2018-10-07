/**
 * Testea polígonos.
 * 
 * @package tests.elements
 * @author Pablo Pizarro
 * @date 18/04/2018
 */

// Importación de librerías
#include <iostream>
#include <cassert>
#include "../../elements/polygon.h"

/**
 * Testea la creación de un polígono.
 */
void test_creation() {
    Polygon<double> poly = Polygon<double>();
    poly.add_point(Point<double>(1, 2));
    poly.add_point(Point<double>(3, 4));
    poly.add_point(Point<double>(5, 6));
}

/**
 * Testea la implementación de CCW.
 */
void test_poly_ccw() {
    // CCW
    Point<float> plist[] = {Point<float>(5, 0), Point<float>(6, 4), Point<float>(4, 5), Point<float>(1, 5),
                            Point<float>(1, 0)};
    Polygon<float> poly = Polygon<float>(plist, 5);
    assert(poly.in_ccw());

    // No CCW
    Point<float> plist2[] = {Point<float>(0, 0), Point<float>(0, 1), Point<float>(1, 1)};
    poly = Polygon<float>(plist2, 3);
    assert(!poly.in_ccw());
}

/**
 * Testea el cálculo de áreas.
 */
void test_poly_area() {
    // Cuadrado de lado 2
    Point<float> plist[] = {Point<float>(0, 0) * 2, Point<float>(1, 0) * 2, Point<float>(1, 1) * 2,
                            Point<float>(0, 1) * 2};
    Polygon<float> poly = Polygon<float>(plist, 4);
    assert(poly.area() == 4.0f);

    // Triangulo
    // https://1.bp.blogspot.com/-AGt5tqYpDhk/UK5hSxmwbZI/AAAAAAAAEo8/khEwS7riKlI/s640/%C3%A1rea.jpg
    Point<float> triangulo[] = {Point<float>(-3, -1), Point<float>(5, -4), Point<float>(-2, 6)};
    poly = Polygon<float>(triangulo, 3);
    assert(poly.area() == 29.5f);
}

/**
 * Chequea contención de puntos.
 */
void test_in_poly() {
    /**
     * Cuadrado de lado 1
     */
    Point<float> plist[] = {Point<float>(0, 0), Point<float>(1, 0), Point<float>(1, 1),
                            Point<float>(0, 1)};
    Polygon<float> poly = Polygon<float>(plist, 4);

    // Creación de varios puntos
    Point<float> p;

    // Deben estar dentro
    p = Point<float>(0.5, 0.5);
    assert(poly.in_poly(p));
    p = Point<float>(0.2, 0.5);
    assert(poly.in_poly(p));

    // Verifica que cada punto de la lista exista en el polígono
    for (auto &i : plist) {
        assert(poly.is_vertex(i));
    }

    // Deben estar fuera
    p = Point<float>(0, 1.1);
    assert(!poly.in_poly(p));
    p = Point<float>(1.1, 0);
    assert(!poly.in_poly(p));

    /**
     * Polígono más complicado
     */
    // y      (0.5,0.5)
    // ^          p4
    // \->x      /  \
    //          /    \           Polygon místico
    //         /  p2  \             > DRINCC PEPSI OR BEPBIS?
    //        /  /  \  \            > BEPES,BOPIS,BEPPS,BEPIS,BEPSIS
    //       / /      \ \        p2: (0.5,0.25)
    //      //          \\
    //     p1            p3
    // (0.0,0.0)      (1.0,0.0)
    Point<float> plist2[] = {Point<float>(0.0, 0.0), Point<float>(0.5, 0.25), Point<float>(1.0, 0.0),
                             Point<float>(0.5, 0.5)};
    poly = Polygon<float>(plist2, 4);

    // Área
    assert(poly.area() == 0.125f);

    // Deben estar fuera
    p = Point<float>(0.5, 0.24);
    assert(!poly.in_poly(p));
    p = Point<float>(0.4, 0.2);
    assert(!poly.in_poly(p));

    // Deben estar dentro, SAD REACC ONLY
    p = Point<float>(0.5, 0.26);
    assert(poly.in_poly(p));
    p = Point<float>(0.1, 0.1);
    assert(poly.in_poly(p));
}

/**
 * Testea el perímetro
 */
void test_poly_perimeter() {
    // Cuadrado 1x1
    Point<float> plist[] = {Point<float>(0, 0), Point<float>(1, 0), Point<float>(1, 1),
                            Point<float>(0, 1)};
    Polygon<float> poly = Polygon<float>(plist, 4);

    assert(poly.perimeter() == 4.0f);

    // Aumenta cada lado del cuadrado al doble, cuadrado 2x2, perimeter 8
    for (auto &i : plist) {
        i *= 2;
    }
    poly = Polygon<float>(plist, 4);
    assert(poly.perimeter() == 8.0f);

    // Vuelve a aumentar el lado al cuadruple, 8x8 perimeter 8x4=32 y área 64
    for (auto &i : plist) {
        i *= 4;
    }
    poly = Polygon<float>(plist, 4);
    assert(poly.perimeter() == 32.0f);
    assert(poly.area() == 64.0f);
}

/**
 * Corre los test.
 */
int main() {
    std::cout << "Test Polygon" << std::endl;

    // Carga los tests
    test_creation();
    test_poly_ccw();
    test_poly_area();
    test_in_poly();
    test_poly_perimeter();

    // Retorna
    return 0;
}