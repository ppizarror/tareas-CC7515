/**
 * Clase Vertex. Contiene un índice (o puntero) a Point y un índice a uno de los
 * Half_Edge que lo contiene.
 *
 * @package half_edge
 * @author Pablo Pizarro
 * @date 01/10/2018
 */

#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-equals-default"
#ifndef T_CC7515_HALFEDGE_VERTEX_H
#define T_CC7515_HALFEDGE_VERTEX_H

// Importación de librerías
#include "../elements/vector.h"

// Definición de clase
template<class T>
class H_Edge;

template<class T>
/**
 * Vértices de la estructura de datos.
 *
 * @tparam T Template
 */
class Vertex {
private:
    Point<T> *p; // Punto del vértice, en 3D
    H_Edge<T> *edge; // Puntero del half-edge que lo contiene
public:

    // Constructor vacío
    Vertex();

    // Constructor
    Vertex(Point<T> *p, H_Edge<T> *he);

    // Destructor
    ~Vertex();

};

template<class T>
/**
 * Constructor vacío.
 *
 * @tparam T Template
 */
Vertex<T>::Vertex() {
}

template<class T>
/**
 * Constructor, sólo toma referencias.
 *
 * @tparam T Template
 * @param p Punto
 * @param he Half edge que apunta al punto
 */
Vertex<T>::Vertex(Point<T> *p, H_Edge<T> *he) {
    this->p = p;
    this->edge = he;
}

template<class T>
/**
 * Destructor.
 *
 * @tparam T Template
 */
Vertex<T>::~Vertex() {
}

#endif //T_CC7515_HALFEDGE_VERTEX_H
#pragma clang diagnostic pop