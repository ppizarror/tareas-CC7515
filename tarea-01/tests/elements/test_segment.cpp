/**
 * Testea segmentos.
 * @author Pablo Pizarro
 * @date 19/04/2018
 */

// Importación de librerías
#include <iostream>
#include <cassert>
#include "../elem/segmento.h"

/**
 * Testea el largo de un segmento.
 */
void testLargo() {
    // Triángulo 3,4 -> 5
    Punto<float> p1 = Punto<float>(0, 0);
    Punto<float> p2 = Punto<float>(3, 4);

    // Crea el segmento
    Segmento<float> s = Segmento<float>(p1, p2);
    assert(s.getLength() == 5.0f); // Debe ser 5
}

/**
 * Testea las ubicaciones punto/segmento.
 */
void testInterseccion() {
    // (0,10)      (10,10)      Un triángulo
    //   p5          p2         p1-p2   Segmento
    //              / .         p3      A la derecha
    //       (5,5) /  .         p4      Sobre el segmento
    //            p4  .         p5      A la izquierda del segmento
    //           /    .
    //         p1 ... p3
    //       (0,0)  (10,0)
    Punto<float> p1 = Punto<float>(0, 0);
    Punto<float> p2 = Punto<float>(10, 10);
    Punto<float> p3 = Punto<float>(10, 0);
    Punto<float> p4 = Punto<float>(5, 5);
    Punto<float> p5 = Punto<float>(0, 10);
    Segmento<float> s = Segmento<float>(p1, p2);

    assert(s.left(p5));
    assert(!s.right(p5));
    assert(s.on(p4));
    assert(s.right(p3));
}

/**
 * Retorna texto SI/NO de acuerdo a 1/0
 * @return
 */
std::string boolString(int v) {
    if (v) {
        return "SI";
    } else {
        return "NO";
    }
}

/**
 * Test de precisión.
 */
void testPrecision() {
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
    Punto<int> p1_i = Punto<int>(0, 0);
    Punto<int> p2_i = Punto<int>(5, 1);

    Punto<double> p1_d = Punto<double>(0, 0);
    Punto<double> p2_d = Punto<double>(5, 1);

    // Puntos sobre el segmento
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
    Punto<int> pin_i = Punto<int>(2, 0.4);
#pragma clang diagnostic pop
    Punto<double> pin_d = Punto<double>(2, 0.4);

    // Puntos a la izquierda estricto del segmento
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
    Punto<int> pleft_i = Punto<int>(2, 0.41);
#pragma clang diagnostic pop
    Punto<double> pleft_d = Punto<double>(2, 0.41);

    // Puntos a la izquierda estricto del segmento MUY PEQUEÑOS
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
    Punto<int> pleftpeq_i = Punto<int>(2, 0.41);
#pragma clang diagnostic pop
    Punto<double> pleftpeq_d = Punto<double>(2, 0.4000000000000000000000000000000000000000000000000000000000000001);

    // Crea los segmentos en entero y double
    Segmento<int> s_i = Segmento<int>(p1_i, p2_i);
    Segmento<double> s_d = Segmento<double>(p1_d, p2_d);

    /**
     * Imprime en consola.
     */
    std::cout << "\nTest de precision:" << std::endl;
    std::cout << "\t[Integer] contiene (2,0.4): " << boolString(s_i.on(pin_i)) << std::endl;
    std::cout << "\t[Double]  contiene (2,0.4): " << boolString(s_d.on(pin_d)) << std::endl;
    std::cout << "\t(2,0.41) izquierda de [Integer]: " << boolString(s_i.left(pleft_i)) << std::endl;
    std::cout << "\t(2,0.41) izquierda de [Double]: " << boolString(s_d.left(pleft_d)) << std::endl;
    std::cout << "\t(2,0.400...01) izquierda de [Integer]: " << boolString(s_i.left(pleftpeq_i)) << std::endl;
    std::cout << "\t(2,0.400...01) izquierda de [Double] : " << boolString(s_d.left(pleftpeq_d)) << std::endl;
}

/**
 * Test punto izquierda
 */
void testIzquierda() {
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
    Punto<int> p1 = Punto<int>(0, 0);
    Punto<int> p2 = Punto<int>(1, 1);
    Punto<int> p3 = Punto<int>(1, 0);
    Segmento<int> s1 = Segmento<int>(p1, p2);
    Segmento<int> s2 = Segmento<int>(p2, p1);

    // Si se va de s1->s2 p3 está a la derecha
    assert(s1.right(p3));

    // Si se va de s2->s1 p3 está a la izquierda
    assert(s2.left(p3));
}

/**
 * Corre los test.
 */
int main() {
    std::cout << "Testeando segmento" << std::endl;

    // Carga los tests
    testLargo();
    testInterseccion();
    testPrecision();
    testIzquierda();

    // Retorna
    return 0;
}