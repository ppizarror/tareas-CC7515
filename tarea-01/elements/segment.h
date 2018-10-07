/**
 * Clase Segmento.
 * Elemento creado por dos puntos, con un determinado largo.
 *
 * @package elements
 * @author Pablo Pizarro
 * @date 18/04/2018
 */

// Definición de constantes
#ifndef T_CC7515_SEGMENT_H
#define T_CC7515_SEGMENT_H
#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-equals-default"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"

// Importación de librerías
#include "vector.h"

template<class T>
/**
 * Clase Segmento, conformado por dos puntos.
 *
 * @tparam T Template
 */
class Segment {
private:

    // Puntos del segmento
    Point<T> *a;
    Point<T> *b;

    // Calcula el doble del área entre el segmento y un punto externo
    T area2(Point<T> &c);

public:

    // Constructor vacío
    Segment();

    // Constructor, recibe dos puntos
    Segment(Point<T> &p1, Point<T> &p2);

    // Retorna el tamaño del segmento
    double get_length() const;

    // Retorna el tamaño del segmento
    double norm() const;

    // Indica si un punto está estrictamente a la izquierda del segmento
    bool left(Point<T> &p);

    // Indica si un punto está estrictamente a la derecha del segmento
    bool right(Point<T> &p);

    // Indica que un punto está encima del segmento
    bool on(Point<T> &p);

    // Indica si un punto está a la izquierda o sobre el segmento
    bool left_on(Point<T> &p);

    // Indica si un punto está a la izquierda o sobre el segmento
    bool right_on(Point<T> &p);

    // Imprime el segmento en consola
    void print() const;
};

template<class T>
/**
 * Constructor.
 *
 * @tparam T
 * @param p1 - Punto 1
 * @param p2 - Punto 2
 */
Segment<T>::Segment(Point<T> &p1, Point<T> &p2) {
    this->a = &p1;
    this->b = &p2;
}

template<class T>
/**
 * Retorna el largo del segmento.
 *
 * @tparam T - Template
 * @return
 */
double Segment<T>::get_length() const {
    return this->a->dist(*this->b);
}

template<class T>
/**
 * Norma = get_length.
 *
 * @tparam T - Template
 * @return
 */
double Segment<T>::norm() const {
    return this->get_length();
}

template<class T>
/**
 * Retorna el área con otro punto.
 *
 * @tparam T - Template
 * @param c - Punto a calcular área
 * @return
 */
T Segment<T>::area2(Point<T> &c) {
    T area = (this->b->get_coord_x() - this->a->get_coord_x()) * (c.get_coord_y() - this->a->get_coord_y()) -
             (this->b->get_coord_y() - this->a->get_coord_y()) * (c.get_coord_x() - this->a->get_coord_x());
    return area;
}

template<class T>
/**
 * Indica si un punto está estrictamente a la izquierda del segmento.
 *
 * @tparam T - Template
 * @param p - Punto
 * @return
 */
bool Segment<T>::left(Point<T> &p) {
    return this->area2(p) > 0.0f;
}

template<class T>
/**
 * Indica si un punto está estrictamente a la derecha del segmento.
 *
 * @tparam T - Template
 * @param p - Punto
 * @return
 */
bool Segment<T>::right(Point<T> &p) {
    return this->area2(p) < 0.0f;
}

template<class T>
/**
 * Indica si un punto está encima del segmento.
 *
 * @tparam T - Template
 * @param p - Punto
 * @return
 */
bool Segment<T>::on(Point<T> &p) {
    return this->area2(p) == 0.0f;
}

template<class T>
/**
 * Indica si un punto está encima o a la izquierda del segmento.
 *
 * @tparam T - Template
 * @param p - Punto
 * @return
 */
bool Segment<T>::left_on(Point<T> &p) {
    return this->area2(p) >= 0.0f;
}

template<class T>
/**
 * Indica si un punto está encima o a la derecha del segmento.
 *
 * @tparam T - Template
 * @param p - Punto
 * @return
 */
bool Segment<T>::right_on(Point<T> &p) {
    return this->area2(p) <= 0.0f;
}

template<class T>
/**
 * Constructor vacío.
 *
 * @tparam T - Template
 */
Segment<T>::Segment() {}

template<class T>
/**
 * Imprime el segmento en la consola.
 *
 * @tparam T - Template
 */
void Segment<T>::print() const {
    std::cout << this->a->toString() << "->" << this->b->toString() << std::endl;
}

#endif //T_CC7515_SEGMENT_H