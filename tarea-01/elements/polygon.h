/**
 * Clase Polígono.
 * Tiene funciones para calcular el área.
 *
 * @package elements
 * @author Pablo Pizarro
 * @date 19/04/2018
 */

// Definición del bloque
#ifndef T_CC7515_POLYGON_H
#define T_CC7515_POLYGON_H
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err60-cpp"
#pragma ide diagnostic ignored "modernize-use-auto"
#pragma ide diagnostic ignored "modernize-use-equals-default"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "readability-redundant-string-init"

// Importación de librerías
#include "segment.h"
#include <cmath>

template<class T>
/**
 * Clase Polígono, conformada por Puntos.
 *
 * @tparam T - Template
 */
class Polygon {
private:

    // Arreglo de puntos
    Point<T> *puntos;

    // Puntos totales
    int totalp = 0;

    // Puntos máximos
    int MAX_POINTS = 100;

    // Calcula el área entre 3 puntos
    T area2(Point<T> &a, Point<T> &b, Point<T> &c) const;

public:

    // Constructor, no toma parámetros
    Polygon();

    // Se crea con lista de puntos
    Polygon(Point<T> *plist, int size);

    // Destructor
    ~Polygon();

    // Agrega un punto
    void add_point(const Point<T> &p);

    // Retorna el polígono en forma de string
    std::string to_string() const;

    // Chequea que el polígono sea CCW
    bool in_ccw() const;

    // Operación concatenación con string
    template<class U>
    friend std::ostream &operator<<(std::ostream &out, const Polygon<U> &p);

    // Imprime el punto en la consola
    void print() const;

    // Asignación
    Polygon<T> &operator=(const Polygon<T> &poly);

    // Calcula el área del polígono
    T area() const;

    // Calcula el perímetro
    T perimeter() const;

    // Comprueba que un punto está dentro del polígono
    bool in_poly(Point<T> &p) const;

    // Comprueba que un punto sea vértice del polígono
    bool is_vertex(Point<T> &p) const;

    // Comprueba que el polígono tiene los mismos puntos que otro
    bool same_points(const Polygon<T> &poly);
};

template<class T>
/**
 * Constructor sin parámetros.
 *
 * @tparam T - Template
 */
Polygon<T>::Polygon() {
    this->puntos = new Point<T>[this->MAX_POINTS];
}

template<class T>
/**
 * Crea polígono usando lista de puntos.
 *
 * @tparam T - Template
 * @param size - Largo de la lista de puntos
 * @param plist - Lista de puntos
 */
Polygon<T>::Polygon(Point<T> *plist, int size) {
    this->MAX_POINTS = std::max(this->MAX_POINTS, size);
    this->puntos = new Point<T>[this->MAX_POINTS];

    // Recorre cada punto y lo añade
    for (int i = 0; i < size; i++) {
        this->add_point(plist[i]);
    }
}

template<class T>
/**
 * Destructor.
 *
 * @tparam T - Template
 */
Polygon<T>::~Polygon() {
}

template<class T>
/**
 * Añade punto al polígono.
 *
 * @tparam T - Template
 * @param p - Punto
 */
void Polygon<T>::add_point(const Point<T> &p) {
    // Si se pasa del límite se aumenta límite de la lista
    if (this->totalp >= this->MAX_POINTS) {
        this->MAX_POINTS *= 10;

        // Se crea una nueva lista
        Point<T> *ptemp = new Point<T>[MAX_POINTS];

        // Se copian los puntos a nueva lista
        for (int i = 0; i < this->totalp; i++) {
            ptemp[i] = this->puntos[i];
        }

        // Se borra lista de puntos vieja
        delete[] this->puntos;

        // Se actualiza puntero
        this->puntos = ptemp;
    }

    // Se guarda el punto
    this->puntos[this->totalp] = p;
    this->totalp += 1;
}

template<class T>
/**
 * Crea string del polígono.
 *
 * @tparam T - Template
 * @return
 */
std::string Polygon<T>::to_string() const {
    std::string s = "";
    for (int i = 0; i < this->totalp; i++) {
        s += this->puntos[i].toString();
        if (i < this->totalp - 1) {
            s += "->";
        }
    }
    return s;
}

template<class T>
/**
 * Chequea que el polígono esté en CCW.
 *
 * @tparam T - Template
 * @return
 */
bool Polygon<T>::in_ccw() const {
    T sum = 0;

    if (this->totalp >= 3) {
        for (int i = 0; i < this->totalp; i++) {
            sum += (this->puntos[(i + 1) % (this->totalp)].getCoordX() - this->puntos[i].getCoordX()) *
                   (this->puntos[i].getCoordY() + this->puntos[(i + 1) % (this->totalp)].getCoordY());
        }

        // Es CCW si la suma es menor a cero
        if (sum < 0) {
            return true;
        }
    }
    return false;
}

template<class T>
/**
 * Operador concatenación con strings.
 *
 * @return
 */
std::ostream &operator<<(std::ostream &out, const Polygon<T> &p) {
    out << p.to_string();
    return out;
}

template<class T>
/**
 * Imprime el punto en la consola.
 *
 * @tparam T - Template
 */
void Polygon<T>::print() const {
    std::cout << this->to_string() << std::endl;
}

template<class T>
/**
 * Operador asignación a otro polígono, clona lista de puntos.
 *
 * @tparam T - Template
 * @param poly - Polígono
 * @return
 */
Polygon<T> &Polygon<T>::operator=(const Polygon<T> &poly) {
    this->totalp = poly.totalp;
    this->MAX_POINTS = poly.MAX_POINTS;
    delete[] this->puntos;

    // Crea nueva lista de puntos
    this->puntos = new Point<T>[this->MAX_POINTS];

    // Copia los puntos
    for (int i = 0; i < this->totalp; i++) {
        this->puntos[i] = poly.puntos[i];
    }
}

template<class T>
/**
 * Calcula el área del polígono.
 *
 * @tparam T - Template
 * @return
 */
T Polygon<T>::area() const {
    if (this->in_ccw() or true) {
        T _sumarea = 0.0;

        // Se crea un punto cualquiera P en el origen para calcular el área
        Point<T> p0 = Point<T>(0.0, 0.0);

        // Se recorre cada punto para calcular el área
        for (int i = 0; i < this->totalp; i++) {
            _sumarea += area2(p0, this->puntos[i], this->puntos[(i + 1) % (this->totalp)]);
        }
        return _sumarea;
    } else {
        throw std::logic_error("No se puede calcular el area a un Polygon que no este en CCW");
    }
}

template<class T>
/**
 * Chequea si un punto está dentro de un polígono.
 *
 * @tparam T - Template
 * @param p - Punto
 * @return
 */
bool Polygon<T>::in_poly(Point<T> &p) const {
    int i, j;
    bool c = false;
    for (i = 0, j = this->totalp - 1; i < this->totalp; j = i++) {
        if (((this->puntos[i].getCoordY() > p.getCoordY()) != (this->puntos[j].getCoordY() > p.getCoordY())) &&
            (p.getCoordX() < (this->puntos[j].getCoordX() - this->puntos[i].getCoordX()) *
                             (p.getCoordY() - this->puntos[i].getCoordY()) /
                             (this->puntos[j].getCoordY() - this->puntos[i].getCoordY()) +
                             this->puntos[i].getCoordX()))
            c = !c;
    }
    return c;
}

template<class T>
/**
 * Calcula el área entre tres puntos.
 *
 * @tparam T - Template
 * @param a - Punto A
 * @param b - Punto B
 * @param c - Punto C
 * @return
 */
T Polygon<T>::area2(Point<T> &a, Point<T> &b, Point<T> &c) const {
    T _area = (b.getCoordX() - a.getCoordX()) * (c.getCoordY() - a.getCoordY()) -
              (b.getCoordY() - a.getCoordY()) * (c.getCoordX() - a.getCoordX());
    return 0.5 * _area;
}

template<class T>
/**
 * Calcula el perímetro de un polígono.
 *
 * @tparam T Template
 * @return
 */
T Polygon<T>::perimeter() const {
    Segment<T> s;
    T sum = 0;

    // Se recorren cada par de puntos creando segmentos
    for (int i = 0; i < this->totalp; i++) {
        s = Segment<T>(this->puntos[i], this->puntos[(i + 1) % (this->totalp)]);
        sum += s.norm();
    }
    return sum;
}

template<class T>
/**
 * Verifica que el polígono tiene iguales puntos que otro.
 *
 * @tparam T - Template
 * @param poly - Polígono a comparar
 * @return
 */
bool Polygon<T>::same_points(const Polygon<T> &poly) {
    // Recorre cada punto del polígono y verifica que exista en el arreglo de puntos del otro
    for (int i = 0; i < this->totalp; i++) {
        if (!poly.is_vertex(this->puntos[i])) {
            std::cout << "El siguiente punto no pertenece a ambas cerraduras:\t" << std::endl;
            this->puntos[i].print();
            return false;
        }
    }
    return true;
}

template<class T>
/**
 * Verifica que un punto es vértice del polígono.
 *
 * @tparam T - Template
 * @param p - Punto
 * @return
 */
bool Polygon<T>::is_vertex(Point<T> &p) const {
    for (int i = 0; i < this->totalp; i++) {
        if (fabs(this->puntos[i].getCoordX() - p.getCoordX()) < 1e-5 &&
            fabs(this->puntos[i].getCoordY() - p.getCoordY()) < 1e-5)
            return true;
    }
    return false;
}

#endif //T_CC7515_POLYGON_H