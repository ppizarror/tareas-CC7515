/**
 * Game of Life. Tarea N3 Computación en GPU.
 * Código en CPU.
 */

#include <cstdlib>
#include <iostream>
#include "GOL.h"

#define SRAND_VALUE 1998 // Semilla para generar numeros random

/**
 * Constructor, crea matriz tamaño NXM.
 *
 * @param N Número de filas
 * @param M Número de columnas
 */
GOL::GOL(int N, int M) {
    // Para las filas fantasmas
    this->N = N + 2;
    this->M = M + 2;

    matriz = new bool[this->N * this->M];
    matrizAux = new bool[this->N * this->M];
}

/**
 * Destructor.
 */
GOL::~GOL() {
    delete[] matriz;
    delete[] matrizAux;
}

/**
 * Imprime la grilla.
 */
void GOL::printGrid() {
    // No imprime las celdas fantasmas
    for (int a = 1; a < N - 1; a++) {
        for (int b = 1; b < M - 1; b++) {
            if (!matriz[a * M + b]) {
                std::cout << " . ";
            } else {
                std::cout << " O ";
            }
            if (b == M - 2) {
                std::cout << std::endl;
            }
        }
    }
}

/**
 * Método que cambia todas las celdas a falso. Incluye las filas fantasmas.
 */
void GOL::setMatrizToFalse() {
    for (int a = 0; a < N; a++) {
        for (int b = 0; b < M; b++) {
            matriz[a * M + b] = false;
            matrizAux[a * M + b] = false;
        }
    }
}

/**
 * Función que ejecuta las reglas del juego de la vida.
 */
void GOL::aplicarReglas() {

    // Reglas generales
    for (int i = 1; i < N - 1; i++) {
        for (int j = 1; j < M - 1; j++) {

            // Calculamos cantidad de vecinos vivos
            int vivos = 0;
            for (int k = -1; k < 2; k++) {
                for (int p = -1; p < 2; p++) {
                    if (!(k == 0 && p == 0)) {
                        if (matriz[(i + k) * M + (j + p)]) {
                            vivos++;
                        }
                    }
                }
            }

            // Reglas del juego
            if ((vivos == 2 || vivos == 3) && matriz[i * M + j]) {
                matrizAux[i * M + j] = true;
            } else if (vivos == 3 && !matriz[i * M + j]) {
                matrizAux[i * M + j] = true;
            } else { //muere de otra forma
                matrizAux[i * M + j] = false;
            }
        }
    }

    // Cambiamos punteros
    aux = matriz;
    matriz = matrizAux;
    matrizAux = aux;

}

/**
 * Coloca las filas fantasmas en verdadero.
 */
void GOL::inicializarBordesMatriz() {
    for (int i = 0; i < N; i++) {
        matriz[i * M + 0] = true;
        matrizAux[i * M + 0] = true;
        matriz[i * M + M - 1] = true;
        matrizAux[i * M + M - 1] = true;
    }

    for (int i = 0; i < M; i++) {
        matriz[i] = true;
        matrizAux[i] = true;
        matriz[(N - 1) * M + i] = true;
        matrizAux[(N - 1) * M + i] = true;
    }
}

/**
 * Funcion que inicializa las matrices colocando los valores en random, no modifica las filas fantasmas.
 *
 * @param probTrue Probabilidad
 */
void GOL::inicializarMatrizRandom(int probTrue) {

    // Aplica semilla para generación valores aleatorios
    srand(SRAND_VALUE);
    for (int i = 1; i < N - 1; i++) {
        for (int j = 1; j < M - 1; j++) {
            int numero = std::rand() % 100;
            if (numero < probTrue) {
                matriz[i * M + j] = true;
                matrizAux[i * M + j] = true;
            }
        }
    }

}