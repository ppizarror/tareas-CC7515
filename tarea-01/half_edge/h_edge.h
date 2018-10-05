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
#include <stdexcept>

/**
 * Clase principal HalfEdge.
 *
 * @tparam T Template
 */
template<class T>
class H_Edge {
private:
    Vertex<T> vert; // Vértice al que apunta
    Face<T> *face = nullptr; // Cara que contiene al Half-Edge
    H_Edge *prev = nullptr, *next = nullptr; // Indica los half-edge anterior y posterior
    H_Edge *pair = nullptr; // Half edge par, que contiene como vértice director el vértice origen

    // Añade el HalfEdge anterior
    void set_prev(H_Edge<T> *he);

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

    // Añade el HalfEdge par
    void set_pair(H_Edge<T> *he);

    // Añade el HalfEdge posterior;
    void set_next(H_Edge<T> *he);

    // Obtiene el HalfEdge par
    H_Edge<T> *get_pair() const;

    // Obtiene el HalfEdge siguiente
    H_Edge<T> *get_next() const;

    // Obtiene el HalfEdge anterior
    H_Edge<T> *get_prev() const;

    // Entrega el punto al que apunta cada HalfEdge
    Point<T> *get_point() const;

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
    this->face->set_hedge(this); // Guarda el último
}

template<class T>
/**
 * Destructor de clase.
 *
 * @tparam T Template
 */
H_Edge<T>::~H_Edge() {
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
    std::cout << "HE -> " << this->vert << " | FACE: " << this->face->get_name() << std::endl;
}

template<class T>
/**
 * Añade el HalfEdge par.
 *
 * @tparam T Template
 * @param he Puntero al HalfEdge
 */
void H_Edge<T>::set_pair(H_Edge<T> *he) {
    if (this == he) throw std::invalid_argument("Pair HalfEdge cant be the same");
    this->pair = he;
}

template<class T>
/**
 * Añade el HalfEdge siguiente.
 *
 * @tparam T Template
 * @param he Puntero al HalfEdge
 */
void H_Edge<T>::set_next(H_Edge<T> *he) {
    if (this == he) throw std::invalid_argument("Next HalfEdge cant be the same");
    this->next = he;
    he->set_prev(this);
}

template<class T>
/**
 * Añade el HalfEdge previo.
 *
 * @tparam T Template
 * @param he Puntero al HalfEdge
 */
void H_Edge<T>::set_prev(H_Edge<T> *he) {
    if (this == he) throw std::invalid_argument("Prev HalfEdge cant be the same");
    this->prev = he;
}

template<class T>
/**
 * Retorna el puntero al HalfEdge par.
 *
 * @tparam T Template
 * @return
 */
H_Edge<T> *H_Edge<T>::get_pair() const {
    return this->pair;
}

template<class T>
/**
 * Retorna el puntero al HalfEdge siguiente.
 *
 * @tparam T Template
 * @return
 */
H_Edge<T> *H_Edge<T>::get_next() const {
    return this->next;
}

template<class T>
/**
 * Retorna el puntero al HalfEdge previo.
 *
 * @tparam T Template
 * @return
 */
H_Edge<T> *H_Edge<T>::get_prev() const {
    return this->prev;
}

template<class T>
/**
 * Retorna el punto al que apunta el HalfEdge.
 *
 * @tparam T Template
 * @return
 */
Point<T> *H_Edge<T>::get_point() const {
    return this->vert.get_point();
}

#endif //T_CC7515_HALFEDGE_HEDGE_H
#pragma clang diagnostic pop