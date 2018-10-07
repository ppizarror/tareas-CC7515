/**
 * Clase LoadOff, carga archivos .off y los retorna como estructura HalfEdge.
 *
 * @package half_edge.loff
 * @author Pablo Pizarro
 * @date 06/10/2018
 */

// Crea el bloque
#ifndef T_CC7515_HALFEDGE_LOFF_LOAD_OFF_H
#define T_CC7515_HALFEDGE_LOFF_LOAD_OFF_H
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err60-cpp"

// Carga librerías
#include "../h_edge.h"
#include <fstream>
#include <stdexcept>
#include <vector>

template<class T>
/**
 * Estructura OFF
 * @tparam T Template (tipo de valor)
 */
struct offObject {
    std::vector<Face<T>> faces;
    int num_faces = 0;
    int num_vertex = 0;
};

template<class T>
/**
 * Clase Load Off, carga .off y retorna distintas estructuras
 * @tparam T
 */
class LoadOff {
private:

    // Elimina carácteres inválidos en un string
    void cstr(std::string *str);

public:

    // Constructor
    LoadOff();

    // Destructor
    ~LoadOff();

    // Carga un archivo y retorna un vector de caras
    offObject<T> load(const std::string &filename);

};

template<class T>
/**
 * Carga un archivo y retorna un vector de caras.
 *
 * @tparam T Template
 * @param filename Nombre del archivo
 * @return Arreglo de caras
 */
offObject<T> LoadOff<T>::load(const std::string &filename) {

    /**
     * Crea la estructura
     */
    offObject<T> off = offObject<T>();
    off.faces = std::vector<Face<T>>();

    /**
     * Carga el archivo
     */
    std::ifstream in(filename);
    std::string readLine;

    /**
     * Verifica si el archivo es .OFF
     */
    std::getline(in, readLine);
    this->cstr(&readLine);
    if (readLine != "OFF") {
        std::cerr << "File '" + filename + "' is not OFF format." << std::endl;
        return off;
    }

    /**
     * Delimitadores de posición
     */
    unsigned long d_1, d_2, d_3;

    /**
     * Carga el número de caras y vértices
     */
    std::getline(in, readLine);
    d_1 = readLine.find(" ", 0);
    off.num_vertex = atoi(readLine.substr(0, d_1 + 1).c_str());
    d_2 = readLine.find(" ", d_1);
    off.num_faces = atoi(readLine.substr(d_1, d_2 + 1).c_str());

    /**
     * Si no hay número de caras o de vértices retorna
     */
    if (off.num_faces == 0 || off.num_vertex == 0) {
        std::cerr << ".OFF file '" + filename + "' does not contain any vertex or vaces." << std::endl;
        return off;
    }

    /**
     * Crea los puntos
     */
    std::vector<Point<T>> p = std::vector<Point<T>>();
    T x, y, z; // Coordenadas

    for (int n = 0; n < off.num_vertex; n++) {
        std::getline(in, readLine);
        this->cstr(&readLine);

        // Crea el punto
        d_1 = readLine.find(" ", 0);
        x = atof(readLine.substr(0, d_1).c_str());
        d_2 = readLine.find(" ", d_1 + 1);
        y = atof(readLine.substr(d_1, d_2).c_str());
        d_3 = readLine.find(" ", d_2 + 1);
        z = atof(readLine.substr(d_2, d_3).c_str());
        p.push_back(Point<T>(x, y, z));

    }

    /**
     * Crea las caras
     */
    int face_np; // Número de puntos de una cara
    int *v; // Arreglo de índices de puntos
    H_Edge<T> *he; // Arreglo de HalfEdges
    Face<T> *face;

    for (int n = 0; n < off.num_faces; n++) {
        std::getline(in, readLine);
        this->cstr(&readLine);

        // Genera una cara
        off.faces.push_back(Face<T>("F" + std::to_string(n + 1)));
        face = &off.faces[n];

        // Obtiene el número de elementos de la cara
        d_1 = readLine.find(" ", 0);
        face_np = static_cast<int>(atof(readLine.substr(0, d_1).c_str()));
        v = new int[face_np + 1];

        // Agrega c/punto de la cara
        for (int i = 0; i < face_np; i++) {
            d_2 = readLine.find(" ", d_1 + 1);
            v[i] = static_cast<int>(atof(readLine.substr(d_1, d_2).c_str()));
            d_1 = d_2;
        }

        // Crea los HalfEdge
        he = new H_Edge<T>[face_np + 1];
        for (int i = 1; i < face_np; i++) {
            he[i] = H_Edge<T>(&p[v[i]], face, std::to_string(v[i - 1]) + "-" + std::to_string(v[i]));
        }
        he[0] = H_Edge<T>(&p[v[0]], face, std::to_string(v[face_np - 1]) + "-" + std::to_string(v[0]));

        // Define relaciones
        for (int i = 0; i < face_np - 1; i++) {
            he[i].set_next(&he[i + 1]);
        }
        he[face_np - 1].set_next(&he[0]);

        face->set_hedge(&he[1]);
        face->print_hedges();

        // Limpia variables
        delete[] v;
        delete[] he;

    }

    /**
     * Retorna el vector de caras
     */
    return off;

}

template<class T>
/**
 * Constructor de la clase.
 *
 * @tparam T Template
 */
LoadOff<T>::LoadOff() {
}

template<class T>
/**
 * Destructor de la clase.
 *
 * @tparam T Template
 */
LoadOff<T>::~LoadOff() {
}

template<class T>
/**
 * Elimina carácteres inválidos en un string.
 *
 * @tparam T Template
 * @param str String
 */
void LoadOff<T>::cstr(std::string *str) {
    str->erase(std::remove(str->begin(), str->end(), '\n'), str->end());
    str->erase(std::remove(str->begin(), str->end(), '\r'), str->end());
}

#endif // T_CC7515_HALFEDGE_LOFF_LOAD_OFF_H
#pragma clang diagnostic pop