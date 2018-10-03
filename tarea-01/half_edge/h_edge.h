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

// Define las clases que se usan
template<class T>
class Face;

template<class T>
class Vertex;

/**
 * Clase principal HalfEdge.
 *
 * @tparam T Template
 */
template<class T>
class H_Edge {
private:
    Vertex<T> *vert;
    Face<T> *face;
    H_Edge *prev, *next;
    H_Edge *pair;
};

#endif //T_CC7515_HALFEDGE_HEDGE_H