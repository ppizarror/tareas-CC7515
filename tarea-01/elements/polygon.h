/**
 * Clase Polígono.
 * Tiene funciones para calcular el área.
 *
 * @author Pablo Pizarro
 * @date 19/04/2018
 */

// Definición del bloque
#ifndef T_CC7515_POLYGON_H
#define T_CC7515_POLYGON_H
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"

// Importación de librerías
#include "segment.h"
#include <cmath>

template<class T>
/**
 * Clase Polígono, conformada por Puntos.
 * @tparam T - Template
 */
class Polygon {
private:
    Punto <T> *puntos;
    int totalp = 0;
    int MAX_POINTS = 100;

    // Calcula el área entre 3 puntos
    T area2(Punto <T> &a, Punto <T> &b, Punto <T> &c) const;

public:
    // Constructor, no toma parámetros
    explicit Poligono();

    // Se crea con lista de puntos
    Poligono(Punto <T> *plist, int size);

    // Destructor
    ~Poligono();

    // Agrega un punto
    void addPoint(const Punto <T> &p);

    // Retorna el polígono en forma de string
    std::string toString() const;

    // Chequea que el polígono sea CCW
    bool inCCW() const;

    // Operación concatenación con string
    template<class U>
    friend std::ostream &operator<<(std::ostream &out, const Poligono<U> &p);

    // Imprime el punto en la consola
    void print() const;

    // Asignación
    Poligono<T> &operator=(const Poligono<T> &poly);

    // Calcula el área del polígono
    T area() const;

    // Calcula el perímetro
    T perimetro() const;

    // Comprueba que un punto está dentro del polígono
    bool inPoly(Punto <T> &p) const;

    // Comprueba que un punto sea vértice del polígono
    bool isVertex(Punto <T> &p) const;

    // Comprueba que el polígono tiene los mismos puntos que otro
    bool mismosPuntos(const Poligono<T> &poly);
};

template<class T>
/**
 * Constructor sin parámetros.
 * @tparam T - Template
 */
Poligono<T>::Poligono() {
    this->puntos = new Punto<T>[this->MAX_POINTS];
}

template<class T>
/**
 * Crea polígono usando lista de puntos.
 * @tparam T - Template
 * @param size - Largo de la lista de puntos
 * @param plist - Lista de puntos
 */
Poligono<T>::Poligono(Punto <T> *plist, int size) {
    this->MAX_POINTS = std::max(this->MAX_POINTS, size);
    this->puntos = new Punto<T>[this->MAX_POINTS];

    // Recorre cada punto y lo añade
    for (int i = 0; i < size; i++) {
        this->addPoint(plist[i]);
    }
}

template<class T>
/**
 * Destructor.
 * @tparam T - Template
 */
Poligono<T>::~Poligono() {
}

template<class T>
/**
 * Añade punto al polígono.
 * @tparam T - Template
 * @param p - Punto
 */
void Poligono<T>::addPoint(const Punto <T> &p) {
    // Si se pasa del límite se aumenta límite de la lista
    if (this->totalp >= this->MAX_POINTS) {
        this->MAX_POINTS *= 10;

        // Se crea una nueva lista
        Punto <T> *ptemp = new Punto<T>[MAX_POINTS];

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
 * @tparam T - Template
 * @return
 */
std::string Poligono<T>::toString() const {
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
 * @tparam T - Template
 * @return
 */
bool Poligono<T>::inCCW() const {
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
 * @return
 */
std::ostream &operator<<(std::ostream &out, const Poligono<T> &p) {
    out << p.toString();
    return out;
}

template<class T>
/**
 * Imprime el punto en la consola.
 * @tparam T - Template
 */
void Poligono<T>::print() const {
    std::cout << this->toString() << std::endl;
}

template<class T>
/**
 * Operador asignación a otro polígono, clona lista de puntos.
 * @tparam T - Template
 * @param poly - Polígono
 * @return
 */
Poligono<T> &Poligono<T>::operator=(const Poligono<T> &poly) {
    this->totalp = poly.totalp;
    this->MAX_POINTS = poly.MAX_POINTS;
    delete[] this->puntos;

    // Crea nueva lista de puntos
    this->puntos = new Punto<T>[this->MAX_POINTS];

    // Copia los puntos
    for (int i = 0; i < this->totalp; i++) {
        this->puntos[i] = poly.puntos[i];
    }
}

template<class T>
/**
 * Calcula el área del polígono.
 * @tparam T - Template
 * @return
 */
T Poligono<T>::area() const {
    if (this->inCCW() or true) {
        T _sumarea = 0.0;

        // Se crea un punto cualquiera P en el origen para calcular el área
        Punto <T> p0 = Punto<T>(0.0, 0.0);

        // Se recorre cada punto para calcular el área
        for (int i = 0; i < this->totalp; i++) {
            _sumarea += area2(p0, this->puntos[i], this->puntos[(i + 1) % (this->totalp)]);
        }
        return _sumarea;
    } else {
        throw std::logic_error("No se puede calcular el area a un poligono que no este en CCW");
    }
}

template<class T>
/**
 * Chequea si un punto está dentro de un polígono.
 * @tparam T - Template
 * @param p - Punto
 * @return
 */
bool Poligono<T>::inPoly(Punto <T> &p) const {
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
 * @tparam T - Template
 * @param a - Punto A
 * @param b - Punto B
 * @param c - Punto C
 * @return
 */
T Poligono<T>::area2(Punto <T> &a, Punto <T> &b, Punto <T> &c) const {
    T _area = (b.getCoordX() - a.getCoordX()) * (c.getCoordY() - a.getCoordY()) -
              (b.getCoordY() - a.getCoordY()) * (c.getCoordX() - a.getCoordX());
    return 0.5 * _area;
}

template<class T>
/**
 * Calcula el perímetro de un polígono.
 * @tparam T Template
 * @return
 */
T Poligono<T>::perimetro() const {
    Segmento <T> s;
    T sum = 0;

    // Se recorren cada par de puntos creando segmentos
    for (int i = 0; i < this->totalp; i++) {
        s = Segmento<T>(this->puntos[i], this->puntos[(i + 1) % (this->totalp)]);
        sum += s.norm();
    }
    return sum;
}

template<class T>
/**
 * Verifica que el polígono tiene iguales puntos que otro
 * @tparam T - Template
 * @param poly - Polígono a comparar
 * @return
 */
bool Poligono<T>::mismosPuntos(const Poligono<T> &poly) {
    // Recorre cada punto del polígono y verifica que exista en el arreglo de puntos del otro
    for (int i = 0; i < this->totalp; i++) {
        if (!poly.isVertex(this->puntos[i])) {
            std::cout << "El siguiente punto no pertenece a ambas cerraduras:\t" << std::endl;
            this->puntos[i].print();
            return false;
        }
    }
    return true;
}

template<class T>
/**
 * Verifica que un punto es vértice del polígono
 * @tparam T - Template
 * @param p - Punto
 * @return
 */
bool Poligono<T>::isVertex(Punto <T> &p) const {
    for (int i = 0; i < this->totalp; i++) {
        if (fabs(this->puntos[i].getCoordX() - p.getCoordX()) < 1e-5 &&
            fabs(this->puntos[i].getCoordY() - p.getCoordY()) < 1e-5)
            return true;
    }
    return false;
}

#pragma clang diagnostic pop
#endif //T1_CC5502_1_POLIGONO_H