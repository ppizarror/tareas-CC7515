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
#include <string>

/**
 * Clase principal HalfEdge.
 *
 * @tparam T Template
 */
template<class T>
class H_Edge {
private:

    // Vértice al que apunta
    Vertex<T> vert;

    // Cara que contiene al Half-Edge
    Face<T> *face = nullptr;

    // Indica los half-edge anterior y posterior
    H_Edge *prev = nullptr, *next = nullptr;

    // Half edge par, que está en la otra cara
    H_Edge *pair = nullptr;

    // Nombre del HalfEdge
    std::string name = "%HALFEDGE_NAME";

    // Añade el HalfEdge anterior
    void set_prev(H_Edge<T> *he);

    // Establece la referencia al HalfEdge par del mismo objeto
    void set_pair_reference(H_Edge<T> *he);

public:

    // Constuctor vacío
    H_Edge();

    // Constructor con un vértice y cara
    H_Edge(Point<T> *p, Face<T> *f);

    // Constructor con vértice, cara y nombre
    H_Edge(Point<T> *p, Face<T> *f, std::string name);

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

    // Retorna el nombre del HalfEdge
    std::string get_name() const;

    // Retorna la forma en string del HalfEdge
    std::string to_string() const;

    // Operación concatenación con string
    template<class U>
    friend std::ostream &operator<<(std::ostream &out, const H_Edge<U> &he);

    // Destruye el HalfEdge
    void destroy();

    // Indica que el HalfEdge no está conectado
    bool is_conected() const;

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
    H_Edge();
    this->vert = Vertex<T>(p, this);
    this->face = f;
    this->face->set_hedge(this); // Guarda el último
}

template<class T>
/**
 * Constructor de clase, recibe el vértice al que apunta, la cara que lo contiene y el nombre.
 *
 * @tparam T Template
 * @param p Punto al que apunta
 * @param f Cara que lo contiene
 * @param name Nombre del HalfEdge
 */
H_Edge<T>::H_Edge(Point<T> *p, Face<T> *f, std::string name) {
    H_Edge();
    this->vert = Vertex<T>(p, this);
    this->face = f;
    this->name = name;
    f->set_hedge(this);
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
    if (this->face == nullptr) {
        std::cout << this->to_string() << std::endl;
        return;
    }
    std::cout << "HE " << this->name << " -> " << this->vert << " | FACE: " << this->face->get_name() << std::endl;
}

template<class T>
/**
 * Añade el HalfEdge par.
 *
 * @tparam T Template
 * @param he Puntero al HalfEdge
 */
void H_Edge<T>::set_pair(H_Edge<T> *he) {
    if (he == nullptr) {
        std::cerr << "HalfEdge pair pointer cannot be null";
        throw std::invalid_argument("Pair HalfEdge pointer cannot be null");
    }
    if (this == he) {
        std::cerr << "HalfEdge pointer can't be the same";
        throw std::invalid_argument("Pair HalfEdge pointer can't be the same");
    }
    if (he->get_face() == this->get_face()) {
        std::cerr << "HalfEdge pointer can't be in the same face of the origin";
        throw std::invalid_argument("HalfEdge pointer can't be in the same face of the origin");
    }
    if (this->pair != nullptr) { // Si ya había sido definida la relación borra el par de la referencia
        this->pair->pair = nullptr;
    }
    this->pair = he;
    he->set_pair_reference(this); // Doble link
}

template<class T>
/**
 * Añade la referencia al HalfEdge par.
 *
 * @tparam T Template
 * @param he Puntero al HalfEdge
 */
void H_Edge<T>::set_pair_reference(H_Edge<T> *he) {
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
    if (he == nullptr) {
        std::cerr << "HalfEdge next pointer cannot be null";
        throw std::invalid_argument("Next HalfEdge cannot be null");
    }
    if (this == he) {
        std::cerr << "HalfEdge pointer " << he << " is invalid";
        throw std::invalid_argument("Next HalfEdge cannot be the same");
    }
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

template<class T>
/**
 * Retorna el nombre del HalfEdge.
 *
 * @tparam T Template
 * @return
 */
std::string H_Edge<T>::get_name() const {
    return this->name;
}

template<class T>
/**
 * Retorna el nombre del HalfEdge.
 *
 * @tparam T Template
 * @return
 */
std::string H_Edge<T>::to_string() const {
    std::string str = "";
    str += "{" + this->get_name();

    // Si tiene par entonces imprime el nombre y el nombre de la cara contenedora
    H_Edge<T> *hp = this->get_pair();
    if (hp != nullptr) {
        str += "/" + hp->get_name() + "<" + hp->get_face()->get_name() + ">";
    }
    str += "}";

    // Retorna el valor
    return str;
}

template<class T>
/**
 * Operador concatenación con strings.
 *
 * @return Concatena con un strng
 */
std::ostream &operator<<(std::ostream &out, const H_Edge<T> &he) {
    out << he.to_string();
    return out;
}

template<class T>
/**
 * Destruye las relaciones del elemento.
 *
 * @tparam T
 */
void H_Edge<T>::destroy() {
    // Obtiene los extremos
    H_Edge<T> *hp = this->prev;
    H_Edge<T> *hn = this->next;

    if (hp == hn && hp != nullptr) {
        hp->next = nullptr;
        hn->prev = nullptr;
    }

    // Ve el anterior
    if (hp != nullptr) {
        hp->next = hn;
        if (hp->next == this || hp->next == hp) hp->next = nullptr;
    }

    // Ve el siguiente
    if (hn != nullptr) {
        hn->prev = hp;
        if (hn->prev == this || hn->prev == hn) hn->prev = nullptr;
    }

    // Ve el par
    if (this->pair != nullptr) {
        this->pair->pair = nullptr;
    }

    // Elimina parámetros internos
    this->next = nullptr;
    this->prev = nullptr;
    this->name = this->name + "<destroyed>";
    this->pair = nullptr;

    // Si el puntero de la cara apunta al mismo entonces lo corre
    if (this->face->get_hedge() == this) {
        if (hp != nullptr && hp != this) {
            this->face->set_hedge(hp);
        } else if (hn != nullptr && hn != this) {
            this->face->set_hedge(hn);
        } else {
            this->face->set_hedge(nullptr);
        };
        this->face = nullptr;
    }
}

template<class T>
/**
 * Indica que la estructura está conectada.
 *
 * @tparam T Template
 * @return
 */
bool H_Edge<T>::is_conected() const {
    return this->pair != nullptr && this->next != nullptr && this->prev != nullptr;
}

#endif //T_CC7515_HALFEDGE_HEDGE_H
#pragma clang diagnostic pop