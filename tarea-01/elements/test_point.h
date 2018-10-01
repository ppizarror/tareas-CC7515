/**
 * Clase Punto.
 *
 * @author Pablo Pizarro
 * @date 30/09/2018
 */

// Definición de constantes
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

// Importación de librerías
#ifndef T1_CC5502_1_PUNTO_H
#define T1_CC5502_1_PUNTO_H

#include <cmath>
#include <iostream>
#include <ctgmath>
#include <stdexcept>

template<class T>
/**
 * Clase Punto en 2D o 3D.
 *
 * @tparam T Template
 */
class Point {
private:
    T coord[3]; // Crea la lista de puntos, 3 dimensiones => se puede para n
    int dim; // Dimensión del punto
public:

    // Point vacío
    Point();

    // Constructor punto en 2D
    Point(T x, T y);

    // Constructor punto en 3D
    Point(T x, T y, T z);

    // Destructor
    ~Point();

    // Obtiene la coordenada x
    T get_coord_x() const;

    // Obtiene la coordenada y
    T get_coord_y() const;

    // Obtiene la coordenada z
    T get_coord_z() const;

    // Establece la coordenada x
    void set_coord_x(T x);

    // Establece la coordenada y
    void set_coord_y(T y);

    // Establece una coordenada z
    void set_coord_z(T z);

    // Retorna el punto en forma de String
    std::string to_string() const;

    // Imprime el punto en la consola
    void print() const;

    // Suma dos puntos y retorna un tercero
    Point<T> operator+(const Point<T> &p) const;

    // Suma un punto a sí mismo
    Point<T> &operator+=(const Point<T> &p);

    // Suma dos puntos y retorna un tercero
    Point<T> operator-(Point<T> &p) const;

    // Resta unaria
    Point<T> operator-() const;

    // Suma un punto a sí mismo
    Point<T> &operator-=(const Point<T> &p);

    // Asignación
    Point<T> &operator=(const Point<T> &p);

    // Divide por un valor
    Point<T> operator/(T v) const;

    // Multiplica por un valor
    Point<T> operator*(T v) const;

    // Divide por un valor y guarda el punto
    Point<T> &operator/=(T v);

    // Multiplica por un valor y guarda el punto
    Point<T> &operator*=(T v);

    // Comprobación igualdad
    bool operator==(const Point<T> &p) const;

    // Comprobación desigualdad
    bool operator!=(const Point<T> &p) const;

    // Retorna valor absoluto
    Point<T> abs() const;

    // Calcula la distancia entre dos puntos
    double dist(Point<T> &p) const;

    // Calcula la distancia al cuadrado entre dos puntos
    double dist2(Point<T> &p) const;

    // Retorna distancia punto al origen
    double dist_origin() const;

    // Retorna la dimensión del punto
    int get_dim() const;

    // Operación concatenación con string
    template<class U>
    friend std::ostream &operator<<(std::ostream &out, const Point<U> &p);

    // Verifica si el punto y los puntos a, b forman una vuelta en ccw
    // +1   ccw
    // 0    colineal
    // -1   no ccw
    int ccw(Point<T> &a, Point<T> &b);

    // Clona el punto
    Point<T> clone();

    // Obtiene el coseno del ángulo entre un punto y otro
    double cos(Point<T> &a);

    // Obtiene el coseno del ángulo entre un punto y otro
    double cos(Point<T> &a, double dist);
};

template<class T>
/**
 * Punto vacío.
 *
 * @tparam T
 */
Point<T>::Point() {
    this->dim = 0;
}

template<class T>
/**
 * Crea punto en 2D.
 *
 * @tparam T Template
 * @param x Coordenada x
 * @param y Coordenada y
 */
Point<T>::Point(T x, T y) {
    this->dim = 2;
    this->set_coord_x(x);
    this->set_coord_y(y);
}

template<class T>
/**
 * Crea punto en 3D.
 *
 * @tparam T Template
 * @param x Coordenada x
 * @param y Coordenada y
 * @param z Coordenada z
 */
Point<T>::Point(T x, T y, T z) {
    this->dim = 3;
    this->set_coord_x(x);
    this->set_coord_y(y);
    this->set_coord_z(z);
}

template<class T>
/**
 * Elimina el objeto de la memoria.
 *
 * @tparam T Template
 */
Point<T>::~Point() {
}

template<class T>
/**
 * Retorna coordenada x.
 *
 * @tparam T Template
 * @return Coordenada
 */
T Point<T>::get_coord_x() const {
    return coord[0];
}

template<class T>
/**
 * Retorna coordenada y.
 *
 * @tparam T Template
 * @return Coordenada
 */
T Point<T>::get_coord_y() const {
    return coord[1];
}

template<class T>
/**
 * Retorna coordenada z. Sólo válido en un punto 3D.
 *
 * @tparam T Template
 * @return Coordenada
 */
T Point<T>::get_coord_z() const {
    if (this->dim < 3) {
        throw std::logic_error("Z coordinate does not exists in a 2D point");
    }
    return coord[2];
}

template<class T>
/**
 * Retorna un string con el punto.
 *
 * @tparam T Template
 * @return String en forma (x,y) o (x,y,z)
 */
std::string Point<T>::to_string() const {
    std::string s = "(" + std::to_string(this->get_coord_x()) + "," + std::to_string(this->get_coord_y()); // Point
    if (this->dim == 2) {
        s += ")";
    } else {
        s += "," + std::to_string(this->get_coord_z()) + ")";
    }
    return s;
}

template<class T>
/**
 * Imprime el punto en la consola.
 *
 * @tparam T Template
 */
void Point<T>::print() const {
    std::cout << this->to_string() << std::endl;
}

template<class T>
/**
 * Suma el punto con otro y retorna un nuevo punto.
 *
 * @tparam T Template
 * @param p Punto a sumar
 * @return Nuevo punto
 */
Point<T> Point<T>::operator+(const Point<T> &p) const {
    if (this->dim == 2 && p.dim == 2) {
        return Point<T>(this->get_coord_x() + p.get_coord_x(), this->get_coord_y() + p.get_coord_y());
    } else if (this->dim == 3 && p.dim == 3) {
        return Point<T>(this->get_coord_x() + p.get_coord_x(), this->get_coord_y() + p.get_coord_y(),
                        this->get_coord_z() + p.get_coord_z());
    } else {
        throw std::logic_error("Invalid dimension to perform add operator");
    }
}

template<class T>
/**
 * Suma un punto al objeto.
 *
 * @tparam T Template
 * @param p Punto a sumar
 */
Point<T> &Point<T>::operator+=(const Point<T> &p) {
    // Se suman primero componente x e y
    this->coord[0] += p.get_coord_x();
    this->coord[1] += p.get_coord_y();

    // Comprobación dimensiones
    if (this->dim == 2 && p.dim == 2) {
    } else if (this->dim == 3 && p.dim == 3) {
        this->coord[2] += p.get_coord_z();
    } else {
        throw std::logic_error("Invalid dimension to perform += operator");
    }
    return *this;
}

template<class T>
/**
 * Resta el punto con otro y retorna uno nuevo.
 *
 * @tparam T Template
 * @param p Punto
 * @return
 */
Point<T> Point<T>::operator-(Point<T> &p) const {
    if (this->dim == 2 && p.dim == 2) {
        return Point<T>(this->get_coord_x() - p.get_coord_x(), this->get_coord_y() - p.get_coord_y());
    } else if (this->dim == 3 && p.dim == 3) {
        return Point<T>(this->get_coord_x() - p.get_coord_x(), this->get_coord_y() - p.get_coord_y(),
                        this->get_coord_z() - p.get_coord_z());
    } else {
        throw std::logic_error("Invalid dimension to perform - operator");
    }
}

template<class T>
/**
 * Resta un punto al objeto.
 *
 * @tparam T Template
 * @param p Punto a sumar
 */
Point<T> &Point<T>::operator-=(const Point<T> &p) {
    // Se suman primero componente x e y
    this->coord[0] -= p.get_coord_x();
    this->coord[1] -= p.get_coord_y();

    // Comprobación dimensiones
    if (this->dim == 2 && p.dim == 2) {
    } else if (this->dim == 3 && p.dim == 3) {
        this->coord[2] -= p.get_coord_z();
    } else {
        throw std::logic_error("Invalid dimension to perform -= operator");
    }
    return *this;
}

template<class T>
/**
 * Resta unaria.
 *
 * @tparam T Template
 * @return
 */
Point<T> Point<T>::operator-() const {
    if (this->dim == 2) {
        return Point<T>(-this->get_coord_x(), -this->get_coord_y());
    } else {
        return Point<T>(-this->get_coord_x(), -this->get_coord_y(), -this->get_coord_z());
    }
}

template<class T>
/**
 * Operación asignación.
 *
 * @tparam T Template
 * @param p Punto
 * @return
 */
Point<T> &Point<T>::operator=(const Point<T> &p) {
    this->coord[0] = p.get_coord_x();
    this->coord[1] = p.get_coord_y();

    // Comprobación dimensiones
    if (p.get_dim() == 3) {
        this->dim = 3;
        this->coord[2] = p.get_coord_z();
    } else {
        this->dim = 2;
    }
    return *this;
}

template<class T>
/**
 * Retorna el valor absoluto del punto.
 *
 * @tparam T Template
 * @return
 */
Point<T> Point<T>::abs() const {
    if (this->dim == 2) {
        return Point<T>(std::abs(this->get_coord_x()), std::abs(this->get_coord_y()));
    } else {
        return Point<T>(std::abs(this->get_coord_x()), std::abs(this->get_coord_y()), std::abs(this->get_coord_z()));
    }
}

template<class T>
/**
 * Retorna distancia al origen del punto.
 *
 * @tparam T Template
 * @return
 */
double Point<T>::dist_origin() const {
    double d;
    if (this->dim == 2) {
        d = sqrt(pow(this->get_coord_x(), 2) + pow(this->get_coord_y(), 2));
    } else {
        d = sqrt(pow(this->get_coord_x(), 2) + pow(this->get_coord_y(), 2) + pow(this->get_coord_z(), 2));
    }
    return d;
}

template<class T>
/**
 * Calcula la distancia al cuadrado entre dos puntos.
 *
 * @tparam Template
 * @param p Punto
 * @return
 */
double Point<T>::dist2(Point<T> &p) const {
    if (this->dim == 2) {
        return pow(this->get_coord_x() - p.get_coord_x(), 2) + pow(this->get_coord_y() - p.get_coord_y(), 2);
    } else {
        return pow(this->get_coord_x() - p.get_coord_x(), 2) + pow(this->get_coord_y() - p.get_coord_y(), 2) +
               pow(this->get_coord_z() - p.get_coord_z(), 2);
    }
}

template<class T>
/**
 * Calcula la distancia entre dos puntos.
 *
 * @tparam Template
 * @param p Punto
 * @return
 */
double Point<T>::dist(Point<T> &p) const {
    return sqrt(this->dist2(p));
}

template<class T>
/**
 * Divide el punto por un valor.
 *
 * @tparam T Template
 * @param v valor
 * @return
 */
Point<T> Point<T>::operator/(T v) const {
    if (this->dim == 2) {
        return Point<T>(this->get_coord_x() / v, this->get_coord_y() / v);
    } else {
        return Point<T>(this->get_coord_x() / v, this->get_coord_y() / v, this->get_coord_z() / v);
    }
}

template<class T>
/**
 * Multiplica el punto por un valor.
 *
 * @tparam T Template
 * @param v valor
 * @return
 */
Point<T> Point<T>::operator*(T v) const {
    if (this->dim == 2) {
        return Point<T>(this->get_coord_x() * v, this->get_coord_y() * v);
    } else {
        return Point<T>(this->get_coord_x() * v, this->get_coord_y() * v, this->get_coord_z() * v);
    }
}

template<class T>
/**
 * Define coordenada x.
 *
 * @tparam T Template
 * @param x Valor de la coordenada
 */
void Point<T>::set_coord_x(T x) {
    this->coord[0] = x;
}

template<class T>
/**
 * Define coordenada y.
 *
 * @tparam T Template
 * @param y Valor de la coordenada
 */
void Point<T>::set_coord_y(T y) {
    this->coord[1] = y;
}

template<class T>
/**
 * Define coordenada z, aumenta dimensión.
 *
 * @tparam T Template
 * @param z Valor de la coordenada
 */
void Point<T>::set_coord_z(T z) {
    this->coord[2] = z;
    if (this->dim == 2) {
        this->dim = 3;
    }
}

template<class T>
/**
 * Divide y guarda en el mismo punto.
 *
 * @tparam T Template
 * @param v Valor a dividir
 * @return Puntero al mismo objeto dividido
 */
Point<T> &Point<T>::operator/=(T v) {
    // Se suman primero componente x e y
    this->coord[0] /= v;
    this->coord[1] /= v;

    // Comprobación dimensiones
    if (this->dim == 3) {
        this->coord[2] /= v;
    }
    return *this;
}

template<class T>
/**
 * Multiplica y guarda en el mismo punto.
 *
 * @tparam T Template
 * @param v Valor a dividir
 * @return Puntero al mismo objeto multiplicado
 */
Point<T> &Point<T>::operator*=(T v) {
    // Se suman primero componente x e y
    this->coord[0] *= v;
    this->coord[1] *= v;

    // Comprobación dimensiones
    if (this->dim == 3) {
        this->coord[2] *= v;
    }
    return *this;
}

template<class T>
/**
 * Verifica igualdad con otro punto.
 *
 * @tparam T Template
 * @param p Punto
 * @return Indica si el punto es igual con otro
 */
bool Point<T>::operator==(const Point<T> &p) const {
    if (this->dim == 2 && p.dim == 2) {
        return this->get_coord_x() == p.get_coord_x() && this->get_coord_y() == p.get_coord_y();
    } else if (this->dim == 3 && p.dim == 3) {
        return this->get_coord_x() == p.get_coord_x() && this->get_coord_y() == p.get_coord_y() &&
               this->get_coord_z() == p.get_coord_z();
    } else {
        throw std::logic_error("Point dimension is not the same");
    }
}

template<class T>
/**
 * Operador desigualdad.
 *
 * @tparam T Template
 * @param p Punto
 * @return Indica si el punto no es igual con otro
 */
bool Point<T>::operator!=(const Point<T> &p) const {
    if (this->dim == 2 && p.dim == 2) {
        return this->get_coord_x() != p.get_coord_x() || this->get_coord_y() != p.get_coord_y();
    } else if (this->dim == 3 && p.dim == 3) {
        return this->get_coord_x() != p.get_coord_x() || this->get_coord_y() != p.get_coord_y() ||
               this->get_coord_z() == p.get_coord_z();
    } else {
        throw std::logic_error("Point dimension is not the same");
    }
}

template<class T>
/**
 * Retorna la dimensión del punto.
 *
 * @tparam T Template
 * @return Dimensión en N
 */
int Point<T>::get_dim() const {
    return this->dim;
}

template<class T>
/**
 * Operador concatenación con strings.
 *
 * @return Concatena con un strng
 */
std::ostream &operator<<(std::ostream &out, const Point<T> &p) {
    out << p.to_string();
    return out;
}

template<class T>
/**
 * Comprueba el orden ccw entre el punto y a,b.
 *
 * @tparam T Template
 * @param a Punto a
 * @param b Punto b
 * @return -1 si no es ccw, 1 si es ccw
 */
int Point<T>::ccw(Point<T> &a, Point<T> &b) {
    T area = (a.get_coord_x() - this->get_coord_x()) * (b.get_coord_y() - this->get_coord_y()) -
             (a.get_coord_y() - this->get_coord_y()) * (b.get_coord_x() - this->get_coord_x());
    if (area > 0.0f)
        return -1; // No son ccw
    else
        return 1; // ccw
}

template<class T>
/**
 * Clona el punto.
 *
 * @tparam T Template
 * @return Punto nuevo
 */
Point<T> Point<T>::clone() {
    if (this->dim < 3)
        return Point<T>(this->get_coord_x(), this->get_coord_y());
    else
        return Point<T>(this->get_coord_z(), this->get_coord_y(), this->get_coord_z());
}

template<class T>
/**
 * Obtiene el coseno del ángulo entre el punto y otro.
 *
 * @tparam T - Template
 * @param a - Punto
 * @return Valor del coseno
 */
double Point<T>::cos(Point<T> &a) {
    if (a.get_coord_x() == this->get_coord_x())
        return 0.0f;
    if (a.get_coord_y() == this->get_coord_y()) {
        if (a.get_coord_x() > this->get_coord_x()) return 1;
        else return -1;
    }
    return acos((double) ((a.get_coord_x() - this->get_coord_x())) / (this->dist(a)));
}

template<class T>
/**
 * Obtiene el coseno del ángulo entre el punto y otro.
 *
 * @tparam T - Template
 * @param a - Punto
 * @return Valor del coseno
 */
double Point<T>::cos(Point<T> &a, double dist) {
    if (a.get_coord_x() == this->get_coord_x() || dist == 0)
        return 0;
    if (a.get_coord_y() == this->get_coord_y()) {
        if (a.get_coord_x() > this->get_coord_x()) return 1;
        else return -1;
    }
    return acos((double) ((a.get_coord_x() - this->get_coord_x()) / dist));
}

#pragma clang diagnostic pop
#endif //T1_CC5502_1_PUNTO_H