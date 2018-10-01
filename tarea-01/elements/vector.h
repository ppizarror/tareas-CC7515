/**
 * Clase Vector.
 * Provee funciones para operaciones producto punto, cruz y normalización.
 *
 * @package elements
 * @author Pablo Pizarro
 * @date 30/09/2018
 */

// Definición de constantes
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

// Inicia el bloque
#ifndef T_CC7515_ELEMENTS_VECTOR_H
#define T_CC7515_ELEMENTS_VECTOR_H

// Importación de librerías
#include <algorithm> // Para max
#include <cmath>
#include <iostream>
#include "point.h"

template<class T>
/**
 * Vector, se puede hacer a partir de punto o coordenadas.
 * Provee funciones para producto cruz, punto, norma.
 *
 * @tparam T Template
 */
class Vector {
private:
    T c[3];
    int dim;
public:

    // Inicia un vector desde un punto
    explicit Vector(Point<T> &p);

    // Inicia un vector con 2 componentes
    Vector(T i, T j);

    // Inicia un vector con 3 componentes
    Vector(T i, T j, T k);

    // Destructor
    ~Vector();

    // Obtiene la coordenada x
    T get_i() const;

    // Obtiene la coordenada y
    T get_j() const;

    // Obtiene la coordenada z
    T get_k() const;

    // Establece la coordenada x
    void set_i(T i);

    // Establece la coordenada y
    void set_j(T j);

    // Establece una coordenada z
    void set_k(T k);

    // Retorna el punto en forma de String
    std::string to_string() const;

    // Imprime el punto en la consola
    void print() const;

    // Normaliza el vector
    void normalize();

    // Retorna vector normalizado
    Vector<T> get_normalized() const;

    // Producto cruz entre dos vectores
    Vector<T> cross(const Vector<T> &v) const;

    // Producto punto entre dos vectores
    Vector<T> dot(const Vector<T> &v) const;

    // Retorna la dimensión del vector
    int get_dim() const;

    // Retorna la norma del vector
    T norm() const;

    // Retorna el valor absoluto del vector
    Vector<T> abs() const;

    // Operación concatenación con string
    template<class U>
    friend std::ostream &operator<<(std::ostream &out, const Vector<U> &v);

    // Asignación
    Vector<T> &operator=(const Vector<T> &v);

    // Suma dos vectores y retorna un tercero
    Vector<T> operator+(const Vector<T> &v) const;

    // Suma un vector a sí mismo
    Vector<T> &operator+=(const Vector<T> &v);

    // Resta dos vectores y retorna un tercero
    Vector<T> operator-(const Vector<T> &v) const;

    // Resta un vector a sí mismo
    Vector<T> &operator-=(const Vector<T> &v);

    // Resta unaria
    Vector<T> operator-() const;

    // Multiplica por un valor y retorna nuevo vector
    Vector<T> operator*(const T v) const;

    // Multiplica por un valor a sí mismo
    Vector<T> &operator*=(const T v);

    // Divide por un valor y retorna nuevo vector
    Vector<T> operator/(const T v) const;

    // Divide por un valor a sí mismo
    Vector<T> &operator/=(const T v);

    // Comprobación igualdad
    bool operator==(const Vector<T> &v) const;

    // Comprobación desigualdad
    bool operator!=(const Vector<T> &v) const;

};

template<class T>
/**
 * Constructor vector 2 componentes.
 * 
 * @tparam T
 * @param i Componente eje x
 * @param j Componente eje y
 */
Vector<T>::Vector(T i, T j) {
    this->set_i(i);
    this->set_j(j);
    this->c[2] = 0.0;
    this->dim = 2;
}

template<class T>
/**
 * Constructor vector 3 componentes.
 * 
 * @tparam T
 * @param i Componente eje x
 * @param j Componente eje y
 * @param k Componente eje z
 */
Vector<T>::Vector(T i, T j, T k) {
    this->dim = 3;
    this->set_i(i);
    this->set_j(j);
    this->set_k(k);
}

template<class T>
/**
 * Destructor.
 * 
 * @tparam T Template
 */
Vector<T>::~Vector() {
}

template<class T>
/**
 * Crea un string con el vector.
 * 
 * @tparam T Template
 * @return String del vector
 */
std::string Vector<T>::to_string() const {
    std::string s = "[" + std::to_string(this->get_i()) + "," + std::to_string(this->get_j());
    if (this->dim == 2) {
        s += "]";
    } else {
        s += "," + std::to_string(this->get_k()) + "]";
    }
    return s;
}

template<class T>
/**
 * Establece componente i (en x).
 * 
 * @tparam T Template
 * @param i Valor de la componente
 */
void Vector<T>::set_i(T i) {
    this->c[0] = i;
}

template<class T>
/**
 * Establece componente j (en y).
 * 
 * @tparam T Template
 * @param j Valor de la componente
 */
void Vector<T>::set_j(T j) {
    this->c[1] = j;
}

template<class T>
/**
 * Establece componente k (en z).
 * 
 * @tparam T Template
 * @param k Valor de la componente
 */
void Vector<T>::set_k(T k) {
    this->c[2] = k;
    if (this->dim == 2) {
        this->dim = 3;
    }
}

template<class T>
/**
 * Retorna la componente en x.
 * 
 * @tparam T Template
 * @return Componente en x
 */
T Vector<T>::get_i() const {
    return this->c[0];
}

template<class T>
/**
 * Retorna la componente en y.
 * 
 * @tparam T Template
 * @return Componente en y
 */
T Vector<T>::get_j() const {
    return this->c[1];
}

template<class T>
/**
 * Retorna la componente en k.
 * 
 * @tparam T Template
 * @return Componente en z
 */
T Vector<T>::get_k() const {
    if (this->dim < 3) {
        throw std::logic_error("K vector component does not exist in a 2D vector");
    }
    return this->c[2];
}

template<class T>
/**
 * Imprime el vector en consola.
 * 
 * @tparam T
 */
void Vector<T>::print() const {
    std::cout << this->to_string() << std::endl;
}

template<class T>
/**
 * Normaliza el vector.
 * 
 * @tparam T Template
 */
void Vector<T>::normalize() {
    T m;
    if (this->dim == 2) {
        m = std::max(this->get_i(), this->get_j());
        this->set_i(this->get_i() / m);
        this->set_j(this->get_j() / m);
    } else {
        m = std::max(std::max(this->get_i(), this->get_j()), this->get_k());
        this->set_i(this->get_i() / m);
        this->set_j(this->get_j() / m);
        this->set_k(this->get_k() / m);
    }
}

template<class T>
/**
 * Producto cruz entre vectores.
 * 
 * @tparam T Template
 * @param v Vector
 * @return Nuevo vector resultante del producto cruz
 */
Vector<T> Vector<T>::cross(const Vector<T> &v) const {
    T i, j, k;
    i = this->get_j() * v.c[2] - this->c[2] * v.get_j();
    j = this->c[2] * v.get_i() - this->get_i() * v.c[2];
    k = this->get_i() * v.get_j() - this->get_j() * v.get_i();
    return Vector<T>(i, j, k);
}

template<class T>
/**
 * Operador de asignación.
 * 
 * @tparam T Template
 * @param v
 * @return Puntero al objeto
 */
Vector<T> &Vector<T>::operator=(const Vector<T> &v) {
    this->set_i(v.get_i());
    this->set_j(v.get_j());
    if (v.dim == 3) {
        this->dim = 3;
        this->set_k(v.get_k());
    } else {
        this->dim = 2;
    }
    return *this;
}

template<class T>
/**
 * Producto punto entre dos vectores.
 * 
 * @tparam T
 * @param v
 * @return Nuevo vector producto punto
 */
Vector<T> Vector<T>::dot(const Vector<T> &v) const {
    T i, j, k;
    i = this->get_i() * v.get_i();
    j = this->get_j() * v.get_j();
    if (this->dim == 2 && v.dim == 2) {
        return Vector<T>(i, j);
    } else if (this->dim == 3 && v.dim == 3) {
        k = this->get_k() * v.get_k();
        return Vector<T>(i, j, k);
    } else {
        throw std::logic_error("Cant perform dot operator between two vectors with different dimension");
    }
}

template<class T>
/**
 * Retorna la dimensión del punto.
 * 
 * @tparam T Template
 * @return Dimensión en N
 */
int Vector<T>::get_dim() const {
    return this->dim;
}

template<class T>
/**
 * Retorna la norma del vector.
 * 
 * @tparam T Template
 * @return Norma del vector
 */
T Vector<T>::norm() const {
    T d;
    if (this->dim == 2) {
        d = sqrt(pow(this->get_i(), 2) + pow(this->get_j(), 2));
    } else {
        d = sqrt(pow(this->get_i(), 2) + pow(this->get_j(), 2) + pow(this->get_k(), 2));
    }
    return d;
}

template<class T>
/**
 * Retorna el valor absoluto del vector.
 * 
 * @tparam T Template
 * @return Vector con valores absolutos
 */
Vector<T> Vector<T>::abs() const {
    if (this->dim == 2) {
        return Vector<T>(std::abs(this->get_i()), std::abs(this->get_j()));
    } else {
        return Vector<T>(std::abs(this->get_i()), std::abs(this->get_j()), std::abs(this->get_k()));
    }
}

template<class T>
/**
 * Crea un vector a partir de un punto.
 * 
 * @tparam T Template
 * @param p Punto
 */
Vector<T>::Vector(Point<T> &p) {
    this->dim = p.get_dim();
    if (this->dim == 2) { // Inicia vector en 2D
        this->set_i(p.get_coord_x());
        this->set_j(p.get_coord_y());
        this->c[2] = 0.0;

    } else {  // Inicia vector en 3D
        this->set_i(p.get_coord_x());
        this->set_j(p.get_coord_y());
        this->set_k(p.get_coord_z());
    }
}

template<class T>
/**
 * Genera un vector normalizado del mismo.
 * 
 * @tparam T Template
 * @return Nuevo vector normalizado
 */
Vector<T> Vector<T>::get_normalized() const {
    Vector<T> *n;
    if (this->dim == 2) {
        n = new Vector<T>(this->get_i(), this->get_j());
    } else {
        n = new Vector<T>(this->get_i(), this->get_j(), this->get_k());
    }
    n->normalize();
    return *n;
}

template<class T>
/**
 * Operador concatenación con strings.
 * 
 * @return String concatenado
 */
std::ostream &operator<<(std::ostream &out, const Vector<T> &v) {
    out << v.to_string();
    return out;
}

template<class T>
/**
 * Suma dos vectores y retorna un tercero.
 *
 * @tparam T Template
 * @param v Vector a sumar
 * @return Nuevo vector sumado
 */
Vector<T> Vector<T>::operator+(const Vector<T> &v) const {
    if (this->dim == 2 && v.get_dim() == 2) {
        return Vector<T>(this->get_i() + v.get_i(), this->get_j() + v.get_j());
    } else if (this->dim == 3 && v.get_dim() == 3) {
        return Vector<T>(this->get_i() + v.get_i(), this->get_j() + v.get_j(), this->get_k() + v.get_k());
    } else {
        throw std::logic_error("Vector dimension is not the same");
    }
}

template<class T>
/**
 * Suma un vector a sí mismo
 *
 * @tparam T Template
 * @param v Vector a sumar
 * @return El puntero al objeto
 */
Vector<T> &Vector<T>::operator+=(const Vector<T> &v) {
    // Se suman componentes en i y j
    this->set_i(this->get_i() + v.get_i());
    this->set_j(this->get_j() + v.get_j());

    // Comprobación dimensiones
    if (this->dim == 3 && v.get_dim() == 2) {
    } else if (this->dim == 3 && v.get_dim() == 3) {
        this->set_k(this->get_k() + v.get_k());
    } else {
        throw std::logic_error("Invalid dimension to perform += operator");
    }

    return *this;
}

template<class T>
/**
 * Resta dos vectores y retorna un tercero.
 *
 * @tparam T Template
 * @param v Vector a sumar
 * @return Nuevo vector sumado
 */
Vector<T> Vector<T>::operator-(const Vector<T> &v) const {
    if (this->dim == 2 && v.get_dim() == 2) {
        return Vector<T>(this->get_i() - v.get_i(), this->get_j() - v.get_j());
    } else if (this->dim == 3 && v.get_dim() == 3) {
        return Vector<T>(this->get_i() - v.get_i(), this->get_j() - v.get_j(), this->get_k() - v.get_k());
    } else {
        throw std::logic_error("Vector dimension is not the same");
    }
}

template<class T>
/**
 * Resta un vector a sí mismo
 *
 * @tparam T Template
 * @param v Vector a sumar
 * @return El puntero al objeto
 */
Vector<T> &Vector<T>::operator-=(const Vector<T> &v) {
    // Se restan componentes en i y j
    this->set_i(this->get_i() - v.get_i());
    this->set_j(this->get_j() - v.get_j());

    // Comprobación dimensiones
    if (this->dim == 3 && v.get_dim() == 2) {
    } else if (this->dim == 3 && v.get_dim() == 3) {
        this->set_k(this->get_k() - v.get_k());
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
 * @return Vector con componentes negativas
 */
Vector<T> Vector<T>::operator-() const {
    if (this->dim == 2) {
        return Vector<T>(-this->get_i(), -this->get_j());
    } else {
        return Vector<T>(-this->get_i(), -this->get_j(), -this->get_k());
    }
}

template<class T>
/**
 * Multiplica por un valor y retorna nuevo vector.
 *
 * @tparam T Template
 * @param v Valor a multiplicar
 * @return Nuevo vector
 */
Vector<T> Vector<T>::operator*(const T v) const {
    if (this->dim == 2) {
        return Vector<T>(this->get_i() * v, this->get_j() * v);
    } else {
        return Vector<T>(this->get_i() * v, this->get_j() * v, this->get_k() * v);
    }
}

template<class T>
/**
 * Multiplica por un valor a sí mismo
 *
 * @tparam T Template
 * @param v Valor a multiplicar
 * @return El puntero al objeto
 */
Vector<T> &Vector<T>::operator*=(const T v) {
    this->set_i(this->get_i() * v);
    this->set_j(this->get_j() * v);
    if (this->dim == 3) {
        this->set_k(this->get_k() * v);
    }
    return *this;
}

template<class T>
/**
 * Divide por un valor y retorna nuevo vector.
 *
 * @tparam T Template
 * @param v Valor a dividir
 * @return Nuevo vector
 */
Vector<T> Vector<T>::operator/(const T v) const {
    if (v == 0) throw std::logic_error("Can't divide by zero");
    if (this->dim == 2) {
        return Vector<T>(this->get_i() / v, this->get_j() / v);
    } else {
        return Vector<T>(this->get_i() / v, this->get_j() / v, this->get_k() / v);
    }
}

template<class T>
/**
 * Divide por un valor a sí mismo
 *
 * @tparam T Template
 * @param v Valor a dividir
 * @return El puntero al objeto
 */
Vector<T> &Vector<T>::operator/=(const T v) {
    if (v == 0) throw std::logic_error("Can't divide by zero");
    this->set_i(this->get_i() / v);
    this->set_j(this->get_j() / v);
    if (this->dim == 3) {
        this->set_k(this->get_k() / v);
    }
    return *this;
}

template<class T>
/**
 * Operador igualdad.
 *
 * @tparam T Template
 * @param v Vector
 * @return Indica si dos vectores son iguales
 */
bool Vector<T>::operator==(const Vector<T> &v) const {
    if (this->dim == 2 && v.get_dim() == 2) {
        return this->get_i() == v.get_i() && this->get_j() == v.get_j();
    } else if (this->dim == 3 && v.get_dim() == 3) {
        return this->get_i() == v.get_i() && this->get_j() == v.get_j() && this->get_k() == v.get_k();
    } else {
        throw std::logic_error("Vector dimension is not the same");
    }
}

template<class T>
/**
 * Operador desigualdad.
 *
 * @tparam T Template
 * @param v Vector
 * @return Indica si dos vectores son distintos
 */
bool Vector<T>::operator!=(const Vector<T> &v) const {
    if (this->dim == 2 && v.get_dim() == 2) {
        return this->get_i() != v.get_i() || this->get_j() != v.get_j();
    } else if (this->dim == 3 && v.get_dim() == 3) {
        return this->get_i() != v.get_i() || this->get_j() != v.get_j() || this->get_k() != v.get_k();
    } else {
        throw std::logic_error("Vector dimension is not the same");
    }
}

#pragma clang diagnostic pop
#endif //T_CC7515_ELEMENTS_VECTOR_H