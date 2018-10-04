/**
 * Clase Cara del Half-Edge. Contiene un puntero al Half-Edge de esta cara en CCW.
 *
 * @package half_edge
 * @author Pablo Pizarro
 * @date 01/10/2018
 */

#ifndef T_CC7515_HALFEDGE_FACE_H
#define T_CC7515_HALFEDGE_FACE_H

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
    H_Edge<T> *edge;
};

#endif //T_CC7515_HALFEDGE_FACE_H