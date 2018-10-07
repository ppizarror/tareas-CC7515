/**
 * Cerradura convexa.
 *
 * @package utils
 * @author Pablo Pizarro
 * @date 18/06/18
 */

// Definición del bloque
#ifndef T_CC7515_UTILS_CONVEX_HULL_H
#define T_CC7515_UTILS_CONVEX_HULL_H
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err60-cpp"
#pragma ide diagnostic ignored "modernize-use-auto"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

// Importación de librerías
#include "../elements/polygon.h"
#include <algorithm>
#include <ctime>
#include <stack>
#include <vector>

// Configuraciones de Graham-Scan
static double GRAHAM_SCAN_PARTITION = 1e6; // Partición de los puntos
static bool GRAHAM_SCAN_USE_QUICKSORT = true; // Usa Quicksort en vez de MergeSort
static bool GRAHAM_SCAN_USE_SORT = true; // Usa sort en vez de Quick-Merge sort

/**
 * Mide el tiempo pasado un tiempo inicial tinit.
 *
 * @param tinit - Tiempo inicial
 */
int compute_time_between(int tinit) {
    return static_cast<int>((clock() - tinit) / double(CLOCKS_PER_SEC) * 1000);
}

template<class T>
/**
 * Intercambia dos elementos i<->j en una lista de puntos.
 *
 * @tparam T - Template
 * @param arr - Arreglo
 * @param i - Posición i
 * @param j - Posición j
 */
void swap_Point(Point<T> *arr, int i, int j) {
    if (i == j) return;
    Point<T> t = arr[i].clone();
    arr[i] = arr[j];
    arr[j] = t;
}

template<class T>
/**
 * Algoritmo de Gift Wrapping.
 *
 * @tparam T - Tipo de datos
 * @param cloud - Nube de puntos a realizar la cerradura convexa
 * @param cloud_size - Número de puntos
 * @return Poligono - Polínono con puntos que representa la cerradura convexa
 */
std::pair<Polygon<T>, int> gift_wrapping(Point<T> *cloud, int cloud_size) {

    /**
     * Caso de borde
     */
    if (cloud_size < 3) {
        std::cerr << "Can't perform gift-wrapping" << std::endl;
        throw std::logic_error("Can't perform gift-wrapping");
    }
    if (cloud_size == 3) {
        Polygon<T> *cerradura = new Polygon<T>(cloud, cloud_size);
        return std::make_pair(*cerradura, cloud_size);
    }

    /**
     * Busca el punto de más a la izquierda y más abajo
     */
    Point<T> pointOnHull;
    int j = 0; // Almacena el índice del punto más hacia la izquierda
    T min_x = cloud[0].get_coord_x();
    T min_y = cloud[0].get_coord_y();
    for (int i = 1; i < cloud_size; i++) {

        // Se encontró un punto más hacia la izquierda y abajo
        if ((cloud[i].get_coord_x() < min_x) or (min_x == cloud[i].get_coord_x() && cloud[i].get_coord_y() < min_y)) {
            j = i; // Actualiza el índice
            min_x = cloud[i].get_coord_x();
            min_y = cloud[i].get_coord_y();
        }

    }
    pointOnHull = cloud[j]; // Define el punto de más hacia la izquierda

    /**
     * Se crea un nuevo arreglo de puntos todos centrados en el plano +X+Y
     */
    T lx = pointOnHull.get_coord_x();
    T ly = pointOnHull.get_coord_y();
    Point<T> *new_cloud = new Point<T>[cloud_size];
    for (int i = 0; i < cloud_size; i++) {
        new_cloud[i] = Point<T>(cloud[i].get_coord_x() - lx, cloud[i].get_coord_y() - ly);
    }
    swap_Point(new_cloud, j, 0);
    pointOnHull = new_cloud[0];

    /**
     * Crea el arreglo de puntos en la cerradura convexa P, ya que en un principio
     * no se sabe cuantos son pueden ser hasta |S|=cloud_size, luego se rehará otro
     * arreglo con tamaño fijo
     */
    Point<T> *P = new Point<T>[cloud_size];

    /**
     * Se recorre cada punto de S, buscando aquellos puntos entre P[i] que formen un arco
     * que deje a todos los demás puntos de la nube hacia la derecha
     */
    int i = 0;
    Point<T> endpoint; // Punto final del segmento que nace en P[i]
    do {
        P[i] = pointOnHull; // Añade el punto que se sabe que pertenece a CH
        endpoint = new_cloud[0];
        Segment<T> arco = Segment<T>(P[i], endpoint);
        double lastdist = -1, newdist;
        for (j = 1; j < cloud_size; j++) {
            if (endpoint == pointOnHull or arco.left(new_cloud[j])) {
                arco = Segment<T>(P[i], endpoint);
                endpoint = new_cloud[j];
                lastdist = P[i].dist2(new_cloud[j]);
            } else if (arco.on(new_cloud[j]) && lastdist != -1) { // Punto colineal
                newdist = P[i].dist2(new_cloud[j]);
                if (newdist > lastdist) {
                    endpoint = new_cloud[j];
                    arco = Segment<T>(P[i], endpoint);
                    lastdist = newdist;
                }
            }
        }
        delete &arco;
        i += 1;
        pointOnHull = endpoint;
    } while (endpoint != P[0]);

    /**
     * Suma el punto inicial restado a P
     */
    Point<T> lrest = Point<T>(lx, ly);
    for (j = 0; j < i; j++) {
        P[j] += lrest;
    }

    /**
     * Crea el polígono
     */
    Polygon<T> *cerradura = new Polygon<T>(P, i);

    /**
     * Borra variables
     */
    delete[] P;
    delete[] new_cloud;

    /**
     * Retorna el polígono de la cerradura y el total de puntos
     */
    return std::make_pair(*cerradura, i);

}

template<class T>
/**
 * Retorna el punto al lado del top del stack de puntos.
 *
 * @tparam T - Template
 * @param S - Stack de puntos
 * @return
 */
Point<T> next_to_top(std::stack<Point<T>> &S) {
    if (S.size() == 1) return S.top();
    Point<T> p = S.top();
    S.pop();
    Point<T> res = S.top();
    S.push(p);
    return res;
}

/**
 * Intercambia dos valores i<->j de una lista de núeros enteros.
 *
 * @param arr - Arreglo
 * @param i - Posición i
 * @param j - Posición j
 */
void swap_int(int *arr, int i, int j) {
    if (i == j) return;
    int t = arr[i];
    arr[i] = arr[j];
    arr[j] = t;
}

/**
 * Clase QuickSort - Ordena la lista de puntos aplicando quicksort orden nlogn peor caso.
 */
class QuickSort {
private:
    int partition(const double *cosp, int *arrpos, int low, int high) {
        double pivot = cosp[arrpos[high]];
        int i = (low - 1);

        for (int j = low; j <= high - 1; j++) {
            if (cosp[arrpos[j]] < pivot) {
                i++;
                swap_int(arrpos, i, j);
            }
        }
        swap_int(arrpos, i + 1, high);
        return (i + 1);
    }

public:
    void quickSort(double *cosp, int *arrpos, int low, int high) {
        if (low < high) {
            int pi = this->partition(cosp, arrpos, low, high);
            quickSort(cosp, arrpos, low, pi - 1);
            quickSort(cosp, arrpos, pi + 1, high);
        }
    }
};

/**
 * Clase MergeSort - Ordena la lista de puntos aplicando mergesort orden nlogn peor caso.
 */
class MergeSort {
private:
    void merge(const double *cosp, int *arrpos, int l, int m, int r) {
        int i, j, k;
        int n1 = m - l + 1;
        int n2 = r - m;

        // Listas temporales
        auto *L = new int[n1];
        auto *R = new int[n2];

        // Copia los datos
        for (i = 0; i < n1; i++)
            L[i] = arrpos[l + i];
        for (j = 0; j < n2; j++)
            R[j] = arrpos[m + 1 + j];

        i = 0;
        j = 0;
        k = l;
        while (i < n1 && j < n2) {
            if (cosp[L[i]] < cosp[R[j]]) {
                arrpos[k] = L[i];
                i++;
            } else {
                arrpos[k] = R[j];
                j++;
            }
            k++;
        }
        while (i < n1) {
            arrpos[k] = L[i];
            i++;
            k++;
        }
        while (j < n2) {
            arrpos[k] = R[j];
            j++;
            k++;
        }

        // Elimina las listas
        delete[] L;
        delete[] R;
    }

public:
    void mergeSort(double *cosp, int *arrpos, int l, int r) {
        if (l < r) {
            int m = l + (r - l) / 2;
            mergeSort(cosp, arrpos, l, m);
            mergeSort(cosp, arrpos, m + 1, r);
            this->merge(cosp, arrpos, l, m, r);
        }
    }
};

template<class T>
/**
 * Algoritmo de Graham Scan, algoritmo sólo valido para 2D.
 *
 * @tparam T - Tipo de datos
 * @param cloud - Nube de puntos a realizar la cerradura convexa
 * @param cloud_size - Número de puntos
 * @return Lista de puntos
 */
std::pair<Point<T> *, int> __graham_scan(Point<T> *cloud, int cloud_size) {

    /**
     * Caso de borde
     */
    if (cloud_size < 3) {
        throw std::logic_error("No es posible generar la cerradura");
    }
    if (cloud_size == 3) {
        return std::make_pair(cloud, cloud_size);
    }

    /**
     * Se elige el punto con la menor coordenada ordenada, si hay varios iguales
     * se elige aquel con el menor x
     */
    T ymin = cloud[0].get_coord_y(); // Valor más chico de y
    int min = 0; // Posición en la nube con el punto ganador
    for (int i = 1; i < cloud_size; i++) { // O(n)
        T y = cloud[i].get_coord_y();

        // Se encontró un punto con menor coordenada y, o uno que tiene igual y pero menor x
        if ((y < ymin) or (y == ymin and cloud[i].get_coord_x() < cloud[min].get_coord_x())) {
            ymin = y;
            min = i;
        }

    }

    /**
     * Crea una lista con las posiciones de cada objeto a ordenar
     */
    int *ppos = new int[cloud_size];
    for (int i = 0; i < cloud_size; i++) {
        ppos[i] = i;
    }
    swap_int(ppos, 0, min); // Deja el pivote al principio

    /**
     * Se genera el pivote
     */
    Point<T> pivote = cloud[min];

    /**
     * Crea una lista con las distancias de cada punto con respecto al pivote
     */
    double *distp = new double[cloud_size];
    for (int i = 0; i < cloud_size; i++) {
        distp[i] = pivote.dist(cloud[i]);
    }
    distp[min] = 0;

    /**
     * Crea una lista con los cosenos de cada punto con respecto al pivote
     */
    double *cosp = new double[cloud_size];
    for (int i = 0; i < cloud_size; i++) {
        cosp[i] = pivote.cos(cloud[i], distp[i]);
    }
    cosp[min] = 0;

    /**
     * Se ordena la lista de acuerdo al ángulo coseno con respecto al pivote, sólo se ordenan las posiciones
     */
    if (GRAHAM_SCAN_USE_SORT) {
        std::sort(ppos + 1, ppos + cloud_size, [&cosp, &distp](int i, int j) {
            if (cosp[i] == cosp[j]) {
                return distp[i] < distp[j];
            } else {
                return cosp[i] < cosp[j];
            }
        });
    } else {
        if (GRAHAM_SCAN_USE_QUICKSORT) {
            QuickSort *qsort = new QuickSort();
            qsort->quickSort(cosp, ppos, 1, cloud_size - 1);
            delete qsort;
        } else {
            MergeSort *msort = new MergeSort();
            msort->mergeSort(cosp, ppos, 1, cloud_size - 1);
            delete msort;
        }
    };

    /**
     * Se crea una nueva lista de acuerdo a las posiciones
     */
    Point<T> *new_cloud = new Point<T>[cloud_size];
    for (int i = 0; i < cloud_size; i++) {
        new_cloud[i] = cloud[ppos[i]];
    }

    /**
     * Si hay dos puntos con igual ángulo se deja aquel con mayor distancia
     */
    int vp = 1; // Indica cuántos puntos válidos existen
    double lang = 0;
    double mxdist = 0, actdist = 0;
    int maxpos = -1;
    Point<T> *final_cloud = new Point<T>[cloud_size];
    final_cloud[0] = new_cloud[0];
    for (int i = 1; i < cloud_size; i++) {
        if (fabs(cosp[ppos[i]] - lang) > 1e-10) { // Si el ángulo cambia se actualiza la distancia
            if (maxpos != -1) {
                final_cloud[vp] = new_cloud[maxpos];
                vp += 1;
            }
            mxdist = distp[ppos[i]];
            lang = cosp[ppos[i]];
            maxpos = i;
            if (i == (cloud_size - 1)) {
                final_cloud[vp] = new_cloud[i];
                vp += 1;
            }
        } else {
            actdist = distp[ppos[i]];
            if (actdist > mxdist) {
                maxpos = i;
                mxdist = actdist;
            }
            if (i == (cloud_size - 1) && maxpos != -1) {
                final_cloud[vp] = new_cloud[maxpos];
                vp += 1;
            }
        }
    }
    if (vp < 3) {
        std::cout << "Can't perform convex hull (vp<3)" << std::endl;
        throw std::logic_error("Can't perform convex hull (vp<3)");
    }

    /**
     * Genera la cerradura
     */
    std::stack<Point<T>> hull;
    hull.push(final_cloud[0]);
    hull.push(final_cloud[1]);
    hull.push(final_cloud[2]);
    for (int i = 3; i < vp; i++) {
        while (next_to_top(hull).ccw(hull.top(), final_cloud[i]) <= 0) {
            hull.pop();
        }
        hull.push(final_cloud[i]);
    }

    /**
     * Crea la lista de la cerradura
     */
    int total_cerradura = static_cast<int>(hull.size());
    Point<T> *P = new Point<T>[total_cerradura];
    for (int i = total_cerradura - 1; i >= 0; i--) {
        P[i] = hull.top();
        hull.pop();
    }

    /**
     * Elimina variables
     */
    delete[] ppos;
    delete[] cosp;
    delete[] distp;
    delete[] new_cloud;
    delete[] final_cloud;

    /**
     * Retorna el par
     */
    return std::make_pair(P, total_cerradura);

};

template<class T>
/**
 * Algoritmo de Graham Scan, algoritmo sólo válido para 2D.
 *
 * @tparam T - Tipo de datos
 * @param cloud - Nube de puntos a realizar la cerradura convexa
 * @param cloud_size - Número de puntos
 * @return Poligono - Polínono con puntos que representa la cerradura convexa
 */
std::pair<Polygon<T>, int> graham_scan(Point<T> *cloud, int cloud_size) {

    /**
     * Divide un arreglo de a grupos y luego mezcla
     */
    if (cloud_size <= GRAHAM_SCAN_PARTITION) {
        std::pair<Point<T> *, int> cerraduraGS = __graham_scan(cloud, cloud_size);
        Polygon<T> poly = Polygon<T>(cerraduraGS.first, cerraduraGS.second);
        return std::make_pair(poly, cerraduraGS.second);
    } else {

        /**
         * Calcula largos de particiones
         */
        int npart = static_cast<int>(cloud_size / GRAHAM_SCAN_PARTITION);
        int partsize = static_cast<int>(GRAHAM_SCAN_PARTITION);

        /**
         * Arreglos con puntos y resultados de cada partición
         */
        // Almacena todos los puntos resultantes de cada partición
        Point<T> *mixed_results = new Point<T>[cloud_size];

        /**
         * Itera sobre cada partición, calcula cerradura y mezcla resultados
         */
        int partdelta = 0; // Se usa para usar el antepenultimo con 3 puntos menos y el último con 3 más
        int k = 0; // Última posición accedido
        int r = 0; // Última posición añadida en mixed_results

        for (int i = 0; i < npart; i++) {
            if (i < npart - 2) { // Cualquier partición
                partdelta = 0;
            } else if (i == (npart - 2)) { // Antepenúltima partición
                partdelta = -3;
            } else if (i == (npart - 1)) { // Última partición
                partdelta = 3;
            }

            // Particiona los puntos
            Point<T> *partial_cloud = new Point<T>[partsize + 3]; // Nube parcial que se calcula
            for (int j = 0; j < (partsize + partdelta); j++) {
                partial_cloud[j] = cloud[k];
                k += 1;
            }

            // Calcula la cerradura
            std::pair<Point<T> *, int> cerraduraGS = __graham_scan(partial_cloud, partsize + partdelta);

            // Añade los puntos de la cerradura
            for (int m = 0; m < cerraduraGS.second; m++) {
                mixed_results[r] = cerraduraGS.first[m];
                r += 1;
            }
            delete[] partial_cloud;
        }

        /**
         * Calcula la cerradura final
         */
        std::pair<Point<T> *, int> cerraduraGS = __graham_scan(mixed_results, r);
        Polygon<T> poly = Polygon<T>(cerraduraGS.first, cerraduraGS.second);

        /**
         * Elimina variables
         */
        delete[] mixed_results;

        /**
         * Retorna el par
         */
        return std::make_pair(poly, cerraduraGS.second);
    }

}

#endif //T_CC7515_UTILS_CONVEX_HULL_H