/**
 * Tests de cerradura convexa
 *
 * @author Pablo Pizarro
 * @date 18/06/18
 */

// Importación de librerías
#include "../utils/cerradura-convexa.h"
#include <cassert>
#include <iostream>
#include <random>
#include <ctime>

/**
 * Genera un número aleatorio entre dos valores flotantes.
 * @param a - Número menor
 * @param b - Número mayor
 * @return
 */
float randomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX; // NOLINT
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

/**
 * Genera un número aleatorio entero entre dos valores.
 * @param a - Número mayor
 * @param b - Número menor
 * @return
 */
int randomInt(int a, int b) {
    return (int) randomFloat(a, b);
}

/**
 * Detiene la ejecución del programa por un determinado número de milisegundos
 * @param miliseconds - Milisegundos
 * @return
 */
int nsleep(long miliseconds) {
    if (miliseconds == 0) return 0;
    struct timespec req, rem; // NOLINT
    if (miliseconds > 999) {
        req.tv_sec = (int) (miliseconds / 1000);
        req.tv_nsec = (miliseconds - ((long) req.tv_sec * 1000)) * 1000000;
    } else {
        req.tv_sec = 0;
        req.tv_nsec = miliseconds * 1000000;
    }
    return nanosleep(&req, &rem);
}

/**
 * Testeo básico, genera figuras simples de pocos puntos.
 */
void testBasico() {
    Punto<float> plist[] = {Punto<float>(5, 0), Punto<float>(6, 4), Punto<float>(4, 5), Punto<float>(1, 5),
                            Punto<float>(1, 0)}; // 5

    // [GiftWrapping]
    std::pair<Poligono<float>, int> cerraduraGW = giftWrapping(plist, 5);
    assert(cerraduraGW.second == 5);

    // [Graham scan]
    std::pair<Poligono<float>, int> cerraduraGS = grahamScan(plist, 5);
    assert(cerraduraGS.second == 5);

    // Verifica que ambos polígonos tengan iguales puntos
    assert(cerraduraGW.first.mismosPuntos(cerraduraGS.first));
}

/**
 * Testeo triangulo con puntos en hipotenusa.
 */
void testTriangulo() {
    Punto<int> triangulo[] = {Punto<int>(0, 0), Punto<int>(-1, -1), Punto<int>(2, 2), Punto<int>(3, 3),
                              Punto<int>(4, 4), Punto<int>(5, 5), Punto<int>(0, 0), Punto<int>(5, 0)}; // 8

    // [GiftWrapping]
    std::pair<Poligono<int>, int> cerraduraGW = giftWrapping(triangulo, 8);
    assert(cerraduraGW.second == 3);

    // [Graham scan]
    std::pair<Poligono<int>, int> cerraduraGS = grahamScan(triangulo, 8);
    assert(cerraduraGS.second == 3);

    // Verifica que ambos polígonos tengan iguales puntos
    assert(cerraduraGW.first.mismosPuntos(cerraduraGS.first));

}

template<class T>
/**
 * Testea un rombo con distintas rotaciones.
 */
void _testRombo(Punto<T> *rombo) {
    // [GiftWrapping]
    std::pair<Poligono<int>, int> cerraduraGW = giftWrapping(rombo, 4);
    assert(cerraduraGW.second == 4);

    // [Graham scan]
    std::pair<Poligono<int>, int> cerraduraGS = grahamScan(rombo, 4);
    assert(cerraduraGS.second == 4);

    // Verifica que ambos polígonos tengan iguales puntos
    assert(cerraduraGW.first.mismosPuntos(cerraduraGS.first));
}

/**
 * Testea la cerradura con la posición del menor en distintos lugares.
 */
void testRombo() {
    Punto<int> rombo[] = {Punto<int>(-1, 0), Punto<int>(0, 1), Punto<int>(1, 0), Punto<int>(0, -1)}; // 4
    _testRombo(rombo);
    swapPunto(rombo, 0, 1);
    _testRombo(rombo);
    swapPunto(rombo, 1, 2);
    _testRombo(rombo);
    swapPunto(rombo, 2, 3);
    _testRombo(rombo);
    swapPunto(rombo, 3, 0);
    _testRombo(rombo);
}

/**
 * Testeo duplicados.
 */
void testDuplicados() {
    Punto<int> plist[] = {Punto<int>(0, 0), Punto<int>(0, 0), Punto<int>(0, 0), Punto<int>(1, 1), Punto<int>(1, 0),
                          Punto<int>(1, 0)}; // 6

    // [GiftWrapping]
    std::pair<Poligono<int>, int> cerraduraGW = giftWrapping(plist, 6);
    assert(cerraduraGW.second == 3);

    // [Graham scan]
    std::pair<Poligono<int>, int> cerraduraGS = grahamScan(plist, 6);
    assert(cerraduraGS.second == 3);

    // Verifica que ambos polígonos tengan iguales puntos
    assert(cerraduraGW.first.mismosPuntos(cerraduraGS.first));
}

/**
 * Genera un cuadrado de 1x1 en flotante con 10 puntos dentro,
 * la cerradura debe ser (0,0),(0,1),(1,1),(1,0)
 */
void testCuadradoChico() {
    Punto<float> *cuadrado = new Punto<float>[10]; // NOLINT

    // Añade los puntos
    for (int i = 0; i < 10; i++) {
        cuadrado[i] = Punto<float>(randomFloat(0, 1), randomFloat(0, 1));
    }
    cuadrado[2] = Punto<float>(0, 0);
    cuadrado[5] = Punto<float>(0, 1);
    cuadrado[7] = Punto<float>(1, 1);
    cuadrado[8] = Punto<float>(1, 0);

    std::cout << "\n[GiftWrapping] Cerradura cuadrado perfecto 1x1 con 10 puntos" << std::endl;
    std::pair<Poligono<float>, int> cerraduraGW = giftWrapping(cuadrado, 10);
    cerraduraGW.first.print();
    assert(cerraduraGW.second == 4);

    std::cout << "\n[Graham Scan] Cerradura cuadrado perfecto 1x1 con 10 puntos" << std::endl;
    std::pair<Poligono<float>, int> cerraduraGS = grahamScan(cuadrado, 10);
    cerraduraGS.first.print();
    assert(cerraduraGS.second == 4);

    // Verifica que ambos polígonos tengan iguales puntos
    assert(cerraduraGW.first.mismosPuntos(cerraduraGS.first));

    // Borra la memoria
    delete[] cuadrado;
}

/**
 * Genera un cuadrado de 1x1 en flotante con 1000 puntos dentro,
 * la cerradura debe ser (0,0),(0,1),(1,1),(1,0)
 */
void testCuadradoMediano() {
    Punto<float> *cuadrado = new Punto<float>[1000]; // NOLINT

    // Añade los puntos
    for (int i = 0; i < 1000; i++) {
        cuadrado[i] = Punto<float>(randomFloat(0, 1), randomFloat(0, 1));
    }
    cuadrado[100] = Punto<float>(0, 0);
    cuadrado[200] = Punto<float>(0, 1);
    cuadrado[300] = Punto<float>(1, 1);
    cuadrado[400] = Punto<float>(1, 0);

    std::cout << "\n[GiftWrapping] Cerradura cuadrado perfecto 1x1 con 1000 puntos" << std::endl;
    std::pair<Poligono<float>, int> cerraduraGW = giftWrapping(cuadrado, 1000);
    cerraduraGW.first.print();
    assert(cerraduraGW.second == 4);

    std::cout << "\n[Graham Scan] Cerradura cuadrado perfecto 1x1 con 1000 puntos" << std::endl;
    std::pair<Poligono<float>, int> cerraduraGS = grahamScan(cuadrado, 1000);
    cerraduraGS.first.print();
    assert(cerraduraGS.second == 4);

    // Verifica que ambos polígonos tengan iguales puntos
    assert(cerraduraGW.first.mismosPuntos(cerraduraGS.first));
    assert(cerraduraGW.second == cerraduraGS.second);

    // Borra la memoria
    delete[] cuadrado;
}

/**
 * Genera un rectángulo de 10000 puntos completamente aleatorio, altura 1 y ancho 10.
 */
void testRectangulo() {
    Punto<float> *rectangulo = new Punto<float>[10000]; // NOLINT

    // Añade los puntos
    for (int i = 0; i < 10000; i++) {
        rectangulo[i] = Punto<float>(randomFloat(0, 10), randomFloat(0, 1));
    }
    rectangulo[1000] = Punto<float>(0, 0);
    rectangulo[2000] = Punto<float>(10, 0);
    rectangulo[3000] = Punto<float>(0, 1);
    rectangulo[4000] = Punto<float>(10, 1);

    // Calcula la cerradura
    std::cout << "\n[GiftWrapping] Cerradura rectangulo 10x1 con 10000 puntos" << std::endl;
    std::pair<Poligono<float>, int> cerraduraGW = giftWrapping(rectangulo, 10000);
    cerraduraGW.first.print();
    assert(cerraduraGW.second == 4);

    // Calcula la cerradura
    std::cout << "\n[Graham Scan] Cerradura rectangulo 10x1 con 10000 puntos" << std::endl;
    std::pair<Poligono<float>, int> cerraduraGS = giftWrapping(rectangulo, 10000);
    cerraduraGS.first.print();
    std::cout << "" << std::endl;
    assert(cerraduraGS.second == 4);

    // Verifica que ambos polígonos tengan iguales puntos
    assert(cerraduraGW.first.mismosPuntos(cerraduraGS.first));

    // Borra la memoria
    delete[] rectangulo;
}

/**
 * Testea una figura sin anclajes predefinidos.
 * @param sz - Número de puntos
 * @param r - Porcentaje de repetidos en vértices
 */
void __testFiguraDeforme(double sz, int r) {
    auto n = static_cast<int>(sz);
    Punto<float> *figura = new Punto<float>[n]; // NOLINT
    int tinit;

    // Total de aleatorios
    auto alt = (int) ((((float) r) / 100) * (float) n);

    // Añade los puntos aleatorios
    for (int i = 0; i < n; i++) {
        figura[i] = Punto<float>(randomFloat(-1, 1), randomFloat(-1, 1));
    }

    // Añade los puntos repetidos
    int rindex;
    int rpos;
    for (int i = 0; i < alt; i++) {
        rindex = randomInt(0, alt - 1);
        rpos = randomInt(i, n - 1);
        figura[rpos] = Punto<float>(figura[rindex].getCoordX(), figura[rindex].getCoordY());
    }

    // Calcula la cerradura con Gift Wrapping
    tinit = clock();
    std::pair<Poligono<float>, int> cerraduraGW = giftWrapping(figura, n);
    int t1 = medirTiempo(tinit);

    // Calcula cerradura con Graham Scan
    tinit = clock();
    std::pair<Poligono<float>, int> cerraduraGS = grahamScan(figura, n);
    int t2 = medirTiempo(tinit);

    // Imprime tiempos
    std::cout << r << "\t" << t1 << "\t" << t2 << std::endl;

    // Verifica que ambos polígonos tengan iguales puntos
    assert(cerraduraGW.first.mismosPuntos(cerraduraGS.first));

    // Borra la memoria
    delete[] figura;
}

/**
 * Test figura deforme, llama a testFiguraDeforme con distintos valores de r (%) porcentaje de puntos repetidos.
 * @param n - Número de puntos
 */
void testeoDeforme(double n) {
    int r[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90};
    std::cout << "[Puntos repetidos r%] Cerradura figura aleatoria con " << n << " puntos" << std::endl;
    for (int i : r) {
        __testFiguraDeforme(n, i);
        nsleep(0);
    }
    std::cout << "============================================================" << std::endl;
}

/**
 * Revisa los algoritmos con un porcentaje de ellos en la cerradura, figura aleatoria.
 * @param sz - Tamaño de la nube de puntos
 * @param s - Porcentaje de la nube en la cerradura
 */
void __testPuntosEnCerradura(double sz, int s) {
    auto n = static_cast<int>(sz);
    Punto<float> *figura = new Punto<float>[n]; // NOLINT
    int tinit;

    // Total de puntos aleatorios
    auto alt = (int) ((((float) s) / 100) * (float) n);

    // Añade los puntos dentro de la cerradura
    for (int i = 0; i < n; i++) {
        figura[i] = Punto<float>(randomFloat(0, 1), randomFloat(0, 1));
    }

    // Calcula la cerradura para añadir los puntos
    std::pair<Punto<float> *, int> cerradura = __grahamScan(figura, n);

    // Añade los puntos de la cerradura a la figura
    int randomPosc = 0, randomPosFig = 0;
    for (int i = 0; i < alt; i++) {
        randomPosc = randomInt(0, cerradura.second - 1); // Posición random dentro de la cerradura
        randomPosFig = randomInt(i, n - 1); // Posición radom dentro de la figura
        figura[randomPosFig] = cerradura.first[randomPosc];
    }

    // Calcula la cerradura con Gift Wrapping
    tinit = clock();
    std::pair<Poligono<float>, int> cerraduraGW = giftWrapping(figura, n);
    int t1 = medirTiempo(tinit);

    // Calcula cerradura con Graham Scan
    tinit = clock();
    std::pair<Poligono<float>, int> cerraduraGS = grahamScan(figura, n);
    int t2 = medirTiempo(tinit);

    // Imprime tiempos
    std::cout << s << "\t" << t1 << "\t" << t2 << std::endl;

    // Verifica que ambos polígonos tengan iguales puntos
    assert(cerraduraGW.first.mismosPuntos(cerraduraGS.first));

    // Borra la memoria
    delete[] figura;
}

/**
 * Testea puntos en cerradura con distintos porcentajes de s, figura cualquiera.
 * @param n - Número de puntos
 */
void testEnCerradura(double n) {
    int s[] = {0, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90};
    std::cout << "[Puntos en cerradura s%] Cerradura figura aleatoria con " << n << " puntos" << std::endl;
    for (int i : s) {
        __testPuntosEnCerradura(n, i);
        nsleep(0);
    }
    std::cout << "============================================================" << std::endl;
}

/**
 * Forza un cuadrado perfecto.
 */
bool CUADRADO_PERFECTO = false;

/**
 * Revisa los algoritmos con un porcentaje de ellos en la cerradura, cuadrado 1x1.
 * @param sz - Tamaño de la nube de puntos dentro del cuadrado
 * @param s - Porcentaje de la nube en la cerradura
 */
void __testPuntosEnCerraduraCuadrado(double sz, int s) {
    auto n = static_cast<int>(sz);
    Punto<float> *figura = new Punto<float>[n]; // NOLINT
    int tinit;

    // Total de puntos aleatorios
    auto alt = (int) ((((float) s) / 100) * (float) n);

    // Añade los puntos dentro de la cerradura
    for (int i = 0; i < n; i++) {
        figura[i] = Punto<float>(randomFloat(0, 1), randomFloat(0, 1));
    }

    // Añade los puntos aleatorios
    int ladoCuadrado = 0, posAleatoria = 0;
    for (int i = 0; i < alt; i++) {
        ladoCuadrado = randomInt(1, 4);
        posAleatoria = randomInt(i, n - 1);
        switch (ladoCuadrado) {
            case 1: // Lado izquierdo
                figura[posAleatoria] = Punto<float>(0, randomFloat(0, 1));
                break;
            case 2: // Lado derecho
                figura[posAleatoria] = Punto<float>(1, randomFloat(0, 1));
                break;
            case 3: // Lado superior
                figura[posAleatoria] = Punto<float>(randomFloat(0, 1), 0);
                break;
            case 4: // Lado inferior
                figura[posAleatoria] = Punto<float>(randomFloat(0, 1), 1);
                break;
            default:
                break;
        }
    }

    // Añade aristas para hacer un cuadrado perfecto
    if (CUADRADO_PERFECTO) {
        auto nstart = static_cast<int>(n * 0.1);
        figura[2 * nstart] = Punto<float>(0, 0); // 20%
        figura[4 * nstart] = Punto<float>(1, 1); // 40%
        figura[6 * nstart] = Punto<float>(0, 1); // 60%
        figura[8 * nstart] = Punto<float>(1, 0); // 80%
    }

    // Calcula la cerradura con Gift Wrapping
    tinit = clock();
    std::pair<Poligono<float>, int> cerraduraGW = giftWrapping(figura, n);
    int t1 = medirTiempo(tinit);

    // Calcula cerradura con Graham Scan
    tinit = clock();
    std::pair<Poligono<float>, int> cerraduraGS = grahamScan(figura, n);
    int t2 = medirTiempo(tinit);

    // Imprime tiempos
    std::cout << s << "\t" << t1 << "\t" << t2 << std::endl;

    // Verifica que ambos polígonos tengan iguales puntos
    assert(cerraduraGW.first.mismosPuntos(cerraduraGS.first));

    // Borra la memoria
    delete[] figura;
}

/**
 * Testea puntos en cerradura con distintos porcentajes de s, cuadrado 1x1.
 * @param n - Número de puntos
 */
void testEnCerraduraCuadrado(double n) {
    int s[] = {0, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90};
    std::cout << "[Puntos en cerradura s%] Cerradura cuadrado 1x1 con " << n << " puntos" << std::endl;
    for (int i : s) {
        __testPuntosEnCerraduraCuadrado(n, i);
        nsleep(0);
    }
    std::cout << "============================================================" << std::endl;
}

/**
 * Tipo de test experimento.
 *  1:  Testeo puntos con porcentaje de repetidos
 *  2:  Testeo puntos en cerradura para figura cualquiera
 *  3:  Testeo puntos en cerradura para un cuadrado 1x1 perfecto
 */
int TIPO_TEST = 2;

/**
* Corre los test.
*/
int main() {
    std::cout << "Testeando cerradura convexa" << std::endl;

    // Carga los tests
    testBasico(); // Test sencillo con puros puntos en la cerradura
    testDuplicados(); // Testea varios puntos duplicados
    testTriangulo(); // Testea puntos colineales en la hipotenusa
    testRombo(); // Test que comprueba distintas posiciones para los pivotes
    testCuadradoChico(); // Test cuadrado 1x1 con 10 puntos aleatorios
    testCuadradoMediano(); // Test cuadrado 1x1 con 1e4 puntos aleatorios
    testRectangulo(); // Test rectángulo 10x1 con 1e4 puntos aleatorios

    double sz[] = {3 * 1e5, 2 * 1e5, 1e5, 5 * 1e4};
    std::cout << "============================================================" << std::endl;
    switch (TIPO_TEST) {
        case 1: // Testeo puntos con porcentaje r (%) repetidos, figura aleatoria
            for (double i:sz) testeoDeforme(i);
            break;
        case 2: // Testeo puntos con porcentaje s (%) en cerradura, figura aleatoria
            for (double i:sz) testEnCerradura(i);
            break;
        case 3: // Testeo puntos con porcentaje s (%) en cerradura, cuadrado 1x1
            for (double i:sz) testEnCerraduraCuadrado(i);
            break;
        default:
            throw std::logic_error("Tipo test desconocido");
    }

    // Retorna
    return 0;
}