/**
 * Clase principal Half-Edge.
 *
 * @package half_edge
 * @author Pablo Pizarro
 * @date 01/10/2018
 */

// Inicia el bloque
#ifndef T_CC7515_HALFEDGE_HEDGE_H
#define T_CC7515_HALFEDGE_HEDGE_H

// Importa librerías
#include "face.h"
#include "vertex.h"

/**
 * Clase principal HalfEdge.
 *
 * @tparam T Template
 */
template<class T>
class H_Edge {
private:
    Vertex<T> *vert; // Vértice al que apunta
    Face<T> *face; // Cara que contiene al Half-Edge
    H_Edge *prev, *next; // Indica los half-edge anterior y posterior
    H_Edge *pair; // Half edge par, que contiene como vértice director el vértice origen
public:

    // Constructor con un vértice
    H_Edge(Vertex<T> &v, Face<T> &f);

    // Destructor
    ~H_Edge();

};

template<class T>
/**
 * Constructor de clase, recibe el vértice al que apunta y la cara que lo contiene.
 *
 * @tparam T Template
 * @param v Vértice al que apunta
 * @param f Cara que lo contiene
 */
H_Edge<T>::H_Edge(Vertex<T> &v, Face<T> &f) {
    this->vert = v;
    this->face = f;
}

template<class T>
/**
 * Destructor de clase.
 *
 * @tparam T Template
 */
H_Edge<T>::~H_Edge() {

}

#endif //T_CC7515_HALFEDGE_HEDGE_H