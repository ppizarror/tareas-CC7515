#include <iostream>
#include <ctime>
#include "GOL.h"
#include <fstream>

/* Declaracion de constantes */
const bool imprimir = false;

/* Rutina Principal */
int main() {

    // Carga NxM desde un archivo
    std::ifstream infile;
    infile.open("NxM.txt");
    int x;
    int N = 0;
    int M = 0;
    int jfile = 0;
    while (infile >> x) {
        if (jfile == 0) { N = x; }
        else { M = x; }
        jfile = 1;
    }
    infile.close();
    printf("Cargando matriz %dx%d\n", N, M);

    // Variables para medir tiempo
    signed t0, t1;
    double time = 0;

    // variables para medir eficiencia
    int Nevaluaciones = 0;

    // Variables para la ejecucion
    GOL *game = new GOL(N, M);

    // Inicializacion de la matriz
    game->setMatrizToFalse();

    // Colocamos los valores iniciales
    game->inicializarBordesMatriz();

    t0 = static_cast<int>(clock());
    while (time < 1.0) {
        if (imprimir) { // Imprimimos de ser necesario
            game->printGrid();
            std::cout << std::endl;
        }

        // Se aplican las reglas del juego
        game->aplicarReglas();

        // Recalculamos valores
        Nevaluaciones += N * M;
        t1 = static_cast<int>(clock());
        time = (double(t1 - t0) / CLOCKS_PER_SEC);
    }

    std::cout << "Tiempo de ejecucion: " << time << std::endl;
    std::cout << "Numero de operaciones efectuadas: " << Nevaluaciones << std::endl;

    // Elimina variables
    delete game;
    return 0;

}