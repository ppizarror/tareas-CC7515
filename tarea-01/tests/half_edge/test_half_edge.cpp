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

template<class T>
/**
 * Indica si dos números son parecidos.
 *
 * @tparam T Template
 * @param a Número a
 * @param b Número b
 * @return
 */
bool num_equal(T a, T b) {
    return fabs(a - b) < 1e15;
}

/**
 * Test básico, genera un cuadrado.
 */
void test_basic() {
   
    /**
     * Crea algunos puntos, modificado por MOD
     *
     * p4 (0,1) -- p3 (1,1)
     * |           |
     * |     F     |
     * |           |
     * p1 (0,0) -- p2 (1,0)
     */
    double MOD = 1.5;
    Face<double> f = Face<double>("F");
    Point<double> p1 = Point<double>(0, 0) * MOD;
    Point<double> p2 = Point<double>(1, 0) * MOD;
    Point<double> p3 = Point<double>(1, 1) * MOD;
    Point<double> p4 = Point<double>(0, 1) * MOD;

    /**
     * Crea los Hedge
     */
    H_Edge<double> he12 = H_Edge<double>(&p2, &f, "12");
    H_Edge<double> he23 = H_Edge<double>(&p3, &f, "23");
    H_Edge<double> he34 = H_Edge<double>(&p4, &f, "34");
    H_Edge<double> he41 = H_Edge<double>(&p1, &f, "41");

    // Realiza algunos prints
    std::cout << "Orden:" << std::endl;
    he12.print();
    he23.print();
    he34.print();
    he41.print();

    // Verifica que sea nulo
    assert(he12.get_next() == nullptr);
    assert(he12.get_name() == "12");

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

    // Verifica que el puntero de la cara sea el mismo
    assert(&f == he12.get_face() && &f == he23.get_face() && &f == he34.get_face());

    // Verifica que ambos he estén dentro de la cara
    assert(f.in_face(&he12) && f.in_face(&he23) && f.in_face(&he34) && f.in_face(&he41));

    // Verifica que punto sea el mismo
    assert(he12.get_point() == &p2);

    /**
     * Verifica propiedades geométricas
     */
    assert(f.get_area() == MOD * MOD);
    assert(f.get_chain_length() == 4);
    assert(f.get_perimeter() == 4 * MOD);
    assert(f.is_ccw());

}

/**
 * Testea dos caras, una en ccw, otra no.
 */
void test_ccw() {

    /**
     * Crea una cara en ccw y otra no, modificada por MOD
     *
     * p3 (0,1)
     * |    \
     * |  T   \
     * |        \
     * p1 (0,0) -- p2 (1,0)
     */
    double MOD = 1.5;
    Face<double> f = Face<double>("T-CCW");
    Point<double> p1 = Point<double>(0, 0) * MOD;
    Point<double> p2 = Point<double>(1, 0) * MOD;
    Point<double> p3 = Point<double>(0, 1) * MOD;

    /**
     * Crea los Hedge
     */
    H_Edge<double> he12 = H_Edge<double>(&p2, &f);
    H_Edge<double> he23 = H_Edge<double>(&p3, &f);
    H_Edge<double> he31 = H_Edge<double>(&p1, &f);

    // Verifica que sea nulo
    assert(he12.get_next() == nullptr);

    /**
     * Crea las relaciones topológicas
     */
    he12.set_next(&he23);
    he23.set_next(&he31);
    he31.set_next(&he12);

    /**
     * Testea propiedades geométricas
     */
    assert(f.is_ccw());
    assert(num_equal<double>(f.get_perimeter(), 2 * MOD + MOD * sqrt(2)));
    assert(f.get_area() == MOD * MOD / 2);

    /**
     * Ahora cambia la relación de orden
     */
    he12.set_next(&he31);
    he23.set_next(&he12);
    he31.set_next(&he23);
    assert(!f.is_ccw());

}

/**
 * Realiza un test básico con una figura compuesta por dos caras.
 */
void test_2face() {

    /**
     * Crea una cara en ccw y otra no, modificada por MOD
     *
     * p3 (0,1) -- p4 (1,1)
     * |    \      |
     * |  T1  \ T2 |
     * |        \  |
     * p1 (0,0) -- p2 (1,0)
     */
    double MOD = 1.5;
    Face<double> f1 = Face<double>("T1");
    Face<double> f2 = Face<double>("T2");
    Point<double> p1 = Point<double>(0, 0) * MOD;
    Point<double> p2 = Point<double>(1, 0) * MOD;
    Point<double> p3 = Point<double>(0, 1) * MOD;
    Point<double> p4 = Point<double>(1, 1) * MOD;

    /**
    * Crea los Hedge
    */
    H_Edge<double> he12 = H_Edge<double>(&p2, &f1, "12");
    H_Edge<double> he23 = H_Edge<double>(&p3, &f1, "23");
    H_Edge<double> he31 = H_Edge<double>(&p1, &f1, "31");
    H_Edge<double> he24 = H_Edge<double>(&p4, &f2, "24");
    H_Edge<double> he43 = H_Edge<double>(&p3, &f2, "43");
    H_Edge<double> he32 = H_Edge<double>(&p2, &f2, "32");

    /**
     * Crea las relaciones topológicas
     */
    // T1
    he12.set_next(&he23);
    he23.set_next(&he31);
    he31.set_next(&he12);

    // T2
    he24.set_next(&he43);
    he43.set_next(&he32);
    he32.set_next(&he24);

    // Par
    he23.set_pair(&he32);

    /**
     * Verifica condiciones geométricas
     */
    assert(num_equal<double>(f1.get_area(), f2.get_area()));
    assert(num_equal<double>(f1.get_perimeter(), f2.get_perimeter()));
    assert(f1.is_ccw() && f2.is_ccw());
    assert(f1.get_chain_length() == f2.get_chain_length() && f1.get_chain_length() == 3);

    /**
     * Verifica condiciones link par
     */
    H_Edge<double> *h = &he12;
    assert(he23.get_pair() == &he32 && he32.get_pair() == &he23);
    assert(h->get_next()->get_pair() == &he32);
    assert(h->get_next()->get_pair()->get_next()->get_next()->get_next()->get_pair()->get_next()->get_next() == &he12);

}

/**
 * Testea eliminación de estructuras.
 * Crea una cara en ccw y otra no, modificada por MOD
 *
 *    p3 (0,1) -- p4 (1,1)
 *    |    \      |
 *    |  T1  \ T2 |
 *    |        \  |
 *    p1 (0,0) -- p2 (1,0)
 *
 */
void test_deletion() {

    /**
     * Crea la estructura
     */
    double MOD = 1.5;
    Face<double> f1 = Face<double>("T1");
    Face<double> f2 = Face<double>("T2");
    Point<double> p1 = Point<double>(0, 0) * MOD;
    Point<double> p2 = Point<double>(1, 0) * MOD;
    Point<double> p3 = Point<double>(0, 1) * MOD;
    Point<double> p4 = Point<double>(1, 1) * MOD;
    H_Edge<double> he12 = H_Edge<double>(&p2, &f1, "12");
    H_Edge<double> he23 = H_Edge<double>(&p3, &f1, "23");
    H_Edge<double> he31 = H_Edge<double>(&p1, &f1, "31");
    H_Edge<double> he24 = H_Edge<double>(&p4, &f2, "24");
    H_Edge<double> he43 = H_Edge<double>(&p3, &f2, "43");
    H_Edge<double> he32 = H_Edge<double>(&p2, &f2, "32");
    he12.set_next(&he23);
    he23.set_next(&he31);
    he31.set_next(&he12);
    he24.set_next(&he43);
    he43.set_next(&he32);
    he32.set_next(&he24);
    he23.set_pair(&he32);

}

/**
 * Corre los tests.
 */
int main() {

    // Corre los tests
    test_basic();
    test_ccw();
    test_2face();
    test_deletion();

    // Retorna
    return 0;

}