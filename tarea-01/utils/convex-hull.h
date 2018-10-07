/**
 * Funciones utilitarias de cerradura convexa
 *
 * @author Pablo Pizarro
 * @date 18/06/18
 */

// Importación de librerías
#include <algorithm>
#include <ctime>
#include <stack>
#include <vector>
#include "../elem/polygon.h"

/**
 * Configuraciones de Graham-Scan
 */
static double GRAHAM_SCAN_PARTITION = 1e6; // Partición de los puntos
static bool GRAHAM_SCAN_USE_QUICKSORT = true; // Usa Quicksort en vez de MergeSort
static bool GRAHAM_SCAN_USE_SORT = true; // Usa sort en vez de Quick-Merge sort

/**
 * Constantes
 */
#ifndef LIB_CC5502_1_CERRADURA_CONVEXA_H
#define LIB_CC5502_1_CERRADURA_CONVEXA_H

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

template<class T>
/**
 * Función utilitaria que imprime la lista de puntos
 * @tparam T - Template
 * @param P - Lista de puntos
 * @param n - Largo de la lista
 */
static void imprimeListaPuntos(Punto <T> *P, int n) {
    for (int i = 0; i < n; i++) {
        std::cout << P[i] << "->";
    }
    std::cout << "" << std::endl;
}

#pragma clang diagnostic pop

/**
 * Mide el tiempo pasado un tiempo inicial tinit.
 * @param tinit - Tiempo inicial
 */
int medirTiempo(int tinit) {
    return static_cast<int>((clock() - tinit) / double(CLOCKS_PER_SEC) * 1000);
}

template<class T>
/**
 * Intercambia dos elementos i<->j en una lista de puntos
 * @tparam T - Template
 * @param arr - Arreglo
 * @param i - Posición i
 * @param j - Posición j
 */
void swapPunto(Punto <T> arr[], int i, int j) {
    if (i == j) return;
    Punto <T> t = arr[i].clonar();
    arr[i] = arr[j];
    arr[j] = t;
}

template<class T>
/**
 * Algoritmo de Gift Wrapping.
 * @tparam T - Tipo de datos
 * @param cloud - Nube de puntos a realizar la cerradura convexa
 * @param cloud_size - Número de puntos
 * @return Poligono - Polínono con puntos que representa la cerradura convexa
 */
std::pair<Poligono < T>, int>
giftWrapping(Punto<T>
*cloud,
int cloud_size
) {

/**
 * Caso de borde
 */
if (cloud_size < 3) {
throw std::logic_error("No es posible generar la cerradura");
}
if (cloud_size == 3) {
Poligono <T> *cerradura = new Poligono<T>(cloud, cloud_size);
return
std::make_pair(*cerradura, cloud_size
);
}

/**
 * Busca el punto de más a la izquierda y más abajo
 */
Punto <T> pointOnHull;
int j = 0; // Almacena el índice del punto más hacia la izquierda
T min_x = cloud[0].getCoordX();
T min_y = cloud[0].getCoordY();
for (
int i = 1;
i<cloud_size;
i++) {
// Se encontró un punto más hacia la izquierda y abajo
if ((cloud[i].

getCoordX()

< min_x) or (min_x == cloud[i].

getCoordX() &&

cloud[i].

getCoordY()

< min_y)) {
j = i; // Actualiza el índice
min_x = cloud[i].getCoordX();
min_y = cloud[i].getCoordY();
}
}
pointOnHull = cloud[j]; // Define el punto de más hacia la izquierda

/**
 * Se crea un nuevo arreglo de puntos todos centrados en el plano +X+Y
 */
T lx = pointOnHull.getCoordX();
T ly = pointOnHull.getCoordY();
Punto <T> *new_cloud = new Punto <T>[cloud_size];
for (
int i = 0;
i<cloud_size;
i++) {
new_cloud[i] =
Punto <T>(cloud[i]
.

getCoordX()

- lx, cloud[i].

getCoordY()

- ly);
}
swapPunto(new_cloud, j,
0);
pointOnHull = new_cloud[0];

/**
 * Crea el arreglo de puntos en la cerradura convexa P, ya que en un principio
 * no se sabe cuantos son pueden ser hasta |S|=cloud_size, luego se rehará otro
 * arreglo con tamaño fijo
 */
Punto <T> *P = new Punto <T>[cloud_size];

/**
 * Se recorre cada punto de S, buscando aquellos puntos entre P[i] que formen un arco
 * que deje a todos los demás puntos de la nube hacia la derecha
 */
int i = 0;
Punto <T> endpoint; // Punto final del segmento que nace en P[i]
do {
P[i] =
pointOnHull; // Añade el punto que se sabe que pertenece a CH
endpoint = new_cloud[0];
Segmento <T> arco = Segmento<T>(P[i], endpoint);
double lastdist = -1, newdist;
for (
j = 1;
j<cloud_size;
j++) {
if (endpoint == pointOnHull or arco.
left(new_cloud[j])
) {
arco = Segmento<T>(P[i], endpoint);
endpoint = new_cloud[j];
lastdist = P[i].dist2(new_cloud[j]);
} else if (arco.
on(new_cloud[j])
&& lastdist != -1) { // Punto colineal
newdist = P[i].dist2(new_cloud[j]);
if (newdist > lastdist) {
endpoint = new_cloud[j];
arco = Segmento<T>(P[i], endpoint);
lastdist = newdist;
}
}
}
delete &
arco;
i += 1;
pointOnHull = endpoint;
} while (endpoint != P[0]);

/**
 * Suma el punto inicial restado a P
 */
Punto <T> lrest = Punto < T > (lx, ly);
for (
j = 0;
j<i;
j++) {
P[j] +=
lrest;
}

/**
 * Crea el polígono
 */
Poligono <T> *cerradura = new Poligono<T>(P, i);

/**
 * Borra variables
 */
delete[]
P;
delete[]
new_cloud;

/**
 * Retorna el polígono de la cerradura y el total de puntos
 */
return
std::make_pair(*cerradura, i
);

}

template<class T>
/**
 * Retorna el punto al lado del top del stack de puntos
 * @tparam T - Template
 * @param S - Stack de puntos
 * @return
 */
Punto <T> nextToTop(std::stack<Punto < T>>

&S) {
if (S.

size()

== 1) return S.

top();

Punto <T> p = S.top();
S.

pop();

Punto <T> res = S.top();
S.
push(p);
return
res;
}

/**
 * Intercambia dos valores i<->j de una lista de núeros enteros
 * @param arr - Arreglo
 * @param i - Posición i
 * @param j - Posición j
 */
void swapInt(int *arr, int i, int j) {
    if (i == j) return;
    int t = arr[i];
    arr[i] = arr[j];
    arr[j] = t;
}

/**
 * Clase QuickSort - Ordena la lista de puntos aplicando quicksort orden nlogn peor caso
 */
class QuickSort {
private:
    int partition(const double *cosp, int *arrpos, int low, int high) {
        double pivot = cosp[arrpos[high]];
        int i = (low - 1);

        for (int j = low; j <= high - 1; j++) {
            if (cosp[arrpos[j]] < pivot) {
                i++;
                swapInt(arrpos, i, j);
            }
        }
        swapInt(arrpos, i + 1, high);
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
 * Clase MergeSort - Ordena la lista de puntos aplicando mergesort orden nlogn peor caso
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
        delete[] L, R;
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
 * @tparam T - Tipo de datos
 * @param cloud - Nube de puntos a realizar la cerradura convexa
 * @param cloud_size - Número de puntos
 * @return Lista de puntos
 */
std::pair<Punto < T> *, int>
__grahamScan(Punto<T>
*cloud,
int cloud_size
) {

/**
 * Caso de borde
 */
if (cloud_size < 3) {
throw std::logic_error("No es posible generar la cerradura");
}
if (cloud_size == 3) {
return
std::make_pair(cloud, cloud_size
);
}

/**
 * Se elige el punto con la menor coordenada ordenada, si hay varios iguales
 * se elige aquel con el menor x
 */
T ymin = cloud[0].getCoordY(); // Valor más chico de y
int min = 0; // Posición en la nube con el punto ganador
for (
int i = 1;
i<cloud_size;
i++) { // O(n)
T y = cloud[i].getCoordY();
// Se encontró un punto con menor coordenada y, o uno que tiene igual y pero menor x
if ((y<ymin) or (y ==
ymin andcloud[i]
.

getCoordX()

< cloud[min].

getCoordX()

)) {
ymin = y;
min = i;
}
}

/**
 * Crea una lista con las posiciones de cada objeto a ordenar
 */
int *ppos = new int[cloud_size];
for (
int i = 0;
i<cloud_size;
i++) {
ppos[i] =
i;
}
swapInt(ppos,
0, min); // Deja el pivote al principio

/**
 * Se genera el pivote
 */
Punto <T> pivote = cloud[min];

/**
 * Crea una lista con las distancias de cada punto con respecto al pivote
 */
double *distp = new double[cloud_size];
for (
int i = 0;
i<cloud_size;
i++) {
distp[i] = pivote.
dist(cloud[i]);
}
distp[min] = 0;

/**
 * Crea una lista con los cosenos de cada punto con respecto al pivote
 */
double *cosp = new double[cloud_size];
for (
int i = 0;
i<cloud_size;
i++) {
cosp[i] = pivote.
cos(cloud[i], distp[i]
);
}
cosp[min] = 0;

/**
 * Se ordena la lista de acuerdo al ángulo coseno con respecto al pivote, sólo se ordenan las posiciones
 */
if (GRAHAM_SCAN_USE_SORT) {
std::sort(ppos
+ 1, ppos + cloud_size,[&cosp, &distp](
int i,
int j
) {
if (cosp[i] == cosp[j]) {
return distp[i] < distp[j];
} else {
return cosp[i] < cosp[j];
}
});
} else {
if (GRAHAM_SCAN_USE_QUICKSORT) {
QuickSort *qsort = new QuickSort();
qsort->
quickSort(cosp, ppos,
1, cloud_size - 1);
delete[]
qsort;
} else {
MergeSort *msort = new MergeSort();
msort->
mergeSort(cosp, ppos,
1, cloud_size - 1);
delete[]
msort;
}
};

/**
 * Se crea una nueva lista de acuerdo a las posiciones
 */
Punto <T> *new_cloud = new Punto <T>[cloud_size];
for (
int i = 0;
i<cloud_size;
i++) {
new_cloud[i] = cloud[ppos[i]];
}

/**
 * Si hay dos puntos con igual ángulo se deja aquel con mayor distancia
 */
int vp = 1; // Indica cuántos puntos válidos existen
double lang = 0;
double mxdist = 0, actdist = 0;
int maxpos = -1;
Punto <T> *final_cloud = new Punto <T>[cloud_size];
final_cloud[0] = new_cloud[0];
for (
int i = 1;
i<cloud_size;
i++) {
if (
fabs(cosp[ppos[i]]
- lang) > 1e-10) { // Si el ángulo cambia se actualiza la distancia
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
if (vp < 3) throw std::logic_error("No es posible generar la cerradura");

/**
 * Genera la cerradura
 */
std::stack<Punto < T>>
hull;
hull.
push(final_cloud[0]);
hull.
push(final_cloud[1]);
hull.
push(final_cloud[2]);
for (
int i = 3;
i<vp;
i++) {
while (
nextToTop(hull)
.
ccw(hull
.

top(), final_cloud[i]

) <= 0) {
hull.

pop();
}
hull.
push(final_cloud[i]);
}

/**
 * Crea la lista de la cerradura
 */
int total_cerradura = hull.size();
Punto <T> *P = new Punto <T>[total_cerradura];
for (
int i = total_cerradura - 1;
i >= 0; i--) {
P[i] = hull.

top();

hull.

pop();
}

/**
 * Elimina variables
 */
delete[]
ppos;
delete[]
cosp;
delete[]
distp;
delete[]
new_cloud;
delete[]
final_cloud;

/**
 * Retorna el par
 */
return
std::make_pair(P, total_cerradura
);

};

template<class T>
/**
 * Algoritmo de Graham Scan, algoritmo sólo valido para 2D.
 * @tparam T - Tipo de datos
 * @param cloud - Nube de puntos a realizar la cerradura convexa
 * @param cloud_size - Número de puntos
 * @return Poligono - Polínono con puntos que representa la cerradura convexa
 */
std::pair<Poligono < T>, int>
grahamScan(Punto<T>
*cloud,
int cloud_size
) {

/**
 * Divide un arreglo de a grupos y luego mezcla
 */
if (cloud_size <= GRAHAM_SCAN_PARTITION) {
std::pair<Punto < T> *, int>
cerraduraGS = __grahamScan(cloud, cloud_size);
Poligono <T> poly = Poligono<T>(cerraduraGS.first, cerraduraGS.second);
return
std::make_pair(poly, cerraduraGS
.second);
} else {

/**
 * Calcula largos de particiones
 */
int npart = static_cast<int>(cloud_size / GRAHAM_SCAN_PARTITION);
int partsize = static_cast<int>(GRAHAM_SCAN_PARTITION);

/**
 * Arreglos con puntos y resultados de cada partición
 */
Punto <T> *mixed_results = new Punto <T>[cloud_size]; // Almacena todos los puntos resultantes de cada partición

/**
 * Itera sobre cada partición, calcula cerradura y mezcla resultados
 */
int partdelta = 0; // Se usa para usar el antepenultimo con 3 puntos menos y el último con 3 más
int k = 0; // Última posición accedido
int r = 0; // Última posición añadida en mixed_results

for (
int i = 0;
i<npart;
i++) {
if (i<npart - 2) { // Cualquier partición
partdelta = 0;
} else if (i == (npart - 2)) { // Antepenúltima partición
partdelta = -3;
} else if (i == (npart - 1)) { // Última partición
partdelta = 3;
}

// Particiona los puntos
Punto <T> *partial_cloud = new Punto <T>[partsize + 3]; // Nube parcial que se calcula
for (
int j = 0;
j < (partsize + partdelta); j++) {
partial_cloud[j] = cloud[k];
k += 1;
}

// Calcula la cerradura
std::pair<Punto < T> *, int>
cerraduraGS = __grahamScan(partial_cloud, partsize + partdelta);

// Añade los puntos de la cerradura
for (
int m = 0;
m<cerraduraGS.
second;
m++) {
mixed_results[r] = cerraduraGS.first[m];
r += 1;
}
delete[]
partial_cloud;
}

/**
 * Calcula la cerradura final
 */
std::pair<Punto < T> *, int>
cerraduraGS = __grahamScan(mixed_results, r);
Poligono <T> poly = Poligono<T>(cerraduraGS.first, cerraduraGS.second);

/**
 * Elimina variables
 */
delete[]
mixed_results;

/**
 * Retorna el par
 */
return
std::make_pair(poly, cerraduraGS
.second);
}

}

#endif //LIB_CC5502_1_CERRADURA_CONVEXA_H