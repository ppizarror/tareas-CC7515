/**
 * Testea vectores.
 * 
 * @author Pablo Pizarro
 * @date 30/09/2018
 */

// Importación de librerías
#include <iostream>
#include <cassert>
#include "../elements/test_vector.h"

/**
 * Testea normalizado de vectores.
 */
void test2D_normalize() {
    Vector<int> v = Vector<int>(1, 2);
    v.normalize();
    assert(v.getI() == 0 && v.getJ() == 1); // 0.5 -> 0 y 1.0 -> 1

    Vector<double> v2 = Vector<double>(1, 2);
    v2.normalize();
    assert(v2.getI() == 0.50f && v2.getJ() == 1.0f); // 0.5->0.5 y 1.0->1.0
}

/**
 * Testea producto punto entre vectores.
 */
void test2D_dot() {
    Vector<float> v1 = Vector<float>(3, 2);
    Vector<float> v2 = Vector<float>(2, -1);
    Vector<float> v3 = v1.dot(v2);
    assert(v3.getI() == 6.0f && v3.getJ() == -2.0f); // (3,2).(2,-1)=(6,-2)

    // Prueba cambiando a ceros
    v3.setI(0);
    v3.setJ(0);
    v1 = v1.dot(v3);
    assert(v1.getI() == 0.0f && v1.getJ() == 0.0f); // (3,2).(0,0)=(0,0)
}

/**
 * Testea producto cruz vectores en 2D.
 */
void test2D_cross() {
    Vector<float> v1 = Vector<float>(3, 2);
    Vector<float> v2 = Vector<float>(2, -1);
    Vector<float> v3 = v1.cross(v2);
    assert(v3.getI() == 0.0f && v3.getJ() == 0.0f && v3.getK() == -7.0f); // (3,2,0)x(2,-1,0)=(0,0,-7)

    v1 = Vector<float>(3, -4);
    v2 = Vector<float>(2, 6);
    v3 = v1.cross(v2);
    std::cout << v1 << " x " << v2 << " = " << v3 << std::endl;
    assert(v3 == Vector<float>(0, 0, 26));
}

/**
 * Testea producto cruz vectores en 3D.
 */
void test3D_cross() {
    Vector<float> v1 = Vector<float>(3, 2);
    Vector<float> v2 = Vector<float>(2, -1);
    Vector<float> v3 = v1.cross(v2);
    assert(v3.getI() == 0.0f && v3.getJ() == 0.0f && v3.getK() == -7.0f); // (3,2,0)x(2,-1,0)=(0,0,-7)

    // Prueba vectores en 3D
    v1 = Vector<float>(2, 0, 1);
    v2 = Vector<float>(1, -1, 3);
    v3 = v1.cross(v2);
    assert(v3.getI() == 1.0f && v3.getJ() == -5.0f && v3.getK() == -2.0f);

    // Vector 3D con componente 0
    v1 = Vector<float>(4, 1, 0);
    v2 = Vector<float>(3, 7, 0);
    v3 = v1.cross(v2);
    assert(v3.getI() == 0.0f && v3.getJ() == 0.0f && v3.getK() == 25.0f);

    // Vector 2D
    v1 = Vector<float>(3, -4);
    v2 = Vector<float>(2, 6);
    v3 = v1.cross(v2);
    std::cout << v1 << " x" << v2 << " = " << v3 << std::endl;

    // Prueba vectores en 3D
    v1 = Vector<float>(3, -4, 7);
    v2 = Vector<float>(2, 6, -1);
    v3 = v1.cross(v2);
    std::cout << v1 << "x" << v2 << " = " << v3 << std::endl;
}

/**
 * Testea vector desde punto.
 */
void test_from_point() {
    // Crea los puntos
    Point<double> p1 = Point<double>(3, -4, 7);
    Point<double> p2 = Point<double>(2, 6, -1);

    // Crea los vectores
    Vector<double> v1 = Vector<double>(p1);
    Vector<double> v2 = Vector<double>(p2);
    Vector<double> v3 = v1.cross(v2);

    // Imprime en consola
    assert(v3.getI() == -38.0f && v3.getJ() == 17.0f && v3.getK() == 26.0f);
}

/**
 * Tests de precisión.
 */
void test_precision() {
    std::cout << "\nTest de precision:" << std::endl;

    // Se crea vector en entero y doble-precisión
    Vector<int> v_i = Vector<int>(-1109, 420, 360);
    Vector<int> v_i2 = Vector<int>(1, -1, 1);

    Vector<double> v_d = Vector<double>(-1109, 420, 360);
    Vector<double> v_d2 = Vector<double>(1, -1, 1);

    // Productos punto y cruz
    Vector<int> vc_i = v_i.cross(v_i2);
    Vector<double> vc_d = v_d.cross(v_d2);

    // Retorna la norma
    std::cout << "\tNorma [Integer]: " << v_i.norm() << std::endl;
    std::cout << "\tNorma [Double] : " << v_d.norm() << std::endl;

    // Se normalizan
    std::cout << "\tVector [Integer] Normalizado: " << v_i.get_normalized() << std::endl;
    std::cout << "\tVector [Double]  Normalizado: " << v_d.get_normalized() << std::endl;

    // Productos cruz
    std::cout << "\tProducto cruz [Integer]: " << vc_i << std::endl;
    std::cout << "\tProducto cruz [Double] : " << vc_d << std::endl;
}

/**
 * Corre los test.
 */
int main() {
    std::cout << "Testeando vector" << std::endl;

    // Carga los tests
    test2D_cross();
    test2D_dot();
    test2D_normalize();
    test_from_point();
    test_precision();

    // Retorna
    return 0;
}