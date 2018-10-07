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
#include <map>

template<class T>
/**
 * Estructura OFF
 * @tparam T Template (tipo de valor)
 */
struct offObject {
    int num_faces = 0;
    int num_vertex = 0;
    std::string name;
    std::vector<Face<T>> faces;
    std::vector<Point<T>> vertex;
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

    // Busca un elemento en el mapa
    H_Edge<T> *find_map(std::map<int, std::map<int, H_Edge<T> *>> *map, int i, int j);

public:

    // Constructor
    LoadOff();

    // Destructor
    ~LoadOff();

    // Carga un archivo y retorna un vector de caras
    offObject<T> load(const std::string &filename, std::string obj_name);

    // Verifica si un off está en ccw
    bool is_ccw(const offObject<T> *off);

    // Imprime el área de la figura, por cada cara y total
    void print_area(const offObject<T> *off);

    // Retorna el área total de la figura
    T get_total_area(const offObject<T> *off);

    // Imprime cada cara del objeto (HalfEdges)
    void print_hedges(const offObject<T> *off);

    // Imprime cada cara del objeto (Points)
    void print_points(const offObject<T> *off);

};

template<class T>
/**
 * Carga un archivo y retorna un vector de caras.
 *
 * @tparam T Template
 * @param filename Nombre del archivo
 * @param obj_name Nombre del objeto
 * @return Arreglo de caras
 */
offObject<T> LoadOff<T>::load(const std::string &filename, std::string obj_name) {

    /**
     * Crea la estructura
     */
    offObject<T> off = offObject<T>();
    off.faces = std::vector<Face<T>>();
    off.name = obj_name;

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
    off.vertex = std::vector<Point<T>>();
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
        off.vertex.push_back(Point<T>(x, y, z));
    }

    /**
     * Crea las caras
     */
    int face_np; // Número de puntos de una cara
    int *v; // Arreglo de índices de puntos
    H_Edge<T> *he; // Arreglo de HalfEdges
    H_Edge<T> *hpair; // Puntero al HalfEdge par encontrado en el mapa
    Face<T> *face;

    /**
     * Crea el mapa, estructura fundamental
     * Almacena si en (i,j) existe HalfEdge -> (j,i) par
     */
    std::map<int, std::map<int, H_Edge<T> *>> hemap;

    /**
     * Crea las caras
     */
    for (int n = 0; n < off.num_faces; n++) {
        off.faces.push_back(Face<T>("F" + std::to_string(n + 1)));
    }

    for (int n = 0; n < off.num_faces; n++) {
        std::getline(in, readLine);
        this->cstr(&readLine);

        // Obtiene la cara
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
        he = new H_Edge<T>[face_np];
        for (int i = 1; i < face_np; i++) {
            he[i] = H_Edge<T>(&off.vertex[v[i]], face, std::to_string(v[i - 1]) + "-" + std::to_string(v[i]));
        }
        he[0] = H_Edge<T>(&off.vertex[v[0]], face, std::to_string(v[face_np - 1]) + "-" + std::to_string(v[0]));

        // Define relaciones de vecindad
        for (int i = 0; i < face_np - 1; i++) {
            he[i].set_next(&he[i + 1]);
        }
        he[face_np - 1].set_next(&he[0]);

        // Guarda el HalfEdge en la cara
        face->set_hedge(&he[1]);

        // Define pares
        for (int i = 1; i < face_np; i++) {
            hemap[v[i - 1]][v[i]] = &he[i]; // Se guarda par (i,j)
            hpair = this->find_map(&hemap, v[i], v[i - 1]);
            if (hpair != nullptr) { // Si (j,i) ya se definió se escribe el par
                he[i].set_pair(hpair);
            }
        }
        hemap[v[face_np - 1]][v[0]] = &he[0]; // Se guarda par (i,j)
        hpair = this->find_map(&hemap, v[0], v[face_np - 1]);
        if (hpair != nullptr) { // Si (j,i) ya se definió se escribe el par
            he[0].set_pair(hpair);
        }

        // Limpia variables
        delete[] v;
        // delete[] he; // Si se elimina esto GG
    }

    /**
     * Retorna el vector de caras
     */
    return off;

}

template<class T>
/**
 * Guarda un elemento en el mapa de HalfEdges que une los vértices (i,j).
 *
 * @tparam T Template
 * @param map Mapa
 * @param i Vértice i
 * @param j Vértice j
 * @return
 */
H_Edge<T> *LoadOff<T>::find_map(std::map<int, std::map<int, H_Edge<T> *>> *map, int i, int j) {
    std::map<int, std::map<int, H_Edge<T> *>> m = *map;
    if (m.find(i) != m.end() && m[i].find(j) != m[i].end()) {
        return m[i][j];
    }
    return nullptr;
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

template<class T>
/**
 * Verifica si un objeto OFF está en ccw.
 *
 * @tparam T Template
 * @param off Objeto off
 * @return
 */
bool LoadOff<T>::is_ccw(const offObject<T> *off) {
    Face<T> face; // Puntero a cada cara
    bool valid = true;
    for (int i = 0; i < off->num_faces; i++) {
        face = off->faces[i];
        if (!face.is_ccw()) {
            std::cout << "Face " + face.get_name() + " is not ccw" << std::endl;
            valid = false;
        }
    }
    return valid;
}

template<class T>
/**
 * Imprime el área de cada cara y la total.
 *
 * @tparam T Template
 * @param off Objeto OFF
 */
void LoadOff<T>::print_area(const offObject<T> *off) {
    T total = 0; // Área total
    T area; // Área de cada cara
    Face<T> face; // Puntero a cada cara
    std::cout << "OFF Object " + off->name << " area:" << std::endl;
    for (int i = 0; i < off->num_faces; i++) {
        face = off->faces[i];
        area = face.get_area();
        std::cout << "\tFace " << face.get_name() << " area: " << area << std::endl;
        total += area;
    }
    std::cout << "\t----------" << std::endl;
    std::cout << "\tTotal area: " << total << std::endl;
}

template<class T>
/**
 * Retorna el área total de la figura.
 *
 * @tparam T Template
 * @param off Objeto
 * @return
 */
T LoadOff<T>::get_total_area(const offObject<T> *off) {
    T total = 0; // Área total
    Face<T> face; // Puntero a cada cara
    for (int i = 0; i < off->num_faces; i++) {
        face = off->faces[i];
        total += face.get_area();
    }
    return total;
}

template<class T>
/**
 * Imprime cada cara del objeto.
 *
 * @tparam T Template
 * @param off Objeto OFF
 */
void LoadOff<T>::print_hedges(const offObject<T> *off) {
    Face<T> face; // Puntero a cada cara
    for (int i = 0; i < off->num_faces; i++) {
        face = off->faces[i];
        face.print_hedges();
    }
}

template<class T>
/**
 * Imprime cada cara del objeto.
 *
 * @tparam T Template
 * @param off Objeto OFF
 */
void LoadOff<T>::print_points(const offObject<T> *off) {
    Face<T> face; // Puntero a cada cara
    for (int i = 0; i < off->num_faces; i++) {
        face = off->faces[i];
        face.print_points();
    }
}

#endif // T_CC7515_HALFEDGE_LOFF_LOAD_OFF_H
#pragma clang diagnostic pop