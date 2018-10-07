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
 * Imprime el título del test
 * @param title Título del test
 */
void print_title(const std::string &title) {
    std::cout << "" << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << title << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;
}

/**
 * Testeo carga archivo bueno
 */
void test_load_good() {

    print_title("Test-load-good");

    /**
     * Carga el archivo bueno
     */
    LoadOff<double> lo = LoadOff<double>();
    offObject<double> off = lo.load("../tests/half_edge/cubes2d_good.off");

    /**
     * Verifica que está en ccw
     */
    assert(lo.is_ccw(&off));


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