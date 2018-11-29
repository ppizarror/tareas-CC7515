#include <iostream>
#include <ctime>
#include "GOL.h"

/* Declaracion de constantes */
const int N = 20;
const int M = 20;
const bool imprimir = false;

/* Rutina Principal */
int main() {

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
    std::cout << "Numero de evaluaciones: " << Nevaluaciones << std::endl;

    // Elimina variables
    delete game;
    return 0;

}