/** Instrucciones
 *
 * El juego comienza con una configuracion al azar entre celdas vivas y muertas.
 *
 * Para modificar los valores de la ejecucion simplemente hay que modificar los
 * valores de las constantes declaradas mas abajo.
 *
 * N: Numero de filas que tendra la matriz que almacene el estado del juego.
 * M: Numero de columnas que tendra la matriz que almacene el estado del juego.
 *
 * BLOCK_SIZE: cantidad de threads que tendra cada bloque.
 * SRAND_VALUE: semilla que se ocupara para generar los numeros al azar.
 * GOLIF: Indicador en caso de que se quiera verificar la cantidad de celdas
 *        vecinas vivas usando solo IF's.
 * IMPRIMIR: Indicador en caso de que se necesite imprimir las matrices (esto
 *           afecta considerablemente el rendimiento de la solucion)
 */

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <fstream>

 /* Declaración de constantes */
#define SRAND_VALUE 1998	// Semilla para generar numeros random
#define IMPRIMIR 0			// Imprimir o no las matrices de entrada y de salida
#define T_LIMIT 1			// Tiempo límite de cálculo

/* Declaración de funciones */
__global__ void GOL(int dimFilas, int dimColumnas, int *grid, int *newGrid);

__global__ void ghostRows(int dimFilas, int dimColumnas, int *grid);

__global__ void ghostCols(int dimFilas, int dimColumnas, int *grid);

__global__ void GOL_IF(int dimFilas, int dimColumnas, int *grid, int *newGrid);

void imprimir(int *matriz, int n, int m);

/* Método principal */
int main(int argc, char *argv[]) {

	// Carga NxM desde un archivo
	std::ifstream infile;
	infile.open("NxM.txt");
	int x;
	int N = 0;
	int M = 0;
	int jfile = 0;
	while (infile >> x) {
		if (jfile == 0) { N = x; }
		else { M = x; }
		jfile = 1;
	}
	infile.close();

	// Carga IF, ejecutar el juego preguntando con IF (0:Falso 1:Verdadero)
	infile.open("IF.txt");
	int GOLIF = 0;
	while (infile >> x) {
		GOLIF = x;
	}
	infile.close();

	// Carga el tamaño de bloque
	infile.open("BLOCK_SIZE.txt");
	int BLOCK_SIZE = 0;
	while (infile >> x) {
		BLOCK_SIZE = x;
	}
	infile.close();

	printf("Cargando matriz %dx%d\n", N, M);
	printf("BLOCK SIZE: %d\n", BLOCK_SIZE);
	if (GOLIF) {
		printf("IF activado\n\n");
	}
	else {
		printf("IF desactivado\n\n");
	}

	int i, j;
	int *h_grid; // Matriz en CPU
	int *d_grid; // Matriz en GPU
	int *d_newGrid; // Matriz auxiliar usada solo en GPU
	int *d_tmpGrid; // Puntero auxiliar para cambiar las matrices

	signed t0, t1; // Variables para medir tiempo
	double time = 0; //variables para medir tiempo
	double Noperaciones = 0; // Variable para medir cantidad de operaciones ejecutadas

	int dimFilas = N; // Dimensiones del juego de la vida (Filas), sin contar las filas fantasmas
	int dimColumnas = M; // Dimensiones del juego de la vida (Columnas), sin contar las columnas fantasmas

	size_t bytes = sizeof(int) * (dimFilas + 2) *
		(dimColumnas + 2);// Se annade mas espacio para dejar filas y columnas fantasmas

	// Solicitamos memoria para la matriz en la CPU
	h_grid = (int *)malloc(bytes);

	// Solicitamos memoria para las matrices en la GPU
	cudaMalloc(&d_grid, bytes);
	cudaMalloc(&d_newGrid, bytes);

	// Colocamos valores aleatorios en la matriz inicialmente
	srand(SRAND_VALUE);
	for (i = 1; i <= dimFilas; i++) {
		for (j = 1; j <= dimColumnas; j++) {
			h_grid[i * (dimColumnas + 2) + j] = rand() % 2;
		}
	}

	// Copiamos valores iniciales de la matriz a la GPU
	cudaMemcpy(d_grid, h_grid, bytes, cudaMemcpyHostToDevice);
	cudaMemcpy(d_newGrid, h_grid, bytes, cudaMemcpyHostToDevice);

	// Establecemos los tamannos de los bloques y la cantidad de bloques a utilizar
	dim3 blockSize(BLOCK_SIZE, BLOCK_SIZE, 1);
	int linGrid = (int)ceil((dimFilas * dimColumnas) / (float)(BLOCK_SIZE * BLOCK_SIZE));
	dim3 gridSize(linGrid, linGrid, 1);

	dim3 cpyBlockSize(BLOCK_SIZE, 1, 1);
	dim3 cpyGridRowsGridSize((int)ceil(dimFilas / (float)cpyBlockSize.x), 1, 1);
	dim3 cpyGridColsGridSize((int)ceil((dimColumnas + 2) / (float)cpyBlockSize.x), 1, 1);

	// Imprimimos de ser el caso
	if (IMPRIMIR) {
		imprimir(h_grid, N, M);
	}

	// Ciclo principal de ejecución
	t0 = static_cast<int>(clock());
	while (time < T_LIMIT) {
		ghostRows <<< cpyGridRowsGridSize, cpyBlockSize >>> (dimFilas, dimColumnas, d_grid);
		ghostCols <<< cpyGridColsGridSize, cpyBlockSize >>> (dimFilas, dimColumnas, d_grid);
		if (GOLIF) {
			GOL_IF <<< gridSize, blockSize >>> (dimFilas, dimColumnas, d_grid, d_newGrid);
		}
		else {
			GOL <<< gridSize, blockSize >>> (dimFilas, dimColumnas, d_grid, d_newGrid);
		}

		// Intercambiamos punteros
		d_tmpGrid = d_grid;
		d_grid = d_newGrid;
		d_newGrid = d_tmpGrid;

		Noperaciones += N * M;

		t1 = static_cast<int>(clock());
		time = (double(t1 - t0) / CLOCKS_PER_SEC);
	} // Fin del ciclo principal de ejecución

	// Pedimos los resultados de vuelta
	cudaMemcpy(h_grid, d_grid, bytes, cudaMemcpyDeviceToHost);

	// Imprimimos de ser el caso
	if (IMPRIMIR) {
		printf("\n");
		imprimir(h_grid, N, M);
	}

	// Imprimimos datos pedidos
	printf("Tiempo total: %f\n", time);
	printf("Numero de operaciones efectuadas: %.0f\n", Noperaciones);

	// Se borra memoria
	cudaFree(d_grid);
	cudaFree(d_newGrid);
	free(h_grid);

	// Retorna main()
	return 0;

}

__global__ void GOL(int dimFilas, int dimColumnas, int *grid, int *newGrid) {
	// Queremos id en [1,dim]
	int iy = blockDim.y * blockIdx.y + threadIdx.y + 1;
	int ix = blockDim.x * blockIdx.x + threadIdx.x + 1;
	int id = iy * (dimColumnas + 2) + ix;

	int numNeighbors;
	if (iy <= dimFilas && ix <= dimColumnas) {

		// Obtenemos la cantidad de vecinos vivos
		numNeighbors = grid[id + (dimColumnas + 2)] + grid[id - (dimColumnas + 2)] // upper lower
			+ grid[id + 1] + grid[id - 1] // right left
			+ grid[id + (dimColumnas + 3)] + grid[id - (dimColumnas + 3)] // diagonals
			+ grid[id - (dimColumnas + 1)] + grid[id + (dimColumnas + 1)];

		int cell = grid[id];

		// Ponemos las reglas del juego
		if (cell == 1 && (numNeighbors == 2 || numNeighbors == 3)) {
			newGrid[id] = 1;
		}
		else if (cell == 0 && numNeighbors == 3) {
			newGrid[id] = 1;
		}
		else {
			newGrid[id] = cell;
		}
	}
}

__global__ void GOL_IF(int dimFilas, int dimColumnas, int *grid, int *newGrid) {
	// Queremos id en [1, dim]
	int iy = blockDim.y * blockIdx.y + threadIdx.y + 1;
	int ix = blockDim.x * blockIdx.x + threadIdx.x + 1;
	int id = iy * (dimColumnas + 2) + ix;

	int numNeighbors = 0;

	if (iy <= dimFilas && ix <= dimColumnas) {

		// Obtenemos la cantidad de vecinos vivos
		if (grid[id + (dimColumnas + 2)]) { numNeighbors++; }
		if (grid[id - (dimColumnas + 2)]) { numNeighbors++; }
		if (grid[id + 1]) { numNeighbors++; }
		if (grid[id - 1]) { numNeighbors++; }
		if (grid[id + (dimColumnas + 3)]) { numNeighbors++; }
		if (grid[id - (dimColumnas + 3)]) { numNeighbors++; }
		if (grid[id - (dimColumnas + 1)]) { numNeighbors++; }
		if (grid[id + (dimColumnas + 1)]) { numNeighbors++; }

		int cell = grid[id];

		// Ponemos las reglas del juego
		if (cell == 1 && (numNeighbors == 2 || numNeighbors == 3)) {
			newGrid[id] = 1;
		}
		else if (cell == 0 && numNeighbors == 3) {
			newGrid[id] = 1;
		}
		else {
			newGrid[id] = cell;
		}
	}
}

__global__ void ghostRows(int dimFilas, int dimColumnas, int *grid) {
	// Queremos id en [1, dim]
	int id = blockDim.x * blockIdx.x + threadIdx.x + 1;
	if (id <= dimColumnas) {
		// Copiamos la primera fila real a la última fila
		grid[(dimColumnas + 2) * (dimFilas + 1) + id] = grid[(dimColumnas + 2) + id];
		// Copiamos la última fila real a la primera fila
		grid[id] = grid[(dimColumnas + 2) * dimFilas + id];
	}
}

__global__ void ghostCols(int dimFilas, int dimColumnas, int *grid) {
	// Queremos id en [0, dim+1]
	int id = blockDim.x * blockIdx.x + threadIdx.x;
	if (id <= dimFilas + 1) {
		// Copia la primera columna real a la ultima
		grid[id * (dimColumnas + 2) + dimFilas + 1] = grid[id * (dimColumnas + 2) + 1];
		// Copia la última columna real a la primera
		grid[id * (dimColumnas + 2)] = grid[id * (dimColumnas + 2) + dimFilas];
	}
}

void imprimir(int *matriz, int n, int m) {
	for (int i = 1; i < n - 1; i++) {
		for (int j = 1; j < m - 1; j++) {
			printf("%d ", matriz[i * m + j]);
		}
		printf("\n");
	}
}