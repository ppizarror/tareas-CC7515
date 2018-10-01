/**
 * Clase Cara del Half-Edge. Contiene un puntero al Half-Edge de esta cara en CCW.
 *
 * @package half_edge
 * @author Pablo Pizarro
 * @date 01/10/2018
 */

#ifndef T_CC7515_HALFEDGE_FACE_H
#define T_CC7515_HALFEDGE_FACE_H

// Importación de librerías
#include "h_edge.h"

class Face {
private:
    H_Edge *edge;
};

#endif //T_CC7515_HALFEDGE_FACE_H