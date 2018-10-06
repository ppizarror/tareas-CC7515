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

// Constantes
const int MAX_RECURSION_DEPTH = 1000;

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
    std::string name = "%FACE_NAME";

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
    int get_chain_length() const;

    // Obtiene el área de la cara
    T get_area() const;

    // Obtiene el perímetro de la cara
    T get_perimeter() const;

    // Obtiene el nombre de la cara
    std::string get_name() const;

    // Indica que la cara está en ccw
    bool is_ccw() const;

    // Verifica si una cara es válida (+3 puntos conexos conectados)
    bool is_valid() const;

    // Imprime los puntos en lista
    void print_points() const;

    // Imprime los HalfEdge en lista
    void print_hedges() const;

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
 * Retorna el número de la cadena de HalfEdges que conforman una cara.
 *
 * @tparam T Template
 * @return
 */
int Face<T>::get_chain_length() const {
    if (this->edge == nullptr) return 0;
    H_Edge<T> *he = this->edge;
    int length = 0; // Contador de la cadena
    while (true) {
        if (he->get_next() == nullptr) {
            std::cerr << "Halfedge " + he->get_name() + " don't point to any next HalfEdge structure" << std::endl;
            throw std::logic_error("Cannot compute chain length of an non-closed Face");
        }
        he = he->get_next();
        length++;
        if (length > MAX_RECURSION_DEPTH) {
            std::cerr << "Face " + this->get_name() << " reached maximum recursion depth" << std::endl;
            throw std::logic_error("Exceeded recursion depth in Face HalfEdge path");
        }
        if (he == this->edge) break; // Se cumple el ciclo, termina
    }
    return length; // Está conectado
}

template<class T>
/**
 * Indica si una cara es válida, debe tener al menos tres puntos, todos conexos entre sí.
 *
 * @tparam T Template
 * @return
 */
bool Face<T>::is_valid() const {
    return this->get_chain_length() >= 3;
}

template<class T>
/**
 * Retorna el área de la cara.
 *
 * @tparam T Template
 * @return
 */
T Face<T>::get_area() const {
    if (!this->is_valid()) return false;

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
    if (!this->is_valid()) return false;

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
    return this->points_area(a, b, c) > 0;
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
    if (!this->is_valid() || hedge == nullptr) return false;
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
    Face(); // Llama al constructor vacío
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

template<class T>
/**
 * Indica que la cara está en CCW.
 *
 * @tparam T Template
 * @return
 */
bool Face<T>::is_ccw() const {
    if (!this->is_valid()) return false;
    H_Edge<T> *he = this->edge;
    H_Edge<T> *he2 = this->edge->get_next(); // Punto siguiente siguiente
    H_Edge<T> *he3 = he2->get_next(); // Punto siguiente siguiente
    while (true) {
        he->get_next();
        he2->get_next();
        he3->get_next();
        if (!this->points_ccw(*he->get_point(), *he2->get_point(), *he3->get_point())) {
            return false;
        }
        if (he == this->edge) break; // Se cumple el ciclo, termina
    }
    return true;
}

template<class T>
/**
 * Imprime la lista de puntos.
 *
 * @tparam T Template
 */
void Face<T>::print_points() const {
    if (this->edge == nullptr) return;
    H_Edge<T> *he = this->edge;
    Point<T> *p;
    std::string point_list = ""; // Almacena la lista en formato a->b->c...
    while (true) { // Recorre cada he y pregunta por los puntos
        p = he->get_point();
        point_list += p->to_string();
        he = he->get_next();
        if (he == nullptr) {
            point_list += "NULL";
            break;
        }
        if (he == this->edge) {
            break;
        } else {
            point_list += " -> ";
        }
    }
    std::cout << this->get_name() << " : " << point_list << std::endl; // Imprime la cadena
}

template<class T>
/**
 * Imprime la estructura de halfEdges.
 *
 * @tparam T Template
 */
void Face<T>::print_hedges() const {
    if (this->edge == nullptr) return;
    H_Edge<T> *he = this->edge;
    std::string edge_list = "";
    while (true) { // Recorre cada he y pregunta por los HE
        edge_list += he->to_string();
        he = he->get_next();
        if (he == nullptr) {
            edge_list += "NULL";
            break;
        }
        if (he == this->edge) {
            break;
        } else {
            edge_list += " -> ";
        }
    }
    std::cout << this->get_name() << " : " << edge_list << std::endl; // Imprime la cadena
}

#endif //T_CC7515_HALFEDGE_FACE_H
#pragma clang diagnostic pop