/**
 * Testea segmentos.
 *
 * @package tests.elements
 * @author Pablo Pizarro
 * @date 19/04/2018
 */

// Define bloque
#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-narrowing-conversions"

// Importación de librerías
#include <iostream>
#include <cassert>
#include "../../elements/segment.h"

/**
 * Testea el largo de un segmento.
 */
void test_length() {
    // Triángulo 3,4 -> 5
    Point<float> p1 = Point<float>(0, 0);
    Point<float> p2 = Point<float>(3, 4);

    // Crea el segmento
    Segment<float> s = Segment<float>(p1, p2);
    assert(s.get_length() == 5.0f); // Debe ser 5
}

/**
 * Testea las ubicaciones punto/segmento.
 */
void test_intersection() {
    // (0,10)      (10,10)      Un triángulo
    //   p5          p2         p1-p2   Segment
    //              / .         p3      A la derecha
    //       (5,5) /  .         p4      Sobre el segmento
    //            p4  .         p5      A la izquierda del segmento
    //           /    .
    //         p1 ... p3
    //       (0,0)  (10,0)
    Point<float> p1 = Point<float>(0, 0);
    Point<float> p2 = Point<float>(10, 10);
    Point<float> p3 = Point<float>(10, 0);
    Point<float> p4 = Point<float>(5, 5);
    Point<float> p5 = Point<float>(0, 10);
    Segment<float> s = Segment<float>(p1, p2);

    assert(s.left(p5));
    assert(!s.right(p5));
    assert(s.on(p4));
    assert(s.right(p3));
}

/**
 * Retorna texto SI/NO de acuerdo a 1/0.
 *
 * @return
 */
std::string bool_string(int v) {
    if (v) {
        return "SI";
    } else {
        return "NO";
    }
}

/**
 * Test de precisión.
 */
void test_presition() {
    /** Crea los puntos
     *  p3(2,0.4) debe estar en Segmento
     *  p4(2,0.41) debe estar estrictamente a la izquierda del Segmento
     *
     *  1               p4         ________ p2
     *  |       ____....p3..^^^^^^
     *  0    p1
     *  |
     *  y\x--0-----1-----2-----3-----4-----5
     */
    Point<int> p1_i = Point<int>(0, 0);
    Point<int> p2_i = Point<int>(5, 1);

    Point<double> p1_d = Point<double>(0, 0);
    Point<double> p2_d = Point<double>(5, 1);

    // Puntos sobre el segmento
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
    Point<int> pin_i = Point<int>(2, 0.4);
#pragma clang diagnostic pop
    Point<double> pin_d = Point<double>(2, 0.4);

    // Puntos a la izquierda estricto del segmento
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
    Point<int> pleft_i = Point<int>(2, 0.41);
#pragma clang diagnostic pop
    Point<double> pleft_d = Point<double>(2, 0.41);

    // Puntos a la izquierda estricto del segmento MUY PEQUEÑOS
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
    Point<int> pleftpeq_i = Point<int>(2, 0.41);
#pragma clang diagnostic pop
    Point<double> pleftpeq_d = Point<double>(2, 0.4000000000000000000000000000000000000000000000000000000000000001);

    // Crea los segmentos en entero y double
    Segment<int> s_i = Segment<int>(p1_i, p2_i);
    Segment<double> s_d = Segment<double>(p1_d, p2_d);

    /**
     * Imprime en consola
     */
    std::cout << "\nTest de precision:" << std::endl;
    std::cout << "\t[Integer] contiene (2,0.4): " << bool_string(s_i.on(pin_i)) << std::endl;
    std::cout << "\t[Double]  contiene (2,0.4): " << bool_string(s_d.on(pin_d)) << std::endl;
    std::cout << "\t(2,0.41) izquierda de [Integer]: " << bool_string(s_i.left(pleft_i)) << std::endl;
    std::cout << "\t(2,0.41) izquierda de [Double]: " << bool_string(s_d.left(pleft_d)) << std::endl;
    std::cout << "\t(2,0.400...01) izquierda de [Integer]: " << bool_string(s_i.left(pleftpeq_i)) << std::endl;
    std::cout << "\t(2,0.400...01) izquierda de [Double] : " << bool_string(s_d.left(pleftpeq_d)) << std::endl;
}

/**
 * Test punto izquierda
 */
void test_left() {
    /**
     * Crea un triangulo
     *
     *
     *           p2
     *         / |
     *       /   |
     *     /     |
     *   /       |
     * p1 ------ p3
     */
    Point<int> p1 = Point<int>(0, 0);
    Point<int> p2 = Point<int>(1, 1);
    Point<int> p3 = Point<int>(1, 0);
    Segment<int> s1 = Segment<int>(p1, p2);
    Segment<int> s2 = Segment<int>(p2, p1);

    // Si se va de s1->s2 p3 está a la derecha
    assert(s1.right(p3));

    // Si se va de s2->s1 p3 está a la izquierda
    assert(s2.left(p3));
}

/**
 * Corre los test.
 */
int main() {

    std::cout << "Test segment" << std::endl;

    // Carga los tests
    test_length();
    test_intersection();
    test_presition();
    test_left();

    // Retorna
    return 0;

}