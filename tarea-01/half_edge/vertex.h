/**
 * Clase Vertex. Contiene un índice (o puntero) a Point y un índice a uno de los
 * Half_Edge que lo contiene.
 *
 * @package half_edge
 * @author Pablo Pizarro
 * @date 01/10/2018
 */

#ifndef T_CC7515_HALFEDGE_VERTEX_H
#define T_CC7515_HALFEDGE_VERTEX_H

// Importación de librerías
#include "../elements/vector.h"
#include "h_edge.h"

template<class T>
/**
 * Vértices de la estructura de datos.
 *
 * @tparam T Template
 */
class Vertex {
private:
    Point<T> p; // Punto del vértice, en 3D
    H_Edge<T> *edge; // Puntero del half-edge que lo contiene
};

#endif //T_CC7515_HALFEDGE_VERTEX_H