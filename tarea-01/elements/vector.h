/**
 * Clase Vector.
 * Provee funciones para operaciones producto punto, cruz y normalización.
 *
 * @author Pablo Pizarro
 * @date 30/09/2018
 */

// Definición de constantes
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

// Importación de librerías
#ifndef T1_CC5502_1_VECTOR_H
#define T1_CC5502_1_VECTOR_H

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
    T getI() const;

    // Obtiene la coordenada y
    T getJ() const;

    // Obtiene la coordenada z
    T getK() const;

    // Establece la coordenada x
    void setI(T i);

    // Establece la coordenada y
    void setJ(T j);

    // Establece una coordenada z
    void setK(T k);

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

    // Asignación
    Vector<T> &operator=(const Vector<T> &v);

    // Retorna la dimensión del vector
    int get_dim() const;

    // Retorna la norma del vector
    T norm() const;

    // Retorna el valor absoluto del vector
    Vector<T> abs() const;

    // Operación concatenación con string
    template<class U>
    friend std::ostream &operator<<(std::ostream &out, const Vector<U> &v);

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
    this->setI(i);
    this->setJ(j);
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
    this->setI(i);
    this->setJ(j);
    this->setK(k);
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
    std::string s = "[" + std::to_string(this->getI()) + "," + std::to_string(this->getJ());
    if (this->dim == 2) {
        s += "]";
    } else {
        s += "," + std::to_string(this->getK()) + "]";
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
void Vector<T>::setI(T i) {
    this->c[0] = i;
}

template<class T>
/**
 * Establece componente j (en y).
 * 
 * @tparam T Template
 * @param j Valor de la componente
 */
void Vector<T>::setJ(T j) {
    this->c[1] = j;
}

template<class T>
/**
 * Establece componente k (en z).
 * 
 * @tparam T Template
 * @param k Valor de la componente
 */
void Vector<T>::setK(T k) {
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
T Vector<T>::getI() const {
    return this->c[0];
}

template<class T>
/**
 * Retorna la componente en y.
 * 
 * @tparam T Template
 * @return Componente en y
 */
T Vector<T>::getJ() const {
    return this->c[1];
}

template<class T>
/**
 * Retorna la componente en k.
 * 
 * @tparam T Template
 * @return Componente en z
 */
T Vector<T>::getK() const {
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
        m = std::max(this->getI(), this->getJ());
        this->setI(this->getI() / m);
        this->setJ(this->getJ() / m);
    } else {
        m = std::max(std::max(this->getI(), this->getJ()), this->getK());
        this->setI(this->getI() / m);
        this->setJ(this->getJ() / m);
        this->setK(this->getK() / m);
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
    i = this->getJ() * v.c[2] - this->c[2] * v.getJ();
    j = this->c[2] * v.getI() - this->getI() * v.c[2];
    k = this->getI() * v.getJ() - this->getJ() * v.getI();
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
    this->setI(v.getI());
    this->setJ(v.getJ());
    if (v.dim == 3) {
        this->dim = 3;
        this->setK(v.getK());
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
    i = this->getI() * v.getI();
    j = this->getJ() * v.getJ();
    if (this->dim == 2 && v.dim == 2) {
        return Vector<T>(i, j);
    } else if (this->dim == 3 && v.dim == 3) {
        k = this->getK() * v.getK();
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
        d = sqrt(pow(this->getI(), 2) + pow(this->getJ(), 2));
    } else {
        d = sqrt(pow(this->getI(), 2) + pow(this->getJ(), 2) + pow(this->getK(), 2));
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
        return Vector<T>(std::abs(this->getI()), std::abs(this->getJ()));
    } else {
        return Vector<T>(std::abs(this->getI()), std::abs(this->getJ()), std::abs(this->getK()));
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
        this->setI(p.get_coord_x());
        this->setJ(p.get_coord_y());
        this->c[2] = 0.0;

    } else {  // Inicia vector en 3D
        this->setI(p.get_coord_x());
        this->setJ(p.get_coord_y());
        this->setK(p.get_coord_z());
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
        n = new Vector<T>(this->getI(), this->getJ());
    } else {
        n = new Vector<T>(this->getI(), this->getJ(), this->getK());
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

#pragma clang diagnostic pop
#endif //T1_CC5502_1_VECTOR_H