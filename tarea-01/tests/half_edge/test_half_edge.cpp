/**
 * Testea estructura Half-Edge.
 *
 * @package tests.half_edge
 * @author Pablo Pizarro
 * @date 01/10/2018
 */

// Define parámetros compilación
#pragma ide diagnostic ignored "performance-unnecessary-value-param"
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
 * Imprime el título del test
 * @param title Título del test
 */
void print_title(std::string title) {
    std::cout << "" << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << title << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;
}

/**
 * Test básico, genera un cuadrado.
 */
void test_basic() {

    print_title("Test-basic");

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

    print_title("Test-ccw");

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
    f.print_hedges();
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

    print_title("Test-2face");

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
    f1.print_hedges();
    f1.print_points();
    f2.print_hedges();
    f2.print_points();
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
 * Testea borrado simple.
 */
void test_deletion_simple() {

    print_title("Test-deletion-simple");

    /**
     * Crea la estructura
     * p1 -> p2 -> p3 -> p4
     *    a     b     c
     */
    Face<double> f = Face<double>("F1");
    Point<double> p1 = Point<double>(0, 0);
    Point<double> p2 = Point<double>(1, 0);
    Point<double> p3 = Point<double>(2, 0);
    Point<double> p4 = Point<double>(3, 0);
    H_Edge<double> b = H_Edge<double>(&p3, &f, "b");
    H_Edge<double> c = H_Edge<double>(&p4, &f, "c");
    H_Edge<double> a = H_Edge<double>(&p2, &f, "a");
    H_Edge<double> d;
    a.set_next(&b);
    b.set_next(&c);

    assert(a.get_next() == &b);
    assert(c.get_prev() == &b);
    b.destroy();
    f.print_hedges();
    assert(a.get_next() == &c);
    assert(c.get_prev() == &a);

    // Si borro a entonces el puntero de la cara debe cambiar a c
    a.destroy();
    assert(a.get_next() == nullptr && c.get_prev() == nullptr);
    f.print_hedges();
    c.destroy();
    f.print_hedges();

    // Testeo cara vacía
    assert(f.get_perimeter() == 0);
    assert(f.get_area() == 0);
    assert(f.get_chain_length() == 0);
    assert(f.get_hedge() == nullptr);

    /**
      * Crea la estructura
      * p1 -> p2 -> p3 -> p4
      *    a     b     c
      */
    a = H_Edge<double>(&p2, &f, "a");
    b = H_Edge<double>(&p3, &f, "b");
    c = H_Edge<double>(&p4, &f, "c");
    d = H_Edge<double>(&p1, &f, "d");
    f.set_hedge(&a);
    a.set_next(&b);
    b.set_next(&c);
    c.set_next(&d);
    d.set_next(&a);
    assert(f.get_hedge() == &a);
    assert(a.get_next() == &b);
    assert(a.get_next()->get_next() == &c);
    assert(a.get_next()->get_next()->get_next() == &d);
    assert(d.get_next() == &a);
    assert(f.get_hedge()->get_next() == &b);
    assert(a.get_prev() == &d);
    f.print_hedges();
    assert(a.get_next() == &b && b.get_next() == &c && c.get_next() == &d && d.get_next() == &a);
    assert(a.get_prev() == &d && d.get_prev() == &c && c.get_prev() == &b && b.get_prev() == &a);

    d.destroy();
    f.print_hedges();
    assert(a.get_next() == &b && b.get_next() == &c && c.get_next() == &a);
    assert(a.get_prev() == &c && c.get_prev() == &b && b.get_prev() == &a);

    b.destroy();
    f.print_hedges();
    assert(a.get_next() == &c && c.get_next() == &a);
    assert(a.get_prev() == &c && c.get_prev() == &a);

    c.destroy();
    f.print_hedges();
    assert(a.get_next() == nullptr);
    assert(a.get_prev() == nullptr);

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
void test_deletion_complex() {

    print_title("Test-deletion-complex");

    /**
     * Crea la estructura
     */
    double MOD = 1.5;
    Face<double> t1 = Face<double>("T1");
    Face<double> t2 = Face<double>("T2");
    Point<double> p1 = Point<double>(0, 0) * MOD;
    Point<double> p2 = Point<double>(1, 0) * MOD;
    Point<double> p3 = Point<double>(0, 1) * MOD;
    Point<double> p4 = Point<double>(1, 1) * MOD;
    H_Edge<double> he23 = H_Edge<double>(&p3, &t1, "23");
    H_Edge<double> he31 = H_Edge<double>(&p1, &t1, "31");
    H_Edge<double> he12 = H_Edge<double>(&p2, &t1, "12");
    H_Edge<double> he24 = H_Edge<double>(&p4, &t2, "24");
    H_Edge<double> he32 = H_Edge<double>(&p2, &t2, "32");
    H_Edge<double> he43 = H_Edge<double>(&p3, &t2, "43");
    he12.set_next(&he23);
    he23.set_next(&he31);
    he31.set_next(&he12);
    he24.set_next(&he43);
    he43.set_next(&he32);
    he32.set_next(&he24);
    he23.set_pair(&he32);
    t1.print_hedges();
    t2.print_hedges();

    /**
     * Borra un elemento
     */
    std::cout << "HE23 DELETED" << std::endl;
    he23.destroy();
    t1.print_hedges();
    t2.print_hedges();
    assert(he32.get_pair() == nullptr && he23.get_pair() == nullptr);

    std::cout << "HE12 DELETED" << std::endl;
    he12.destroy();
    t1.print_hedges();
    assert(!he31.is_conected());
    assert(t1.get_chain_length() == 1);

}

/**
 * Testea las caras con largos infinitos.
 */
void test_infinite_loop() {

    print_title("Test-infinite-loop");

    /**
     * Crea la estructura
     * x -> p2 -> p3 -> p4
     *   a     b     c
     */
    Face<double> f = Face<double>("F1");

    Point<double> p1 = Point<double>(0, 0);
    Point<double> p2 = Point<double>(1, 0);
    Point<double> p3 = Point<double>(2, 0);

    H_Edge<double> b = H_Edge<double>(&p2, &f, "b");
    H_Edge<double> c = H_Edge<double>(&p3, &f, "c");
    H_Edge<double> a = H_Edge<double>(&p1, &f, "a");

    a.set_next(&b);
    b.set_next(&c);
    c.set_next(&b); // Acá hay un loop, salta de a>b>c>b>c...

    /**
     * Chequea que el largo de la cadena sea un error
     */
    f.print_hedges();
    f.print_points();
    assert(f.get_chain_length() == -1);
    assert(f.get_area() == 0);
    assert(!f.is_ccw());
    assert(!f.is_valid());
    assert(!f.in_face(&a));
    assert(!f.in_face(&b));
    assert(!f.in_face(&c));

}

/**
 * Testea que las caras estén conectadas de forma correcta.
 */
void test_connected_faces() {

    print_title("Test-connected-faces");

    /**
     * Crea la siguiente figura de 4 caras
     *
     *        6
     *      / C \
     *     4  -- 5
     *   / A\ D / B\
     *  1 --  2 --  3
     *
     */
    Face<double> a = Face<double>("A");
    Face<double> b = Face<double>("B");
    Face<double> c = Face<double>("C");
    Face<double> d = Face<double>("D");

    Point<double> p1 = Point<double>(0, 0);
    Point<double> p2 = Point<double>(1, 0);
    Point<double> p3 = Point<double>(2, 0);
    Point<double> p4 = Point<double>(0.5, 1);
    Point<double> p5 = Point<double>(1.5, 1);
    Point<double> p6 = Point<double>(1, 2);

    H_Edge<double> he12 = H_Edge<double>(&p2, &a, "12");
    H_Edge<double> he24 = H_Edge<double>(&p4, &a, "12");
    H_Edge<double> he41 = H_Edge<double>(&p1, &a, "12");
    H_Edge<double> he23 = H_Edge<double>(&p3, &b, "23");
    H_Edge<double> he35 = H_Edge<double>(&p5, &b, "35");
    H_Edge<double> he52 = H_Edge<double>(&p2, &b, "52");
    H_Edge<double> he45 = H_Edge<double>(&p5, &c, "45");
    H_Edge<double> he56 = H_Edge<double>(&p6, &c, "56");
    H_Edge<double> he64 = H_Edge<double>(&p4, &c, "64");
    H_Edge<double> he25 = H_Edge<double>(&p5, &d, "25");
    H_Edge<double> he54 = H_Edge<double>(&p4, &d, "54");
    H_Edge<double> he42 = H_Edge<double>(&p2, &d, "42");

    // Establece primer Edge
    a.set_hedge(&he12);
    b.set_hedge(&he23);
    c.set_hedge(&he45);
    d.set_hedge(&he25);

    // Crea relaciones siguientes
    he12.set_next(&he24); // A
    he24.set_next(&he41);
    he41.set_next(&he12);
    he23.set_next(&he35); // B
    he35.set_next(&he52);
    he52.set_next(&he23);
    he45.set_next(&he56); // C
    he56.set_next(&he64);
    he64.set_next(&he45);
    he54.set_next(&he42); // D
    he42.set_next(&he25);
    he25.set_next(&he54);

    // Establece pares
    he24.set_pair(&he42);
    he25.set_pair(&he52);
    he54.set_pair(&he45);

    /**
     * Imprime topología
     */
    a.print_hedges();
    b.print_hedges();
    c.print_hedges();
    d.print_hedges();

    /**
     * Verifica topología
     */
    assert(a.is_ccw() && b.is_ccw() && c.is_ccw() && d.is_ccw());
    assert(a.is_valid());

    /**
     * Verifica que no se puedan establecer relaciones inválidas
     */
    try {
        he24.set_pair(&he12);
        assert(false);
    } catch (...) {
        static_assert(true, "");
    }
    assert(he24.get_pair() == &he42);

}

/**
 * Corre los tests.
 */
int main() {

    // Mensaje en consola
    std::cout << "TEST HALF EDGE" << std::endl;

    // Corre los tests
    test_basic();
    test_ccw();
    test_2face();
    test_deletion_simple();
    test_deletion_complex();
    test_infinite_loop();
    test_connected_faces();

    // Retorna
    return 0;

}