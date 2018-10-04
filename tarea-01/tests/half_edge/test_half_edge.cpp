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
#include <cassert>
#include "../../half_edge/h_edge.h"

/**
 * Test básico, genera un cuadrado.
 */
void test_basic() {

    /**
     * Crea una cara
     */
    Face<double> f = Face<double>();

    /**
     * Crea algunos puntos, modificado por MOD
     *
     * p4 (0,1) -- p3 (1,1)
     * |           |
     * |           |
     * p1 (0,0) -- p2 (1,0)
     */
    double MOD = 1.5;
    Point<double> p1 = Point<double>(0, 0) * MOD;
    Point<double> p2 = Point<double>(1, 0) * MOD;
    Point<double> p3 = Point<double>(1, 1) * MOD;
    Point<double> p4 = Point<double>(0, 1) * MOD;

    /**
     * Crea los Hedge
     */
    H_Edge<double> he12 = H_Edge<double>(&p2, &f);
    H_Edge<double> he21 = H_Edge<double>(&p1, &f);
    H_Edge<double> he23 = H_Edge<double>(&p3, &f);
    H_Edge<double> he32 = H_Edge<double>(&p2, &f);
    H_Edge<double> he34 = H_Edge<double>(&p4, &f);
    H_Edge<double> he43 = H_Edge<double>(&p3, &f);
    H_Edge<double> he14 = H_Edge<double>(&p4, &f);
    H_Edge<double> he41 = H_Edge<double>(&p1, &f);

    // Realiza algunos prints
    std::cout << "Orden CCW" << std::endl;
    he12.print();
    he23.print();
    he34.print();
    he41.print();

    // Vé que edges están en face
    assert(f.number_edges() == 8);

    // Verifica que el puntero de la cara sea el mismo
    assert(&f == he12.get_face() && &f == he21.get_face());

    // Verifica que ambos he estén dentro de la cara
    assert(f.in_face_half_edge(&he12) && f.in_face_half_edge(&he21));

    // Verifica que punto sea el mismo
    assert(he12.get_point() == &p2);

    /**
     * Crea las relaciones pares
     */
    he12.set_pair(&he21);
    he23.set_pair(&he32);
    he34.set_pair(&he43);
    he14.set_pair(&he41);

    // Verifica relaciones
    assert(he12.get_pair() == &he21 && he21.get_pair() == &he12);
    assert(he23.get_pair() == &he32 && he32.get_pair() == &he23);
    assert(he34.get_pair() == &he43 && he43.get_pair() == &he34);
    assert(he41.get_pair() == &he14 && he14.get_pair() == &he41);

    /**
     * Crea los siguiente/posterior
     */
    he12.set_next(&he23);
    he23.set_next(&he34);
    he34.set_next(&he41);
    he41.set_next(&he12);

    // Verifica ciclo
    assert(he12.get_next() == &he23);
    assert(he23.get_prev() == &he12);
    assert(he12.get_next()->get_next()->get_next()->get_next() == &he12);
    assert(he12.get_prev()->get_prev()->get_prev()->get_prev() == &he12);

    /**
     * Verifica propiedades geométricas
     */
    assert(f.get_area() == MOD * MOD);
    assert(f.chain_length() == 4);

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