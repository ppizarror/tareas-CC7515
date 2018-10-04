/**
 * Clase Cara del Half-Edge. Contiene un puntero al Half-Edge de esta cara en CCW.
 *
 * @package half_edge
 * @author Pablo Pizarro
 * @date 01/10/2018
 */

#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "modernize-use-equals-default"
#ifndef T_CC7515_HALFEDGE_FACE_H
#define T_CC7515_HALFEDGE_FACE_H

// Importa librerías
#include <iostream>
#include <vector>

// Definición de clase
template<class T>
class H_Edge;

template<class T>
/**
 * Clase cara.
 *
 * @tparam T Template
 */
class Face {
private:
    std::vector<H_Edge<T> *> edges; // Vector de todos los edges que llegan a esa cara
public:

    // Inicializa la cara
    Face();

    // Añade un HalfEdge a la lista
    void add_hedge(H_Edge<T> *hedge);

    // Elimina un HalfEdge de la lista
    void remove_hedge(H_Edge<T> *hedge);

    // Verifica si un HalfEdge está dentro de la lista o no
    bool in_face_half_edge(H_Edge<T> *hedge) const;

    // Retorna el número de edges
    unsigned long number_edges() const;

};

template<class T>
/**
 * Crea la cara.
 *
 * @tparam T
 */
Face<T>::Face() {
}

template<class T>
/**
 * Añade una referencia a un hedge a la lista.
 *
 * @tparam T Template
 * @param hedge Puntero al edge
 */
void Face<T>::add_hedge(H_Edge<T> *hedge) {
    this->edges.push_back(hedge);
}

template<class T>
/**
 * Remueve un edge de la cara.
 *
 * @tparam T Template
 * @param hedge Puntero al edge
 */
void Face<T>::remove_hedge(H_Edge<T> *hedge) {
    for (unsigned i = 0; i < this->edges.size(); ++i) {
        if (this->edges[i] == hedge) {
            this->edges.erase(this->edges.begin() + i);
            return;
        }
    }
}

template<class T>
/**
 * Retorna el número de edges que llegan a la cara.
 *
 * @tparam T Template
 * @return Número de edges
 */
unsigned long Face<T>::number_edges() const {
    return this->edges.size();
}

template<class T>
/**
 * Indica si un HalfEdge está dentro de una cara o no.
 *
 * @tparam T Template
 * @param hedge HalfEdGE
 * @return
 */
bool Face<T>::in_face_half_edge(H_Edge<T> *hedge) const {
    for (unsigned i = 0; i < this->edges.size(); ++i) {
        if (this->edges[i] == hedge) {
            return true;
        }
    }
    return false;
}

#endif //T_CC7515_HALFEDGE_FACE_H