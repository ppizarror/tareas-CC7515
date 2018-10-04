/**
 * Testea estructura Half-Edge.
 *
 * @package tests.elements
 * @author Pablo Pizarro
 * @date 01/10/2018
 */

// Define parámetros compilación
#pragma ide diagnostic ignored "modernize-use-auto"

// Importa librerías
#include "../../half_edge/h_edge.h"

/**
 * Test básico, genera un cuadrado.
 */
void test_basic() {

    // Crea una cara
    Face<double> f = Face<double>();

    // Crea algunos puntos
    //
    // p4 (0,1) -- p3 (1,1)
    // |           |
    // |           |
    // p1 (0,0) -- p2 (1,0)
    Point<double> p1 = Point<double>(0, 0);
    Point<double> p2 = Point<double>(1, 0);
    Point<double> p3 = Point<double>(1, 1);
    Point<double> p4 = Point<double>(0, 1);

    // Crea los Hedge
    H_Edge<double> he12 = H_Edge<double>(&p2, &f);
    H_Edge<double> he21 = H_Edge<double>(&p1, &f);
    
    // Vé que edges están en face
    

}

/**
 * Corre los tests.
 */
int main() {

    // Corre los tests
    test_basic();

    // Retorna
    return 0;

}