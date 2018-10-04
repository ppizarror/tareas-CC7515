/**
 * Clase principal Half-Edge.
 *
 * @package half_edge
 * @author Pablo Pizarro
 * @date 01/10/2018
 */

// Inicia el bloque
#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-equals-default"
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
    Vertex<T> vert; // Vértice al que apunta
    Face<T> *face; // Cara que contiene al Half-Edge
    H_Edge *prev, *next; // Indica los half-edge anterior y posterior
    H_Edge *pair; // Half edge par, que contiene como vértice director el vértice origen
public:

    // Constuctor vacío
    H_Edge();

    // Constructor con un vértice
    H_Edge(Point<T> *p, Face<T> *f);

    // Destructor
    ~H_Edge();

    // Retorna la cara
    Face<T> *get_face() const;

    // Imprime en consola
    void print() const;

};

template<class T>
/**
 * Constructor vacío.
 *
 * @tparam T Template
 */
H_Edge<T>::H_Edge() {
}

template<class T>
/**
 * Constructor de clase, recibe el vértice al que apunta y la cara que lo contiene.
 *
 * @tparam T Template
 * @param p Punto al que apunta
 * @param f Cara que lo contiene
 */
H_Edge<T>::H_Edge(Point<T> *p, Face<T> *f) {
    this->vert = Vertex<T>(p, this);
    this->face = f;
    this->face->add_hedge(this);
}

template<class T>
/**
 * Destructor de clase.
 *
 * @tparam T Template
 */
H_Edge<T>::~H_Edge() {
    this->face->remove_hedge(this);

}

template<class T>
/**
 * Retorna la cara.
 *
 * @tparam T Template
 * @return Cara asociada al HEdge
 */
Face<T> *H_Edge<T>::get_face() const {
    return this->face;
}

template<class T>
/**
 * Imprime en consola.
 *
 * @tparam T Template
 */
void H_Edge<T>::print() const {
    std::cout << "he -> " << this->vert << std::endl;
}

#endif //T_CC7515_HALFEDGE_HEDGE_H
#pragma clang diagnostic pop