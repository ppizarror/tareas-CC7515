/**
 * Game of Life. Tarea N3 Computación en GPU.
 * Código en CPU.
 */

#ifndef GAMEOFLIFECPU_GOL_H
#define GAMEOFLIFECPU_GOL_H

class GOL {
private:

    // Constantes
    int N;
    int M;

    //Variables para la ejecucion
    bool *matriz;
    bool *matrizAux;
    bool *aux;

public:

    // Constructor
    GOL(int N, int M);

    // Destructor
    virtual ~GOL();

    // Método que imprime la matriz en pantalla. No incliye las filas fantasmas
    void printGrid();

    // Método que cambia todas las celdas a falso. Incluye las filas fantasmas
    void setMatrizToFalse();

    // Función que ejecuta las reglas del juego de la vida
    void aplicarReglas();

    // Coloca las filas fantasmas en verdadero
    void inicializarBordesMatriz();

    /* Funcion que inicializa las matrices colocando los valores en random, no
     * modifica las filas fantasmas.
     *
     * @Param probTrue: Probabilidad de que una celda sea verdadera.
     */
    void inicializarMatrizRandom(int probTrue);

};

#endif // GAMEOFLIFECPU_GOL_H