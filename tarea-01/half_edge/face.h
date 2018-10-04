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
    void add_hedge(const H_Edge<T> *hedge);

    // Elimina un HalfEdge de la lista
    void remove_hedge(const H_Edge<T> *hedge);
    
    // Retorna el número de edges
    int number_edges();

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
 * @tparam T
 * @param hedge
 */
void Face<T>::add_hedge(const H_Edge<T> *hedge) {
    this->edges.push_back(hedge);
}

template<class T>
void Face<T>::remove_hedge(const H_Edge<T> *hedge) {
    for (unsigned i = 0; i < this->edges.size(); ++i) {
        if (this->edges[i] == hedge) {
            std::cout << "te pille" << std::endl;
        }
    }
}

#endif //T_CC7515_HALFEDGE_FACE_H