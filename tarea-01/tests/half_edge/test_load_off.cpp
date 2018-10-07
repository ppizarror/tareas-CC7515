/**
 * Testea carga archivos .off
 *
 * @package tests.half_edge
 * @author Pablo Pizarro
 * @date 06/10/2018
 */

// Importa librerías
#include <cassert>
#include "../../half_edge/loff/load_off.h"

/**
 * Testeo carga archivo bueno
 */
void test_load_good() {

    /**
     * Carga el archivo bueno
     */
    LoadOff<double> loff = LoadOff<double>();
    offObject<double> g = loff.load("../tests/half_edge/cubes2d_good.off");

}

/**
 * Corre los tests.
 */
int main() {

    // Mensaje en consola
    std::cout << "TEST LOAD OFF" << std::endl;

    // Corre los tests
    test_load_good();

    // Retorna
    return 0;

}