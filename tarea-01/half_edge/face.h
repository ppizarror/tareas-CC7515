/**
 * Clase Cara del Half-Edge. Contiene un puntero al Half-Edge de esta cara en CCW.
 *
 * @package half_edge
 * @author Pablo Pizarro
 * @date 01/10/2018
 */

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "modernize-use-equals-default"
#ifndef T_CC7515_HALFEDGE_FACE_H
#define T_CC7515_HALFEDGE_FACE_H

// Importa librerías
#include "../elements/vector.h"
#include <iostream>
#include <string>

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

    // Puntero a un edge de la cara
    H_Edge<T> *edge = nullptr;

    // Nombre de la cara
    std::string name = "FACE";

    // Calcula el determinante de
    // | a b c |
    // | d e f |
    // | g h i |
    T determinant(T a, T b, T c, T d, T e, T f, T g, T h, T i) const;

    // Calcula el área entre 3 puntos
    T points_area(const Point<T> &a, const Point<T> &b, const Point<T> &c) const;

    // Calcula la distancia entre dos puntos
    T points_dist(const Point<T> &a, const Point<T> &b) const;

    // Verifica si tres puntos están en ccw
    bool points_ccw(const Point<T> &a, const Point<T> &b, const Point<T> &c) const;

public:

    // Inicializa la cara
    Face();

    // Inicializa la cara con un nombre
    Face(std::string name);

    // Añade un HalfEdge a la lista
    void set_hedge(H_Edge<T> *hedge);

    // Verifica si un HalfEdge está dentro de la lista o no
    bool in_face(H_Edge<T> *hedge) const;

    // Retorna el número de la cadena
    int chain_length() const;

    // Obtiene el área de la cara
    T get_area() const;

    // Obtiene el perímetro de la cara
    T get_perimeter() const;

    // Obtiene el nombre de la cara
    std::string get_name() const;

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
 * Se guarda la referencia al puntero.
 *
 * @tparam T Template
 * @param hedge Puntero al edge
 */
void Face<T>::set_hedge(H_Edge<T> *hedge) {
    this->edge = hedge;
}

template<class T>
/**
 * Retorna el área de la cara.
 *
 * @tparam T Template
 * @return
 */
T Face<T>::get_area() const {
    if (this->chain_length() < 3) return 0;

    // Itera desde el primer HE, para ello recorre next hasta llegar al mismo
    T tarea;
    H_Edge<T> *he = this->edge;
    H_Edge<T> *he1, *he2;
    Point<T> p0 = *he->get_point(); // Elige un punto cualquiera

    while (true) {
        he1 = he->get_next();
        he2 = he->get_next()->get_next();
        tarea += this->points_area(p0, *he1->get_point(), *he2->get_point());
        he = he->get_next();
        if (he == this->edge) break; // Se cumple el ciclo, termina
    }

    return tarea;
}

template<class T>
/**
 * Retorna el perímetro de la cara.
 *
 * @tparam T Template
 * @return
 */
T Face<T>::get_perimeter() const {
    if (this->chain_length() < 3) return 0;

    // Itera desde el primer HE, para ello recorre next hasta llegar al mismo
    T tperim;
    H_Edge<T> *he = this->edge;
    H_Edge<T> *he1; // Almacena el siguiente

    while (true) {
        he1 = he->get_next();
        tperim += this->points_dist(*he->get_point(), *he1->get_point());
        he = he->get_next();
        if (he == this->edge) break; // Se cumple el ciclo, termina
    }

    return tperim;
}


template<class T>
/**
 * Calcula el determinante de:
 *  | a b c |
 *  | d e f |
 *  | g h i |
 *
 * @tparam T Template
 * @param a
 * @param b
 * @param c
 * @param d
 * @param e
 * @param f
 * @param g
 * @param h
 * @param i
 * @return
 */
T Face<T>::determinant(T a, T b, T c, T d, T e, T f, T g, T h, T i) const {
    return a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
}

template<class T>
/**
 * Calcula el área entre tres puntos, algoritmo visto en CC5502.
 *
 * @tparam T Template
 * @param a Punto A
 * @param b Punto B
 * @param c Punto C
 * @return Área entre los tres puntos, puedes ser positiva o negativa
 */
T Face<T>::points_area(const Point<T> &a, const Point<T> &b, const Point<T> &c) const {
    T _area;
    if (a.get_dim() == 2 && b.get_dim() == 2 && c.get_dim() == 2) {
        _area = this->determinant(a.get_coord_x(), a.get_coord_y(), 1,
                                  b.get_coord_x(), b.get_coord_y(), 1,
                                  c.get_coord_x(), c.get_coord_y(), 1);
    } else if (a.get_dim() == 3 && b.get_dim() == 3 && c.get_dim() == 3) {
        _area = this->determinant(a.get_coord_x(), a.get_coord_y(), a.get_coord_z(),
                                  b.get_coord_x(), b.get_coord_y(), b.get_coord_z(),
                                  c.get_coord_x(), c.get_coord_y(), c.get_coord_z());
    } else {
        throw std::invalid_argument("Cant perform area with different point coordinates");
    }
    return 0.5 * _area;
}

template<class T>
/**
 * Calcula distancia entre dos puntos.
 *
 * @tparam T Template
 * @param a Punto A
 * @param b Punto B
 * @return retorna ||A-B||
 */
T Face<T>::points_dist(const Point<T> &a, const Point<T> &b) const {
    Point<T> c = b - a;
    return Vector<T>(c).norm();
}

template<class T>
/**
 * Verifica si tres puntos están en ccw o no.
 *
 * @tparam T Template
 * @param a Punto A
 * @param b Punto B
 * @param c Punto C
 * @return
 */
bool Face<T>::points_ccw(const Point<T> &a, const Point<T> &b, const Point<T> &c) const {
    return false;
}

template<class T>
/**
 * Retorna el número de la cadena de HalfEdges que conforman una cara.
 *
 * @tparam T Template
 * @return
 */
int Face<T>::chain_length() const {
    if (this->edge == nullptr) return 0;
    H_Edge<T> *he = this->edge;
    int chain = 0;
    while (true) {
        he = he->get_next();
        chain++;
        if (he == this->edge) break; // Se cumple el ciclo, termina
    }
    return chain;
}

template<class T>
/**
 * Indica si un HalfEdge está en una cara o no.
 *
 * @tparam T Template
 * @param hedge Puntero al HalfEdge objetivo
 * @return
 */
bool Face<T>::in_face(H_Edge<T> *hedge) const {
    if (this->edge == nullptr || hedge == nullptr) return false;
    H_Edge<T> *he = this->edge;
    while (true) {
        if (he == hedge) return true;
        he = he->get_next();
        if (he == this->edge) break; // Se cumple el ciclo, termina
    }
    return false;
}

template<class T>
/**
 * Inicia la cara con un nombre.
 *
 * @tparam T Template
 * @param name Nombre de la cara
 */
Face<T>::Face(std::string name) {
    this->name = name;
}

template<class T>
/**
 * Retorna el nombre de la cara.
 *
 * @tparam T Template
 * @return
 */
std::string Face<T>::get_name() const {
    return this->name;
}

#endif //T_CC7515_HALFEDGE_FACE_H
#pragma clang diagnostic pop